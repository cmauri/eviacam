/////////////////////////////////////////////////////////////////////////////
// Name:        wmotioncalibrationx.cpp
// Purpose:  
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by: 
// Created:     
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

#include "wmotioncalibrationx.h"

////@begin XPM images
////@end XPM images

#define TIMER_ID 1234

/*!
 * WMotionCalibrationX type definition
 */

IMPLEMENT_DYNAMIC_CLASS( WMotionCalibrationX, wxDialog )


/*!
 * WMotionCalibrationX event table definition
 */

BEGIN_EVENT_TABLE( WMotionCalibrationX, wxDialog )

////@begin WMotionCalibrationX event table entries
////@end WMotionCalibrationX event table entries

END_EVENT_TABLE()


/*!
 * WMotionCalibrationX constructors
 */

WMotionCalibrationX::WMotionCalibrationX()
{
    Init();
}

WMotionCalibrationX::WMotionCalibrationX( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*!
 * WMotionCalibrationX creator
 */

bool WMotionCalibrationX::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin WMotionCalibrationX creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end WMotionCalibrationX creation
    return true;
}


/*!
 * WMotionCalibrationX destructor
 */

WMotionCalibrationX::~WMotionCalibrationX()
{
////@begin WMotionCalibrationX destruction
////@end WMotionCalibrationX destruction
}


/*!
 * Member initialisation
 */

void WMotionCalibrationX::Init()
{
////@begin WMotionCalibrationX member initialisation
    m_staticText = NULL;
////@end WMotionCalibrationX member initialisation
}


/*!
 * Control creation for WMotionCalibrationX
 */

void WMotionCalibrationX::CreateControls()
{    
////@begin WMotionCalibrationX content construction
    WMotionCalibrationX* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    m_staticText = new wxStaticText( itemDialog1, wxID_STATIC, _("Move your head left and right."), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer2->Add(m_staticText, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

////@end WMotionCalibrationX content construction
}


/*!
 * Should we show tooltips?
 */

bool WMotionCalibrationX::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap WMotionCalibrationX::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin WMotionCalibrationX bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end WMotionCalibrationX bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon WMotionCalibrationX::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin WMotionCalibrationX icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end WMotionCalibrationX icon retrieval
}
