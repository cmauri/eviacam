/////////////////////////////////////////////////////////////////////////////
// Name:        wwizard.h
// Purpose:     
// Author:      C�sar Mauri Loba
// Modified by: 
// Created:     Mon 18 Oct 2010 09:57:22 CEST
// RCS-ID:      
// Copyright:   (C) 2008 Cesar Mauri from CREA Sistemes Informatics
// License:     
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
class WizardPage1;
class WizardPage2;
class WizardPage6;
class wxSpinCtrl;
class wxToggleButton;
class WizardPage7;
class WizardPage3;
class WizardPage4;
////@end forward declarations
class CViacamController;

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_WWIZARD 10090
#define ID_WIZARDPAGE_INI 10091
#define ID_WIZARDPAGE_CAMERA 10115
#define ID_BUTTON5 10124
#define ID_WIZARDPAGE_CALIB1 10093
#define ID_CHECKBOX_PERFORM_CALIBRATION 10095
#define ID_WIZARDPAGE_CALIB2 10096
#define ID_WIZARDPAGE_CALIB3 10116
#define ID_SPINCTRL 10118
#define ID_SPINCTRL1 10119
#define ID_TOGGLEBUTTON 10117
#define ID_WIZARDPAGE_CLICK1 10120
#define ID_WIZARDPAGE_CLICK2 10097
#define ID_RADIOBUTTON_DWELL_CLICK 10099
#define ID_RADIOBUTTON_GESTURE_CLICK 10100
#define ID_RADIOBUTTON_NONE_CLICK 10098
#define ID_TOGGLEBUTTON1 10121
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
    WWizard( wxWindow* parent, CViacamController* pViacamController, wxWindowID id = SYMBOL_WWIZARD_IDNAME, const wxPoint& pos = wxDefaultPosition );	

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_WWIZARD_IDNAME, const wxPoint& pos = wxDefaultPosition );

    /// Destructor
    ~WWizard();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin WWizard event handler declarations

    /// wxEVT_WIZARD_FINISHED event handler for ID_WWIZARD
    void OnWwizardFinished( wxWizardEvent& event );

////@end WWizard event handler declarations

////@begin WWizard member function declarations

    /// Runs the wizard
    bool Run();

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end WWizard member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();
	
	inline CViacamController* GetViacamController();
//	inline WizardPage* GetNextPage();
	
	
	inline const bool GetRunAtStartup() const;
	inline void SetRunAtStartup(bool value);

	inline const bool GetPerformCalibration() const;
	inline void SetPerformCalibration(bool value);
	
	inline const bool GetIsMotionEnabled() const;
	inline void SetIsMotionEnabled(bool value);

	inline const EClickMode GetClickMode() const;
	inline void SetClickMode(EClickMode value);

private:
////@begin WWizard member variables
////@end WWizard member variables
	
	CViacamController* m_pViacamController;
	bool m_runWizardAtStartup;
	bool m_performCalibration;
	bool m_isMotionEnabled;
	EClickMode m_clickMode;
//	WizardPage* m_wizardPage3;
	
};

	inline CViacamController* WWizard::GetViacamController()
	{
		return m_pViacamController;
	}
	/*
	inline WizardPage* WWizard::GetNextPage()
	{
		return m_wizardPage3;
	}*/
	
	inline const bool WWizard::GetRunAtStartup() const
	{
		return m_runWizardAtStartup;	
	}
	
	inline void WWizard::SetRunAtStartup(bool value)
	{
		m_runWizardAtStartup = value;
	}

	inline const bool WWizard::GetPerformCalibration() const
	{
		return m_performCalibration;	
	}
	
	inline void WWizard::SetPerformCalibration(bool value)
	{
		m_performCalibration = value;
	}

	inline const bool WWizard::GetIsMotionEnabled() const
	{
		return m_isMotionEnabled;	
	}
	
	inline void WWizard::SetIsMotionEnabled(bool value)
	{
		m_isMotionEnabled = value;
	}
	
	inline const WWizard::EClickMode WWizard::GetClickMode() const
	{
		return m_clickMode;	
	}
	
	inline void WWizard::SetClickMode(EClickMode value)
	{
		m_clickMode = value;
	}


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

////@begin WizardPage3 event handler declarations

    /// wxEVT_WIZARD_PAGE_CHANGED event handler for ID_WIZARDPAGE_CLICK2
    void OnWizardpage3Changed( wxWizardEvent& event );

    /// wxEVT_WIZARD_PAGE_CHANGING event handler for ID_WIZARDPAGE_CLICK2
    void OnWizardpage3Changing( wxWizardEvent& event );

    /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_DWELL_CLICK
    void OnRadiobuttonDwellClickSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_GESTURE_CLICK
    void OnRadiobuttonGestureClickSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_NONE_CLICK
    void OnRadiobuttonNoneClickSelected( wxCommandEvent& event );

    /// wxEVT_LEFT_DOWN event handler for ID_TOGGLEBUTTON1
    void OnLeftDownTest( wxMouseEvent& event );

    /// wxEVT_LEFT_UP event handler for ID_TOGGLEBUTTON1
    void OnLeftUpTest( wxMouseEvent& event );

    /// wxEVT_LEFT_DCLICK event handler for ID_TOGGLEBUTTON1
    void OnLeftDClickTest( wxMouseEvent& event );

    /// wxEVT_RIGHT_UP event handler for ID_TOGGLEBUTTON1
    void OnRightUpTest( wxMouseEvent& event );

////@end WizardPage3 event handler declarations

	void OnTimer(wxTimerEvent& event);

////@begin WizardPage3 member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end WizardPage3 member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin WizardPage3 member variables
    wxRadioButton* m_rbDwellClick;
    wxRadioButton* m_rbGestureClick;
    wxRadioButton* m_rbNoneClick;
    wxToggleButton* m_toggleTest;
////@end WizardPage3 member variables
	WWizard* m_wizardParent;
	wxTimer m_timer;
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

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_TOGGLEBUTTON
    void OnTogglebuttonClick( wxCommandEvent& event );

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
    wxToggleButton* m_toggleEnableMotion;
////@end WizardPage6 member variables
	WWizard* m_wizardParent;
};

/*!
 * WizardPage7 class declaration
 */

class WizardPage7: public wxWizardPageSimple
{    
    DECLARE_DYNAMIC_CLASS( WizardPage7 )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    WizardPage7();

    WizardPage7( wxWizard* parent );

    /// Creation
    bool Create( wxWizard* parent );

    /// Destructor
    ~WizardPage7();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin WizardPage7 event handler declarations

////@end WizardPage7 event handler declarations

////@begin WizardPage7 member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end WizardPage7 member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin WizardPage7 member variables
////@end WizardPage7 member variables
	WWizard* m_wizardParent;
};

#endif
    // _WWIZARD_H_
