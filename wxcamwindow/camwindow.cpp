/////////////////////////////////////////////////////////////////////////////
// Name:        camwindow.cpp
// Purpose:		wxPanel derived class to show live camera image
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by: 
// Created:     
// Copyright:   (C) 2008-2010 Cesar Mauri Loba - CREA Software Systems
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

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#include <wx/debug.h>
#endif

#include <string.h>
#include <cvaux.h>
#include <highgui.h>

#include "camwindow.h"
#include "visiblenormroi.h"
#include "wxnormroi.h"

#define MIN_WIDTH 160
#define MIN_HEIGHT 144
#define VP_ALIGNMENT 4

// New event to comunicate worker and GUI
DECLARE_EVENT_TYPE(wxEVT_MY_REFRESH, -1)
DEFINE_EVENT_TYPE(wxEVT_MY_REFRESH)

// implement message map
BEGIN_EVENT_TABLE(CCamWindow, wxPanel)
	EVT_PAINT (CCamWindow::OnPaint)
	EVT_SIZE (CCamWindow::OnSize) 
	EVT_MOTION (CCamWindow::OnEvtMotion)
	EVT_LEFT_DCLICK (CCamWindow::OnEvtLeftDClick)
	EVT_COMMAND  (wxID_ANY, wxEVT_MY_REFRESH, CCamWindow::OnRecvRefresh)
	
END_EVENT_TABLE()

CCamWindow::CCamWindow()
{
	Init();
}

//CCamWindow::CCamWindow( wxWindow *parent, const wxPoint& pos, const wxSize& size ) :
//	wxPanel(parent, -1, pos, size, wxSIMPLE_BORDER )
CCamWindow::CCamWindow(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size,
					   long style) //, const wxString& name)
{
	Init();
    Create( parent, id, pos, size, style ); //, name );
}

bool CCamWindow::Create(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, 
						long style) //, const wxString& name)
{
	 return wxPanel::Create (parent, id, pos, size, style); //, name);
}

// asserts if registered controls found
CCamWindow::~CCamWindow(void)
{
	assert (m_ControlList.empty());
}

void CCamWindow::Init()
{
	// Get canvas width/height
	m_nImgWidth= 1;
	m_nImgHeight= 1;

	m_ImageShowed= true;
	m_AccessingImage= false;

	m_SharedImage.Create (1, 1);
	m_DisplayImage.Create (1, 1);	

	m_prevCursor.x= 0;
	m_prevCursor.y= 0;
}

// If parent is a top level window resizes it's client area
void CCamWindow::ResizeParentClientArea(int width, int height)
{	
	assert ((width % VP_ALIGNMENT)== 0);

	// TODO: parent should set size automatically changing own size
	// as a workdaround size is propagated to parent if is top level window
	wxWindow* pParent= GetParent();
	if (pParent && pParent->IsTopLevel()) pParent->SetClientSize (width, height);		
}

// OnSize
void CCamWindow::OnSize (wxSizeEvent& event)
{
	bool changed= false;

	int width = event.GetSize().GetWidth();
	int height = event.GetSize().GetHeight();
	
//	printf ("OnSize - Before. W: %d, H: %d\n", width, height);
	// New requested size should be aligned to 4 or 8 bytes.
	// So test it and force the alignment when necessary
	if (width % VP_ALIGNMENT) 
	{
		width= width + VP_ALIGNMENT - (width % VP_ALIGNMENT);
		changed= true;
	}
	
	if (width< MIN_WIDTH) { width= MIN_WIDTH; changed= true; }
	if (height< MIN_HEIGHT) { height= MIN_HEIGHT; changed= true; }
		
	if (changed) {
		SetSize (width, height);
		//ResizeParentClientArea(width, height);
	}	
	event.Skip (true);	
}

