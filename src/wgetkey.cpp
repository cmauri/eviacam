/////////////////////////////////////////////////////////////////////////////
// Name:        wgetkey.cpp
// Purpose:     
// Author:      César Mauri Loba
// Modified by: 
// Created:     01/07/2008 16:35:20
// RCS-ID:      
// Copyright:   (C) 2008 Cesar Mauri from CREA Sistemes Informatics
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

#include "wgetkey.h"
#include <wx/timer.h>
#include <wx/defs.h>

////@begin XPM images
////@end XPM images
#define TIMER_ID 1234

/*!
 * WGetKey type definition
 */

IMPLEMENT_DYNAMIC_CLASS( WGetKey, wxDialog )


/*!
 * WGetKey event table definition
 */

BEGIN_EVENT_TABLE( WGetKey, wxDialog )

////@begin WGetKey event table entries
    EVT_LEFT_DOWN( WGetKey::OnLeftDown )

////@end WGetKey event table entries
	EVT_TIMER(TIMER_ID, WGetKey::OnTimer)

END_EVENT_TABLE()


/*!
 * WGetKey constructors
 */

WGetKey::WGetKey() : m_timer(this, TIMER_ID)
{
    Init();
}

WGetKey::WGetKey( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style ) : m_timer(this, TIMER_ID)
{
    Init();
    Create(parent, id, caption, pos, size, style);
}

/*!
 * Activationkey creator
 */

bool WGetKey::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin WGetKey creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end WGetKey creation
    return true;
}


/*!
 * WGetKey destructor
 */

WGetKey::~WGetKey()
{
////@begin WGetKey destruction
////@end WGetKey destruction
}


/*!
 * Member initialisation
 */

void WGetKey::Init()
{
////@begin WGetKey member initialisation
////@end WGetKey member initialisation
}


/*!
 * Control creation for Activationkey
 */

void WGetKey::CreateControls()
{    
////@begin WGetKey content construction
    WGetKey* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxStaticText* itemStaticText3 = new wxStaticText( itemDialog1, wxID_STATIC, _("Press the desired key or 'Esc' to cancel this action."), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer2->Add(itemStaticText3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    // Connect events and objects
    itemStaticText3->Connect(wxID_STATIC, wxEVT_LEFT_DOWN, wxMouseEventHandler(WGetKey::OnLeftDown), NULL, this);
////@end WGetKey content construction
	m_timer.Start(50);
}


/*!
 * Should we show tooltips?
 */

bool WGetKey::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap WGetKey::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin WGetKey bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end WGetKey bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon WGetKey::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin WGetKey icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end WGetKey icon retrieval
}


void WGetKey::OnTimer(wxTimerEvent& event)
{
	CKeyboardCode kc = CKeyboardCode::ReadKeyCode();
	
	if (kc.IsValid()) {
		if (kc== CKeyboardCode::FromWXKeyCode (WXK_ESCAPE)) {
			EndModal(wxID_NO);		
		} else {		
			m_keyCode = kc;
			EndModal(wxID_YES);
		}
		m_timer.Stop();	
	}
	event.Skip(false);
}

CKeyboardCode WGetKey::GetKeyCode()
{
	return m_keyCode;
}

/*!
 * wxEVT_LEFT_DOWN event handler for ID_ACTIVATIONKEY
 */

void WGetKey::OnLeftDown( wxMouseEvent& event )
{
	EndModal(wxID_NO);
	m_timer.Stop();	
	event.Skip(false);
}

