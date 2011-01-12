/////////////////////////////////////////////////////////////////////////////
// Name:        visiblenormroi.cpp
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
#include "visiblenormroi.h"

#define SELECTION_TOLERANCE 3

CVisibleNormROI::CVisibleNormROI(void) : CNormROI()
{		
	m_cursorOver= OVER_NONE;
	m_showOrientation= false;
	m_degradation= 0;
}

CVisibleNormROI::CVisibleNormROI(float width, float height, float x, float y) : 
	CNormROI (width, height, x, y)
{
	m_cursorOver= OVER_NONE;
	m_showOrientation= false;
	m_degradation= 0;
}

CVisibleNormROI::~CVisibleNormROI(void)
{
}

//
// Locking support. Locks on parent
//
inline void CVisibleNormROI::Lock()
{
	if (m_pParentROI)
		((CVisibleNormROI *) m_pParentROI)->Lock();
	else
		m_CriticalSection.Enter();
}

inline void CVisibleNormROI::Unlock()
{
	if (m_pParentROI)
		((CVisibleNormROI *) m_pParentROI)->Unlock();
	else
		m_CriticalSection.Leave();	
}
	


void CVisibleNormROI::SetP1Resize (const float x, const float y)
{
	Lock();

	CNormROI::SetP1Resize (x, y);
	
	Unlock();
}

void CVisibleNormROI::SetP1Move (const float x, const float y)
{
	Lock();

	CNormROI::SetP1Move (x, y);
	
	Unlock();
}

void CVisibleNormROI::SetP2Resize (const float x, const float y)
{
	Lock();

	CNormROI::SetP2Resize (x, y);
	
	Unlock();
}

void CVisibleNormROI::SetCenter (const float x, const float y)
{
	Lock();

	CNormROI::SetCenter (x, y);
	
	Unlock();
}

void CVisibleNormROI::GetCenter (float& x, float& y)
{
	Lock();

	CNormROI::GetCenter (x, y);
	
	Unlock();
}

void CVisibleNormROI::SetSize (const float width, const float height)
{
	Lock();

	CNormROI::SetSize (width, height);
	
	Unlock();
}

void CVisibleNormROI::GetSize (float& width, float& height)
{
	Lock();

	CNormROI::GetSize (width, height);
	
	Unlock();
}

// Overriden IPL methods to support locking
void CVisibleNormROI::SetP1ResizeImg (const CIplImage *pImg, const int x, const int y)
{
	Lock();

	CNormROI::SetP1ResizeImg (pImg, x, y);

	Unlock();
}

void CVisibleNormROI::SetP1MoveImg (const CIplImage *pImg, const int x, const int y)
{
	Lock();

	CNormROI::SetP1MoveImg (pImg, x, y);

	Unlock();
}

void CVisibleNormROI::SetP2ResizeImg (const CIplImage *pImg, const int x, const int y)
{
	Lock();

	CNormROI::SetP2ResizeImg (pImg, x, y);

	Unlock();
}

void CVisibleNormROI::SetCenterImg (const CIplImage *pImg, const int x, const int y)
{
	Lock();

	CNormROI::SetCenterImg (pImg, x, y);

	Unlock();
}

void CVisibleNormROI::GetCenterImg (const CIplImage *pImg, int& x, int& y)
{
	Lock();

	CNormROI::GetCenterImg (pImg, x, y);

	Unlock();
}

void CVisibleNormROI::SetSizeImg (const CIplImage *pImg, const int width, const int height)
{
	Lock();

	CNormROI::SetSizeImg (pImg, width, height);

	Unlock();
}

void CVisibleNormROI::GetBoxImg (const CIplImage *pImg, int& x, int& y, int& width, int& height)
{
	Lock();

	CNormROI::GetBoxImg (pImg, x, y, width, height);

	Unlock();
}

void CVisibleNormROI::GetBoxImg (const CIplImage *pImg, CvRect& box)
{
	Lock();

	CNormROI::GetBoxImg (pImg, box);

	Unlock();
}

bool CVisibleNormROI::RegisterChildROI (CVisibleNormROI* pNormROI) 
{ 
	// Ensures that child is really a CVisibleNormROI object 
	// (perhaps somewhat paranoic hehe)
	if (!dynamic_cast<CVisibleNormROI *>(pNormROI))
	{
		assert (false);
		return false;
	}

	// Lock child
	pNormROI->m_CriticalSection.Enter();

	// Lock this
	Lock();

	bool retval= CNormROI::RegisterChildROI (pNormROI);

	// Unlock child
	pNormROI->m_CriticalSection.Leave();

	// Unlock this
	Unlock();

	return retval;
}

bool CVisibleNormROI::UnregisterChildROI (CVisibleNormROI* pNormROI)
{
	Lock();

	bool retval= CNormROI::UnregisterChildROI (pNormROI);

	Unlock();

	return retval;
}


