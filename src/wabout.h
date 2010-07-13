/////////////////////////////////////////////////////////////////////////////
// Name:        wabout.h
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

#ifndef _WABOUT_H_
#define _WABOUT_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/html/htmlwin.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_WABOUT 10027
#define ID_HTMLWINDOW 10030
#define ID_BUTTON_CLOSE 10029
#define SYMBOL_WABOUT_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxDIALOG_MODAL|wxTAB_TRAVERSAL
#define SYMBOL_WABOUT_TITLE _("About Enable Viacam")
#define SYMBOL_WABOUT_IDNAME ID_WABOUT
#define SYMBOL_WABOUT_SIZE wxDefaultSize
#define SYMBOL_WABOUT_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * WAbout class declaration
 */

class WAbout: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( WAbout )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    WAbout();
    WAbout( wxWindow* parent, wxWindowID id = SYMBOL_WABOUT_IDNAME, const wxString& caption = SYMBOL_WABOUT_TITLE, const wxPoint& pos = SYMBOL_WABOUT_POSITION, const wxSize& size = SYMBOL_WABOUT_SIZE, long style = SYMBOL_WABOUT_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_WABOUT_IDNAME, const wxString& caption = SYMBOL_WABOUT_TITLE, const wxPoint& pos = SYMBOL_WABOUT_POSITION, const wxSize& size = SYMBOL_WABOUT_SIZE, long style = SYMBOL_WABOUT_STYLE );

    /// Destructor
    ~WAbout();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin WAbout event handler declarations

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_CLOSE
    void OnButtonCloseClick( wxCommandEvent& event );

////@end WAbout event handler declarations

////@begin WAbout member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end WAbout member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin WAbout member variables
////@end WAbout member variables
};

#endif
    // _WABOUT_H_
