/////////////////////////////////////////////////////////////////////////////
// Name:        crvnormroi.cpp
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
#include "crvnormroi.h"

// Area limits
#define MIN_WIDTH_HEIGHT	(float) 0.041666666666666666666666666666667		// 6 píxels over 144
#define MAX_WIDTH_HEIGHT	1.0f

CNormROI::CNormROI(void)
{
	m_x= 0.0;
	m_y= 0.0;
	m_width= MIN_WIDTH_HEIGHT;
	m_height= MIN_WIDTH_HEIGHT;	
	m_rotation= 0.0;
	m_pParentROI= NULL;
	SetCenter (0.5f, 0.5f);
}

CNormROI::CNormROI(float width, float height, float x, float y)
{
	m_x= 0.0;
	m_y= 0.0;
	m_width= MIN_WIDTH_HEIGHT;
	m_height= MIN_WIDTH_HEIGHT;	
	m_rotation= 0.0;
	m_pParentROI= NULL;

	SetSize (width, height);
	if (x!= -1.0f && y!= -1.0f)	SetCenter (x, y);
	else SetCenter (0.5f, 0.5f);
}

void CNormROI::Destroy()
{
	// Detach itself from parent
	if (m_pParentROI) {
		bool check= m_pParentROI->UnregisterChildROI (this);
		((void)check);	// Remove warning
		assert (check);
		assert (m_pParentROI== NULL);
	}

	TNormROIListIterator i;

	i= m_childROIs.begin();

	while (i!= m_childROIs.end())
	{
		delete (*i);
		//i= m_childROIs.erase (i);
		i= m_childROIs.begin();
	}
	
	assert (m_childROIs.empty());
}

// delete registered childs
CNormROI::~CNormROI(void)
{
	Destroy();
}

//
// Working with native coordinates
//

void CNormROI::CheckInvariant() {
	assert (m_x>= 0.0f && m_x< MAX_WIDTH_HEIGHT);
	assert (m_y>= 0.0f && m_y< MAX_WIDTH_HEIGHT);
	assert (m_width>= MIN_WIDTH_HEIGHT && m_width<= MAX_WIDTH_HEIGHT);
	assert (m_height>= MIN_WIDTH_HEIGHT && m_height<= MAX_WIDTH_HEIGHT);
	assert (m_x + m_width<= MAX_WIDTH_HEIGHT);
	assert (m_y + m_height<= MAX_WIDTH_HEIGHT);
}

// Trick to see a float as int. Only for little-endian machines.
// TODO: seriously, change the internal implementation of this class!
typedef union {
	float f;
	int i32;
} FloatInt32;

#define COMPILE_TIME_ASSERT(pred) switch(0){case 0:case pred:;}

void CNormROI::FitInternalState() {
	// Fix internal state to make sure class invariant is honoured.
	// Invariant may have been violated by imprecisions on FP calculations
	// TODO: replace FP arithmetic with integer arithmetic.
	COMPILE_TIME_ASSERT(sizeof(FloatInt32)== sizeof(float));
	COMPILE_TIME_ASSERT(sizeof(FloatInt32)== sizeof(int));

	if (m_width< MIN_WIDTH_HEIGHT) m_width= MIN_WIDTH_HEIGHT;
	if (m_x + m_width> MAX_WIDTH_HEIGHT) {
		FloatInt32 f2i;
		if (m_width> MIN_WIDTH_HEIGHT) {
			f2i.f= m_width;
			f2i.i32--;
			m_width= f2i.f;
		}
		else {
			f2i.f= m_x;
			f2i.i32--;
			m_x= f2i.f;
		}
	}

	if (m_height< MIN_WIDTH_HEIGHT) m_height= MIN_WIDTH_HEIGHT;
	if (m_y + m_height> MAX_WIDTH_HEIGHT) {
		FloatInt32 f2i;
		if (m_height> MIN_WIDTH_HEIGHT) {
			f2i.f= m_height;
			f2i.i32--;
			m_height= f2i.f;
		}
		else {
			f2i.f= m_y;
			f2i.i32--;
			m_y= f2i.f;
		}
	}
}

