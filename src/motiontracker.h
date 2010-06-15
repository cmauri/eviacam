/////////////////////////////////////////////////////////////////////////////
// Name:        motiontracker.h
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

#ifndef MOTIONTRACKER_H
#define MOTIONTRACKER_H

#include "crvimage.h"
#include "crvhistogram.h"
#include "visiblenormroi.h"
#include "configbase.h"

class CMotionTracker : public CConfigBase
{
	// Methods
public:
	CMotionTracker ();

	void ProcessImage (CIplImage& image, float& xVel, float& yVel);

	const bool GetTrackFace () const { return m_trackFace; }
	void SetTrackFace (bool state) { m_trackFace= state; }
	
	inline const bool GetShowTrackFaceFilter () const;
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

	CIplImage m_imgBinFace;
	CIplImage m_imgPrevProc, m_imgCurrProc;
	CIplImage m_imgPrev, m_imgCurr;
	CIplImage m_imgVelX, m_imgVelY;
//	CIplImage m_imgSqrVelX, m_imgSqrVelY;
	TCrvLookupTable m_prevLut;
//	CCriticalSection m_critSect;
	
  // Private methods
	void AllocWorkingSpace (CIplImage &image);
	void TrackMotion (CIplImage &image, float &xVel, float &yVel);
	int PreprocessImage ();
	void PostProcessImage ();
	void ComputeFaceTrackArea (CIplImage &image);

};

inline const bool CMotionTracker::GetShowTrackFaceFilter () const 
{ 
	return m_showColorTrackerResult; 
}

#endif // MOTIONTRACKER_H
