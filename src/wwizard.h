/////////////////////////////////////////////////////////////////////////////
// Name:        wwizard.h
// Purpose:     
// Author:      César Mauri Loba
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
////@end includes
#include <wx/timer.h>

/*!
 * Forward declarations
 */

////@begin forward declarations
class WizardPage;
class WizardPage1;
class WizardPage2;
class WizardPage3;
class WizardPage4;
////@end forward declarations
class CViacamController;

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_WWIZARD 10090
#define ID_WIZARDPAGE 10091
#define ID_CHECKBOX1 10092
#define ID_WIZARDPAGE1 10093
#define ID_CHECKBOX_PERFORM_CALIBRATION 10095
#define ID_WIZARDPAGE2 10096
#define ID_WIZARDPAGE3 10097
#define ID_RADIOBUTTON_NONE_CLICK 10098
#define ID_RADIOBUTTON_DWELL_CLICK 10099
#define ID_RADIOBUTTON_GESTURE_CLICK 10100
#define ID_BUTTON4 10101
#define wxID_STATIC_TEST 10102
#define ID_WIZARDPAGE4 10103
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
	inline WizardPage* GetNextPage();
	
	
	inline const bool GetRunAtStartup() const;
	inline void SetRunAtStartup(bool value);

	inline const bool GetPerformCalibration() const;
	inline void SetPerformCalibration(bool value);
	
	inline const EClickMode GetClickMode() const;
	inline void SetClickMode(EClickMode value);

private:
////@begin WWizard member variables
////@end WWizard member variables
	
	CViacamController* m_pViacamController;
	bool m_runWizardAtStartup;
	bool m_performCalibration;
	EClickMode m_clickMode;
	WizardPage* m_wizardPage3;
	
};

	inline CViacamController* WWizard::GetViacamController()
	{
		return m_pViacamController;
	}
	
	inline WizardPage* WWizard::GetNextPage()
	{
		return m_wizardPage3;
	}
	
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

    /// wxEVT_WIZARD_PAGE_CHANGED event handler for ID_WIZARDPAGE
    void OnWizardpageChanged( wxWizardEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX1
    void OnCheckboxRunWizardAtStartupClick( wxCommandEvent& event );

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
    wxCheckBox* m_chkRunWizardAtStartup;
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

    /// wxEVT_WIZARD_PAGE_CHANGED event handler for ID_WIZARDPAGE1
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

    /// wxEVT_WIZARD_PAGE_CHANGED event handler for ID_WIZARDPAGE2
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

    /// wxEVT_WIZARD_PAGE_CHANGED event handler for ID_WIZARDPAGE3
    void OnWizardpage3Changed( wxWizardEvent& event );

    /// wxEVT_WIZARD_PAGE_CHANGING event handler for ID_WIZARDPAGE3
    void OnWizardpage3Changing( wxWizardEvent& event );

    /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_NONE_CLICK
    void OnRadiobuttonNoneClickSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_DWELL_CLICK
    void OnRadiobuttonDwellClickSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_GESTURE_CLICK
    void OnRadiobuttonGestureClickSelected( wxCommandEvent& event );

    /// wxEVT_LEFT_DOWN event handler for ID_BUTTON4
    void OnLeftDown( wxMouseEvent& event );

    /// wxEVT_LEFT_UP event handler for ID_BUTTON4
    void OnLeftUp( wxMouseEvent& event );

    /// wxEVT_LEFT_DCLICK event handler for ID_BUTTON4
    void OnLeftDClick( wxMouseEvent& event );

    /// wxEVT_RIGHT_UP event handler for ID_BUTTON4
    void OnRightUp( wxMouseEvent& event );

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
    wxRadioButton* m_rbNoneClick;
    wxRadioButton* m_rbDwellClick;
    wxRadioButton* m_rbGestureClick;
    wxButton* m_buttonTest;
    wxStaticText* m_staticTest;
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
////@end WizardPage4 member variables
};

#endif
    // _WWIZARD_H_
