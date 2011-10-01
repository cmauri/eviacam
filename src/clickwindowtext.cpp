/////////////////////////////////////////////////////////////////////////////
// Name:        clickwindowtext.cpp
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

//#include "wxappbar.h"
#include "clickwindowtext.h"
#include "clickwindowcontroller.h"


////@begin includes
////@end includes

////@begin XPM images
#include "icons/eviacam_mini.xpm"
////@end XPM images

/*!
 * CClickWindowText type definition
 */

IMPLEMENT_DYNAMIC_CLASS( CClickWindowText, CClickWindow )


/*!
 * CClickWindowText event table definition
 */

BEGIN_EVENT_TABLE( CClickWindowText, CClickWindow )

////@begin CClickWindowText event table entries
////@end CClickWindowText event table entries

END_EVENT_TABLE()

/*!
 * CClickWindowText constructors
 */

CClickWindowText::CClickWindowText()
{
    Init();
}

CClickWindowText::CClickWindowText( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
	Init();
	Create(parent, id, caption, pos, size, style);
}


/*!
 * CClickWindow creator
 */

bool CClickWindowText::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin CClickWindowText creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    CClickWindow::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
////@end CClickWindowText creation
    return true;
}


/*!
 * CClickWindowText destructor
 */

CClickWindowText::~CClickWindowText()
{
////@begin CClickWindowText destruction
////@end CClickWindowText destruction
}


/*!
 * Member initialisation
 */

void CClickWindowText::Init()
{
////@begin CClickWindowText member initialisation
    m_noClickButton = NULL;
    m_leftButton = NULL;
    m_middleButton = NULL;
    m_rightButton = NULL;
    m_dragButton = NULL;
    m_dblClickButton = NULL;
    m_btnShowFrame = NULL;
////@end CClickWindowText member initialisation
}


/*!
 * Control creation for CClickWindow
 */

