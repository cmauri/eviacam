/////////////////////////////////////////////////////////////////////////////
// Name:        activationkey.h
// Purpose:     
// Author:      César Mauri Loba
// Modified by: 
// Created:     Thu 01 Jul 2010 13:41:05 CEST
// RCS-ID:      
// Copyright:   (C) 2008 Cesar Mauri from CREA Sistemes Informatics
// License:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _ACTIVATIONKEY_H_
#define _ACTIVATIONKEY_H_


/*!
 * Includes
 */

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
#define SYMBOL_ACTIVATIONKEY_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxDIALOG_MODAL|wxTAB_TRAVERSAL
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

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_ACTIVATIONKEY_IDNAME, const wxString& caption = SYMBOL_ACTIVATIONKEY_TITLE, const wxPoint& pos = SYMBOL_ACTIVATIONKEY_POSITION, const wxSize& size = SYMBOL_ACTIVATIONKEY_SIZE, long style = SYMBOL_ACTIVATIONKEY_STYLE );

    /// Destructor
    ~Activationkey();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin Activationkey event handler declarations

////@end Activationkey event handler declarations

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

};

#endif
    // _ACTIVATIONKEY_H_