// Find minimum child's P1 coordinates
void CNormROI::FindMinChildP1 (float& x, float& y)
{
	TNormROIListIterator i;

	for(i= m_childROIs.begin(); i != m_childROIs.end(); ++i)
	{
		(*i)->FindMinChildP1Rec (x, y);
	}
}

void CNormROI::FindMinChildP1Rec (float& x, float& y)
{
	TNormROIListIterator i;

	for(i= m_childROIs.begin(); i != m_childROIs.end(); ++i)
	{
		(*i)->FindMinChildP1Rec (x, y);
	}

	if (x> m_x) x= m_x;
	if (y> m_y) y= m_y;
}

// Find miaximum child's P2 coordinates
void CNormROI::FindMaxChildP2 (float& x, float& y)
{
	TNormROIListIterator i;

	for(i= m_childROIs.begin(); i != m_childROIs.end(); ++i)
	{
		(*i)->FindMaxChildP2Rec (x, y);
	}
}

void CNormROI::FindMaxChildP2Rec (float& x, float& y)
{
	TNormROIListIterator i;

	for(i= m_childROIs.begin(); i != m_childROIs.end(); ++i)
	{
		(*i)->FindMaxChildP2Rec (x, y);
	}

	if (x< m_x + m_width) x= m_x + m_width;
	if (y< m_y + m_height) y= m_y + m_height;
}

void CNormROI::SetP1Resize (const float x, const float y)
{
	float min_p1x, min_p1y, max_p1x, max_p1y;

	CvPoint2D32f p2_lim;

	p2_lim.x= m_x + m_width;
	p2_lim.y= m_y + m_height;

	if (m_pParentROI)
	{
		min_p1x= m_pParentROI->m_x;
		min_p1y= m_pParentROI->m_y;
	}
	else
	{
		 min_p1x= min_p1y= 0.0f;
	}
	max_p1x= p2_lim.x - MIN_WIDTH_HEIGHT;
	max_p1y= p2_lim.y - MIN_WIDTH_HEIGHT;
	FindMinChildP1 (max_p1x, max_p1y);
	assert (max_p1x>= 0.0f);
	assert (max_p1y>= 0.0f);

	if (x< min_p1x) m_x= min_p1x;
	else if (x> max_p1x) m_x= max_p1x;
	else m_x= x;

	if (y< min_p1y) m_y= min_p1y;
	else if (y> max_p1y) m_y= max_p1y;
	else m_y= y;

	m_width= p2_lim.x - m_x;
	m_height= p2_lim.y - m_y;

	FitInternalState();

	CheckInvariant();
}	
		
void CNormROI::SetP1Move (const float x, const float y)
{
	float min_p1x, min_p1y, max_p1x, max_p1y;
	float min_p2x_lim, min_p2y_lim;

	// Compute minimum valid p1 coordinates
	if (m_pParentROI)
	{
		min_p1x= m_pParentROI->m_x;
		min_p1y= m_pParentROI->m_y;
	}
	else
	{
		 min_p1x= min_p1y= 0.0f;
	}
	min_p2x_lim= min_p1x + m_width;
	min_p2y_lim= min_p1y + m_height;
	FindMaxChildP2 (min_p2x_lim, min_p2y_lim);
		
	min_p1x= min_p2x_lim - m_width;
	min_p1y= min_p2y_lim - m_height;
	if (min_p1x< 0.0f) min_p1x= 0.0f;
	if (min_p1y< 0.0f) min_p1y= 0.0f;

	// Compute maximum valid p1 coordinates
	if (m_pParentROI)
	{
		max_p1x= m_pParentROI->m_x + m_pParentROI->m_width - m_width;
		max_p1y= m_pParentROI->m_y + m_pParentROI->m_height - m_height;
	}
	else
	{
		max_p1x= MAX_WIDTH_HEIGHT - m_width;
		max_p1y= MAX_WIDTH_HEIGHT - m_height;
	}
	FindMinChildP1 (max_p1x, max_p1y);
	assert (max_p1x>= 0.0f);
	assert (max_p1y>= 0.0f);

	// Apply restrictions
	if (x< min_p1x) m_x= min_p1x;
	else if (x> max_p1x) m_x= max_p1x;
	else m_x= x;

	if (y< min_p1y) m_y= min_p1y;
	else if (y> max_p1y) m_y= max_p1y;
	else m_y= y;

	FitInternalState();

	CheckInvariant();
}

