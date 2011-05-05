/////////////////////////////////////////////////////////////////////////////
// Name:        cclickwindowbitmap.cpp
// Purpose:     
// Author:      César Mauri Loba
// Modified by: 
// Created:     16/11/2009 15:10:47
// RCS-ID:      
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

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "clickwindowbitmap.h"

////@begin XPM images
#include "icons/eviacam.xpm"
#include "icons/buttonNoClick.xpm"
#include "icons/buttonLeft.xpm"
#include "icons/buttonLeftDisabled.xpm"
#include "icons/buttonMiddle.xpm"
#include "icons/buttonMiddleDisabled.xpm"
#include "icons/buttonRight.xpm"
#include "icons/buttonRightDisabled.xpm"
#include "icons/buttonDrag.xpm"
#include "icons/buttonDragDisabled.xpm"
#include "icons/buttonDblLeft.xpm"
#include "icons/buttonDblLeftDisabled.xpm"
#include "icons/buttonHideMainWindow.xpm"
////@end XPM images

#include "icons/buttonDblLeftLocked.xpm"
#include "icons/buttonDblLeftSelectedLocked.xpm"
#include "icons/buttonDblLeftSelected.xpm"
#include "icons/buttonDragLocked.xpm"
#include "icons/buttonDragSelectedLocked.xpm"
#include "icons/buttonDragSelected.xpm"
#include "icons/buttonLeftLocked.xpm"
#include "icons/buttonLeftSelectedLocked.xpm"
#include "icons/buttonLeftSelected.xpm"
#include "icons/buttonNoClickSelected.xpm"
#include "icons/buttonMiddleLocked.xpm"
#include "icons/buttonMiddleSelectedLocked.xpm"
#include "icons/buttonMiddleSelected.xpm"
#include "icons/buttonRightLocked.xpm"
#include "icons/buttonRightSelectedLocked.xpm"
#include "icons/buttonRightSelected.xpm"
#include "icons/buttonShowMainWindow.xpm"


/*!
 * CClickWindowBitmap type definition
 */

IMPLEMENT_DYNAMIC_CLASS( CClickWindowBitmap, CClickWindow )


/*!
 * CClickWindowBitmap event table definition
 */

BEGIN_EVENT_TABLE( CClickWindowBitmap, CClickWindow )

////@begin CClickWindowBitmap event table entries
////@end CClickWindowBitmap event table entries

END_EVENT_TABLE()


/*!
 * CClickWindowBitmap constructors
 */

CClickWindowBitmap::CClickWindowBitmap()
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
    Init();
}

CClickWindowBitmap::CClickWindowBitmap( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
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
    Init();
    Create(parent, id, caption, pos, size, style);
}

/*!
 * CClickWindowBitmap creator
 */

bool CClickWindowBitmap::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin CClickWindowBitmap creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    CClickWindow::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end CClickWindowBitmap creation
    return true;
}


/*!
 * CClickWindowBitmap destructor
 */

CClickWindowBitmap::~CClickWindowBitmap()
{
////@begin CClickWindowBitmap destruction
////@end CClickWindowBitmap destruction
}


/*!
 * Member initialisation
 */

void CClickWindowBitmap::Init()
{
////@begin CClickWindowBitmap member initialisation
    m_btnNoClick = NULL;
    m_btnLeft = NULL;
    m_btnMiddle = NULL;
    m_btnRight = NULL;
    m_btnDrag = NULL;
    m_btnDblLeft = NULL;
    m_btnShowFrame = NULL;
////@end CClickWindowBitmap member initialisation
}


/*!
 * Control creation for CClickWindowBitmap
 */

