/////////////////////////////////////////////////////////////////////////////
// Name:        camwindow.h
// Purpose:		wxPanel derived class to show live camera image
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

#ifndef _CAMWINDOW_H_
#define _CAMWINDOW_H_

// Includes
#include <wx/panel.h>
#include <wx/bitmap.h>
#include <crvimage.h>
#include <list>
using namespace std;

// Forward class declarations
class CVisibleNormROI;
class CWXNormROI;

class CCamWindow : public wxPanel
{     
public:
	//CCamWindow( wxWindow* parent, const wxPoint& pos, const wxSize& size );
	CCamWindow();
	CCamWindow(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxSIMPLE_BORDER); //, const wxString& name = _T("camwindow"));
	
	bool Create(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxSIMPLE_BORDER); //, const wxString& name = _T("camwindow"));
	
	virtual ~CCamWindow(void);

	// Draw method
	void DrawCam (IplImage* pImg);

	// True if all ok, false if control already registered
	bool RegisterControl (CVisibleNormROI* pVisibleNormROI);

	// True if all ok, false if control wasn't registered
	bool UnregisterControl (CVisibleNormROI* pVisibleNormROI);	

private:
	typedef std::list<CWXNormROI> TWXNormROIList;
	typedef TWXNormROIList::iterator TWXNormROIListIterator;

	void OnPaint(wxPaintEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnEvtMotion ( wxMouseEvent& event );
	void OnEvtLeftDClick ( wxMouseEvent& event );
	void OnRecvRefresh( wxCommandEvent &event );
	//TWXNormROIListIterator FindControl (CVisibleNormROI* pVisibleNormROI);
	bool FindControl (CVisibleNormROI* pVisibleNormROI, TWXNormROIListIterator &it);
	
	void ResizeParentClientArea(int width, int height);

	void Init();

protected:
	DECLARE_EVENT_TABLE()

private:
	wxCriticalSection	m_ImageCopyMutex;
	wxCriticalSection	m_ListMutex;
	wxBitmap	m_Bitmap;
	volatile bool	m_ImageShowed;
	volatile bool	m_AccessingImage;
	
	// Previous captured image size
	int		m_nImgWidth;
	int		m_nImgHeight;
	CIplImage m_SharedImage;
	CIplImage m_DisplayImage;	
	TWXNormROIList m_ControlList;
	wxPoint m_prevCursor;
};

#endif
