/////////////////////////////////////////////////////////////////////////////
// Name:        visionpipeline.cpp
// Purpose:  
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by: 
// Created:     
// Copyright:   (C) 2008 Cesar Mauri Loba - CREA Software Systems
// 
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
/////////////////////////////////////////////////////////////////////////////
#include "visionpipeline.h"
#include "eviacamapp.h"
#include "viacamcontroller.h"

#include "crvcolor.h"
#include "crvmisc.h"
#include "crvskindetection.h"
#include "crvimage.h"

#include <math.h>

// Constants
#define DEFAULT_TRACK_AREA_WIDTH_PERCENT 0.50f
#define DEFAULT_TRACK_AREA_HEIGHT_PERCENT 0.30f
#define DEFAULT_TRACK_AREA_X_CENTER_PERCENT 0.5f
#define DEFAULT_TRACK_AREA_Y_CENTER_PERCENT 0.5f
#define FACE_DETECTED_THRESHOLD 0
#define FRAMES_TO_EVALUATE_FACE_DETECTION 100


CVisionPipeline::CVisionPipeline () 
{
	InitDefaults();	
}

void CVisionPipeline::AllocWorkingSpace (CIplImage &image)
{
	bool retval;

	if (!m_imgVelX.Initialized () ||
		image.Width() != m_imgVelX.Width() ||
		image.Height() != m_imgVelX.Height() ) {

		retval= m_imgBinFace.Create (image.Width(), image.Height(), 
									IPL_DEPTH_8U, "GRAY");
		assert (retval);

		retval= m_imgPrev.Create (image.Width(), image.Height(), 
								  IPL_DEPTH_8U, "GRAY");
		assert (retval);

		retval= m_imgCurr.Create (image.Width(), image.Height(), 
								  IPL_DEPTH_8U, "GRAY");
		assert (retval);

		retval= m_imgPrevProc.Create (image.Width(), image.Height(), 
								  IPL_DEPTH_8U, "GRAY");
		assert (retval);

		retval= m_imgCurrProc.Create (image.Width(), image.Height(), 
								  IPL_DEPTH_8U, "GRAY");
		assert (retval);

		retval= m_imgVelX.Create (image.Width(), image.Height(), 
								  IPL_DEPTH_32F, "GRAY");
		assert (retval);

		retval= m_imgVelY.Create (image.Width(), image.Height(), 
								  IPL_DEPTH_32F, "GRAY");
		assert (retval);
	}
}

void CVisionPipeline::ComputeFaceTrackArea (CIplImage &image)
{
	int cx, cy;
	float sx,sy;
	CvRect curBox;
	CvSeq *face = cvHaarDetectObjects(
		image.ptr(),
		m_faceCascade,
		m_storage,
  		1.25, 2, 0,
		cvSize(120./3, 90./3)
	);
	
	if (face && face->total>0)
	{
		CvRect* faceRect = (CvRect*)cvGetSeqElem(face, 0);

		m_trackArea.GetBoxImg(&image, curBox);
		
		sx= (faceRect->width * 0.1f + curBox.width * 0.9f);
		sy= (faceRect->height * 0.1f +  curBox.height * 0.9f);

		m_trackArea.SetSizeImg(&image,sx,sy);

		// Combine with new detected location
		cx= (int) ((faceRect->x+faceRect->width/2) * 0.1f + (curBox.x+curBox.width/2) * 0.9f);
		cy= (int) ((faceRect->y+faceRect->height/2) * 0.1f + (curBox.y+curBox.height/2) * 0.9f);

		// Set new box centre
		m_trackArea.SetCenterImg (&image, cx, cy);
		
		//Consider the use of a timer.
		m_framesWithFaceDetected = FRAMES_TO_EVALUATE_FACE_DETECTION;

	} else 
	{
		if (m_framesWithFaceDetected > 0)
			m_framesWithFaceDetected--;
	}
	
	if (GetEnableWhenFaceDetected() && IsFaceDetected() != wxGetApp().GetController().GetEnabled())
		wxGetApp().GetController().SetEnabled(IsFaceDetected(), true);
		
	cvClearMemStorage(m_storage);
}

bool CVisionPipeline::IsFaceDetected ()
{
	return (m_framesWithFaceDetected > FACE_DETECTED_THRESHOLD);
}

