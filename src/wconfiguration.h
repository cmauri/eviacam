/////////////////////////////////////////////////////////////////////////////
// Name:        wconfiguration.h
// Purpose:     
// Author:      César Mauri Loba
// Modified by: 
// Created:     29/12/2008 16:35:20
// RCS-ID:      
// Copyright:   (C) 2008 Cesar Mauri from CREA Sistemes Informatics
// License:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _WCONFIGURATION_H_
#define _WCONFIGURATION_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/listbook.h"
#include "wx/notebook.h"
#include "wx/spinctrl.h"
////@end includes
#include "cautostart.h"
#include <wx/dialog.h>
#include <wx/checkbox.h>
#include <wx/combobox.h>
#include <wx/choice.h>
#include <wx/button.h>
//#include "cmotioncalibration.h"

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxSpinCtrl;
////@end forward declarations
class CViacamController;

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_WCONFIGURATION 10014
#define ID_LISTBOOK 10051
#define ID_NOTEBOOK 10019
#define ID_PANEL_MOTION 10021
#define ID_SPINCTRL_XSPEED 10026
#define ID_SPINCTRL_YSPEED 10000
#define ID_SPINCTRL_ACCELERATION 10001
#define ID_SPINCTRL_SMOOTHNESS 10002
#define ID_SPINCTRL_EASYSTOP 10003
#define ID_CHECKBOX_ENABLE_AT_STARTUP 10023
#define ID_BUTTON 10074
#define ID_PANEL_WORKSPACE 10061
#define ID_CHECKBOX_WORKSPACE_LIMIT 10062
#define ID_SPINCTRL_TOP_WORKSPACE 10063
#define ID_SPINCTRL_LEFT_WORKSPACE 10064
#define ID_SPINCTRL_RIGHT_WORKSPACE 10065
#define ID_SPINCTRL_BOTTOM_WORKSPACE 10066
#define ID_PANEL_CLICK 10024
#define ID_SPINCTRL_DWELL_TIME 10005
#define ID_SPINCTRL_DWELL_AREA 10004
#define ID_CHECKBOX_ALLOW_CONSECUTIVE 10032
#define ID_CHECKBOX_BEEP_ON_CLICK 10033
#define ID_CHECKBOX_SHOW_CLICK_COUNTDOWN 10034
#define ID_CHECKBOX_ENABLE_DWELL 10031
#define ID_CHECKBOX_CLICKWIN_AT_STARTUP 10022
#define ID_CHECKBOX_SHOW_CLICKWIN 10037
#define ID_COMBOBOX_DESIGN 10035
#define ID_CHOICE_BEHAVIOUR 10006
#define ID_PANEL_ADVANCED 10025
#define ID_CHECKBOX_AUTO_LOCATE_FACE 10038
#define ID_CHECKBOX_SHOW_LOCATE_FACE_FILTER 10039
#define ID_TEXTCTRL_ONSCREENKEYBOARDCOMMAND 10036
#define ID_BUTTON_ONSCREENKEYBOARDCOMMAND 10049
#define ID_CHECKBOX_ACTIVATION_KEY 10067
#define ID_TEXTCTRL_ACTIVATION_KEY 10068
#define ID_BUTTON_ACTIVATION_KEY 10069
#define ID_PANEL 10050
#define ID_CHECKBOX_STARTUP 10071
#define ID_CHOICE_PROFILE 10015
#define ID_BUTTON_ADD_PROFILE 10017
#define ID_BUTTON_DEL_PROFILE 10018
#define ID_TEXTCTRL_SELECTED_CAMERA 10052
#define ID_BUTTON_CAMERA_SETTINGS 10053
#define ID_BUTTON_CHANGE_CAMERA 10054
#define ID_CHOICE_LANGUAGE 10012
#define ID_CHECKBOX_RUN_STARTUP 10016
#define ID_BUTTON_OK 10040
#define ID_BUTTON_CANCEL 10041
#define SYMBOL_WCONFIGURATION_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxDIALOG_MODAL|wxTAB_TRAVERSAL
#define SYMBOL_WCONFIGURATION_TITLE _("Configuration")
#define SYMBOL_WCONFIGURATION_IDNAME ID_WCONFIGURATION
#define SYMBOL_WCONFIGURATION_SIZE wxDefaultSize
#define SYMBOL_WCONFIGURATION_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * WConfiguration class declaration
 */

