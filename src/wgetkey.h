/////////////////////////////////////////////////////////////////////////////
// Name:        wgetkey.h
// Purpose:  
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by: 
// Created:     01/07/2010
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
#ifndef _WGETKEY_H_
#define _WGETKEY_H_


/*!
 * Includes
 */

//#include <wx/timer.h>
#include <wx/dialog.h>

#include "keyboardcode.h"

////@begin includes
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
#define ID_ACTIVATIONKEY 10070
#define ID_TEXTCTRL 10041
#define SYMBOL_WGETKEY_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxWANTS_CHARS|wxTAB_TRAVERSAL
#define SYMBOL_WGETKEY_TITLE _("Get key")
#define SYMBOL_WGETKEY_IDNAME ID_ACTIVATIONKEY
#define SYMBOL_WGETKEY_SIZE wxDefaultSize
#define SYMBOL_WGETKEY_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * WGetKey class declaration
 */

class WGetKey: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( WGetKey )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors  
    WGetKey();
    WGetKey( wxWindow* parent, wxWindowID id = SYMBOL_WGETKEY_IDNAME, const wxString& caption = SYMBOL_WGETKEY_TITLE, const wxPoint& pos = SYMBOL_WGETKEY_POSITION, const wxSize& size = SYMBOL_WGETKEY_SIZE, long style = SYMBOL_WGETKEY_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_WGETKEY_IDNAME, const wxString& caption = SYMBOL_WGETKEY_TITLE, const wxPoint& pos = SYMBOL_WGETKEY_POSITION, const wxSize& size = SYMBOL_WGETKEY_SIZE, long style = SYMBOL_WGETKEY_STYLE );

    /// Destructor
    ~WGetKey();

	KeyboardCode GetKeyCode();

private:
    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin WGetKey event handler declarations

    /// wxEVT_LEFT_DOWN event handler for ID_ACTIVATIONKEY
    void OnLeftDown( wxMouseEvent& event );

    /// wxEVT_KEY_UP event handler for ID_TEXTCTRL
    void OnKeyUp( wxKeyEvent& event );

////@end WGetKey event handler declarations

////@begin WGetKey member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end WGetKey member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin WGetKey member variables
    wxTextCtrl* m_txtKey;
////@end WGetKey member variables

	KeyboardCode m_keyCode;
};

#endif
    // _ACTIVATIONKEY_H_
