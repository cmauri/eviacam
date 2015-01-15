/////////////////////////////////////////////////////////////////////////////
// Name:        clickwindow.cpp
// Purpose:  
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by: 
// Created:     
// Copyright:   (C) 2008-09 Cesar Mauri Loba - CREA Software Systems
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
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wxappbar.h"
#include "clickwindow.h"
#include "clickwindowcontroller.h"

/*!
 * CClickWindow type definition
 */

//IMPLEMENT_DYNAMIC_CLASS( CClickWindow, WXAppBar )


/*!
 * CClickWindow event table definition
 */

BEGIN_EVENT_TABLE( CClickWindow, WXAppBar )

	EVT_CLOSE( CClickWindow::OnCloseWindow )

END_EVENT_TABLE()

/*!
 * CClickWindow constructors
 */

CClickWindow::CClickWindow()
{
    Init();
}

CClickWindow::CClickWindow( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
	Init();
	Create(parent, id, caption, pos, size, style);
}


/*!
 * CClickWindow creator
 */

bool CClickWindow::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
	SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
	bool retval= WXAppBar::Create( parent, id, caption, pos, size, style );
	//SetSticky(true);
	SetEntryInTaskBar (false);
	SetEntryInPager (false);
	SetAcceptFocus (false);
	return retval;
}


/*!
 * CClickWindow destructor
 */

CClickWindow::~CClickWindow()
{
}


/*!
 * Member initialisation
 */

void CClickWindow::Init()
{
	m_pController= NULL;
}


/*!
 * Control creation for CClickWindow
 */

void CClickWindow::ConnectEvents()
{  
    GetNoClickButton()->Connect(GetNoClickButton()->GetId(), wxEVT_LEFT_DOWN, wxMouseEventHandler(CClickWindow::OnEventSkipperMouse), NULL, this);
    GetNoClickButton()->Connect(GetNoClickButton()->GetId(), wxEVT_LEFT_UP, wxMouseEventHandler(CClickWindow::OnLeftUp), NULL, this);
    GetNoClickButton()->Connect(GetNoClickButton()->GetId(), wxEVT_LEFT_DCLICK, wxMouseEventHandler(CClickWindow::OnEventSkipperMouse), NULL, this);
    GetNoClickButton()->Connect(GetNoClickButton()->GetId(), wxEVT_ENTER_WINDOW, wxMouseEventHandler(CClickWindow::OnEnterWindow), NULL, this);
    GetNoClickButton()->Connect(GetNoClickButton()->GetId(), wxEVT_SET_FOCUS, wxFocusEventHandler(CClickWindow::OnEventSkipperFocus), NULL, this);
    GetLeftButton()->Connect(GetLeftButton()->GetId(), wxEVT_LEFT_DOWN, wxMouseEventHandler(CClickWindow::OnEventSkipperMouse), NULL, this);
    GetLeftButton()->Connect(GetLeftButton()->GetId(), wxEVT_LEFT_UP, wxMouseEventHandler(CClickWindow::OnLeftUp), NULL, this);
    GetLeftButton()->Connect(GetLeftButton()->GetId(), wxEVT_LEFT_DCLICK, wxMouseEventHandler(CClickWindow::OnEventSkipperMouse), NULL, this);
    GetLeftButton()->Connect(GetLeftButton()->GetId(), wxEVT_ENTER_WINDOW, wxMouseEventHandler(CClickWindow::OnEnterWindow), NULL, this);
    GetLeftButton()->Connect(GetLeftButton()->GetId(), wxEVT_SET_FOCUS, wxFocusEventHandler(CClickWindow::OnEventSkipperFocus), NULL, this);
   GetMiddleButton()->Connect(GetMiddleButton()->GetId(), wxEVT_LEFT_DOWN, wxMouseEventHandler(CClickWindow::OnEventSkipperMouse), NULL, this);
    GetMiddleButton()->Connect(GetMiddleButton()->GetId(), wxEVT_LEFT_UP, wxMouseEventHandler(CClickWindow::OnLeftUp), NULL, this);
    GetMiddleButton()->Connect(GetMiddleButton()->GetId(), wxEVT_LEFT_DCLICK, wxMouseEventHandler(CClickWindow::OnEventSkipperMouse), NULL, this);
    GetMiddleButton()->Connect(GetMiddleButton()->GetId(), wxEVT_ENTER_WINDOW, wxMouseEventHandler(CClickWindow::OnEnterWindow), NULL, this);
    GetMiddleButton()->Connect(GetMiddleButton()->GetId(), wxEVT_SET_FOCUS, wxFocusEventHandler(CClickWindow::OnEventSkipperFocus), NULL, this);
    GetRightButton()->Connect(GetRightButton()->GetId(), wxEVT_LEFT_DOWN, wxMouseEventHandler(CClickWindow::OnEventSkipperMouse), NULL, this);
    GetRightButton()->Connect(GetRightButton()->GetId(), wxEVT_LEFT_UP, wxMouseEventHandler(CClickWindow::OnLeftUp), NULL, this);
    GetRightButton()->Connect(GetRightButton()->GetId(), wxEVT_LEFT_DCLICK, wxMouseEventHandler(CClickWindow::OnEventSkipperMouse), NULL, this);
    GetRightButton()->Connect(GetRightButton()->GetId(), wxEVT_ENTER_WINDOW, wxMouseEventHandler(CClickWindow::OnEnterWindow), NULL, this);
    GetRightButton()->Connect(GetRightButton()->GetId(), wxEVT_SET_FOCUS, wxFocusEventHandler(CClickWindow::OnEventSkipperFocus), NULL, this);
    GetDragButton()->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(CClickWindow::OnEventSkipperMouse), NULL, this);
    GetDragButton()->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(CClickWindow::OnLeftUp), NULL, this);
    GetDragButton()->Connect(wxEVT_LEFT_DCLICK, wxMouseEventHandler(CClickWindow::OnEventSkipperMouse), NULL, this);
    GetDragButton()->Connect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(CClickWindow::OnEnterWindow), NULL, this);
    GetDragButton()->Connect(wxEVT_SET_FOCUS, wxFocusEventHandler(CClickWindow::OnEventSkipperFocus), NULL, this);
    GetDblClickButton()->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler(CClickWindow::OnEventSkipperMouse), NULL, this);
    GetDblClickButton()->Connect( wxEVT_LEFT_UP, wxMouseEventHandler(CClickWindow::OnLeftUp), NULL, this);
    GetDblClickButton()->Connect( wxEVT_LEFT_DCLICK, wxMouseEventHandler(CClickWindow::OnEventSkipperMouse), NULL, this);
    GetDblClickButton()->Connect( wxEVT_ENTER_WINDOW, wxMouseEventHandler(CClickWindow::OnEnterWindow), NULL, this);
    GetDblClickButton()->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler(CClickWindow::OnEventSkipperFocus), NULL, this);
    GetShowFrame()->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler(CClickWindow::OnEventSkipperMouse), NULL, this);
    GetShowFrame()->Connect( wxEVT_LEFT_UP, wxMouseEventHandler(CClickWindow::OnLeftUp), NULL, this);
    GetShowFrame()->Connect( wxEVT_LEFT_DCLICK, wxMouseEventHandler(CClickWindow::OnEventSkipperMouse), NULL, this);
    GetShowFrame()->Connect( wxEVT_ENTER_WINDOW, wxMouseEventHandler(CClickWindow::OnEnterWindow), NULL, this);
    GetShowFrame()->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler(CClickWindow::OnEventSkipperFocus), NULL, this);
}