void CClickWindowBitmap::CreateControls()
{    
////@begin CClickWindowBitmap content construction
    CClickWindowBitmap* itemCClickWindow1 = this;

    this->SetBackgroundColour(wxColour(102, 102, 102));
    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    itemCClickWindow1->SetSizer(itemBoxSizer2);

    wxStaticBitmap* itemStaticBitmap3 = new wxStaticBitmap( itemCClickWindow1, wxID_STATIC, itemCClickWindow1->GetBitmapResource(wxT("icons/eviacam.xpm")), wxDefaultPosition, wxSize(32, 32), 0 );
    itemBoxSizer2->Add(itemStaticBitmap3, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemBoxSizer2->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_btnNoClick = new wxBitmapButton( itemCClickWindow1, ID_BITMAPBUTTON_NOCLICK, itemCClickWindow1->GetBitmapResource(wxT("icons/buttonNoClick.xpm")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|wxNO_BORDER );
    if (CClickWindowBitmap::ShowToolTips())
        m_btnNoClick->SetToolTip(_("No click"));
    itemBoxSizer2->Add(m_btnNoClick, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    itemBoxSizer2->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_btnLeft = new wxBitmapButton( itemCClickWindow1, ID_BITMAPBUTTON_LEFT, itemCClickWindow1->GetBitmapResource(wxT("icons/buttonLeft.xpm")), wxDefaultPosition, wxDefaultSize, wxNO_BORDER );
    wxBitmap m_btnLeftBitmapDisabled(itemCClickWindow1->GetBitmapResource(wxT("icons/buttonLeftDisabled.xpm")));
    m_btnLeft->SetBitmapDisabled(m_btnLeftBitmapDisabled);
    if (CClickWindowBitmap::ShowToolTips())
        m_btnLeft->SetToolTip(_("Left"));
    itemBoxSizer2->Add(m_btnLeft, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    m_btnMiddle = new wxBitmapButton( itemCClickWindow1, ID_BITMAPBUTTON_MIDDLE, itemCClickWindow1->GetBitmapResource(wxT("icons/buttonMiddle.xpm")), wxDefaultPosition, wxDefaultSize, wxNO_BORDER );
    wxBitmap m_btnMiddleBitmapDisabled(itemCClickWindow1->GetBitmapResource(wxT("icons/buttonMiddleDisabled.xpm")));
    m_btnMiddle->SetBitmapDisabled(m_btnMiddleBitmapDisabled);
    if (CClickWindowBitmap::ShowToolTips())
        m_btnMiddle->SetToolTip(_("Middle"));
    itemBoxSizer2->Add(m_btnMiddle, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    m_btnRight = new wxBitmapButton( itemCClickWindow1, ID_BITMAPBUTTON_RIGHT, itemCClickWindow1->GetBitmapResource(wxT("icons/buttonRight.xpm")), wxDefaultPosition, wxDefaultSize, wxNO_BORDER );
    wxBitmap m_btnRightBitmapDisabled(itemCClickWindow1->GetBitmapResource(wxT("icons/buttonRightDisabled.xpm")));
    m_btnRight->SetBitmapDisabled(m_btnRightBitmapDisabled);
    if (CClickWindowBitmap::ShowToolTips())
        m_btnRight->SetToolTip(_("Right"));
    itemBoxSizer2->Add(m_btnRight, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    m_btnDrag = new wxBitmapButton( itemCClickWindow1, ID_BITMAPBUTTON_DRAG, itemCClickWindow1->GetBitmapResource(wxT("icons/buttonDrag.xpm")), wxDefaultPosition, wxDefaultSize, wxNO_BORDER );
    wxBitmap m_btnDragBitmapDisabled(itemCClickWindow1->GetBitmapResource(wxT("icons/buttonDragDisabled.xpm")));
    m_btnDrag->SetBitmapDisabled(m_btnDragBitmapDisabled);
    if (CClickWindowBitmap::ShowToolTips())
        m_btnDrag->SetToolTip(_("Drag"));
    itemBoxSizer2->Add(m_btnDrag, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    m_btnDblLeft = new wxBitmapButton( itemCClickWindow1, ID_BITMAPBUTTON_DBLCLICK, itemCClickWindow1->GetBitmapResource(wxT("icons/buttonDblLeft.xpm")), wxDefaultPosition, wxDefaultSize, wxNO_BORDER );
    wxBitmap m_btnDblLeftBitmapDisabled(itemCClickWindow1->GetBitmapResource(wxT("icons/buttonDblLeftDisabled.xpm")));
    m_btnDblLeft->SetBitmapDisabled(m_btnDblLeftBitmapDisabled);
    if (CClickWindowBitmap::ShowToolTips())
        m_btnDblLeft->SetToolTip(_("Double Click"));
    itemBoxSizer2->Add(m_btnDblLeft, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    itemBoxSizer2->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_btnShowFrame = new wxBitmapButton( itemCClickWindow1, ID_BITMAPBUTTON, itemCClickWindow1->GetBitmapResource(wxT("icons/buttonHideMainWindow.xpm")), wxDefaultPosition, wxDefaultSize, wxNO_BORDER );
    if (CClickWindowBitmap::ShowToolTips())
        m_btnShowFrame->SetToolTip(_("Hide main window"));
    itemBoxSizer2->Add(m_btnShowFrame, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    // Connect events and objects
    m_btnNoClick->Connect(ID_BITMAPBUTTON_NOCLICK, wxEVT_ENTER_WINDOW, wxMouseEventHandler(CClickWindowBitmap::OnEnterWindow), NULL, this);
////@end CClickWindowBitmap content construction

	ConnectEvents ();
}


/*!
 * Should we show tooltips?
 */

bool CClickWindowBitmap::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap CClickWindowBitmap::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin CClickWindowBitmap bitmap retrieval
    wxUnusedVar(name);
    if (name == _T("icons/eviacam.xpm"))
    {
        wxBitmap bitmap(eviacam);
        return bitmap;
    }
    else if (name == _T("icons/buttonNoClick.xpm"))
    {
        wxBitmap bitmap(buttonNoClick);
        return bitmap;
    }
    else if (name == _T("icons/buttonLeft.xpm"))
    {
        wxBitmap bitmap(buttonLeft);
        return bitmap;
    }
    else if (name == _T("icons/buttonLeftDisabled.xpm"))
    {
        wxBitmap bitmap(buttonLeftDisabled);
        return bitmap;
    }
    else if (name == _T("icons/buttonMiddle.xpm"))
    {
        wxBitmap bitmap(buttonMiddle);
        return bitmap;
    }
    else if (name == _T("icons/buttonMiddleDisabled.xpm"))
    {
        wxBitmap bitmap(buttonMiddleDisabled);
        return bitmap;
    }
    else if (name == _T("icons/buttonRight.xpm"))
    {
        wxBitmap bitmap(buttonRight);
        return bitmap;
    }
    else if (name == _T("icons/buttonRightDisabled.xpm"))
    {
        wxBitmap bitmap(buttonRightDisabled);
        return bitmap;
    }
    else if (name == _T("icons/buttonDrag.xpm"))
    {
        wxBitmap bitmap(buttonDrag);
        return bitmap;
    }
    else if (name == _T("icons/buttonDragDisabled.xpm"))
    {
        wxBitmap bitmap(buttonDragDisabled);
        return bitmap;
    }
    else if (name == _T("icons/buttonDblLeft.xpm"))
    {
        wxBitmap bitmap(buttonDblLeft);
        return bitmap;
    }
    else if (name == _T("icons/buttonDblLeftDisabled.xpm"))
    {
        wxBitmap bitmap(buttonDblLeftDisabled);
        return bitmap;
    }
    else if (name == _T("icons/buttonHideMainWindow.xpm"))
    {
        wxBitmap bitmap(buttonHideMainWindow);
        return bitmap;
    }
    return wxNullBitmap;
////@end CClickWindowBitmap bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon CClickWindowBitmap::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin CClickWindowBitmap icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end CClickWindowBitmap icon retrieval
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

void CClickWindowBitmap::UpdateButtons (bool noClickStatus, CClickWindowController::EButton selected, CClickWindowController::EButton locked)
{
	// No click button
	
	if (noClickStatus)
	{
		m_btnNoClick->SetBitmapLabel (m_bmpButtonNoClick);
		m_btnLeft->Enable();
		m_btnMiddle->Enable();
		m_btnRight->Enable();
		m_btnDrag->Enable();
		m_btnDblLeft->Enable();
	}
	else
	{
		m_btnNoClick->SetBitmapLabel (m_bmpButtonNoClickSelected);
		m_btnLeft->Disable();
		m_btnMiddle->Disable();
		m_btnRight->Disable();
		m_btnDrag->Disable();
		m_btnDblLeft->Disable();
	}
		

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
	if (event.GetShow())	
	{
		m_btnShowFrame->SetBitmapLabel(m_bmpButtonHideMainWindow);
		m_btnShowFrame->SetToolTip(_("Hide main window"));
	}
	else
	{
		m_btnShowFrame->SetBitmapLabel(m_bmpButtonShowMainWindow);
		m_btnShowFrame->SetToolTip(_("Show main window"));
	}
	// Set to true to propagate events to the text mode clickwindow
	event.Skip (true);
}


/*!
 * wxEVT_ENTER_WINDOW event handler for ID_BITMAPBUTTON_NOCLICK
 */

void CClickWindowBitmap::OnEnterWindow( wxMouseEvent& event )
{
	event.Skip(true);
}