void CClickWindowText::CreateControls()
{  
////@begin CClickWindowText content construction
    CClickWindowText* itemCClickWindow1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    itemCClickWindow1->SetSizer(itemBoxSizer2);

    m_noClickButton = new wxToggleButton( itemCClickWindow1, ID_BUTTON_NOCLICK, _("No click"), wxDefaultPosition, wxDefaultSize, 0 );
    m_noClickButton->SetValue(false);
    itemBoxSizer2->Add(m_noClickButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    wxStaticBitmap* itemStaticBitmap4 = new wxStaticBitmap( itemCClickWindow1, wxID_STATIC, itemCClickWindow1->GetBitmapResource(wxT("icons/eviacam_mini.xpm")), wxDefaultPosition, wxSize(16, 16), 0 );
    itemBoxSizer2->Add(itemStaticBitmap4, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    m_leftButton = new wxToggleButton( itemCClickWindow1, ID_BTN_LEFT, _("#Left#"), wxDefaultPosition, wxDefaultSize, 0 );
    m_leftButton->SetValue(false);
    itemBoxSizer2->Add(m_leftButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    m_middleButton = new wxToggleButton( itemCClickWindow1, ID_BTN_MIDDLE, _("#Middle#"), wxDefaultPosition, wxDefaultSize, 0 );
    m_middleButton->SetValue(false);
    itemBoxSizer2->Add(m_middleButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    m_rightButton = new wxToggleButton( itemCClickWindow1, ID_BTN_RIGHT, _("#Right#"), wxDefaultPosition, wxDefaultSize, 0 );
    m_rightButton->SetValue(false);
    itemBoxSizer2->Add(m_rightButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    m_dragButton = new wxToggleButton( itemCClickWindow1, ID_BTN_DRAG, _("#Drag#"), wxDefaultPosition, wxDefaultSize, 0 );
    m_dragButton->SetValue(false);
    itemBoxSizer2->Add(m_dragButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    m_dblClickButton = new wxToggleButton( itemCClickWindow1, ID_BTN_DBLCLICK, _("#Double Click#"), wxDefaultPosition, wxDefaultSize, 0 );
    m_dblClickButton->SetValue(false);
    itemBoxSizer2->Add(m_dblClickButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

    m_btnShowFrame = new wxButton( itemCClickWindow1, ID_BUTTON_SHOW_FRAME, _("Hide main window"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer2->Add(m_btnShowFrame, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

////@end CClickWindowText content construction

	ConnectEvents();
}

/*!
 * Should we show tooltips?
 */

bool CClickWindowText::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap CClickWindowText::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin CClickWindowText bitmap retrieval
    wxUnusedVar(name);
    if (name == _T("icons/eviacam_mini.xpm"))
    {
        wxBitmap bitmap(eviacam_mini);
        return bitmap;
    }
    return wxNullBitmap;
////@end CClickWindowText bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon CClickWindowText::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin CClickWindowText icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end CClickWindowText icon retrieval
}

void CClickWindowText::UpdateButtons (bool noClickStatus, CClickWindowController::EButton selected, CClickWindowController::EButton locked)
{
	// Update no click button
	m_noClickButton->SetValue (!noClickStatus);
	
	// Update selected button (foreground color)
	switch (selected)
	{
	case CClickWindowController::LEFT:
		UpdateSelectedButtons (GetLeftButton(), GetMiddleButton(), GetRightButton(), GetDragButton(), GetDblClickButton());
		break;
	case CClickWindowController::MIDDLE:
		UpdateSelectedButtons (GetMiddleButton(), GetRightButton(), GetDragButton(), GetDblClickButton(), GetLeftButton());
		break;
	case CClickWindowController::RIGHT:
		UpdateSelectedButtons (GetRightButton(), GetDragButton(), GetDblClickButton(), GetLeftButton(), GetMiddleButton());
		break;
	case CClickWindowController::DRAG:
		UpdateSelectedButtons (GetDragButton(), GetDblClickButton(), GetLeftButton(), GetMiddleButton(), GetRightButton());
		break;
	case CClickWindowController::DBLCLICK:
		UpdateSelectedButtons (GetDblClickButton(), GetLeftButton(), GetMiddleButton(), GetRightButton(), GetDragButton());
		break;
	default:
		assert (false);
	}

	// Update locked button (toggled)
	switch (locked)
	{
	case CClickWindowController::LEFT:
		UpdateLockedButtons (GetLeftButton(), GetMiddleButton(), GetRightButton(), GetDragButton(), GetDblClickButton());
		break;
	case CClickWindowController::MIDDLE:
		UpdateLockedButtons (GetMiddleButton(), GetRightButton(), GetDragButton(), GetDblClickButton(), GetLeftButton());
		break;
	case CClickWindowController::RIGHT:
		UpdateLockedButtons (GetRightButton(), GetDragButton(), GetDblClickButton(), GetLeftButton(), GetMiddleButton());
		break;
	case CClickWindowController::DRAG:
		UpdateLockedButtons (GetDragButton(), GetDblClickButton(), GetLeftButton(), GetMiddleButton(), GetRightButton());
		break;
	case CClickWindowController::DBLCLICK:
		UpdateLockedButtons (GetDblClickButton(), GetLeftButton(), GetMiddleButton(), GetRightButton(), GetDragButton());
		break;
	default:
		assert (false);
	}
}

void CClickWindowText::UpdateSelectedButtons (wxControl* btnSelected, wxControl* btnNoSelected1, 
								 wxControl* btnNoSelected2,wxControl* btnNoSelected3,wxControl* btnNoSelected4)
{
	if (!((wxToggleButton*)btnSelected)->GetValue()) ((wxToggleButton*)btnSelected)->SetValue (true);
	if (((wxToggleButton*)btnNoSelected1)->GetValue()) ((wxToggleButton*)btnNoSelected1)->SetValue (false);
	if (((wxToggleButton*)btnNoSelected2)->GetValue()) ((wxToggleButton*)btnNoSelected2)->SetValue (false);
	if (((wxToggleButton*)btnNoSelected3)->GetValue())((wxToggleButton*) btnNoSelected3)->SetValue (false);
	if (((wxToggleButton*)btnNoSelected4)->GetValue())((wxToggleButton*) btnNoSelected4)->SetValue (false);
	
}

void CClickWindowText::UpdateLockedButtons (wxControl* btnLocked, wxControl* btnNoLocked1, 
											 wxControl* btnNoLocked2, wxControl* btnNoLocked3, wxControl* btnNoLocked4)
{
	wxString label= ((wxToggleButton*)btnLocked)->GetLabel();
	label.SetChar (0, wxChar('['));
	label.SetChar (label.Length()-1, wxChar(']'));
	((wxToggleButton*)btnLocked)->SetLabel(label);

	label= ((wxToggleButton*)btnNoLocked1)->GetLabel();
	label.SetChar (0, wxChar(' '));
	label.SetChar (label.Length()-1, wxChar(' '));
	((wxToggleButton*)btnNoLocked1)->SetLabel(label);

	label= ((wxToggleButton*)btnNoLocked2)->GetLabel();
	label.SetChar (0, wxChar(' '));
	label.SetChar (label.Length()-1, wxChar(' '));
	((wxToggleButton*)btnNoLocked2)->SetLabel(label);

	label= ((wxToggleButton*)btnNoLocked3)->GetLabel();
	label.SetChar (0, wxChar(' '));
	label.SetChar (label.Length()-1, wxChar(' '));
	((wxToggleButton*)btnNoLocked3)->SetLabel(label);

	label= ((wxToggleButton*)btnNoLocked4)->GetLabel();
	label.SetChar (0, wxChar(' '));
	label.SetChar (label.Length()-1, wxChar(' '));
	((wxToggleButton*)btnNoLocked4)->SetLabel(label);
}

wxControl* CClickWindowText::GetNoClickButton()
{
	return m_noClickButton;
}

wxControl* CClickWindowText::GetLeftButton()
{
	return m_leftButton;
}

wxControl* CClickWindowText::GetMiddleButton()
{
	return m_middleButton;
}

wxControl* CClickWindowText::GetRightButton()
{
	return m_rightButton;
}

wxControl* CClickWindowText::GetDragButton()
{
	return m_dragButton;
}

wxControl* CClickWindowText::GetDblClickButton()
{
	return m_dblClickButton;
}

wxControl* CClickWindowText::GetShowFrame()
{
	return m_btnShowFrame;
}
	
void CClickWindowText::OnMainWindowShow ( wxShowEvent& event )
{
	if (event.GetShow())
		m_btnShowFrame->SetLabel(_("Hide main window"));
	else
		m_btnShowFrame->SetLabel(_("Show main window"));		
	event.Skip (false);
}
