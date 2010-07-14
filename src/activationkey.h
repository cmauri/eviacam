/////////////////////////////////////////////////////////////////////////////
// Name:        keyboardbitmapcheck.h
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

#ifndef _ACTIVATIONKEY_H_
#define _ACTIVATIONKEY_H_


/*!
 * Includes
 */
#include "wconfiguration.h"
#include <X11/Xlib.h>

////@begin includes
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

class CViacamController;

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_ACTIVATIONKEY 10070
#define SYMBOL_ACTIVATIONKEY_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxDIALOG_MODAL|wxTAB_TRAVERSAL
#define SYMBOL_ACTIVATIONKEY_TITLE _("Activation key")
#define SYMBOL_ACTIVATIONKEY_IDNAME ID_ACTIVATIONKEY
#define SYMBOL_ACTIVATIONKEY_SIZE wxSize(400, 300)
#define SYMBOL_ACTIVATIONKEY_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Activationkey class declaration
 */

class Activationkey: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( Activationkey )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors  
    Activationkey();
    Activationkey( wxWindow* parent, wxWindowID id = SYMBOL_ACTIVATIONKEY_IDNAME, const wxString& caption = SYMBOL_ACTIVATIONKEY_TITLE, const wxPoint& pos = SYMBOL_ACTIVATIONKEY_POSITION, const wxSize& size = SYMBOL_ACTIVATIONKEY_SIZE, long style = SYMBOL_ACTIVATIONKEY_STYLE );
	Activationkey( wxWindow* parent, CViacamController* pViacamController, wxWindowID id = SYMBOL_ACTIVATIONKEY_IDNAME, const wxString& caption = SYMBOL_ACTIVATIONKEY_TITLE, const wxPoint& pos = SYMBOL_ACTIVATIONKEY_POSITION, const wxSize& size = SYMBOL_ACTIVATIONKEY_SIZE, long style = SYMBOL_ACTIVATIONKEY_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_ACTIVATIONKEY_IDNAME, const wxString& caption = SYMBOL_ACTIVATIONKEY_TITLE, const wxPoint& pos = SYMBOL_ACTIVATIONKEY_POSITION, const wxSize& size = SYMBOL_ACTIVATIONKEY_SIZE, long style = SYMBOL_ACTIVATIONKEY_STYLE );

    /// Destructor
    ~Activationkey();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin Activationkey event handler declarations

    /// wxEVT_LEFT_DOWN event handler for ID_ACTIVATIONKEY
    void OnLeftDown( wxMouseEvent& event );

////@end Activationkey event handler declarations
	void OnTimer(wxTimerEvent& event);
	int GetKeyCode();
	wxString GetKeyName();
	void StartTimer();
	void StopTimer();

////@begin Activationkey member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end Activationkey member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin Activationkey member variables
////@end Activationkey member variables
private:
	wxTimer m_timer;
	CViacamController* m_pViacamController;
	WConfiguration* m_pConfiguration;
	KeySym m_keyCode;

};

#endif
    // _ACTIVATIONKEY_H_