//
// Interaction code
//
void CVisibleNormROI::GetDirectionSegment (const CvSize& winSize, CvPoint& p1, CvPoint& p2)
{
	GetP1P2Integer (winSize, p1, p2);
	float line_lenght= sqrtf (
		powf((float)(p2.x - p1.x), 2.0f) + 
		powf((float)(p2.y - p1.y), 2.0f)) * 1.1f / 2.0f;
	
	float cx= (float)(p1.x + p2.x) / 2.0f;
	float cy= (float)(p1.y + p2.y) / 2.0f;

	p1.x= (int) (cx + 0.5f); 
	p1.y= (int) (cy + 0.5f);
	p2.x= (int) (cx + (line_lenght) * cos (GetRotation()) + 0.5f);
	p2.y= (int) (cy - (line_lenght) * sin (GetRotation()) + 0.5f);
}

// Cursor motion, no click
int CVisibleNormROI::OnMouseMovedNoClick (const CvSize& winSize, const CvPoint& cursor)
{
	Lock ();

	int retval= OnMouseMovedNoClickRec (winSize, cursor);

	Unlock();

	return retval;
}

int CVisibleNormROI::OnMouseMovedNoClickRec (const CvSize& winSize, const CvPoint& cursor)
{
	int retval;
	TNormROIListIterator i;

	retval= OnMouseMovedNoClick0 (winSize, cursor);
	if (retval!= OVER_NONE) return retval;

	for(i= m_childROIs.begin(); i != m_childROIs.end(); ++i)
	{
		CVisibleNormROI* pChild= dynamic_cast<CVisibleNormROI *>(*i);
		assert (pChild);
		retval= pChild->OnMouseMovedNoClickRec (winSize, cursor);
		if (retval!= OVER_NONE) return retval;
	}	
	return OVER_NONE;
}

int CVisibleNormROI::OnMouseMovedNoClick0 (const CvSize& winSize, const CvPoint& cursor)
{
	CvPoint p1, p2, or_p1, or_p2;
	
	GetDirectionSegment (winSize, or_p1, or_p2);
	GetP1P2Integer (winSize, p1, p2);
		
	// Motion only. Used to update mouse pointer.
	m_cursorOver= OVER_NONE;

	if (m_showOrientation && cursor.x>= or_p2.x - SELECTION_TOLERANCE && cursor.x<= or_p2.x + SELECTION_TOLERANCE &&
		cursor.y>= or_p2.y - SELECTION_TOLERANCE && cursor.y<= or_p2.y + SELECTION_TOLERANCE)
	{
		m_cursorOver= OVER_ORIENTER;
	}
	else if (cursor.x>= p1.x - SELECTION_TOLERANCE && cursor.x<= p1.x + SELECTION_TOLERANCE &&
			 cursor.y>= p1.y - SELECTION_TOLERANCE && cursor.y<= p1.y + SELECTION_TOLERANCE)
	{
		m_cursorOver= OVER_UL_CORNER;
	}
	else if (cursor.x>= p2.x - SELECTION_TOLERANCE && cursor.x<= p2.x + SELECTION_TOLERANCE &&
			 cursor.y>= p2.y - SELECTION_TOLERANCE && cursor.y<= p2.y + SELECTION_TOLERANCE)
	{	
		m_cursorOver= OVER_BR_CORNER;
	}
	else
	{
		if (cursor.y>= p1.y - SELECTION_TOLERANCE && cursor.y<= p2.y + SELECTION_TOLERANCE)
		{
			// Cursor in Y range, check is cursor over vertical lines
			if (cursor.x>= p1.x - SELECTION_TOLERANCE && cursor.x<= p1.x + SELECTION_TOLERANCE) 
				m_cursorOver= OVER_LEFT_LINE;
			else if (cursor.x>= p2.x - SELECTION_TOLERANCE && cursor.x<= p2.x + SELECTION_TOLERANCE)
				m_cursorOver= OVER_RIGHT_LINE;
		}
		if (cursor.x>= p1.x - SELECTION_TOLERANCE && cursor.x<= p2.x + SELECTION_TOLERANCE)
		{
			// Cursor in X range, check is cursor over horizontal lines
			if (cursor.y>=  p1.y - SELECTION_TOLERANCE && cursor.y<=  p1.y + SELECTION_TOLERANCE) 
				m_cursorOver= OVER_UPPER_LINE;
			else if (cursor.y>= p2.y - SELECTION_TOLERANCE && cursor.y<= p2.y + SELECTION_TOLERANCE)
				m_cursorOver= OVER_BOTTOM_LINE;
		}
	}	

	return m_cursorOver;
}

// Cursor motion, left button down
bool CVisibleNormROI::OnMouseMovedClick (const CvSize& winSize, const CvPoint& cursor, const CvPoint& prevCursor)
{
	Lock ();

	bool retval= OnMouseMovedClickRec (winSize, cursor, prevCursor);

	Unlock();

	return retval;
}

