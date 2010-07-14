/////////////////////////////////////////////////////////////////////////////
// Name:        wconfiguration.cpp
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

#include "activationkey.h"

#include "viacamcontroller.h"
#include "keyboardbitmapcheck.h"
#include "wconfiguration.h"
#include "wx/timer.h"

////@begin XPM images
////@end XPM images
#define TIMER_ID 1234
#define ESCAPE_KEYSYM 65307

/*!
 * Activationkey type definition
 */

IMPLEMENT_DYNAMIC_CLASS( Activationkey, wxDialog )


/*!
 * Activationkey event table definition
 */

BEGIN_EVENT_TABLE( Activationkey, wxDialog )

////@begin Activationkey event table entries
    EVT_CLOSE( Activationkey::OnCloseWindow )
    EVT_KEY_DOWN( Activationkey::OnKeyDown )

////@end Activationkey event table entries
	EVT_TIMER(TIMER_ID, Activationkey::OnTimer)

END_EVENT_TABLE()


/*!
 * Activationkey constructors
 */

Activationkey::Activationkey() : m_timer(this, TIMER_ID)
{
    Init();
}

Activationkey::Activationkey( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style ) : m_timer(this, TIMER_ID)
{
    Init();
    Create(parent, id, caption, pos, size, style);
}

Activationkey::Activationkey( wxWindow* parent, CViacamController* pViacamController, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style ) : m_timer(this, TIMER_ID)
{
	m_pViacamController= pViacamController;
	Init();
    Create(parent, id, caption, pos, size, style);
}

Activationkey::Activationkey( wxWindow* parent, CViacamController* pViacamController, WConfiguration* pConfiguration, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style ) : m_timer(this, TIMER_ID)
{
	m_pViacamController= pViacamController;
	m_pConfiguration= pConfiguration;
	Init();
    Create(parent, id, caption, pos, size, style);
}

/*!
 * Activationkey creator
 */

bool Activationkey::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin Activationkey creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end Activationkey creation
    return true;
}


/*!
 * Activationkey destructor
 */

Activationkey::~Activationkey()
{
////@begin Activationkey destruction
////@end Activationkey destruction
}


/*!
 * Member initialisation
 */

void Activationkey::Init()
{
////@begin Activationkey member initialisation
////@end Activationkey member initialisation
}


/*!
 * Control creation for Activationkey
 */

void Activationkey::CreateControls()
{    
////@begin Activationkey content construction
    Activationkey* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxStaticText* itemStaticText3 = new wxStaticText( itemDialog1, wxID_STATIC, _("Press the desired key or 'Esc' to cancel this action."), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer2->Add(itemStaticText3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

////@end Activationkey content construction
	m_timer.Start(50);
}


/*!
 * Should we show tooltips?
 */

bool Activationkey::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap Activationkey::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin Activationkey bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end Activationkey bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon Activationkey::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin Activationkey icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end Activationkey icon retrieval
}

/*!
 * wxEVT_CLOSE_WINDOW event handler for ID_ACTIVATIONKEY
 */

void Activationkey::OnCloseWindow( wxCloseEvent& event )
{
//EndModal();
}

void Activationkey::OnTimer(wxTimerEvent& event)
{
	KeySym keyCode = 0;
	
	keyCode = CKeyboardBitmapCheck::ReadKeySym();

	if (keyCode != 0) {
		if (keyCode != ESCAPE_KEYSYM) {
			m_keyCode = keyCode;
			EndModal(wxID_YES);		
		} else {		
			EndModal(wxID_NO);
		}
		m_timer.Stop();	
	}	
}

int Activationkey::GetKeyCode()
{
	return (int) m_keyCode;
}

wxString Activationkey::GetKeyName()
{
	return CKeyboardBitmapCheck::GetKeyName(m_keyCode);
}	

void Activationkey::StartTimer()
{
	m_timer.Start(33);
}

void Activationkey::StopTimer()
{
	m_timer.Stop();
}




/*!
 * wxEVT_KEY_DOWN event handler for ID_ACTIVATIONKEY
 */

void Activationkey::OnKeyDown( wxKeyEvent& event )
{
	event.Skip(false);
}

