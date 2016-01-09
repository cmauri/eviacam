/////////////////////////////////////////////////////////////////////////////
// Name:        cclickwindowbitmap.cpp
// Purpose:     
// Author:      Cesar Mauri Loba
// Modified by: 
// Created:     16/11/2009 15:10:47
// RCS-ID:      
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

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif


#include "clickwindowbitmap.h"

#include "resources/buttonNoClick.xpm"
#include "resources/buttonLeft.xpm"
#include "resources/buttonLeftDisabled.xpm"
#include "resources/buttonMiddle.xpm"
#include "resources/buttonMiddleDisabled.xpm"
#include "resources/buttonRight.xpm"
#include "resources/buttonRightDisabled.xpm"
#include "resources/buttonDrag.xpm"
#include "resources/buttonDragDisabled.xpm"
#include "resources/buttonDblLeft.xpm"
#include "resources/buttonDblLeftDisabled.xpm"
#include "resources/buttonHideMainWindow.xpm"
#include "resources/buttonDblLeftLocked.xpm"
#include "resources/buttonDblLeftSelectedLocked.xpm"
#include "resources/buttonDblLeftSelected.xpm"
#include "resources/buttonDragLocked.xpm"
#include "resources/buttonDragSelectedLocked.xpm"
#include "resources/buttonDragSelected.xpm"
#include "resources/buttonLeftLocked.xpm"
#include "resources/buttonLeftSelectedLocked.xpm"
#include "resources/buttonLeftSelected.xpm"
#include "resources/buttonNoClickSelected.xpm"
#include "resources/buttonMiddleLocked.xpm"
#include "resources/buttonMiddleSelectedLocked.xpm"
#include "resources/buttonMiddleSelected.xpm"
#include "resources/buttonRightLocked.xpm"
#include "resources/buttonRightSelectedLocked.xpm"
#include "resources/buttonRightSelected.xpm"
#include "resources/buttonShowMainWindow.xpm"


/*!
 * CClickWindowBitmap constructors
 */

CClickWindowBitmap::CClickWindowBitmap( wxWindow* parent, const wxString& name)
: m_bmpButtonDblLeftLocked(buttonDblLeftLocked)
, m_bmpButtonDblLeftSelectedLocked(buttonDblLeftSelectedLocked)
, m_bmpButtonDblLeftSelected(buttonDblLeftSelected)
, m_bmpButtonDblLeft(buttonDblLeft)
, m_bmpButtonDragLocked(buttonDragLocked)
, m_bmpButtonDragSelectedLocked(buttonDragSelectedLocked)
, m_bmpButtonDragSelected(buttonDragSelected)
, m_bmpButtonDrag(buttonDrag)
, m_bmpButtonHideMainWindow(buttonHideMainWindow)
, m_bmpButtonLeftLocked(buttonLeftLocked)
, m_bmpButtonLeftSelectedLocked(buttonLeftSelectedLocked)
, m_bmpButtonLeftSelected(buttonLeftSelected)
, m_bmpButtonLeft(buttonLeft)
, m_bmpButtonNoClickSelected(buttonNoClickSelected)
, m_bmpButtonNoClick(buttonNoClick)
, m_bmpButtonMiddleLocked(buttonMiddleLocked)
, m_bmpButtonMiddleSelectedLocked(buttonMiddleSelectedLocked)
, m_bmpButtonMiddleSelected(buttonMiddleSelected)
, m_bmpButtonMiddle(buttonMiddle)
, m_bmpButtonRightLocked(buttonRightLocked)
, m_bmpButtonRightSelectedLocked(buttonRightSelectedLocked)
, m_bmpButtonRightSelected(buttonRightSelected)
, m_bmpButtonRight(buttonRight)
, m_bmpButtonShowMainWindow(buttonShowMainWindow)
{
	m_btnNoClick = NULL;
	m_btnLeft = NULL;
	m_btnMiddle = NULL;
	m_btnRight = NULL;
	m_btnDrag = NULL;
	m_btnDblLeft = NULL;
	m_btnShowFrame = NULL;

    Create(parent, name);
}

/*!
 * CClickWindowBitmap destructor
 */

CClickWindowBitmap::~CClickWindowBitmap()
{
}

/*!
 * Control creation for CClickWindowBitmap
 */

void CClickWindowBitmap::CreateControls(const wxString& name)
{    
    if (!wxXmlResource::Get()->LoadDialog(this, GetParent(), name))
        wxLogError(wxT("Missing wxXmlResource::Get()->Load() in OnInit()?"));
    m_btnNoClick = XRCCTRL(*this, "ID_BITMAPBUTTON_NOCLICK", wxBitmapButton);
    m_btnLeft = XRCCTRL(*this, "ID_BITMAPBUTTON_LEFT", wxBitmapButton);
    m_btnMiddle = XRCCTRL(*this, "ID_BITMAPBUTTON_MIDDLE", wxBitmapButton);
    m_btnRight = XRCCTRL(*this, "ID_BITMAPBUTTON_RIGHT", wxBitmapButton);
    m_btnDrag = XRCCTRL(*this, "ID_BITMAPBUTTON_DRAG", wxBitmapButton);
    m_btnDblLeft = XRCCTRL(*this, "ID_BITMAPBUTTON_DBLCLICK", wxBitmapButton);
    m_btnShowFrame = XRCCTRL(*this, "ID_BITMAPBUTTON", wxBitmapButton);

	ConnectEvents ();
}