bool CVisibleNormROI::OnMouseMovedClickRec (const CvSize& winSize, const CvPoint& cursor, const CvPoint& prevCursor)
{
	TNormROIListIterator i;

	if (OnMouseMovedClick0 (winSize, cursor, prevCursor)) return true;

	for(i= m_childROIs.begin(); i != m_childROIs.end(); ++i)
	{
		CVisibleNormROI* pChild= dynamic_cast<CVisibleNormROI *>(*i);
		assert (pChild);
		if (pChild->OnMouseMovedClickRec (winSize, cursor, prevCursor)) return true;		
	}	
	return false;
}

bool CVisibleNormROI::OnMouseMovedClick0 (const CvSize& winSize, const CvPoint& cursor, const CvPoint& prevCursor)
{
	CvPoint p1, p2, or_p1, or_p2;
	
	GetDirectionSegment (winSize, or_p1, or_p2);
	GetP1P2Integer (winSize, p1, p2);
		
	if (m_cursorOver> OVER_NONE)
	{		
		switch (m_cursorOver)
		{
			case OVER_LEFT_LINE: 
				p1.x+= cursor.x - prevCursor.x; 				
				SetP1ResizeInteger (winSize, p1.x, p1.y);
				break;
			case OVER_BOTTOM_LINE: 
				p2.y+= cursor.y - prevCursor.y;
				SetP2ResizeInteger (winSize, p2.x, p2.y);
				break;
			case OVER_RIGHT_LINE: 
				p2.x+= cursor.x - prevCursor.x;
				SetP2ResizeInteger (winSize, p2.x, p2.y);
				break;			
			case OVER_UPPER_LINE: 
				p1.y+= cursor.y - prevCursor.y; 				
				SetP1ResizeInteger (winSize, p1.x, p1.y);
				break;
			case OVER_ORIENTER:				
				or_p2.x= cursor.x;
				or_p2.y= cursor.y;
				SetRotation (atan2f((float) (or_p1.y - or_p2.y), (float) (or_p2.x - or_p1.x)));
				break;
			case OVER_UL_CORNER:
			case OVER_BR_CORNER:
				p1.x+= cursor.x - prevCursor.x;
				p1.y+= cursor.y - prevCursor.y;			
				SetP1MoveInteger (winSize, p1.x, p1.y);
				break;
		}
		return true;
	}
	return false;
}

// Painting
void CVisibleNormROI::OnPaint (const CvSize& winSize, CIplImage *pImg)
{
	Lock ();
	OnPaintRec (winSize, pImg);
	Unlock();
}

void CVisibleNormROI::OnPaintRec (const CvSize& winSize, CIplImage *pImg)
{		
	TNormROIListIterator i;

	for(i= m_childROIs.begin(); i != m_childROIs.end(); ++i)
	{
		CVisibleNormROI* pChild= dynamic_cast<CVisibleNormROI *>(*i);
		assert (pChild);
		pChild->OnPaintRec (winSize, pImg);
	}	
	OnPaint0 (winSize, pImg);
}

void CVisibleNormROI::OnPaint0 (const CvSize& winSize, CIplImage *pImg)
{
	int thickness;
	
	CvPoint p1, p2;
	GetP1P2Integer (winSize, p1, p2);

	if (m_cursorOver>= OVER_LEFT_LINE && m_cursorOver<= OVER_BR_CORNER)  thickness= 3;
	else thickness= 1;
	cvRectangle (pImg->ptr(), p1, p2, CV_RGB( 255-m_degradation, 255-m_degradation, m_degradation ), thickness, 4);

	// Affordances
	CvPoint pa, pb;
	pa.x= p1.x - thickness;
	pa.y= p1.y - thickness;
	pb.x= p1.x + thickness;
	pb.y= p1.y + thickness;

	cvRectangle (pImg->ptr(), pa, pb, CV_RGB( 255-m_degradation, 255-m_degradation, m_degradation ), CV_FILLED );

	pa.x= p2.x - thickness;
	pa.y= p2.y - thickness;
	pb.x= p2.x + thickness;
	pb.y= p2.y + thickness;

	cvRectangle (pImg->ptr(), pa, pb, CV_RGB( 255-m_degradation, 255-m_degradation, m_degradation ), CV_FILLED );

	GetDirectionSegment (winSize, p1, p2);
	
	if (m_showOrientation)
	{
		if (m_cursorOver== OVER_ORIENTER)  thickness= 3;
		else thickness= 1;
		cvLine (pImg->ptr(), p1, p2, CV_RGB( 255,255,0 ), thickness, CV_AA );
		cvCircle(pImg->ptr(), p2, SELECTION_TOLERANCE, CV_RGB( 255,255,0 ), thickness, CV_AA );
	}
}