int CVisionPipeline::PreprocessImage ()
{
	TCrvHistogram his;
	int range;
		
	crvHistogram (m_imgCurr.ptr(), his);
	range= crvNormalizeHistogram (his, m_prevLut, 50);

	crvLUTTransform (m_imgPrev.ptr(), m_imgPrevProc.ptr(), m_prevLut);
	crvLUTTransform (m_imgCurr.ptr(), m_imgCurrProc.ptr(), m_prevLut);		

	return 0;
}

void CVisionPipeline::PostProcessImage ()
{
	/*
	Histogram his;
	int range;
		
	IplHistogram (m_imgCurr.ptr(), his);
	range= IplNormalizeHistogram (his, m_prevLut, 50);
	*/

}

#define COMP_MATRIX_WIDTH	15
#define COMP_MATRIX_HEIGHT	15
typedef float TAnalisysMatrix[COMP_MATRIX_WIDTH][COMP_MATRIX_HEIGHT];

void MatrixMeanImageCells (CIplImage *pCImg, TAnalisysMatrix &m)
{
	int x, y, mx, my, xCount, yCount, xIni, yIni, xLim, yLim;
	float *pSrc;
//	int validCellsCount= COMP_MATRIX_WIDTH * COMP_MATRIX_HEIGHT;

	assert (pCImg->ptr()->depth== IPL_DEPTH_32F);

	// Limits where function should be applied
	crvGetROILimits (pCImg->ptr(), xIni, yIni, xLim, yLim);

	float compBoxWidth= ((float) (xLim - xIni + 1)) / COMP_MATRIX_WIDTH;
	int iCompBoxWidth= (int) compBoxWidth;
	float compBoxHeight= ((float) (yLim - yIni + 1)) / COMP_MATRIX_HEIGHT;
	int iCompBoxHeight= (int) compBoxHeight;

	for (my= 0; my< COMP_MATRIX_HEIGHT; ++my) {
		for (mx= 0; mx< COMP_MATRIX_WIDTH; ++mx) {
			m[mx][my]= 0.0f;

			y= yIni + (int)((float) my * compBoxHeight);
			for (yCount= 0; yCount< iCompBoxHeight; ++yCount) {
				x= xIni + (int) ((float) mx * compBoxWidth);
				pSrc= (float *) crvImgOffset (pCImg->ptr(), x, y);

				for (xCount= 0; xCount< iCompBoxWidth; ++xCount) {
					m[mx][my]+= *pSrc;
					++pSrc;
				}
				m[mx][my]/= (float) (iCompBoxWidth * iCompBoxHeight);
				++y;
			}			
		}
	}
}

void CVisionPipeline::TrackMotion (CIplImage &image, float &xVel, float &yVel)
{
	CvTermCriteria term;
	CvRect box;
	static TAnalisysMatrix velXMatrix, velYMatrix, velModulusMatrix;

	crvColorToGray (image.ptr(), m_imgCurr.ptr());
	
	// Prepare ROI's
	m_imgPrev.PushROI ();
	m_imgCurr.PushROI ();
	m_imgVelX.PushROI ();
	m_imgVelY.PushROI ();

	m_trackArea.GetBoxImg (&image, box);

	m_imgPrev.SetROI (box); 
	m_imgCurr.SetROI (box);

	m_imgPrevProc.SetROI (box);
	m_imgCurrProc.SetROI (box);

	m_imgVelX.SetROI (box); 
	m_imgVelY.SetROI (box);

	// Pre-processing
	PreprocessImage ();

	// Compute optical flow
	term.type= CV_TERMCRIT_ITER;
	term.max_iter= 6;
	cvCalcOpticalFlowHS (m_imgPrevProc.ptr(), m_imgCurrProc.ptr(), 0,
						 m_imgVelX.ptr(), m_imgVelY.ptr(), 0.001, term);

	MatrixMeanImageCells (&m_imgVelX, velXMatrix);
	MatrixMeanImageCells (&m_imgVelY, velYMatrix);

	int x, y;
	float velModulusMax= 0;		

	// Compute modulus for every motion cell
	for (x= 0; x< COMP_MATRIX_WIDTH; ++x) {
		for (y= 0; y< COMP_MATRIX_HEIGHT; ++y) {
			velModulusMatrix[x][y]= 
				(velXMatrix[x][y] * velXMatrix[x][y] + velYMatrix[x][y] * velYMatrix[x][y]);
			
			if (velModulusMax< velModulusMatrix[x][y]) velModulusMax= velModulusMatrix[x][y];			
		}		
	}

	// Select valid cells (i.e. those with enough motion)
	int validCells= 0;
	xVel= yVel= 0;
	for (x= 0; x< COMP_MATRIX_WIDTH; ++x) {
		for (y= 0; y< COMP_MATRIX_HEIGHT; ++y) {
			if (velModulusMatrix[x][y]> (0.05 * velModulusMax) ) {
				++validCells;
				xVel+= velXMatrix[x][y];
				yVel+= velYMatrix[x][y];				
			}
		}
	}

	// Ensure minimal area to avoid extremely high values
	int cellArea= (box.width * box.height) / (COMP_MATRIX_WIDTH * COMP_MATRIX_HEIGHT);
	if (cellArea== 0) cellArea= 1;
	int minValidCells= (3000 / cellArea);
	if (validCells< minValidCells) validCells= minValidCells;

	// Calcula velocitat.
	xVel= - (xVel / (float) validCells) * 40;
	yVel= (yVel / (float) validCells) * 80;
	
	// Restore ROI's
	m_imgPrev.PopROI ();
	m_imgCurr.PopROI ();
	m_imgVelX.PopROI ();
	m_imgVelY.PopROI ();
}

