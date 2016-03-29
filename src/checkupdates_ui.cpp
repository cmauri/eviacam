/////////////////////////////////////////////////////////////////////////////
// Name:        checkupdates_gui.cpp
// Purpose:     Check updates dialog
// Author:      César Mauri Loba
// Created:     12/10/2012 20:15:41
// Copyright:   (C) 2008-14 Cesar Mauri from CREA Sistemes Informatics
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

#include "checkupdates_ui.h"
#include "checkupdates.h"
#include "config.h"
#include "website.h"

////@begin XPM images
////@end XPM images

using namespace eviacam;

/*!
 * CheckUpdatesUI type definition
 */

IMPLEMENT_DYNAMIC_CLASS( CheckUpdatesUI, wxDialog )


/*!
 * CheckUpdatesUI event table definition
 */

BEGIN_EVENT_TABLE( CheckUpdatesUI, wxDialog )

////@begin CheckUpdatesUI event table entries
    EVT_HYPERLINK( ID_HYPERLINKCTRL_WEBSITE, CheckUpdatesUI::OnHyperlinkctrlWebsiteHyperlinkClicked )
    EVT_BUTTON( ID_BUTTON_CHECKUPDATE_CLOSE, CheckUpdatesUI::OnButtonCheckupdateCloseClick )
////@end CheckUpdatesUI event table entries

	EVT_TIMER( -1, CheckUpdatesUI::OnTimer )
END_EVENT_TABLE()


/*!
 * CheckUpdatesUI constructors
 */

CheckUpdatesUI::CheckUpdatesUI()
{
    Init();
}

CheckUpdatesUI::CheckUpdatesUI( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*!
 * Checkforupdates creator
 */

bool CheckUpdatesUI::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin CheckUpdatesUI creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end CheckUpdatesUI creation

	

    return true;
}


/*!
 * CheckUpdatesUI destructor
 */

CheckUpdatesUI::~CheckUpdatesUI()
{
////@begin CheckUpdatesUI destruction
////@end CheckUpdatesUI destruction
}


/*!
 * Member initialisation
 */

void CheckUpdatesUI::Init()
{
////@begin CheckUpdatesUI member initialisation
    m_msg1 = NULL;
    m_msg2 = NULL;
    m_link = NULL;
////@end CheckUpdatesUI member initialisation
}


/*!
 * Control creation for Checkforupdates
 */

void CheckUpdatesUI::CreateControls()
{    
////@begin CheckUpdatesUI content construction
    CheckUpdatesUI* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    itemBoxSizer3->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer3->Add(itemBoxSizer5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_msg1 = new wxStaticText( itemDialog1, wxID_STATIC, _("Checking for updates. Please wait."), wxDefaultPosition, wxDefaultSize, 0 );
    m_msg1->SetFont(wxFont(int(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT).GetPointSize()*1.2), wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT).GetFamily(), wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT).GetStyle(), wxFONTWEIGHT_BOLD, false, wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT).GetFaceName()));
    itemBoxSizer5->Add(m_msg1, 0, wxALIGN_LEFT|wxALL, 5);

    m_msg2 = new wxStaticText( itemDialog1, wxID_STATIC, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    m_msg2->SetFont(wxFont(int(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT).GetPointSize()*1.2), wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT).GetFamily(), wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT).GetStyle(), wxFONTWEIGHT_BOLD, false, wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT).GetFaceName()));
    itemBoxSizer5->Add(m_msg2, 0, wxALIGN_LEFT|wxALL, 5);

    m_link = new wxHyperlinkCtrl( itemDialog1, ID_HYPERLINKCTRL_WEBSITE, _("Visit Enable Viacam website"), wxEmptyString, wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
    m_link->SetFont(wxFont(int(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT).GetPointSize()*1.2), wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT).GetFamily(), wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT).GetStyle(), wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT).GetWeight(), true, wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT).GetFaceName()));
    m_link->Show(false);
    itemBoxSizer5->Add(m_link, 0, wxALIGN_LEFT|wxALL, 5);

    itemBoxSizer3->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton10 = new wxButton( itemDialog1, ID_BUTTON_CHECKUPDATE_CLOSE, _("&Close"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer2->Add(itemButton10, 0, wxALIGN_RIGHT|wxALL, 5);

////@end CheckUpdatesUI content construction
}


/*!
 * Should we show tooltips?
 */

bool CheckUpdatesUI::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap CheckUpdatesUI::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin CheckUpdatesUI bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end CheckUpdatesUI bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon CheckUpdatesUI::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin CheckUpdatesUI icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end CheckUpdatesUI icon retrieval
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_CHECKUPDATE_CLOSE
 */

void CheckUpdatesUI::OnButtonCheckupdateCloseClick( wxCommandEvent& event )
{
	EndModal(wxCANCEL);
    event.Skip(false);
}


/*!
 * wxEVT_COMMAND_HYPERLINK event handler for ID_HYPERLINKCTRL_WEBSITE
 */

void CheckUpdatesUI::OnHyperlinkctrlWebsiteHyperlinkClicked( wxHyperlinkEvent& event )
{
	eviacam::OpenEViacamWebsiteOnBrowser();
    event.Skip(false);
}

void CheckUpdatesUI::StartProgress()
{
	// Start timer
	m_timer.SetOwner(this);
	m_timer.Start(200);
}

void CheckUpdatesUI::StopProgress()
{
	// Stop timer
	m_timer.Stop();
}

void CheckUpdatesUI::SetResults(const wxString& txt1, const wxString& txt2, bool showLink)
{
	m_msg1->SetLabel(txt1);
	m_msg2->SetLabel(txt2);
	m_link->Show(showLink);

	// Fit content
	if (GetSizer())	{
		GetSizer()->SetSizeHints(this);
	}
	Centre();
}

void CheckUpdatesUI::OnTimer(wxTimerEvent& event)
{
	// Update progress indicator
	wxString txt= m_msg2->GetLabel();

	if (txt.Len()> 15) txt= _T(".");
	else txt+=  _T(".");

	m_msg2->SetLabel (txt);

	event.Skip(false);
}

