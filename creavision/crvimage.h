/////////////////////////////////////////////////////////////////////////////
// Name:        crvimage.h
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
#ifndef CRVIMAGE_H
#define CRVIMAGE_H

#include <assert.h>
#include <cv.h>

class CIplImage
{
public:

  // Methods
	// Construction
	CIplImage ();
	CIplImage (IplImage *pImg);
	CIplImage (	int width, int height, int depth= IPL_DEPTH_8U,  
				const char *pColorOrder= "GRAY");
	~CIplImage ();

	// Creation/Initialization
	bool Create (int width, int height, 
				unsigned int depth= IPL_DEPTH_8U, const char *pColorOrder= "GRAY", 
				int origin= IPL_ORIGIN_TL, int align= IPL_ALIGN_QWORD );

	bool Import (IplImage *pImage);	// Imported images are not automatically released
	
	// Returns the internal pointer, which is set to NULL, and forgets 
	// about its deallocation
	IplImage* Detach();
	void Free ();
	void Swap (CIplImage *pOtherImg);
	void Reset ();
	

	// ROI
	bool SetROI (int x, int y, int width, 
				 int height, unsigned int coi= 0);
	bool SetROI (IplROI &roi) {
		return SetROI (	roi.xOffset, roi.yOffset, 
						roi.width, roi.height, roi.coi); }
	bool SetROI (CvRect &rect) {
		return SetROI (	rect.x, rect.y, rect.width, rect.height); }
	void GetROI (IplROI &roi);
	void ResetROI ();

	void PushROI ();
	void PopROI ();

	// Data accessors
	bool Initialized () const { return m_pIplImage!= NULL; }
	IplImage *ptr () { return m_pIplImage; }
	const IplImage *ptr () const { return m_pIplImage; }
	int Width () const { assert (m_pIplImage); return m_pIplImage->width; }
	int Height () const { assert (m_pIplImage); return m_pIplImage->height; }
	CvSize GetSize() const { assert (m_pIplImage); return cvSize (m_pIplImage->width, m_pIplImage->height); }
	// 0 - top-left origin, 1 - bottom-left origin (Windows bitmaps style) 
	int Origin () const {	assert (m_pIplImage); return m_pIplImage->origin; }
	int Depth () const { assert (m_pIplImage); return m_pIplImage->depth; }
	int Align() const { assert (m_pIplImage); return m_pIplImage->align; }
	
	// IO
	bool Load (char *pFilename);
	bool Save (char *pFilename);
	void Show (char *id);

private:
	enum { ROI_STACK_SIZE= 10 };

  // Attributes
	IplImage *m_pIplImage;
	bool m_importedImage;
	IplROI *m_importedROI;
	IplROI m_roiStack[ROI_STACK_SIZE];
	int m_roiStackPtr;

  // Private methods
	void InitROIStack (int width, int height);
	void Init(); 
};

#endif	// IPLIMAGE_H
