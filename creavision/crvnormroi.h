/////////////////////////////////////////////////////////////////////////////
// Name:        crvnormroi.h
// Purpose:  Stores normalized ROI coordinates. Image resize is supported.
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by: 
// Created:     30/05/2008
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
#ifndef CRV_NORMROI_H
#define CRV_NORMROI_H

#include "crvimage.h"
#include <list>
using namespace std;

class CNormROI
{
public:
	CNormROI(void);
	CNormROI(float width, float height, float x= -1.0f, float y= -1.0f);
	virtual ~CNormROI(void);

	// Working with native coordinates
	void SetP1Resize (const float x, const float y);
	void SetP1Move (const float x, const float y);
	void SetP2Resize (const float x, const float y);
	void SetCenter (const float x, const float y);
	void GetCenter (float& x, float& y);
	void SetSize (const float width, const float height);
	void GetSize (float& width, float& height);
	
	// Get/Set position using relative arbitrary integer coordinates
	void SetP1ResizeInteger (const CvSize& size, const int x, const int y);
	void SetP1MoveInteger (const CvSize& size, const int x, const int y);
	void SetP2ResizeInteger (const CvSize& size, const int x, const int y);
	void SetCenterInteger (const CvSize& size, const int x, const int y);
	void GetCenterInteger (const CvSize& size, int& x, int& y);
	void SetSizeInteger (const CvSize& size, const int width, const int height);
	void GetBoxInteger (const CvSize& size, int& x, int& y, int& width, int& height);
	void GetBoxInteger (const CvSize& size, CvRect& box);
	void GetP1P2Integer (const CvSize& size, CvPoint& p1, CvPoint& p2);

	float GetRotation () { return m_rotation; }
	void SetRotation (float value) { m_rotation= value; }

	// Get/Set position using real CIplImage coordinates.
	// CIplImage object can change over time because
	// there is no stored reference	
	void SetP1ResizeImg (const CIplImage *pImg, const int x, const int y);
	void SetP1MoveImg (const CIplImage *pImg, const int x, const int y);
	void SetP2ResizeImg (const CIplImage *pImg, const int x, const int y);
	void SetCenterImg (const CIplImage *pImg, const int x, const int y);
	void GetCenterImg (const CIplImage *pImg, int& x, int& y);
	void SetSizeImg (const CIplImage *pImg, const int width, const int height);
	void GetBoxImg (const CIplImage *pImg, int& x, int& y, int& width, int& height);
	void GetBoxImg (const CIplImage *pImg, CvRect& box);
	
	
	// True if all ok, false if error
	bool RegisterChildROI (CNormROI* pNormROI);
	bool UnregisterChildROI (CNormROI* pNormROI);

protected:
    void FindMinChildP1 (float& x, float& y);
	void FindMinChildP1Rec (float& x, float& y);
	void FindMaxChildP2 (float& x, float& y);
	void FindMaxChildP2Rec (float& x, float& y);

	// Destruction
	virtual void Destroy();

	// Coordinate conversion
	void Integer2Normalized (const CvSize& size, const int ix, const int iy, float &nx, float &ny);
	void Normalized2Integer (const CvSize& size, const float nx, const float ny, int &ix, int &iy);

	// ROI hierarchy
	typedef std::list<CNormROI *> TNormROIList;
	typedef TNormROIList::iterator TNormROIListIterator;
	CNormROI* m_pParentROI;	
	TNormROIList m_childROIs;

private:
	void CheckInvariant();
	void FitInternalState();

	// Normalized coordinates
	//
	// TODO: implement these using integer arithmethic only,
	// for instance, using 32bit integers and a certain range 
	// (say 0..32768) thus avoiding FP arithmethic ans its 
	// inherent "unexpected" behaviour.
	// 
	float m_x, m_y, m_width, m_height;
	float m_rotation;
};

#endif