void CNormROI::SetP2Resize (const float x, const float y)
{
	// Setting P2 always resize
	float min_p2x, min_p2y, max_p2x, max_p2y;

	min_p2x= m_x + MIN_WIDTH_HEIGHT;
	min_p2y= m_y + MIN_WIDTH_HEIGHT;
	assert (min_p2x<= MAX_WIDTH_HEIGHT);
	assert (min_p2y<= MAX_WIDTH_HEIGHT);
	FindMaxChildP2 (min_p2x, min_p2y);

	if (m_pParentROI)
	{
		max_p2x= m_pParentROI->m_x + m_pParentROI->m_width;
		max_p2y= m_pParentROI->m_y + m_pParentROI->m_height;
	}
	else
	{
		max_p2x= max_p2y= MAX_WIDTH_HEIGHT;
	}

	if (x< min_p2x) m_width= min_p2x - m_x;
	else if (x> max_p2x) m_width= max_p2x - m_x;
	else m_width= x - m_x;

	if (y< min_p2y) m_height= min_p2y - m_y;
	else if (y> max_p2y) m_height= max_p2y - m_y;
	else m_height= y - m_y;

	FitInternalState();

	CheckInvariant();
}

void CNormROI::SetCenter (const float x, const float y)
{
	SetP1Move (x - (m_width / 2.0f), y - (m_height / 2.0f));
}

void CNormROI::GetCenter (float& x, float& y)
{
	x= m_x + (m_width / 2.0f);
	y= m_y + (m_height / 2.0f);
}

void CNormROI::SetSize (const float width, const float height)
{
	SetP2Resize (width + m_x, height + m_y);
}

void CNormROI::GetSize (float& width, float& height)
{
	width= m_width;
	height= m_height;
}

//
// Working with generic integer coordinates
//
inline void CNormROI::Integer2Normalized (const CvSize& size, const int ix, const int iy, float &nx, float &ny)
{
	assert (size.width> 0);
	assert (size.height> 0);
	
	nx= (float) ix / (float) size.width;
	ny= (float) iy / (float) size.height;
}

inline void CNormROI::Normalized2Integer (const CvSize& size, const float nx, const float ny, int &ix, int &iy)
{
	assert (size.width> 0);
	assert (size.height> 0);

	// Round using the float value just below 0.5. We do this way to avoid
	// overflows when both coordinate and size finish with 0.5
	ix= (int) (nx * (float) size.width + 0.4999999701976776f);
	iy= (int) (ny * (float) size.height + 0.4999999701976776f);
}

//inline 
void CNormROI::SetP1ResizeInteger (const CvSize& size, const int x, const int y)
{
	float new_x, new_y;

	Integer2Normalized (size, x, y, new_x, new_y);

	SetP1Resize (new_x, new_y);
}

//inline 
void CNormROI::SetP1MoveInteger (const CvSize& size, const int x, const int y)
{
	float new_x, new_y;

	Integer2Normalized (size, x, y, new_x, new_y);

	SetP1Move (new_x, new_y);
}

//inline 
void CNormROI::SetP2ResizeInteger (const CvSize& size, const int x, const int y)
{
	float new_x, new_y;

	Integer2Normalized (size, x, y, new_x, new_y);

	SetP2Resize (new_x, new_y);
}

//inline 
void CNormROI::SetCenterInteger (const CvSize& size, const int x, const int y)
{
	float new_x, new_y;

	Integer2Normalized (size, x, y, new_x, new_y);

	SetCenter (new_x, new_y);
}

void CNormROI::GetCenterInteger (const CvSize& size, int& x, int& y)
{
	float fx, fy;

	GetCenter (fx, fy);

	Normalized2Integer (size, fx, fy, x, y);	
}

//inline 
void CNormROI::SetSizeInteger (const CvSize& size, const int width, const int height)
{
	float new_width, new_height;

	Integer2Normalized (size, width, height, new_width, new_height);

	SetSize (new_width, new_height);
}