void CVisionPipeline::ProcessImage (CIplImage& image, float& xVel, float& yVel)
{
	AllocWorkingSpace (image);

	if (m_trackFace) ComputeFaceTrackArea (image);

	TrackMotion (image, xVel, yVel);	

	if (m_trackFace && m_showColorTrackerResult) {
		// Copy face tracker output image to resulting image
		cvMerge( m_imgBinFace.ptr(), m_imgBinFace.ptr(), m_imgBinFace.ptr(), NULL, image.ptr());
	}

	// Store current image as previous
	m_imgPrev.Swap (&m_imgCurr);	
}

// Configuration methods
void CVisionPipeline::InitDefaults()
{
	m_framesWithFaceDetected = 0;
	m_trackFace= true;
	m_enableWhenFaceDetected= false;
	m_showColorTrackerResult= false;
	m_trackArea.SetSize (DEFAULT_TRACK_AREA_WIDTH_PERCENT, DEFAULT_TRACK_AREA_HEIGHT_PERCENT);
	m_trackArea.SetCenter (DEFAULT_TRACK_AREA_X_CENTER_PERCENT, DEFAULT_TRACK_AREA_Y_CENTER_PERCENT);
	m_faceCascade = (CvHaarClassifierCascade*)cvLoad("data/haarcascade_frontalface_alt.xml", 0, 0, 0);
	m_storage = cvCreateMemStorage(0);
}

void CVisionPipeline::WriteProfileData(wxConfigBase* pConfObj)
{
	float xc, yc, width, height;

	pConfObj->SetPath (_T("motionTracker"));	

	pConfObj->Write(_T("trackFace"), m_trackFace);
	pConfObj->Write(_T("enableWhenFaceDetected"), m_enableWhenFaceDetected);

	m_trackArea.GetSize (width, height);
	
	pConfObj->Write (_T("trackAreaWidth"), (double) width);
	pConfObj->Write (_T("trackAreaHeight"), (double) height);

	if (!m_trackFace) {		
		m_trackArea.GetCenter (xc, yc);
		pConfObj->Write (_T("trackAreaCenterX"), (double) xc);
		pConfObj->Write (_T("trackAreaCenterY"), (double) yc);
	}

	pConfObj->SetPath (_T(".."));
}

void CVisionPipeline::ReadProfileData(wxConfigBase* pConfObj)
{
	// Ensure proper default values if read fails
	double	xc= DEFAULT_TRACK_AREA_X_CENTER_PERCENT, 
				yc= DEFAULT_TRACK_AREA_Y_CENTER_PERCENT,
                width= DEFAULT_TRACK_AREA_WIDTH_PERCENT,
				height= DEFAULT_TRACK_AREA_HEIGHT_PERCENT;

	pConfObj->SetPath (_T("motionTracker"));
	pConfObj->Read(_T("trackFace"), &m_trackFace);
	pConfObj->Read(_T("enableWhenFaceDetected"), &m_enableWhenFaceDetected);
	pConfObj->Read (_T("trackAreaWidth"), &width);
	pConfObj->Read (_T("trackAreaHeight"), &height);
	
	m_trackArea.SetSize ((float) width, (float)height);

	if (!m_trackFace) {
		pConfObj->Read (_T("trackAreaCenterX"), &xc);
		pConfObj->Read (_T("trackAreaCenterY"), &yc);

		m_trackArea.SetCenter ((float)xc, (float)yc);		
	}

	pConfObj->SetPath (_T(".."));
}
