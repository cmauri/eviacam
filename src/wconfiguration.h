/////////////////////////////////////////////////////////////////////////////
// Name:        wconfiguration.h
// Purpose:     
// Author:      Cesar Mauri Loba
// Modified by: 
// Created:     29/12/2008 16:35:20
// RCS-ID:      
// Copyright:   (C) 2008 Cesar Mauri from CREA Sistemes Informatics
// License:     
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

#ifndef _WCONFIGURATION_H_
#define _WCONFIGURATION_H_


/*!
 * Includes
 */

#include "hotkeymanager.h"

////@begin includes
#include "wx/listbook.h"
#include "wx/notebook.h"
#include "wx/spinctrl.h"
////@end includes

#include <wx/dialog.h>
#include <wx/checkbox.h>
#include <wx/combobox.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/statbox.h>

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxSpinCtrl;
class wxFlexGridSizer;
////@end forward declarations
class wxPanel;

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
#define ID_BUTTON 10074
#define ID_PANEL_WORKSPACE 10061
#define ID_CHECKBOX_WORKSPACE_LIMIT 10062
#define ID_SPINCTRL_TOP_WORKSPACE 10063
#define ID_SPINCTRL_LEFT_WORKSPACE 10064
#define ID_SPINCTRL_RIGHT_WORKSPACE 10065
#define ID_SPINCTRL_BOTTOM_WORKSPACE 10066
#define ID_CHECKBOX2 10067
#define ID_PANEL_CLICK 10024
#define ID_CHECKBOX_ENABLE_DWELL 10031
#define ID_STATIC_DWELL_TIME 10072
#define ID_SPINCTRL_DWELL_TIME 10005
#define ID_STATIC_DWELL_AREA 10073
#define ID_SPINCTRL_DWELL_AREA 10004
#define ID_CHECKBOX_ALLOW_CONSECUTIVE 10032
#define ID_CHECKBOX_BEEP_ON_CLICK 10033
#define ID_CHECKBOX_ALLOW_VISUAL_ALERTS 10089
#define ID_STATICBOX_CLICK_WIN 10007
#define ID_CHECKBOX_SHOW_CLICKWIN 10037
#define ID_STATIC_DESIGN 10081
#define ID_CHOICE_DESIGN 10087
#define ID_STATIC_BEHAVIOUR 10082
#define ID_CHOICE_BEHAVIOUR 10006
#define ID_CHOICE5 10068
#define ID_CHECKBOX_AUTOHIDE 10113
#define ID_STATICBOX_GESTURE_CLICK 10008
#define ID_CHECKBOX 10034
#define ID_STATIC_MOVE_LEFT 10083
#define ID_CHOICE 10035
#define ID_STATIC_MOVE_RIGHT 10084
#define ID_CHOICE1 10088
#define ID_STATIC_MOVE_UP 10085
#define ID_CHOICE2 10102
#define ID_STATIC_MOVE_DOWN 10086
#define ID_CHOICE3 10105
#define ID_PANEL_KEYS 10110
#define ID_PANEL_ADVANCED 10025
#define ID_CHECKBOX_STARTUP 10071
#define ID_CHECKBOX_ENABLE_AT_STARTUP 10023
#define ID_TEXTCTRL_ONSCREENKEYBOARDCOMMAND 10036
#define ID_BUTTON_ONSCREENKEYBOARDCOMMAND 10049
#define ID_CHECKBO_LEGACY_TRACKER 10009
#define ID_CHECKBOX_AUTO_LOCATE_FACE 10038
#define ID_CHOICE4 10109
#define ID_CHECKBOX_ENABLE_WHEN_FACE_DETECTED 10039
#define ID_SPINCTRL2 10107
#define ID_SPIN_SEND_ACTION_WAIT 10131
#define ID_PANEL 10050
#define ID_CHOICE_PROFILE 10015
#define ID_BUTTON_ADD_PROFILE 10017
#define ID_BUTTON_DEL_PROFILE 10018
#define ID_TEXTCTRL_SELECTED_CAMERA 10052
#define ID_BUTTON_CAMERA_SETTINGS 10053
#define ID_BUTTON_CHANGE_CAMERA 10054
#define ID_CHOICE_LANGUAGE 10012
#define ID_CHECKBOX_CHECK_UPDATES 10010
#define SYMBOL_WCONFIGURATION_STYLE wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_WCONFIGURATION_TITLE _("Configuration")
#define SYMBOL_WCONFIGURATION_IDNAME ID_WCONFIGURATION
#define SYMBOL_WCONFIGURATION_SIZE wxDefaultSize
#define SYMBOL_WCONFIGURATION_POSITION wxDefaultPosition
////@end control identifiers
#define ID_COMBOBOX_LEFT 10007
#define ID_COMBOBOX_RIGHT 10008
#define ID_COMBOBOX_TOP 10009
#define ID_COMBOBOX_BOTTOM 10010



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
	
    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_WCONFIGURATION_IDNAME, const wxString& caption = SYMBOL_WCONFIGURATION_TITLE, const wxPoint& pos = SYMBOL_WCONFIGURATION_POSITION, const wxSize& size = SYMBOL_WCONFIGURATION_SIZE, long style = SYMBOL_WCONFIGURATION_STYLE );

    /// Destructor
    ~WConfiguration();

