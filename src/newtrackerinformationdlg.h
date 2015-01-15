/////////////////////////////////////////////////////////////////////////////
// Name:        newtrackerinformationdlg.h
// Purpose:     
// Author:      César Mauri Loba
// Modified by: 
// Created:     21/12/2014 14:03:05
// RCS-ID:      
// Copyright:   (C) 2008 Cesar Mauri from CREA Sistemes Informatics
// License:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _NEWTRACKERINFORMATIONDLG_H_
#define _NEWTRACKERINFORMATIONDLG_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/statline.h"
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
#define ID_NEWTRACKERINFORMATIONDLG 10000
#define ID_RADIOBUTTON_NEW_TRACKER 10001
#define ID_RADIOBUTTON_LEGACY_TRACKER 10002
#define ID_CHECKBOX_SHOW_ON_STARTUP 10003
#define SYMBOL_NEWTRACKERINFORMATIONDLG_STYLE wxDEFAULT_DIALOG_STYLE|wxCAPTION|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_NEWTRACKERINFORMATIONDLG_TITLE _("New Tracker Information")
#define SYMBOL_NEWTRACKERINFORMATIONDLG_IDNAME ID_NEWTRACKERINFORMATIONDLG
#define SYMBOL_NEWTRACKERINFORMATIONDLG_SIZE wxDefaultSize
#define SYMBOL_NEWTRACKERINFORMATIONDLG_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * NewTrackerInformationDlg class declaration
 */

class NewTrackerInformationDlg: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( NewTrackerInformationDlg )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    NewTrackerInformationDlg();
    NewTrackerInformationDlg( wxWindow* parent, wxWindowID id = SYMBOL_NEWTRACKERINFORMATIONDLG_IDNAME, const wxString& caption = SYMBOL_NEWTRACKERINFORMATIONDLG_TITLE, const wxPoint& pos = SYMBOL_NEWTRACKERINFORMATIONDLG_POSITION, const wxSize& size = SYMBOL_NEWTRACKERINFORMATIONDLG_SIZE, long style = SYMBOL_NEWTRACKERINFORMATIONDLG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_NEWTRACKERINFORMATIONDLG_IDNAME, const wxString& caption = SYMBOL_NEWTRACKERINFORMATIONDLG_TITLE, const wxPoint& pos = SYMBOL_NEWTRACKERINFORMATIONDLG_POSITION, const wxSize& size = SYMBOL_NEWTRACKERINFORMATIONDLG_SIZE, long style = SYMBOL_NEWTRACKERINFORMATIONDLG_STYLE );

    /// Destructor
    ~NewTrackerInformationDlg();

private:
    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin NewTrackerInformationDlg event handler declarations

    /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_LEGACY_TRACKER
    void OnRadiobuttonLegacyTrackerSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_SHOW_ON_STARTUP
    void OnCheckboxShowOnStartupClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
    void OnOkClick( wxCommandEvent& event );

////@end NewTrackerInformationDlg event handler declarations

////@begin NewTrackerInformationDlg member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end NewTrackerInformationDlg member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin NewTrackerInformationDlg member variables
    wxCheckBox* m_chkShowOnStartup;
////@end NewTrackerInformationDlg member variables
	bool m_useLegacyTracker;
	bool m_showOnStartup;
};

#endif
    // _NEWTRACKERINFORMATIONDLG_H_
