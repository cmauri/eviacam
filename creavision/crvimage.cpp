/////////////////////////////////////////////////////////////////////////////
// Name:        crvimage.cpp
// Purpose:  
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by: 
// Created:     10/01/2008
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
#include "crvimage.h"
#include <highgui.h>
#include <string.h>
#include <assert.h>

// Construction
CIplImage::CIplImage ()
{
	Init();
}

CIplImage::CIplImage (IplImage *pImg)
{
	Init();
	Import (pImg);
}

CIplImage::CIplImage (int width, int height, int depth, const char *pColorOrder)
{
	Init();

	bool retval= Create (width, height, depth, pColorOrder);
#ifndef NDEBUG
	assert (retval);
#else
	(void) (retval);
#endif
}

CIplImage::~CIplImage ()
{
	Free ();
}

void CIplImage::Init()
{
	m_pIplImage= NULL;
	m_importedImage= false;
	m_importedROI= NULL;
	m_roiStackPtr= 0;
}

void CIplImage::InitROIStack (int width, int height)
{
	m_roiStack[0].coi= 0;
	m_roiStack[0].xOffset= 0;
	m_roiStack[0].yOffset= 0;
	m_roiStack[0].width= width;
	m_roiStack[0].height= height;	
	
	m_roiStackPtr= 0;
}

// Creation
bool CIplImage::Create (int width, int height, unsigned int depth, const char *pColorOrder, int origin, int align )
{
	int nChannels= 0;
	const char *pColorModel= NULL;
	bool alphaChannel= false;

	assert (width> 0 && width< 4000);
	assert (height> 0 && height< 4000);
	assert (pColorOrder);
	assert (depth== IPL_DEPTH_1U  || depth== IPL_DEPTH_8U  ||
			depth== IPL_DEPTH_8S  || depth== IPL_DEPTH_16U ||
			depth== IPL_DEPTH_16S || depth== IPL_DEPTH_32S ||
			depth== IPL_DEPTH_32F);

	Free ();

	InitROIStack (width, height);

	if ( !strcmp (pColorOrder, "GRAY") ||
		 !strcmp (pColorOrder, "G") ) {
		nChannels= 1; pColorModel= "GRAY";
	}
	else if ( !strcmp (pColorOrder, "BGR") ||
			  !strcmp (pColorOrder, "RGB") ) {
		nChannels= 3; pColorModel= "RGB";
	}
	else if ( !strcmp (pColorOrder, "RGBA") ||
			  !strcmp (pColorOrder, "BGRA") ) {
		nChannels= 4; pColorModel= "RGB"; alphaChannel= true;	
	}
	else if ( !strcmp (pColorOrder, "YUV") ) {
		nChannels= 3; pColorModel= "YUV";
	}
	else assert (0);

	m_pIplImage= cvCreateImageHeader( cvSize(width,height), depth, nChannels );
	if (!m_pIplImage) {	assert (0);	return false; }
	m_pIplImage->alphaChannel= (alphaChannel? 1 : 0);
	strncpy (m_pIplImage->colorModel, pColorModel, 4);
	strncpy (m_pIplImage->channelSeq, pColorOrder, 4);
	m_pIplImage->dataOrder= IPL_DATA_ORDER_PIXEL;
	m_pIplImage->origin= origin;
	m_pIplImage->align=	align;
	//m_pIplImage->widthStep=
	m_pIplImage->roi= &m_roiStack[0];
	//assert (m_pIplImage->roi== &m_roiStack[0]);
	assert (m_pIplImage->imageData== NULL);

	cvCreateData( m_pIplImage );
	
	if (CV_StsOk != cvGetErrStatus()) {
		assert (0);
		
		m_pIplImage->roi= NULL;
		cvReleaseImageHeader( &m_pIplImage );
		m_pIplImage= NULL;
		return false;
	}

	return true;
}

bool CIplImage::Import (IplImage *pImage)
{
	assert (pImage);

	// Cannot import the same image
	assert (pImage!= this->m_pIplImage);
	if (pImage== this->m_pIplImage) return false;

	Free ();
	
	m_pIplImage= pImage;
	m_importedImage= true;
	InitROIStack (pImage->width, pImage->height);
	m_importedROI= pImage->roi;
	if (pImage->roi) m_roiStack[m_roiStackPtr]= *pImage->roi;
	pImage->roi= &m_roiStack[m_roiStackPtr];

	return true;
}

void CIplImage::Free ()
{
	bool wasImported= m_importedImage;
	IplImage* retval= this->Detach();
	if (retval && !wasImported) cvReleaseImage( &retval );
}

IplImage* CIplImage::Detach()
{
	if (!m_pIplImage) return NULL;

	if (m_importedImage) 
		m_pIplImage->roi= m_importedROI;
	else 
		m_pIplImage->roi= NULL;

	IplImage* retval= m_pIplImage;
	
	Init ();

	return retval;	
}