private:
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

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX2
    void OnCheckboxWrapPointer( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_ENABLE_DWELL
    void OnCheckboxEnableDwellClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL_DWELL_TIME
    void OnSpinctrlDwellTimeUpdated( wxSpinEvent& event );

    /// wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL_DWELL_AREA
    void OnSpinctrlDwellAreaUpdated( wxSpinEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_ALLOW_CONSECUTIVE
    void OnCheckboxAllowConsecutiveClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_BEEP_ON_CLICK
    void OnCheckboxBeepOnClickClick( wxCommandEvent& event );

#if defined(__WXGTK__)
    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_ALLOW_VISUAL_ALERTS
    void OnCheckboxAllowVisualAlertsClick( wxCommandEvent& event );

#endif
    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_SHOW_CLICKWIN
    void OnCheckboxShowClickwinClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE_DESIGN
    void OnChoiceDesignSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE_BEHAVIOUR
    void OnChoiceBehaviourSelected( wxCommandEvent& event );

#if defined(__WXGTK__)
    /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE5
    void OnChoiceClickWindowModeSelected( wxCommandEvent& event );

#endif
#if defined(__WXGTK__)
    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_AUTOHIDE
    void OnCheckboxAutohideClick( wxCommandEvent& event );

#endif
#if defined(__WXGTK__)
    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX
    void OnCheckboxEnableGestureClick( wxCommandEvent& event );

#endif
#if defined(__WXGTK__)
    /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE
    void OnLeftGestureChoiceSelected( wxCommandEvent& event );

#endif
#if defined(__WXGTK__)
    /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE1
    void OnRightGestureChoiceSelected( wxCommandEvent& event );

#endif
#if defined(__WXGTK__)
    /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE2
    void OnUpGestureChoiceSelected( wxCommandEvent& event );

#endif
#if defined(__WXGTK__)
    /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE3
    void OnDownGestureChoiceSelected( wxCommandEvent& event );

#endif
#if defined(__WXGTK__)
    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_STARTUP
    void OnCheckboxStartupClick( wxCommandEvent& event );

#endif
    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_ENABLE_AT_STARTUP
    void OnCheckboxEnableAtStartupClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL_ONSCREENKEYBOARDCOMMAND
    void OnTextctrlOnscreenkeyboardcommandTextUpdated( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_ONSCREENKEYBOARDCOMMAND
    void OnButtonOnscreenkeyboardcommandClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBO_LEGACY_TRACKER
    void OnCheckboLegacyTrackerClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_AUTO_LOCATE_FACE
    void OnCheckboxAutoLocateFaceClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE4
    void OnChoCpuUsageSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_ENABLE_WHEN_FACE_DETECTED
    void OnCheckboxEnableWhenFaceDetectedClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL2
    void OnSpinLocateFaceTimeoutUpdated( wxSpinEvent& event );

    /// wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPIN_SEND_ACTION_WAIT
    void OnSpinSendActionWaitUpdated( wxSpinEvent& event );

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

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_CHECK_UPDATES
    void OnCheckboxCheckUpdatesClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
    void OnOkClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL
    void OnCancelClick( wxCommandEvent& event );

////@end WConfiguration event handler declarations

    /// wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_COMBOBOX_LEFT
    void OnComboboxLeftSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_COMBOBOX_RIGHT
    void OnComboboxRightSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_COMBOBOX_TOP
    void OnComboboxTopSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_COMBOBOX_BOTTOM
    void OnComboboxBottomSelected( wxCommandEvent& event );

	void OnHotkeyCheckboxClick( wxCommandEvent& event );
	void OnHotkeyButtonClick( wxCommandEvent& event );

////@begin WConfiguration member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end WConfiguration member function declarations

	void CreateHotkey (const eviacam::HotKey& kc, wxWindow* parent, wxSizer* sizer);

    /// Should we show tooltips?
    static bool ShowToolTips();

	// Enable/disable GUI areas
	void UpdateGUIClickOptions();
	void EnableGUIGeneralClickOptions(bool enable);
	void EnableGUIGestureOptions(bool enable);
	void EnableGUIClickWindowOptions(bool enable);

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
    wxButton* m_buttonCalibrateMotion;
    wxCheckBox* m_chkEnabledWorkspace;
    wxSpinCtrl* m_spin_top_workspace;
    wxSpinCtrl* m_spin_left_workspace;
    wxSpinCtrl* m_spin_right_workspace;
    wxSpinCtrl* m_spin_bottom_workspace;
    wxCheckBox* m_chkWrapPointer;
    wxPanel* m_panelClick;
    wxCheckBox* m_chkDwellClickEnabled;
    wxStaticText* m_stDwellTime;
    wxSpinCtrl* m_spinDwellTime;
    wxStaticText* m_stDwellArea;
    wxSpinCtrl* m_spinDwellArea;
    wxCheckBox* m_chkAllowConsecutiveClick;
    wxCheckBox* m_chkBeepOnClick;
#if defined(__WXGTK__)
    wxCheckBox* m_chkAllowVisualAlerts;
#endif
    wxStaticBox* m_sboxClickWin;
    wxCheckBox* m_chkShowClickWin;
    wxStaticText* m_stDesign;
    wxChoice* m_choClickWindowDesign;
    wxStaticText* m_stBehaviour;
    wxChoice* m_choClickWindowBehaviour;
#if defined(__WXGTK__)
    wxStaticText* m_stDocking;
#endif
#if defined(__WXGTK__)
    wxChoice* m_choDockingMode;
#endif
#if defined(__WXGTK__)
    wxCheckBox* m_chkAutohide;
#endif
#if defined(__WXGTK__)
    wxStaticBox* m_sboxGestureClick;
#endif
#if defined(__WXGTK__)
    wxCheckBox* m_chkEnableGestureClick;
#endif
#if defined(__WXGTK__)
    wxStaticText* m_stMoveLeft;
#endif
#if defined(__WXGTK__)
    wxChoice* m_choLeft;
#endif
#if defined(__WXGTK__)
    wxStaticText* m_stMoveRight;
#endif
#if defined(__WXGTK__)
    wxChoice* m_choRight;
#endif
#if defined(__WXGTK__)
    wxStaticText* m_stMoveUp;
#endif
#if defined(__WXGTK__)
    wxChoice* m_choUp;
#endif
#if defined(__WXGTK__)
    wxStaticText* m_stMoveDown;
#endif
#if defined(__WXGTK__)
    wxChoice* m_choDown;
#endif
    wxStaticBox* m_panelKeys;
    wxFlexGridSizer* m_hotkeysSizer;
#if defined(__WXGTK__)
    wxCheckBox* m_chkStartup;
#endif
    wxCheckBox* m_chkEnabledAtStartup;
    wxTextCtrl* m_txtOnScreenKeyboardCommand;
    wxButton* m_btntOnScreenKeyboardCommand;
    wxStaticBox* m_sizerFaceLocalization;
    wxCheckBox* m_chkLegacyTracker;
    wxCheckBox* m_chkAutoLocateFace;
    wxStaticText* m_staticCpuUsage;
    wxChoice* m_choCpuUsage;
    wxCheckBox* m_chkEnableWhenFaceDetected;
    wxStaticText* m_staticLocateFaceTimeout2;
    wxSpinCtrl* m_spinLocateFaceTimeout;
    wxStaticText* m_staticLocateFaceTimeout;
    wxSpinCtrl* m_spinSendActionWait;
    wxChoice* m_choProfile;
    wxButton* m_btnAddProfile;
    wxButton* m_btnDeleteProfile;
    wxTextCtrl* m_txtSelectedCamera;
    wxButton* m_btnCameraSettings;
    wxChoice* m_choLanguage;
    wxCheckBox* m_chkCheckUpdatesAtStartup;
    wxButton* m_btnCancel;
////@end WConfiguration member variables
    wxComboBox* m_cmbLeft;
    wxComboBox* m_cmbRight;
    wxComboBox* m_cmbTop;
    wxComboBox* m_cmbBottom;
	std::vector<wxControl*> m_controlList;
	int m_lastId;
	bool m_dirty;
};


#endif
    // _WCONFIGURATION_H_
