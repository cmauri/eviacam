/////////////////////////////////////////////////////////////////////////////
// Name:        wwizard.h
// Purpose:     
// Author:      Cesar Mauri Loba
// Modified by: 
// Created:     Mon 18 Oct 2010 09:57:22 CEST
// RCS-ID:      
// Copyright:   (C) 2008 Cesar Mauri from CREA Sistemes Informatics
// License:
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

#ifndef _WWIZARD_H_
#define _WWIZARD_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/wizard.h"
#include "wx/statline.h"
#include "wx/spinctrl.h"
#include "wx/tglbtn.h"
////@end includes
#include <wx/timer.h>

/*!
 * Forward declarations
 */

////@begin forward declarations
class WizardPage;
class WizardPage5;
class WizardPage8;
class WizardPage1;
class WizardPage2;
class WizardPage6;
class wxSpinCtrl;
class wxToggleButton;
class WizardPage3;
class WizardPage4;
////@end forward declarations
class wxCheckBox;
class wxRadioButton;
class wxStaticText;

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_WWIZARD 10090
#define ID_WIZARDPAGE_INI 10091
#define ID_WIZARDPAGE_CAMERA 10115
#define ID_BUTTON5 10124
#define ID_WIZARDPAGE_TRACKER 10016
#define ID_CHECKBOX_LOCATE_FACE 10022
#define ID_CHECKBOX_ENABLE_WHEN_FACE_DETECTED 10106
#define ID_WIZARDPAGE_CALIB1 10093
#define ID_CHECKBOX_PERFORM_CALIBRATION 10095
#define ID_WIZARDPAGE_CALIB2 10096
#define ID_WIZARDPAGE_CALIB3 10116
#define ID_SPINCTRL 10118
#define ID_SPINCTRL1 10119
#define ID_TOGGLE_HEAD_TRACKING_PAGE6 10117
#define ID_WIZARDPAGE_CLICK2 10097
#define ID_TOGGLE_TEST 10121
#define ID_RADIOBUTTON_DWELL_CLICK 10099
#define ID_RADIOBUTTON_GESTURE_CLICK 10100
#define ID_RADIOBUTTON_NONE_CLICK 10098
#define ID_TOGGLE_HEAD_TRACKING_PAGE3 10120
#define ID_WIZARDPAGE_FINAL 10103
#define ID_CHECKBOX1 10092
#define ID_CHECKBOX3 10101
#define ID_CHECKBOX4 10123
#define SYMBOL_WWIZARD_IDNAME ID_WWIZARD
////@end control identifiers

/*!
 * WWizard class declaration
 */

class WWizard: public wxWizard
{    
    DECLARE_DYNAMIC_CLASS( WWizard )
    DECLARE_EVENT_TABLE()

public:
    enum EClickMode { NONE, DWELL, GESTURE };
	
	/// Constructors
    WWizard();
    WWizard( wxWindow* parent, wxWindowID id = SYMBOL_WWIZARD_IDNAME, const wxPoint& pos = wxDefaultPosition );  

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_WWIZARD_IDNAME, const wxPoint& pos = wxDefaultPosition );

    /// Destructor
    ~WWizard();

	const bool GetPerformCalibration() const { return m_performCalibration;	}
	void SetPerformCalibration(bool value) { m_performCalibration= value; }
	
private:
    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin WWizard event handler declarations

    /// wxEVT_WIZARD_CANCEL event handler for ID_WWIZARD
    void OnWwizardCancel( wxWizardEvent& event );

    /// wxEVT_WIZARD_FINISHED event handler for ID_WWIZARD
    void OnWwizardFinished( wxWizardEvent& event );

////@end WWizard event handler declarations
public:
////@begin WWizard member function declarations

    /// Runs the wizard
    bool Run();

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end WWizard member function declarations

	void UpdateToggleButtonStatus (wxToggleButton& toggle);
	void OnToggleHeadTrackingClick (wxCommandEvent& event, wxToggleButton& toggle);
	void OnToggleHeadTrackingUpdate(wxUpdateUIEvent& event, wxToggleButton& toggle);

private:
    /// Should we show tooltips?
    static bool ShowToolTips();
	
////@begin WWizard member variables
////@end WWizard member variables
	
	bool m_performCalibration;
};
	
/*!
 * WizardPage class declaration
 */

class WizardPage: public wxWizardPageSimple
{    
    DECLARE_DYNAMIC_CLASS( WizardPage )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    WizardPage();

    WizardPage( wxWizard* parent );

    /// Creation
    bool Create( wxWizard* parent );

