/////////////////////////////////////////////////////////////////////////////
// Name:        warnbaroverlap.cpp
// Purpose:     
// Author:      César Mauri Loba
// Modified by: 
// Created:     Mon 14 Mar 2011 18:13:37 CET
// RCS-ID:      
// Copyright:   (C) 2008 Cesar Mauri from CREA Sistemes Informatics
// Licence:     
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

#include "warnbaroverlap.h"


////@begin XPM images
////@end XPM images


/*!
 * WarnBarOverlap type definition
 */

IMPLEMENT_DYNAMIC_CLASS( WarnBarOverlap, wxDialog )


/*!
 * WarnBarOverlap event table definition
 */

BEGIN_EVENT_TABLE( WarnBarOverlap, wxDialog )

////@begin WarnBarOverlap event table entries
    EVT_BUTTON( ID_BUTTON4, WarnBarOverlap::OnButtonOkClick )

////@end WarnBarOverlap event table entries

END_EVENT_TABLE()


/*!
 * WarnBarOverlap constructors
 */

WarnBarOverlap::WarnBarOverlap()
{
    Init();
}

WarnBarOverlap::WarnBarOverlap( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*!
 * WarnBarOverlap creator
 */

bool WarnBarOverlap::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin WarnBarOverlap creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end WarnBarOverlap creation
    return true;
}


/*!
 * WarnBarOverlap destructor
 */

WarnBarOverlap::~WarnBarOverlap()
{
////@begin WarnBarOverlap destruction
////@end WarnBarOverlap destruction
}


/*!
 * Member initialisation
 */

void WarnBarOverlap::Init()
{
////@begin WarnBarOverlap member initialisation
    m_chkWarnBarOverlap = NULL;
    m_buttonOk = NULL;
////@end WarnBarOverlap member initialisation
}


/*!
 * Control creation for WarnBarOverlap
 */

void WarnBarOverlap::CreateControls()
{    
////@begin WarnBarOverlap content construction
    WarnBarOverlap* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxStaticText* itemStaticText3 = new wxStaticText( itemDialog1, wxID_STATIC, _("The chosen Click window position is taken up by another bar.\nThis can prevent it from working properly when auto-hide mode is enabled.\nIt is recommended to move the Click window to a free position."), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer2->Add(itemStaticText3, 0, wxALIGN_LEFT|wxALL, 5);

    itemBoxSizer2->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    m_chkWarnBarOverlap = new wxCheckBox( itemDialog1, ID_CHECKBOX5, _("Do not show this message again"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkWarnBarOverlap->SetValue(false);
    itemBoxSizer2->Add(m_chkWarnBarOverlap, 0, wxALIGN_LEFT|wxALL, 5);

    m_buttonOk = new wxButton( itemDialog1, ID_BUTTON4, _("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer2->Add(m_buttonOk, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

////@end WarnBarOverlap content construction
}


/*!
 * Should we show tooltips?
 */

bool WarnBarOverlap::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap WarnBarOverlap::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin WarnBarOverlap bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end WarnBarOverlap bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon WarnBarOverlap::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin WarnBarOverlap icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end WarnBarOverlap icon retrieval
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON4
 */

void WarnBarOverlap::OnButtonOkClick( wxCommandEvent& event )
{
	EndModal (m_chkWarnBarOverlap->IsChecked() ? 1 : 0);
	event.Skip(false);
}

