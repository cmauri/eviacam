/////////////////////////////////////////////////////////////////////////////
// Name:        capturethread.h
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


#ifndef _CAPTURETHREAD_H_
#define _CAPTURETHREAD_H_

#include <wx/thread.h>
//#include <cv.h>
typedef struct _IplImage IplImage;

// Forward class definitions
class CCamera;
class CCamWindow;

// Abstract class for thread callback
class CProcessImage
{
public:
	virtual void ProcessImage (IplImage *pImage)= 0;
	virtual ~CProcessImage() {}
};

class CCaptureThread : public wxThread
{
public:
	CCaptureThread (CCamera *pCamera, CCamWindow *pCamWindow= NULL, 
					CProcessImage *pProcessImage= NULL, wxThreadKind kind = wxTHREAD_DETACHED);
	~CCaptureThread ();

	// Thread entry point
	virtual wxThread::ExitCode Entry();
	virtual wxThreadError Create(unsigned int stackSize = 0);
	void Finish() { m_Life= false; }

private:
	volatile bool m_Life;
	CCamWindow *m_pCamWindow;
	CCamera *m_pCamera;
	CProcessImage *m_pProcessImage;	
};

#endif

