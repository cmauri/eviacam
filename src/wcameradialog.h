/////////////////////////////////////////////////////////////////////////////
// Name:        wcameradialog.h
// Purpose:     
// Author:      C�sar Mauri Loba
// Modified by: 
// Created:     Fri 22 Oct 2010 10:02:49 CEST
// RCS-ID:      
// Copyright:   (C) 2008 Cesar Mauri from CREA Sistemes Informatics
// License:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _WCAMERADIALOG_H_
#define _WCAMERADIALOG_H_


/*!
 * Includes
 */

////@begin includes
////@end includes
#include <vector>

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxFlexGridSizer;
////@end forward declarations
class CCamera;
class CCameraControl;

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_WCAMERADIALOG 10104
#define ID_SCROLLEDWINDOW1 10111
#define SYMBOL_WCAMERADIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxDIALOG_MODAL|wxTAB_TRAVERSAL
#define SYMBOL_WCAMERADIALOG_TITLE _("Camera settings")
#define SYMBOL_WCAMERADIALOG_IDNAME ID_WCAMERADIALOG
#define SYMBOL_WCAMERADIALOG_SIZE wxDefaultSize
#define SYMBOL_WCAMERADIALOG_POSITION wxDefaultPosition
////@end control identifiers

union ControlType
{
	wxSlider* slider;
	wxCheckBox* checkbox;
	wxChoice*	choice;
};

struct ControlInfo
{
	const CCameraControl* cc;
	ControlType type;
	wxTextCtrl* textCtrl;
};

using namespace std;

/*!
 * WCameraDialog class declaration
 */

class WCameraDialog: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( WCameraDialog )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    WCameraDialog();
    WCameraDialog( wxWindow* parent, wxWindowID id = SYMBOL_WCAMERADIALOG_IDNAME, const wxString& caption = SYMBOL_WCAMERADIALOG_TITLE, const wxPoint& pos = SYMBOL_WCAMERADIALOG_POSITION, const wxSize& size = SYMBOL_WCAMERADIALOG_SIZE, long style = SYMBOL_WCAMERADIALOG_STYLE );
	WCameraDialog( wxWindow* parent, CCamera* pCamera, wxWindowID id = SYMBOL_WCAMERADIALOG_IDNAME, const wxString& caption = SYMBOL_WCAMERADIALOG_TITLE, const wxPoint& pos = SYMBOL_WCAMERADIALOG_POSITION, const wxSize& size = SYMBOL_WCAMERADIALOG_SIZE, long style = SYMBOL_WCAMERADIALOG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_WCAMERADIALOG_IDNAME, const wxString& caption = SYMBOL_WCAMERADIALOG_TITLE, const wxPoint& pos = SYMBOL_WCAMERADIALOG_POSITION, const wxSize& size = SYMBOL_WCAMERADIALOG_SIZE, long style = SYMBOL_WCAMERADIALOG_STYLE );

    /// Destructor
    ~WCameraDialog();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin WCameraDialog event handler declarations

////@end WCameraDialog event handler declarations
	void OnCheckboxClick( wxCommandEvent& event );
	void OnChoiceSelected( wxCommandEvent& event );
	void OnSliderUpdated( wxCommandEvent& event );

////@begin WCameraDialog member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end WCameraDialog member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();
	
	void CreateCameraControlWidget (const CCameraControl& cc, wxWindow* parent, wxSizer* sizer);

////@begin WCameraDialog member variables
    wxScrolledWindow* m_scrollWindow;
    wxFlexGridSizer* m_controlsSizer;
////@end WCameraDialog member variables
	CCamera* m_pCamera;
	int m_lastId;
	vector<ControlInfo> m_controlList;
};

#endif
    // _WCAMERADIALOG_H_