    /// Destructor
    ~WizardPage();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();
	

////@begin WizardPage event handler declarations

////@end WizardPage event handler declarations

////@begin WizardPage member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end WizardPage member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin WizardPage member variables
    wxStaticBitmap* m_eviacamLogo;
////@end WizardPage member variables
	WWizard* m_wizardParent;
};

/*!
 * WizardPage1 class declaration
 */

class WizardPage1: public wxWizardPageSimple
{    
    DECLARE_DYNAMIC_CLASS( WizardPage1 )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    WizardPage1();

    WizardPage1( wxWizard* parent );

    /// Creation
    bool Create( wxWizard* parent );

    /// Destructor
    ~WizardPage1();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin WizardPage1 event handler declarations

    /// wxEVT_WIZARD_PAGE_CHANGED event handler for ID_WIZARDPAGE_CALIB1
    void OnWizardpage1Changed( wxWizardEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_PERFORM_CALIBRATION
    void OnCheckboxPerformCalibrationClick( wxCommandEvent& event );

////@end WizardPage1 event handler declarations

////@begin WizardPage1 member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end WizardPage1 member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin WizardPage1 member variables
    wxCheckBox* m_chkPerformCalibration;
////@end WizardPage1 member variables
	WWizard* m_wizardParent;
};

/*!
 * WizardPage2 class declaration
 */

class WizardPage2: public wxWizardPageSimple
{    
    DECLARE_DYNAMIC_CLASS( WizardPage2 )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    WizardPage2();

    WizardPage2( wxWizard* parent );

    /// Creation
    bool Create( wxWizard* parent );

    /// Destructor
    ~WizardPage2();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin WizardPage2 event handler declarations

    /// wxEVT_WIZARD_PAGE_CHANGED event handler for ID_WIZARDPAGE_CALIB2
    void OnWizardpage2Changed( wxWizardEvent& event );

////@end WizardPage2 event handler declarations

////@begin WizardPage2 member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end WizardPage2 member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin WizardPage2 member variables
////@end WizardPage2 member variables
	WWizard* m_wizardParent;
};

/*!
 * WizardPage3 class declaration
 */

class WizardPage3: public wxWizardPageSimple
{    
    DECLARE_DYNAMIC_CLASS( WizardPage3 )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    WizardPage3();

    WizardPage3( wxWizard* parent );

    /// Creation
    bool Create( wxWizard* parent );

    /// Destructor
    ~WizardPage3();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();
private:
////@begin WizardPage3 event handler declarations

    /// wxEVT_WIZARD_PAGE_CHANGED event handler for ID_WIZARDPAGE_CLICK2
    void OnWizardpage3Changed( wxWizardEvent& event );

    /// wxEVT_LEFT_DOWN event handler for ID_TOGGLE_TEST
    void OnLeftDownTest( wxMouseEvent& event );

    /// wxEVT_LEFT_UP event handler for ID_TOGGLE_TEST
    void OnLeftUpTest( wxMouseEvent& event );

    /// wxEVT_LEFT_DCLICK event handler for ID_TOGGLE_TEST
    void OnLeftDClickTest( wxMouseEvent& event );

    /// wxEVT_MIDDLE_UP event handler for ID_TOGGLE_TEST
    void OnMiddleUpTest( wxMouseEvent& event );

    /// wxEVT_RIGHT_UP event handler for ID_TOGGLE_TEST
    void OnRightUpTest( wxMouseEvent& event );

    /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_DWELL_CLICK
    void OnRadiobuttonDwellClickSelected( wxCommandEvent& event );

#if defined(__WXGTK__)
    /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_GESTURE_CLICK
    void OnRadiobuttonGestureClickSelected( wxCommandEvent& event );

#endif
    /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_NONE_CLICK
    void OnRadiobuttonNoneClickSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_TOGGLE_HEAD_TRACKING_PAGE3
    void OnToggleHeadTrackingPage3Click( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_TOGGLE_HEAD_TRACKING_PAGE3
    void OnToggleHeadTrackingPage3Update( wxUpdateUIEvent& event );

////@end WizardPage3 event handler declarations


////@begin WizardPage3 member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end WizardPage3 member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

	void UpdateRadioButtons();

////@begin WizardPage3 member variables
    wxToggleButton* m_toggleTest;
    wxRadioButton* m_rbDwellClick;
#if defined(__WXGTK__)
    wxRadioButton* m_rbGestureClick;
#endif
    wxRadioButton* m_rbNoneClick;
    wxToggleButton* m_toggleHeadTrackingPage3;
////@end WizardPage3 member variables
	WWizard* m_wizardParent;
	bool m_isLeftDown;
	bool m_isDoubleClick;
	wxLongLong m_lastLeftDown;
};

/*!
 * WizardPage4 class declaration
 */

class WizardPage4: public wxWizardPageSimple
{    
    DECLARE_DYNAMIC_CLASS( WizardPage4 )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    WizardPage4();