// DrawCam. Called from the worker thread
void CCamWindow::DrawCam (IplImage* pImg)
{	
	int convertFlags= 0;

	// If last image not shown yet don't update
	assert (pImg);
	if (m_ImageShowed && pImg)
	{
		m_ImageCopyMutex.Enter();

		if (m_AccessingImage) 
		{
			// GUI thread is processing image, don't update
			m_ImageCopyMutex.Leave();
			return;
		}
		else
		{
			// Exclusive access to shared image
			m_AccessingImage= true;		
		}
		m_ImageCopyMutex.Leave();
		
		// Check that image is RGB with channel order RGB or BGR
		wxASSERT_MSG 
			(pImg->nChannels== 3 &&
			((pImg->channelSeq[0]== 'R' && pImg->channelSeq[1]== 'G' && pImg->channelSeq[2]== 'B') ||
			(pImg->channelSeq[0]== 'B' && pImg->channelSeq[1]== 'G' && pImg->channelSeq[2]== 'R')),
			_T("Wrong image format. It should be RGB or BGR") );
		
		//
		// Adapt image format to show on the screen
		//			
		
		// Allocate shared image if size changed	
		if (pImg->width!= m_SharedImage.Width() || pImg->height!= m_SharedImage.Height())
			m_SharedImage.Create (pImg->width, pImg->height, pImg->depth, "RGB", pImg->origin, pImg->align);
		
		
		assert (pImg->origin== 0);
		if (pImg->channelSeq[0]== 'B' && pImg->channelSeq[1]== 'G' && pImg->channelSeq[2]== 'R')
		{
			convertFlags|= CV_CVTIMG_SWAP_RB;
		//	pImg->channelSeq[0]= 'R';
		//	pImg->channelSeq[2]= 'B';
		}
		if (convertFlags) cvConvertImage ( pImg, m_SharedImage.ptr(), convertFlags );
		else cvCopy( pImg, m_SharedImage.ptr() );
		m_ImageShowed= false;
		
		// Release exclusive access to image
		m_AccessingImage= false;

		// When calling a GUI function from a thread different than 
		// the main one (this method is usually called from a worker thread)
		// synchonization is needed (under GTK+ is mandatory, for example).
		// but it seems that under Windows no GUI Mutex is needed. Futhermore,
		// wxMutexGuiEnter() blocks when pull down a menu from the menu bar
		// For more info check WX source thread sample 
		
		// As wxPostEvent is the recommended synchronization mechanism we'll use it.
		// Hug! wxPostEvent also blocks if main thread is processing an event :-(		
		// This problem has been reported as being caused on gtk by the call to
		// wxWakeUpIdle() inside wxPostEvent. See, for instance:
		// http://osdir.com/ml/lib.wxwindows.general/2003-10/msg00026.html
		// In theory if the macro __WXGTK20__ is defined this must not happen.
		// Since version 2.9.0 there is another funtion to post events "wxQueueEvent"
		// That should be investigated
		wxCommandEvent event(wxEVT_MY_REFRESH);
		wxPostEvent(this, event);		
	}	
}

