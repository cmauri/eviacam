/////////////////////////////////////////////////////////////////////////////
// Name:        clickwindow.cpp
// Purpose:  
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by: 
// Created:     
// Copyright:   (C) 2008-15 Cesar Mauri Loba - CREA Software Systems
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
	m_pController= NULL;
}

bool CClickWindow::Create(wxWindow* parent, const wxString& name)
{
	SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
	SetParent(parent);
	CreateControls(name);

	SetSticky(true);
	SetEntryInTaskBar(false);
	SetEntryInPager(false);
	SetAcceptFocus(false);

	if (GetSizer())
	{
		GetSizer()->SetSizeHints(this);
	}
	return true;
}

/*!
 * CClickWindow destructor
 */

CClickWindow::~CClickWindow()
{
}

/*
 * Connect events to button
 */
void CClickWindow::ConnectButtonEvents(wxControl* c) {
	int id = c->GetId();

	c->Connect(id, wxEVT_LEFT_DOWN, wxMouseEventHandler(CClickWindow::OnEventSkipperMouse), NULL, this);
	c->Connect(id, wxEVT_LEFT_UP, wxMouseEventHandler(CClickWindow::OnLeftUp), NULL, this);
	c->Connect(id, wxEVT_RIGHT_DOWN, wxMouseEventHandler(CClickWindow::OnEventSkipperMouse), NULL, this);
	c->Connect(id, wxEVT_RIGHT_UP, wxMouseEventHandler(CClickWindow::OnLeftUp), NULL, this);
	c->Connect(id, wxEVT_LEFT_DCLICK, wxMouseEventHandler(CClickWindow::OnEventSkipperMouse), NULL, this);
	c->Connect(id, wxEVT_ENTER_WINDOW, wxMouseEventHandler(CClickWindow::OnEnterWindow), NULL, this);
	c->Connect(id, wxEVT_SET_FOCUS, wxFocusEventHandler(CClickWindow::OnEventSkipperFocus), NULL, this);
}

/*!
 * Control creation for CClickWindow
 */

void CClickWindow::ConnectEvents() {
	ConnectButtonEvents(GetNoClickButton());
	ConnectButtonEvents(GetLeftButton());
	ConnectButtonEvents(GetMiddleButton());
	ConnectButtonEvents(GetRightButton());
	ConnectButtonEvents(GetDragButton());
	ConnectButtonEvents(GetDblClickButton());
	ConnectButtonEvents(GetShowFrame());
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
	return CClickWindowController::NO_CLICK;	// Avoid compilation warning
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