CClickWindowController::EButton CClickWindow::ButtonId2EButton (int id)
{
	if (id== GetNoClickButton()->GetId()) return CClickWindowController::NO_CLICK;
    if (id== GetLeftButton()->GetId()) return CClickWindowController::LEFT;
    if (id== GetMiddleButton()->GetId()) return CClickWindowController::MIDDLE;
    if (id== GetRightButton()->GetId()) return CClickWindowController::RIGHT;
    if (id== GetDragButton()->GetId()) return CClickWindowController::DRAG;
    if (id== GetDblClickButton()->GetId()) return CClickWindowController::DBLCLICK;
	
    assert (false);
	return CClickWindowController::NO_CLICK;	// Avoid warning
}

void CClickWindow::OnCloseWindow( wxCloseEvent& event )
{
	bool cVeto= event.CanVeto();
	
	if (cVeto) event.Skip (false);
	else Destroy();
}

void CClickWindow::OnLeftUp( wxMouseEvent& event )
{
	if (event.GetId()== GetShowFrame()->GetId())
		m_pController->NotifyShowMainWindowClick ();
	else
		m_pController->NotifyButtonClick (ButtonId2EButton (event.GetId()));
	
	event.Skip(false);
}

/*!
 * wxEVT_ENTER_WINDOW event handler for ID_BUTTON_NOCLICK
 */

void CClickWindow::OnEnterWindow( wxMouseEvent& event )
{
	if (event.GetId()!= GetShowFrame()->GetId())
		m_pController->NotifyButtonEnter (ButtonId2EButton (event.GetId()));
	event.Skip(false);
}

void CClickWindow::OnMainWindowShow ( wxShowEvent& event )
{
	event.Skip(false);
}

void CClickWindow::OnEventSkipperMouse( wxMouseEvent& event )
{
	event.Skip(false);
}
	
void CClickWindow::OnEventSkipperFocus( wxFocusEvent& event )
{
	event.Skip(false);
}