//inline 
void CNormROI::GetBoxInteger (const CvSize& size, int& x, int& y, int& width, int& height)
{
	// DEBUG, 
	CheckInvariant();

	Normalized2Integer (size, m_x, m_y, x, y);
	Normalized2Integer (size, m_width, m_height, width, height);

	assert (x + width<= size.width);
	assert (y + height<= size.height);
}

void CNormROI::GetBoxInteger (const CvSize& size, CvRect& box)
{
	GetBoxInteger (size, box.x, box.y, box.width, box.height);
}

void CNormROI::GetP1P2Integer (const CvSize& size, CvPoint& p1, CvPoint& p2)
{
	Normalized2Integer (size, m_x, m_y, p1.x, p1.y);
	Normalized2Integer (size, m_x + m_width, m_y + m_height, p2.x, p2.y);
}

//
// Facilities to work with CIplImage images
//
void CNormROI::SetP1ResizeImg (const CIplImage *pImg, const int x, const int y)
{
	assert (pImg);
	assert (pImg->Origin()== IPL_ORIGIN_TL);

	SetP1ResizeInteger (pImg->GetSize(), x, y);
}

void CNormROI::SetP1MoveImg (const CIplImage *pImg, const int x, const int y)
{
	assert (pImg);
	assert (pImg->Origin()== IPL_ORIGIN_TL);

	SetP1MoveInteger (pImg->GetSize(), x, y);
}

void CNormROI::SetP2ResizeImg (const CIplImage *pImg, const int x, const int y)
{
	assert (pImg);
	assert (pImg->Origin()== IPL_ORIGIN_TL);

	SetP2ResizeInteger (pImg->GetSize(), x, y);	
}

void CNormROI::SetCenterImg (const CIplImage *pImg, const int x, const int y)
{
	assert (pImg);
	assert (pImg->Origin()== IPL_ORIGIN_TL);
	
	SetCenterInteger (pImg->GetSize(), x, y);
}

void CNormROI::GetCenterImg (const CIplImage *pImg, int& x, int& y)
{
	assert (pImg);
	assert (pImg->Origin()== IPL_ORIGIN_TL);

	GetCenterInteger (pImg->GetSize(), x, y);	
}

void CNormROI::SetSizeImg (const CIplImage *pImg, const int width, const int height)
{
	assert (pImg);
	assert (pImg->Origin()== IPL_ORIGIN_TL);

	SetSizeInteger (pImg->GetSize(), width, height);
}

void CNormROI::GetBoxImg (const CIplImage *pImg, int& x, int& y, int& width, int& height)
{
	assert (pImg);
	assert (pImg->Origin()== IPL_ORIGIN_TL);

	GetBoxInteger (pImg->GetSize(), x, y, width, height);
}

void CNormROI::GetBoxImg (const CIplImage *pImg, CvRect& box)
{
	GetBoxImg (pImg, box.x, box.y, box.width, box.height);
}

//
// Registration service
//
bool CNormROI::RegisterChildROI (CNormROI* pNormROI)
{
	assert (pNormROI);
	assert (pNormROI!= this);
		
	TNormROIListIterator i;

	for(i= m_childROIs.begin(); i != m_childROIs.end(); ++i)
	{
		if ((*i)== pNormROI) return false;	// Already registered
	}

	// Check that no registered with other parent
	if (pNormROI->m_pParentROI) return false;

	m_childROIs.push_back (pNormROI);
	pNormROI->m_pParentROI= this;

	// Ensures that child is inside parent
	if (pNormROI->m_width> m_width) pNormROI->m_width= m_width;
	if (pNormROI->m_height> m_height) pNormROI->m_height= m_height;
	pNormROI->SetP1Move (pNormROI->m_x, pNormROI->m_y);	

	return true;
}

bool CNormROI::UnregisterChildROI (CNormROI* pNormROI)
{
	assert (pNormROI);

	TNormROIListIterator i;

	for(i= m_childROIs.begin(); i != m_childROIs.end(); ++i)
	{
		if ((*i)== pNormROI) break;	// Found!
	}
	if (i== m_childROIs.end()) return false;	// Not found
	
	assert ((*i)->m_pParentROI);
	(*i)->m_pParentROI= NULL;
	m_childROIs.erase (i);

	return true;
}
