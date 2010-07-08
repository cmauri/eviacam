/////////////////////////////////////////////////////////////////////////////
// Name:        activationkey.cpp
// Purpose:     
// Author:      César Mauri Loba
// Modified by: 
// Created:     Thu 01 Jul 2010 13:41:05 CEST
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

#include "activationkey.h"

////@begin XPM images
////@end XPM images


/*!
 * Activationkey type definition
 */

IMPLEMENT_DYNAMIC_CLASS( Activationkey, wxDialog )


/*!
 * Activationkey event table definition
 */

BEGIN_EVENT_TABLE( Activationkey, wxDialog )

////@begin Activationkey event table entries
////@end Activationkey event table entries

END_EVENT_TABLE()


/*!
 * Activationkey constructors
 */

Activationkey::Activationkey()
{
    Init();
}

Activationkey::Activationkey( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
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