// OnPaint. Called on paint event
void CCamWindow::OnPaint (wxPaintEvent& event)
{
	event.Skip();	// Avoid compilation warning. This is the defaul behavior

	// Create and check DC 
	// Note that In a paint event handler, the application must always create a wxPaintDC object, 
	// even if you do not use it. Otherwise, under MS Windows, refreshing for this and other 
	// windows will go wrong.
	wxPaintDC dc(this);
	if(!dc.Ok()) return;

	// TODO: does nothing under Linux KDE	
	if (!IsShown()) return;

	// Not implemented for 2.6?
	//if (!IsShownOnScreen()) return;	

	// If current image already shown wait for the next frame.
	// Note this is also necessary to avoid program crash due a void 
	// shared image during the initialization process
	if (m_ImageShowed) return;

	// Exclusive access to shared image
	m_ImageCopyMutex.Enter();
	if (m_AccessingImage) 
	{
		// Worker thread is processing image, don't update
		m_ImageCopyMutex.Leave();
		return;
	}
	else
	{
		// Exclusive access to shared image
		m_AccessingImage= true;	
	}
	m_ImageCopyMutex.Leave();

	// Image size changed
	if (m_nImgWidth!= m_SharedImage.Width() || m_nImgHeight!= m_SharedImage.Height())
	{
		m_nImgWidth= m_SharedImage.Width();
		m_nImgHeight= m_SharedImage.Height();
		ResizeParentClientArea(m_nImgWidth, m_nImgHeight);
		//NotifyResizeParent();
	}

	// Allocate DisplayImage when needed
	int vpWidth, vpHeight;
	GetSize(&vpWidth, &vpHeight);
	if (vpWidth % 4) vpWidth= vpWidth + 4 - (vpWidth % 4);
	if (vpWidth!= m_DisplayImage.Width() || vpHeight!= m_DisplayImage.Height())
	{
		// Allocate shared image if size changed
		m_DisplayImage.Create (vpWidth, vpHeight, m_SharedImage.Depth(), "RGB", m_SharedImage.Origin(), m_SharedImage.Align());		
	}

	// Scale image
	cvResize( m_SharedImage.ptr(), m_DisplayImage.ptr(), CV_INTER_NN );
	
   	// Working with shared image finished
	m_AccessingImage= false;

	// Draw controls
	m_ListMutex.Enter();
	TWXNormROIListIterator i;
	for(i= m_ControlList.begin(); i != m_ControlList.end(); ++i)
	{
		(*i).OnPaint (&m_DisplayImage);
	}
	m_ListMutex.Leave();

	// To wxWidgets
	unsigned char *rawData;
	CvSize roiSize;
	int step = 0;
	cvGetRawData( m_DisplayImage.ptr(), &rawData, &step, &roiSize );
	
	wxImage wxImg= wxImage(vpWidth, vpHeight, rawData, true );
	
	// convert to bitmap to be used by the window to draw
	m_Bitmap= wxBitmap( wxImg );
		
	wxCoord x, y, width, height;
	dc.GetClippingBox( &x, &y, &width, &height );
	dc.DrawBitmap( m_Bitmap, x, y );
	
	m_ImageShowed= true;
}



//CCamWindow::TWXNormROIListIterator CCamWindow::FindControl (CVisibleNormROI* pVisibleNormROI)
bool CCamWindow::FindControl (CVisibleNormROI* pVisibleNormROI, CCamWindow::TWXNormROIListIterator& it)
{
//	TWXNormROIListIterator it;

	for(it= m_ControlList.begin(); it != m_ControlList.end(); ++it)
	{
		if ((*it).GetVisibleNormROI() == pVisibleNormROI) return true;
	}

	//return NULL;
	return false;
}

bool CCamWindow::RegisterControl (CVisibleNormROI* pVisibleNormROI)
{	
	assert (pVisibleNormROI);
		
	m_ListMutex.Enter();

	TWXNormROIListIterator it;	

	if (FindControl (pVisibleNormROI, it)) 
	{
		// Control already registered
		m_ListMutex.Leave();
		return false;
	}

	m_ControlList.push_back (CWXNormROI(pVisibleNormROI, this));

	m_ListMutex.Leave();

	return true;
}

bool CCamWindow::UnregisterControl (CVisibleNormROI* pVisibleNormROI)
{
	assert (pVisibleNormROI);
		
	m_ListMutex.Enter();

	TWXNormROIListIterator it; //= FindControl (pVisibleNormROI);

	if (!FindControl (pVisibleNormROI, it)) 
	{
		// Control not found
		m_ListMutex.Leave();
		return false;
	}
	
	m_ControlList.erase (it);

	m_ListMutex.Leave();
	
	return true;
}

void CCamWindow::OnEvtMotion ( wxMouseEvent& event )
{
	TWXNormROIListIterator i;

	m_ListMutex.Enter();

	for(i= m_ControlList.begin(); i != m_ControlList.end(); ++i)
	{
		if ((*i).OnEvtMotion (event, m_prevCursor)) break;
	}

	m_ListMutex.Leave();

	m_prevCursor= event.GetPosition ();

	event.Skip();
}

void CCamWindow::OnEvtLeftDClick ( wxMouseEvent& event )
{
	TWXNormROIListIterator i;

	m_ListMutex.Enter();

	for(i= m_ControlList.begin(); i != m_ControlList.end(); ++i)
	{
		if ((*i).OnEvtLeftDClick (event)) break;
	}
	m_ListMutex.Leave();

	event.Skip();
}

void CCamWindow::OnRecvRefresh( wxCommandEvent& WXUNUSED(event) )
{
	Refresh(false);		
}