wxControl* CClickWindowBitmap::GetNoClickButton()
{
	return m_btnNoClick;
}

wxControl* CClickWindowBitmap::GetLeftButton()
{
	return m_btnLeft;
}

wxControl* CClickWindowBitmap::GetMiddleButton()
{
	return m_btnMiddle;
}

wxControl* CClickWindowBitmap::GetRightButton()
{
	return m_btnRight;
}

wxControl* CClickWindowBitmap::GetDragButton()
{
	return m_btnDrag;
}

wxControl* CClickWindowBitmap::GetDblClickButton()
{
	return m_btnDblLeft;
}

wxControl* CClickWindowBitmap::GetShowFrame()
{
	return m_btnShowFrame;
}

void CClickWindowBitmap::UpdateButtons (bool noClickStatus, CClickWindowController::EButton selected,
		CClickWindowController::EButton locked)
{
	// No click button
	
	if (noClickStatus) {
		m_btnNoClick->SetBitmapLabel (m_bmpButtonNoClick);
		m_btnLeft->Enable();
		m_btnMiddle->Enable();
		m_btnRight->Enable();
		m_btnDrag->Enable();
		m_btnDblLeft->Enable();
	}
	else {
		m_btnNoClick->SetBitmapLabel (m_bmpButtonNoClickSelected);
		m_btnLeft->Disable();
		m_btnMiddle->Disable();
		m_btnRight->Disable();
		m_btnDrag->Disable();
		m_btnDblLeft->Disable();
	}

	/* Update bitmaps */
	if (selected== CClickWindowController::LEFT)
		if (locked== CClickWindowController::LEFT)
			m_btnLeft->SetBitmapLabel (m_bmpButtonLeftSelectedLocked);
		else
			m_btnLeft->SetBitmapLabel (m_bmpButtonLeftSelected);
	else
		if (locked== CClickWindowController::LEFT)
			m_btnLeft->SetBitmapLabel (m_bmpButtonLeftLocked);
		else
			m_btnLeft->SetBitmapLabel (m_bmpButtonLeft);
		
	if (selected== CClickWindowController::MIDDLE)
		if (locked== CClickWindowController::MIDDLE)
			m_btnMiddle->SetBitmapLabel (m_bmpButtonMiddleSelectedLocked);
		else
			m_btnMiddle->SetBitmapLabel (m_bmpButtonMiddleSelected);
	else
		if (locked== CClickWindowController::MIDDLE)
			m_btnMiddle->SetBitmapLabel (m_bmpButtonMiddleLocked);
		else
			m_btnMiddle->SetBitmapLabel (m_bmpButtonMiddle);

	if (selected== CClickWindowController::RIGHT)
		if (locked== CClickWindowController::RIGHT)
			m_btnRight->SetBitmapLabel (m_bmpButtonRightSelectedLocked);
		else
			m_btnRight->SetBitmapLabel (m_bmpButtonRightSelected);
	else
		if (locked== CClickWindowController::RIGHT)
			m_btnRight->SetBitmapLabel (m_bmpButtonRightLocked);
		else
			m_btnRight->SetBitmapLabel (m_bmpButtonRight);

	if (selected== CClickWindowController::DRAG)
		if (locked== CClickWindowController::DRAG)
			m_btnDrag->SetBitmapLabel (m_bmpButtonDragSelectedLocked);
		else
			m_btnDrag->SetBitmapLabel (m_bmpButtonDragSelected);
	else
		if (locked== CClickWindowController::DRAG)
			m_btnDrag->SetBitmapLabel (m_bmpButtonDragLocked);
		else
			m_btnDrag->SetBitmapLabel (m_bmpButtonDrag);

	if (selected== CClickWindowController::DBLCLICK)
		if (locked== CClickWindowController::DBLCLICK)
			m_btnDblLeft->SetBitmapLabel (m_bmpButtonDblLeftSelectedLocked);
		else
			m_btnDblLeft->SetBitmapLabel (m_bmpButtonDblLeftSelected);
	else
		if (locked== CClickWindowController::DBLCLICK)
			m_btnDblLeft->SetBitmapLabel (m_bmpButtonDblLeftLocked);
		else
			m_btnDblLeft->SetBitmapLabel (m_bmpButtonDblLeft);
}

void CClickWindowBitmap::OnMainWindowShow ( wxShowEvent& event )
{
	if (event.IsShown()) {
		m_btnShowFrame->SetBitmapLabel(m_bmpButtonHideMainWindow);
		m_btnShowFrame->SetToolTip(_("Hide main window"));
	}
	else {
		m_btnShowFrame->SetBitmapLabel(m_bmpButtonShowMainWindow);
		m_btnShowFrame->SetToolTip(_("Show main window"));
	}
	// Set to true to propagate events to the text mode clickwindow
	event.Skip (true);
}

