/////////////////////////////////////////////////////////////////////////////
// Name:        visionpipeline.h
// Purpose:  
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by: 
// Created:     
// Copyright:   (C) 2008-11 Cesar Mauri Loba - CREA Software Systems
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

#ifndef VISIONPIPELINE_H
#define VISIONPIPELINE_H

#include "crvimage.h"
#include "crvhistogram.h"
#include "configbase.h"
#include "visiblenormroi.h"
#include "wxutil/waittime.h"


class CVisionPipeline : public CConfigBase
{
	// Methods
public:
	CVisionPipeline ();

	void ProcessImage (CIplImage& image, float& xVel, float& yVel);

	const bool GetTrackFace () const { return m_trackFace; }
	void SetTrackFace (bool state) { m_trackFace= state; }
	const bool GetEnableWhenFaceDetected () const { return m_enableWhenFaceDetected; }
	void SetEnableWhenFaceDetected (bool state) { m_enableWhenFaceDetected= state; }
	bool IsFaceDetected ();
	CWaitTime* GetWaitTime () { return m_waitTime; }
	
	const bool GetShowTrackFaceFilter () const { 
		return m_showColorTrackerResult; 
	}
	void SetShowTrackFaceFilter (bool state) { m_showColorTrackerResult= state; }

	CVisibleNormROI* GetTrackAreaControl () { return &m_trackArea; }

	// Configuration methods
	virtual void InitDefaults();
    virtual void WriteProfileData(wxConfigBase* pConfObj);
    virtual void ReadProfileData(wxConfigBase* pConfObj);

private:
	// Track area
	CVisibleNormROI m_trackArea;

	bool m_trackFace;
	bool m_showColorTrackerResult;
	bool m_enableWhenFaceDetected;
	CWaitTime* m_waitTime;

	CIplImage m_imgBinFace;
	CIplImage m_imgPrevProc, m_imgCurrProc;
	CIplImage m_imgPrev, m_imgCurr;
	CIplImage m_imgVelX, m_imgVelY;
	TCrvLookupTable m_prevLut;
	
	CvHaarClassifierCascade* m_faceCascade;
	CvMemStorage* m_storage;
	
	// Private methods
	void AllocWorkingSpace (CIplImage &image);
	void TrackMotion (CIplImage &image, float &xVel, float &yVel);
	int PreprocessImage ();
	void PostProcessImage ();
	void ComputeFaceTrackArea (CIplImage &image);
};

#endif
