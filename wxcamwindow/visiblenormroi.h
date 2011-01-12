/////////////////////////////////////////////////////////////////////////////
// Name:        visiblenormroi.h
// Purpose:  This class is used to implement ROI like objects which can
//           be showed through CCamWindow pannels and modified 
//			 interactively with the mouse. Inhetits CNormROI. The class is
//			 thread safe.	
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
#ifndef IMAGEAREA_H
#define IMAGEAREA_H

#include "crvnormroi.h"
#include <wx/gdicmn.h>
#include <wx/thread.h>

class CVisibleNormROI : protected CNormROI
{
public:
	CVisibleNormROI(void);
	CVisibleNormROI(float width, float height, float x= -1.0f, float y= -1.0f);
	virtual ~CVisibleNormROI(void);

	// Working with native coordinates (overriden)
	void SetP1Resize (const float x, const float y);
	void SetP1Move (const float x, const float y);
	void SetP2Resize (const float x, const float y);
	void SetCenter (const float x, const float y);
	void GetCenter (float& x, float& y);
	void SetSize (const float width, const float height);
	void GetSize (float& width, float& height);

	// These are overriden to support locking
	void SetP1ResizeImg (const CIplImage *pImg, const int x, const int y);
	void SetP1MoveImg (const CIplImage *pImg, const int x, const int y);
	void SetP2ResizeImg (const CIplImage *pImg, const int x, const int y);
	void SetCenterImg (const CIplImage *pImg, const int x, const int y);
	void GetCenterImg (const CIplImage *pImg, int& x, int& y);
	void SetSizeImg (const CIplImage *pImg, const int width, const int height);
	void GetBoxImg (const CIplImage *pImg, int& x, int& y, int& width, int& height);
	void GetBoxImg (const CIplImage *pImg, CvRect& box);

	float GetRotation () { return CNormROI::GetRotation(); }
	void SetRotation (float value) { CNormROI::SetRotation(value); }
	
	void SetDegradation (int value) { m_degradation= value; }

	bool GetShowOrientation () const { return m_showOrientation; }
	void SetShowOrientation (const bool value) { m_showOrientation= value; }
		
	enum { OVER_NONE= 0, OVER_LEFT_LINE, OVER_BOTTOM_LINE, OVER_RIGHT_LINE, OVER_UPPER_LINE, 
		   OVER_ORIENTER, OVER_UL_CORNER, OVER_BR_CORNER };

	// Returns where is the cursor over
	int OnMouseMovedNoClick (const CvSize& winSize, const CvPoint& cursor);
	bool OnMouseMovedClick (const CvSize& winSize, const CvPoint& cursor, const CvPoint& prevCursor);
	void OnPaint (const CvSize& winSize, CIplImage *pImg);

	bool RegisterChildROI (CVisibleNormROI* pNormROI);
	bool UnregisterChildROI (CVisibleNormROI* pNormROI);
	
private:
	void GetDirectionSegment (const CvSize& winSize, CvPoint& p1, CvPoint& p2);

	int OnMouseMovedNoClickRec (const CvSize& winSize, const CvPoint& cursor);
	int OnMouseMovedNoClick0 (const CvSize& winSize, const CvPoint& cursor);

	bool OnMouseMovedClickRec (const CvSize& winSize, const CvPoint& cursor, const CvPoint& prevCursor);
	bool OnMouseMovedClick0 (const CvSize& winSize, const CvPoint& cursor, const CvPoint& prevCursor);
	
	void OnPaintRec (const CvSize& winSize, CIplImage *pImg);
	void OnPaint0 (const CvSize& winSize, CIplImage *pImg);
	
	void Lock();
	void Unlock();
		
	wxCriticalSection m_CriticalSection;
	int m_cursorOver;
	bool m_showOrientation;
	int m_degradation;
};

#endif