    WizardPage4( wxWizard* parent );

    /// Creation
    bool Create( wxWizard* parent );

    /// Destructor
    ~WizardPage4();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin WizardPage4 event handler declarations

    /// wxEVT_WIZARD_PAGE_CHANGED event handler for ID_WIZARDPAGE_FINAL
    void OnWizardpageFinalPageChanged( wxWizardEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX1
    void OnCheckboxRunWizardAtStartupClick( wxCommandEvent& event );

#if defined(__WXGTK__)
    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX3
    void OnCheckboxStartupClick( wxCommandEvent& event );

#endif
    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX4
    void OnCheckboxEnabledAtStartupClick( wxCommandEvent& event );

////@end WizardPage4 event handler declarations

////@begin WizardPage4 member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end WizardPage4 member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin WizardPage4 member variables
    wxCheckBox* m_chkRunWizardAtStartup;
#if defined(__WXGTK__)
    wxCheckBox* m_chkStartup;
#endif
    wxCheckBox* m_chkEnabledAtStartup;
////@end WizardPage4 member variables
	WWizard* m_wizardParent;
};

/*!
 * WizardPage5 class declaration
 */

class WizardPage5: public wxWizardPageSimple
{    
    DECLARE_DYNAMIC_CLASS( WizardPage5 )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    WizardPage5();

    WizardPage5( wxWizard* parent );

    /// Creation
    bool Create( wxWizard* parent );

    /// Destructor
    ~WizardPage5();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin WizardPage5 event handler declarations

    /// wxEVT_WIZARD_PAGE_CHANGED event handler for ID_WIZARDPAGE_CAMERA
    void OnWizardpageCameraPageChanged( wxWizardEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON5
    void OnButtonCameraSettingsClick( wxCommandEvent& event );

////@end WizardPage5 event handler declarations

////@begin WizardPage5 member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end WizardPage5 member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin WizardPage5 member variables
    wxStaticText* m_staticCameraName;
    wxStaticText* m_staticFramerate;
    wxStaticText* m_staticCameraInstructions;
    wxButton* m_btnCameraSettings;
////@end WizardPage5 member variables
	WWizard* m_wizardParent;
};

/*!
 * WizardPage6 class declaration
 */

class WizardPage6: public wxWizardPageSimple
{    
    DECLARE_DYNAMIC_CLASS( WizardPage6 )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    WizardPage6();

    WizardPage6( wxWizard* parent );

    /// Creation
    bool Create( wxWizard* parent );

    /// Destructor
    ~WizardPage6();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin WizardPage6 event handler declarations

    /// wxEVT_WIZARD_PAGE_CHANGED event handler for ID_WIZARDPAGE_CALIB3
    void OnWizardpageCalib3PageChanged( wxWizardEvent& event );

    /// wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL
    void OnSpinctrlXSpeedUpdated( wxSpinEvent& event );

    /// wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL1
    void OnSpinctrlYSpeedUpdated( wxSpinEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_TOGGLE_HEAD_TRACKING_PAGE6
    void OnToggleHeadTrackingPage6Click( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_TOGGLE_HEAD_TRACKING_PAGE6
    void OnToggleHeadTrackingPage6Update( wxUpdateUIEvent& event );

////@end WizardPage6 event handler declarations

////@begin WizardPage6 member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end WizardPage6 member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin WizardPage6 member variables
    wxSpinCtrl* m_spinXSpeed;
    wxSpinCtrl* m_spinYSpeed;
    wxToggleButton* m_toggleHeadTrackingPage6;
////@end WizardPage6 member variables
	WWizard* m_wizardParent;
};

/*!
 * WizardPage8 class declaration
 */

class WizardPage8: public wxWizardPageSimple
{    
    DECLARE_DYNAMIC_CLASS( WizardPage8 )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    WizardPage8();

    WizardPage8( wxWizard* parent );

    /// Creation
    bool Create( wxWizard* parent );

    /// Destructor
    ~WizardPage8();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin WizardPage8 event handler declarations

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_LOCATE_FACE
    void OnCheckboxLocateFaceClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_ENABLE_WHEN_FACE_DETECTED
    void OnCheckboxEnableWhenFaceDetectedClick( wxCommandEvent& event );

////@end WizardPage8 event handler declarations

////@begin WizardPage8 member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end WizardPage8 member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin WizardPage8 member variables
    wxCheckBox* m_chkLocateFace;
    wxCheckBox* m_chkEnableWhenFaceDetected;
////@end WizardPage8 member variables
};

#endif
    // _WWIZARD_H_