class WConfiguration: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( WConfiguration )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    WConfiguration();
    WConfiguration( wxWindow* parent, wxWindowID id = SYMBOL_WCONFIGURATION_IDNAME, const wxString& caption = SYMBOL_WCONFIGURATION_TITLE, const wxPoint& pos = SYMBOL_WCONFIGURATION_POSITION, const wxSize& size = SYMBOL_WCONFIGURATION_SIZE, long style = SYMBOL_WCONFIGURATION_STYLE );
	WConfiguration( wxWindow* parent, CViacamController* pViacamController, wxWindowID id = SYMBOL_WCONFIGURATION_IDNAME, const wxString& caption = SYMBOL_WCONFIGURATION_TITLE, const wxPoint& pos = SYMBOL_WCONFIGURATION_POSITION, const wxSize& size = SYMBOL_WCONFIGURATION_SIZE, long style = SYMBOL_WCONFIGURATION_STYLE );
	
    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_WCONFIGURATION_IDNAME, const wxString& caption = SYMBOL_WCONFIGURATION_TITLE, const wxPoint& pos = SYMBOL_WCONFIGURATION_POSITION, const wxSize& size = SYMBOL_WCONFIGURATION_SIZE, long style = SYMBOL_WCONFIGURATION_STYLE );

    /// Destructor
    ~WConfiguration();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin WConfiguration event handler declarations

    /// wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL_XSPEED
    void OnSpinctrlXspeedUpdated( wxSpinEvent& event );

    /// wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL_YSPEED
    void OnSpinctrlYspeedUpdated( wxSpinEvent& event );

    /// wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL_ACCELERATION
    void OnSpinctrlAccelerationUpdated( wxSpinEvent& event );

    /// wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL_SMOOTHNESS
    void OnSpinctrlSmoothnessUpdated( wxSpinEvent& event );

    /// wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL_EASYSTOP
    void OnSpinctrlEasystopUpdated( wxSpinEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_ENABLE_AT_STARTUP
    void OnCheckboxEnableAtStartupClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON
    void OnButtonClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_WORKSPACE_LIMIT
    void OnCheckboxWorkspaceLimitClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL_TOP_WORKSPACE
    void OnSpinctrlTopWorkspaceUpdated( wxSpinEvent& event );

    /// wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL_LEFT_WORKSPACE
    void OnSpinctrlLeftWorkspaceUpdated( wxSpinEvent& event );

    /// wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL_RIGHT_WORKSPACE
    void OnSpinctrlRightWorkspaceUpdated( wxSpinEvent& event );

    /// wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL_BOTTOM_WORKSPACE
    void OnSpinctrlBottomWorkspaceUpdated( wxSpinEvent& event );

    /// wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL_DWELL_TIME
    void OnSpinctrlDwellTimeUpdated( wxSpinEvent& event );

    /// wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL_DWELL_AREA
    void OnSpinctrlDwellAreaUpdated( wxSpinEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_ALLOW_CONSECUTIVE
    void OnCheckboxAllowConsecutiveClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_BEEP_ON_CLICK
    void OnCheckboxBeepOnClickClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_SHOW_CLICK_COUNTDOWN
    void OnCheckboxShowClickCountdownClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_ENABLE_DWELL
    void OnCheckboxEnableDwellClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_CLICKWIN_AT_STARTUP
    void OnCheckboxClickwinAtStartupClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_SHOW_CLICKWIN
    void OnCheckboxShowClickwinClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_COMBOBOX_DESIGN
    void OnComboboxDesignSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE_BEHAVIOUR
    void OnChoiceBehaviourSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_AUTO_LOCATE_FACE
    void OnCheckboxAutoLocateFaceClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_SHOW_LOCATE_FACE_FILTER
    void OnCheckboxShowLocateFaceFilterClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL_ONSCREENKEYBOARDCOMMAND
    void OnTextctrlOnscreenkeyboardcommandTextUpdated( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_ONSCREENKEYBOARDCOMMAND
    void OnButtonOnscreenkeyboardcommandClick( wxCommandEvent& event );

#if defined(__WXGTK__)
    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_ACTIVATION_KEY
    void OnCheckboxActivationKeyClick( wxCommandEvent& event );

#endif
#if defined(__WXGTK__)
    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_ACTIVATION_KEY
    void OnButtonActivationKeyClick( wxCommandEvent& event );

#endif
#if defined(__WXGTK__)
    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_STARTUP
    void OnCheckboxStartupClick( wxCommandEvent& event );

#endif
    /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE_PROFILE
    void OnChoiceProfileSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_ADD_PROFILE
    void OnButtonAddProfileClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_DEL_PROFILE
    void OnButtonDelProfileClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_CAMERA_SETTINGS
    void OnButtonCameraSettingsClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_CHANGE_CAMERA
    void OnButtonChangeCameraClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE_LANGUAGE
    void OnChoiceLanguageSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_OK
    void OnButtonOkClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_CANCEL
    void OnButtonCancelClick( wxCommandEvent& event );

////@end WConfiguration event handler declarations

////@begin WConfiguration member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end WConfiguration member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();
    
    void SetActivationKey(wxString keyName);

private:

	// Read info from model classes
	void InitializeData ();
	void Changed ();
	void UnChanged ();

////@begin WConfiguration member variables
    wxSpinCtrl* m_spinXSpeed;
    wxSpinCtrl* m_spinYSpeed;
    wxSpinCtrl* m_spinAcceleration;
    wxSpinCtrl* m_spinSmoothness;
    wxSpinCtrl* m_spinEasyStop;
    wxCheckBox* m_chkEnabledAtStartup;
    wxButton* m_buttonCalibrateMotion;
    wxCheckBox* m_chkEnabledWorkspace;
    wxSpinCtrl* m_spin_top_workspace;
    wxSpinCtrl* m_spin_left_workspace;
    wxSpinCtrl* m_spin_right_workspace;
    wxSpinCtrl* m_spin_bottom_workspace;
    wxSpinCtrl* m_spinDwellTime;
    wxSpinCtrl* m_spinDwellArea;
    wxCheckBox* m_chkAllowConsecutiveClick;
    wxCheckBox* m_chkBeepOnClick;
    wxCheckBox* m_chkShowClickCountdown;
    wxCheckBox* m_chkDwellClickEnabled;
    wxCheckBox* m_chkOpenClickWinAtStartup;
    wxCheckBox* m_chkShowClickWin;
    wxComboBox* m_cmbClickWindowDesign;
    wxChoice* m_choClickWindowBehaviour;
    wxCheckBox* m_chkAutoLocateFace;
    wxCheckBox* m_chkShowAutoLocateFaceFilter;
    wxTextCtrl* m_txtOnScreenKeyboardCommand;
    wxButton* m_btntOnScreenKeyboardCommand;
#if defined(__WXGTK__)
    wxCheckBox* m_chkActivationKey;
#endif
#if defined(__WXGTK__)
    wxTextCtrl* m_txtActivationKey;
#endif
#if defined(__WXGTK__)
    wxButton* m_buttonActivationKey;
#endif
#if defined(__WXGTK__)
    wxCheckBox* m_chkStartup;
#endif
    wxChoice* m_choProfile;
    wxButton* m_btnAddProfile;
    wxButton* m_btnDeleteProfile;
    wxTextCtrl* m_txtSelectedCamera;
    wxButton* m_btnCameraSettings;
    wxChoice* m_choLanguage;
    wxButton* m_btnCancel;
////@end WConfiguration member variables

	CViacamController* m_pViacamController;
    CAutostart* m_pAutostart;
	bool m_dirty;
	//CMotionCalibration* m_pMotionCalibration;
};


#endif
    // _WCONFIGURATION_H_
