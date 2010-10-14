/////////////////////////////////////////////////////////////////////////////
// Name:        capturethread.cpp
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

#ifdef __GNUG__
#pragma implementation
#pragma interface
#endif

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
#endif //precompiled headers

#include <cv.h>

#include "capturethread.h"
#include "crvcamera.h"
#include "camwindow.h"

// Constructor
CCaptureThread::CCaptureThread (CCamera *pCamera, CCamWindow *pCamWindow, 
						CProcessImage *pProcessImage, wxThreadKind kind) : wxThread (kind)
{
	assert (pCamera!= NULL);
	m_pCamera= pCamera;
	m_pCamWindow= pCamWindow;
	m_pProcessImage= pProcessImage;
	m_Life = true;
}

// Destructor
CCaptureThread::~CCaptureThread ()
{
}

wxThreadError CCaptureThread::Create(unsigned int stackSize)
{
	if (m_pCamera== NULL) return wxTHREAD_NO_RESOURCE;
	//if (!m_pCamera->Open()) return wxTHREAD_NO_RESOURCE;

	return wxThread::Create (stackSize);
}

wxThread::ExitCode CCaptureThread::Entry( )
{
	// Camera shold be opened inside calling thread
	if (!m_pCamera->Open())
	{
		// This code never should be reached, but it may happen: there is a race condition between
		// testing camera availabiliy and effectively using it.
		wxMessageDialog errorMsg (NULL, _("Camera not detected. Aborting"), _("eViacam error"), wxOK | wxICON_ERROR);
		errorMsg.ShowModal();
		exit (0);
	}

	// Start thread main loop
	while (m_Life)
	{
		if (TestDestroy()) break;
		
		IplImage* pImg= m_pCamera->QueryFrame();

		if (pImg== NULL) wxMilliSleep(20); // If no result wait some time to avoid consuming all CPU
		else {
			if (m_pProcessImage) m_pProcessImage->ProcessImage (pImg);
			if (m_pCamWindow) m_pCamWindow->DrawCam (pImg);	
		}
	}

	m_pCamera->Close();
	
	return NULL;
}
