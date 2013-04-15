/////////////////////////////////////////////////////////////////////////////
// Name:        warnbaroverlap.h
// Purpose:     
// Author:      César Mauri Loba
// Modified by: 
// Created:     Mon 14 Mar 2011 18:13:37 CET
// RCS-ID:      
// Copyright:   (C) 2008 Cesar Mauri from CREA Sistemes Informatics
// License:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _WARNBAROVERLAP_H_
#define _WARNBAROVERLAP_H_


/*!
 * Includes
 */

////@begin includes
////@end includes
#include <wx/intl.h>

class wxCheckBox;
class wxButton;

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_WARNBAROVERLAP 10114
#define ID_CHECKBOX5 10122
#define ID_BUTTON4 10125
#define SYMBOL_WARNBAROVERLAP_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_WARNBAROVERLAP_TITLE _("Overlapping warning")
#define SYMBOL_WARNBAROVERLAP_IDNAME ID_WARNBAROVERLAP
#define SYMBOL_WARNBAROVERLAP_SIZE wxSize(400, 300)
#define SYMBOL_WARNBAROVERLAP_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * WarnBarOverlap class declaration
 */

class WarnBarOverlap: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( WarnBarOverlap )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    WarnBarOverlap();
    WarnBarOverlap( wxWindow* parent, wxWindowID id = SYMBOL_WARNBAROVERLAP_IDNAME, const wxString& caption = SYMBOL_WARNBAROVERLAP_TITLE, const wxPoint& pos = SYMBOL_WARNBAROVERLAP_POSITION, const wxSize& size = SYMBOL_WARNBAROVERLAP_SIZE, long style = SYMBOL_WARNBAROVERLAP_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_WARNBAROVERLAP_IDNAME, const wxString& caption = SYMBOL_WARNBAROVERLAP_TITLE, const wxPoint& pos = SYMBOL_WARNBAROVERLAP_POSITION, const wxSize& size = SYMBOL_WARNBAROVERLAP_SIZE, long style = SYMBOL_WARNBAROVERLAP_STYLE );

    /// Destructor
    ~WarnBarOverlap();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin WarnBarOverlap event handler declarations

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON4
    void OnButtonOkClick( wxCommandEvent& event );

////@end WarnBarOverlap event handler declarations

////@begin WarnBarOverlap member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end WarnBarOverlap member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin WarnBarOverlap member variables
    wxCheckBox* m_chkWarnBarOverlap;
    wxButton* m_buttonOk;
////@end WarnBarOverlap member variables
};

#endif
    // _WARNBAROVERLAP_H_