void CIplImage::Swap (CIplImage *pOtherImg)
{
	int i;

	if (this== pOtherImg) return;	// Nohing to do

	// Copy other image to tmp
	IplImage *tmp_pIplImage= pOtherImg->m_pIplImage;
	bool tmp_importedImage= pOtherImg->m_importedImage;
	IplROI *tmp_importedROI= pOtherImg->m_importedROI;
	IplROI tmp_roiStack[ROI_STACK_SIZE];
	int tmp_roiStackPtr= pOtherImg->m_roiStackPtr;
	for (i= 0; i<= tmp_roiStackPtr; i++) {
		tmp_roiStack[i]= pOtherImg->m_roiStack[i]; 
	}
	// 'Copy' current to other
	pOtherImg->m_pIplImage= m_pIplImage;
	pOtherImg->m_importedImage= m_importedImage;
	pOtherImg->m_importedROI= m_importedROI;
	pOtherImg->m_roiStackPtr= m_roiStackPtr;
	for (i= 0; i<= pOtherImg->m_roiStackPtr; i++) {
		pOtherImg->m_roiStack[i]= m_roiStack[i];
	}
	
	if (pOtherImg->m_pIplImage) {
		pOtherImg->m_pIplImage->roi= 
			&pOtherImg->m_roiStack[pOtherImg->m_roiStackPtr];
	}

	// 'Copy' tmp to current
	m_pIplImage= tmp_pIplImage;
	m_importedImage= tmp_importedImage;
	m_importedROI= tmp_importedROI;
	m_roiStackPtr= tmp_roiStackPtr;
	for (i= 0; i<= m_roiStackPtr; i++) {
		m_roiStack[i]= tmp_roiStack[i];
	}
	if (m_pIplImage) { m_pIplImage->roi= &m_roiStack[m_roiStackPtr]; }
}

void CIplImage::Reset ()
{
	assert (m_pIplImage);
	if (m_pIplImage== NULL) return;
	cvSetZero(m_pIplImage);
}

// ROI
bool CIplImage::SetROI (int x, int y, int width, 
						int height, unsigned int coi)
{
	assert (m_pIplImage);
	assert (m_pIplImage->roi== &m_roiStack[m_roiStackPtr]);
	assert (x>= 0 && y>= 0 && width> 0 && height> 0);
	
	// Check limits for the ROI
	if (x< 0 || (x + width > m_pIplImage->width)) {
		assert (false);
		return false;
	}
	if (y< 0 || (y + height > m_pIplImage->height)) {
		assert (false);
		return false;
	}

	// ROI Ok
	m_roiStack[m_roiStackPtr].coi= coi;
	m_roiStack[m_roiStackPtr].xOffset= x;
	m_roiStack[m_roiStackPtr].yOffset= y;
	m_roiStack[m_roiStackPtr].width= width;
	m_roiStack[m_roiStackPtr].height= height;
	
	return true;
}

void CIplImage::GetROI (IplROI &roi)
{
	assert (m_pIplImage);
	assert (m_pIplImage->roi== &m_roiStack[m_roiStackPtr]);

	roi= m_roiStack[m_roiStackPtr];	
}

void CIplImage::ResetROI ()
{
	assert (m_pIplImage);
	assert (m_pIplImage->roi== &m_roiStack[m_roiStackPtr]);
	
	m_pIplImage->roi->coi= 0;
	m_pIplImage->roi->xOffset= 0;
	m_pIplImage->roi->yOffset= 0;
	m_pIplImage->roi->width= m_pIplImage->width;
	m_pIplImage->roi->height= m_pIplImage->height;
}

void CIplImage::PushROI ()
{
	assert (m_pIplImage);
	assert (m_pIplImage->roi== &m_roiStack[m_roiStackPtr]);
	assert (m_roiStackPtr< ROI_STACK_SIZE - 1);

	m_roiStack[m_roiStackPtr + 1]= m_roiStack[m_roiStackPtr];
	m_roiStackPtr++;
	m_pIplImage->roi= &m_roiStack[m_roiStackPtr];
}

void CIplImage::PopROI ()
{
	assert (m_pIplImage);
//	assert (m_pIplImage->roi== &m_roiStack[m_roiStackPtr]);
	assert (m_roiStackPtr> 0);

	m_roiStackPtr--;
	m_pIplImage->roi= &m_roiStack[m_roiStackPtr];
}

bool CIplImage::Load (char *pFilename)
{
	Free ();

	m_pIplImage = cvvLoadImage( pFilename );

	if (m_pIplImage== NULL) return false;

	return true;
}

bool CIplImage::Save (char *pFilename)
{
	assert (m_pIplImage);
	assert (pFilename);

	return (cvvSaveImage (pFilename, m_pIplImage)== 0 ? false : true);
}

void CIplImage::Show (char *id)
{
    assert (m_pIplImage);
	
	cvvNamedWindow(id, CV_WINDOW_AUTOSIZE );  
    cvvShowImage( id, m_pIplImage );
#ifndef linux
	cvWaitKey (1);
#endif
}
