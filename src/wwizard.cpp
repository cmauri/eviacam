/////////////////////////////////////////////////////////////////////////////
// Name:        wwizard.cpp
// Purpose:     
// Author:      César Mauri Loba
// Modified by: 
// Created:     Mon 18 Oct 2010 09:57:22 CEST
// RCS-ID:      
// Copyright:   (C) 2008 Cesar Mauri from CREA Sistemes Informatics
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "wwizard.h"
#include "viacamcontroller.h"
#include "mouseoutput.h"
#include <wx/timer.h>

////@begin XPM images
////@end XPM images

#define TIMER_ID 1234

/*!
 * WWizard type definition
 */

IMPLEMENT_DYNAMIC_CLASS( WWizard, wxWizard )


/*!
 * WWizard event table definition
 */

BEGIN_EVENT_TABLE( WWizard, wxWizard )

////@begin WWizard event table entries
    EVT_WIZARD_FINISHED( ID_WWIZARD, WWizard::OnWwizardFinished )

////@end WWizard event table entries

END_EVENT_TABLE()


/*!
 * WWizard constructors
 */

WWizard::WWizard()
{
    Init();
}

WWizard::WWizard( wxWindow* parent, wxWindowID id, const wxPoint& pos )
{
    Init();
    Create(parent, id, pos);
}

WWizard::WWizard( wxWindow* parent, CViacamController* pViacamController, wxWindowID id, const wxPoint& pos )
{
    Init();
    Create(parent, id, pos);
	m_pViacamController = pViacamController;
}


/*!
 * WWizard creator
 */

bool WWizard::Create( wxWindow* parent, wxWindowID id, const wxPoint& pos )
{
////@begin WWizard creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxBitmap wizardBitmap(GetBitmapResource(wxT("wizard.png")));
    wxWizard::Create( parent, id, _("Wizard"), wizardBitmap, pos, wxDEFAULT_DIALOG_STYLE|wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX );

    CreateControls();
////@end WWizard creation
    return true;
}


/*!
 * WWizard destructor
 */

WWizard::~WWizard()
{
////@begin WWizard destruction
////@end WWizard destruction
}


/*!
 * Member initialisation
 */

void WWizard::Init()
{
////@begin WWizard member initialisation
////@end WWizard member initialisation
	m_pViacamController = NULL;
	m_runWizardAtStartup = true;
	m_performCalibration = true;
}


/*!
 * Control creation for WWizard
 */

void WWizard::CreateControls()
{    
////@begin WWizard content construction
    WWizard* itemWizard1 = this;

    WizardPage* itemWizardPageSimple2 = new WizardPage( itemWizard1 );
    itemWizard1->GetPageAreaSizer()->Add(itemWizardPageSimple2);

    WizardPage1* itemWizardPageSimple6 = new WizardPage1( itemWizard1 );
    itemWizard1->GetPageAreaSizer()->Add(itemWizardPageSimple6);

    WizardPage2* itemWizardPageSimple11 = new WizardPage2( itemWizard1 );
    itemWizard1->GetPageAreaSizer()->Add(itemWizardPageSimple11);

    WizardPage3* itemWizardPageSimple13 = new WizardPage3( itemWizard1 );
    itemWizard1->GetPageAreaSizer()->Add(itemWizardPageSimple13);

    WizardPage4* itemWizardPageSimple22 = new WizardPage4( itemWizard1 );
    itemWizard1->GetPageAreaSizer()->Add(itemWizardPageSimple22);

    wxWizardPageSimple* lastPage = NULL;
    if (lastPage)
        wxWizardPageSimple::Chain(lastPage, itemWizardPageSimple2);
    lastPage = itemWizardPageSimple2;
    if (lastPage)
        wxWizardPageSimple::Chain(lastPage, itemWizardPageSimple6);
    lastPage = itemWizardPageSimple6;
    if (lastPage)
        wxWizardPageSimple::Chain(lastPage, itemWizardPageSimple11);
    lastPage = itemWizardPageSimple11;
    if (lastPage)
        wxWizardPageSimple::Chain(lastPage, itemWizardPageSimple13);
    lastPage = itemWizardPageSimple13;
    if (lastPage)
        wxWizardPageSimple::Chain(lastPage, itemWizardPageSimple22);
    lastPage = itemWizardPageSimple22;
////@end WWizard content construction

	m_wizardPage3 = (WizardPage*) itemWizardPageSimple13;
}


/*!
 * Runs the wizard.
 */

bool WWizard::Run()
{
    wxWindowList::compatibility_iterator node = GetChildren().GetFirst();

    while (node)
    {
        wxWizardPage* startPage = wxDynamicCast(node->GetData(), wxWizardPage);
    
		if (startPage)
		{
			FinishLayout();
			ShowPage(startPage);
			Show(true);
			return true;
		}
		node = node->GetNext();
    }
	
    return false;
}


/*!
 * Should we show tooltips?
 */

bool WWizard::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap WWizard::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin WWizard bitmap retrieval
    wxUnusedVar(name);
    if (name == _T("wizard.png"))
    {
        wxBitmap bitmap(_T("wizard.png"), wxBITMAP_TYPE_PNG);
        return bitmap;
    }
    return wxNullBitmap;
////@end WWizard bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon WWizard::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin WWizard icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end WWizard icon retrieval
}


/*!
 * WizardPage type definition
 */

IMPLEMENT_DYNAMIC_CLASS( WizardPage, wxWizardPageSimple )


/*!
 * WizardPage event table definition
 */

BEGIN_EVENT_TABLE( WizardPage, wxWizardPageSimple )

////@begin WizardPage event table entries
    EVT_WIZARD_PAGE_CHANGED( -1, WizardPage::OnWizardpageChanged )

    EVT_CHECKBOX( ID_CHECKBOX1, WizardPage::OnCheckboxRunWizardAtStartupClick )

////@end WizardPage event table entries

END_EVENT_TABLE()


/*!
 * WizardPage constructors
 */

WizardPage::WizardPage()
{
    Init();
}

WizardPage::WizardPage( wxWizard* parent )
{
    Init();
    Create( parent );
	m_wizardParent = (WWizard*) parent;
}


/*!
 * WizardPage creator
 */

bool WizardPage::Create( wxWizard* parent )
{
////@begin WizardPage creation
    wxBitmap wizardBitmap(wxNullBitmap);
    wxWizardPageSimple::Create( parent, NULL, NULL, wizardBitmap );

    CreateControls();
    if (GetSizer())
        GetSizer()->Fit(this);
////@end WizardPage creation
    return true;
}


/*!
 * WizardPage destructor
 */

WizardPage::~WizardPage()
{
////@begin WizardPage destruction
////@end WizardPage destruction
}


/*!
 * Member initialisation
 */

void WizardPage::Init()
{
////@begin WizardPage member initialisation
    m_chkRunWizardAtStartup = NULL;
////@end WizardPage member initialisation
}


/*!
 * Control creation for WizardPage
 */

void WizardPage::CreateControls()
{    
////@begin WizardPage content construction
    WizardPage* itemWizardPageSimple2 = this;

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxVERTICAL);
    itemWizardPageSimple2->SetSizer(itemBoxSizer3);

    wxStaticText* itemStaticText4 = new wxStaticText( itemWizardPageSimple2, wxID_STATIC, _("This wizard guides you through simple steps\nto configure eViacam."), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(itemStaticText4, 0, wxALIGN_LEFT|wxALL, 5);

    m_chkRunWizardAtStartup = new wxCheckBox( itemWizardPageSimple2, ID_CHECKBOX1, _("Run this wizard at startup"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkRunWizardAtStartup->SetValue(true);
    itemBoxSizer3->Add(m_chkRunWizardAtStartup, 0, wxALIGN_LEFT|wxALL, 5);

////@end WizardPage content construction
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX1
 */

void WizardPage::OnCheckboxRunWizardAtStartupClick( wxCommandEvent& event )
{
	m_wizardParent->SetRunAtStartup(m_chkRunWizardAtStartup->GetValue());
	event.Skip(false);
}


/*!
 * Should we show tooltips?
 */

bool WizardPage::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap WizardPage::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin WizardPage bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end WizardPage bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon WizardPage::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin WizardPage icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end WizardPage icon retrieval
}


/*!
 * WizardPage1 type definition
 */

IMPLEMENT_DYNAMIC_CLASS( WizardPage1, wxWizardPageSimple )


/*!
 * WizardPage1 event table definition
 */

BEGIN_EVENT_TABLE( WizardPage1, wxWizardPageSimple )

////@begin WizardPage1 event table entries
    EVT_WIZARD_PAGE_CHANGED( -1, WizardPage1::OnWizardpage1Changed )

    EVT_CHECKBOX( ID_CHECKBOX_PERFORM_CALIBRATION, WizardPage1::OnCheckboxPerformCalibrationClick )

////@end WizardPage1 event table entries

END_EVENT_TABLE()


/*!
 * WizardPage1 constructors
 */

WizardPage1::WizardPage1()
{
    Init();
}

WizardPage1::WizardPage1( wxWizard* parent )
{
    Init();
    Create( parent );
	m_wizardParent = (WWizard*) parent;
}


/*!
 * WizardPage1 creator
 */

bool WizardPage1::Create( wxWizard* parent )
{
////@begin WizardPage1 creation
    wxBitmap wizardBitmap(wxNullBitmap);
    wxWizardPageSimple::Create( parent, NULL, NULL, wizardBitmap );

    CreateControls();
    if (GetSizer())
        GetSizer()->Fit(this);
////@end WizardPage1 creation
    return true;
}


/*!
 * WizardPage1 destructor
 */

WizardPage1::~WizardPage1()
{
////@begin WizardPage1 destruction
////@end WizardPage1 destruction
}


/*!
 * Member initialisation
 */

void WizardPage1::Init()
{
////@begin WizardPage1 member initialisation
    m_chkPerformCalibration = NULL;
////@end WizardPage1 member initialisation
}


/*!
 * Control creation for WizardPage1
 */

void WizardPage1::CreateControls()
{    
////@begin WizardPage1 content construction
    WizardPage1* itemWizardPageSimple6 = this;

    wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxVERTICAL);
    itemWizardPageSimple6->SetSizer(itemBoxSizer7);

    wxStaticText* itemStaticText8 = new wxStaticText( itemWizardPageSimple6, wxID_STATIC, _("Calibration pointer:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer7->Add(itemStaticText8, 0, wxALIGN_LEFT|wxALL, 5);

    wxStaticText* itemStaticText9 = new wxStaticText( itemWizardPageSimple6, wxID_STATIC, _("eViacam will be enabled in next window to\ncalibrate the speed of the pointer.\n\nTo move the pointer you should move your head."), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer7->Add(itemStaticText9, 0, wxALIGN_LEFT|wxALL, 5);

    m_chkPerformCalibration = new wxCheckBox( itemWizardPageSimple6, ID_CHECKBOX_PERFORM_CALIBRATION, _("Perform calibration"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkPerformCalibration->SetValue(true);
    itemBoxSizer7->Add(m_chkPerformCalibration, 0, wxALIGN_LEFT|wxALL, 5);

////@end WizardPage1 content construction
}


/*!
 * Should we show tooltips?
 */

bool WizardPage1::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap WizardPage1::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin WizardPage1 bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end WizardPage1 bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon WizardPage1::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin WizardPage1 icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end WizardPage1 icon retrieval
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_PERFORM_CALIBRATION
 */

void WizardPage1::OnCheckboxPerformCalibrationClick( wxCommandEvent& event )
{
	m_wizardParent->SetPerformCalibration(m_chkPerformCalibration->GetValue());
	event.Skip(false);	
}




/*!
 * WizardPage2 type definition
 */

IMPLEMENT_DYNAMIC_CLASS( WizardPage2, wxWizardPageSimple )


/*!
 * WizardPage2 event table definition
 */

BEGIN_EVENT_TABLE( WizardPage2, wxWizardPageSimple )

////@begin WizardPage2 event table entries
    EVT_WIZARD_PAGE_CHANGED( -1, WizardPage2::OnWizardpage2Changed )

////@end WizardPage2 event table entries

END_EVENT_TABLE()


/*!
 * WizardPage2 constructors
 */

WizardPage2::WizardPage2()
{
    Init();
}

WizardPage2::WizardPage2( wxWizard* parent )
{
    Init();
    Create( parent );
	m_wizardParent = (WWizard*) parent;
}


/*!
 * WizardPage2 creator
 */

bool WizardPage2::Create( wxWizard* parent )
{
////@begin WizardPage2 creation
    wxBitmap wizardBitmap(wxNullBitmap);
    wxWizardPageSimple::Create( parent, NULL, NULL, wizardBitmap );

    CreateControls();
////@end WizardPage2 creation
    return true;
}


/*!
 * WizardPage2 destructor
 */

WizardPage2::~WizardPage2()
{
////@begin WizardPage2 destruction
////@end WizardPage2 destruction
}


/*!
 * Member initialisation
 */

void WizardPage2::Init()
{
////@begin WizardPage2 member initialisation
////@end WizardPage2 member initialisation
}


/*!
 * Control creation for WizardPage2
 */

void WizardPage2::CreateControls()
{    
////@begin WizardPage2 content construction
    WizardPage2* itemWizardPageSimple11 = this;

    wxStaticText* itemStaticText12 = new wxStaticText( itemWizardPageSimple11, wxID_STATIC, _("Calibration pointer:"), wxDefaultPosition, wxDefaultSize, 0 );

////@end WizardPage2 content construction
}


/*!
 * Should we show tooltips?
 */

bool WizardPage2::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap WizardPage2::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin WizardPage2 bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end WizardPage2 bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon WizardPage2::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin WizardPage2 icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end WizardPage2 icon retrieval
}


/*!
 * wxEVT_WIZARD_PAGE_CHANGED event handler for ID_WIZARDPAGE2
 */

void WizardPage2::OnWizardpage2Changed( wxWizardEvent& event )
{
	if (m_wizardParent->GetPerformCalibration() && event.GetDirection())
		m_wizardParent->GetViacamController()->StartMotionCalibration();

		if (event.GetDirection())
			m_wizardParent->ShowPage(GetNext());
		else
			m_wizardParent->ShowPage(GetPrev());
}



/*!
 * WizardPage3 type definition
 */

IMPLEMENT_DYNAMIC_CLASS( WizardPage3, wxWizardPageSimple )


/*!
 * WizardPage3 event table definition
 */

BEGIN_EVENT_TABLE( WizardPage3, wxWizardPageSimple )

////@begin WizardPage3 event table entries
    EVT_WIZARD_PAGE_CHANGED( -1, WizardPage3::OnWizardpage3Changed )
    EVT_WIZARD_PAGE_CHANGING( -1, WizardPage3::OnWizardpage3Changing )

    EVT_RADIOBUTTON( ID_RADIOBUTTON_NONE_CLICK, WizardPage3::OnRadiobuttonNoneClickSelected )

    EVT_RADIOBUTTON( ID_RADIOBUTTON_DWELL_CLICK, WizardPage3::OnRadiobuttonDwellClickSelected )

    EVT_RADIOBUTTON( ID_RADIOBUTTON_GESTURE_CLICK, WizardPage3::OnRadiobuttonGestureClickSelected )

////@end WizardPage3 event table entries

	EVT_TIMER(TIMER_ID, WizardPage3::OnTimer)

END_EVENT_TABLE()


/*!
 * WizardPage3 constructors
 */

WizardPage3::WizardPage3() : m_timer(this, TIMER_ID)
{
    Init();
}

WizardPage3::WizardPage3( wxWizard* parent ) : m_timer(this, TIMER_ID)
{
    Init();
    Create( parent );
	m_wizardParent = (WWizard*) parent;
}


/*!
 * WizardPage3 creator
 */

bool WizardPage3::Create( wxWizard* parent )
{
////@begin WizardPage3 creation
    wxBitmap wizardBitmap(wxNullBitmap);
    wxWizardPageSimple::Create( parent, NULL, NULL, wizardBitmap );

    CreateControls();
    if (GetSizer())
        GetSizer()->Fit(this);
////@end WizardPage3 creation
    return true;
}


/*!
 * WizardPage3 destructor
 */

WizardPage3::~WizardPage3()
{
////@begin WizardPage3 destruction
////@end WizardPage3 destruction
}


/*!
 * Member initialisation
 */

void WizardPage3::Init()
{
////@begin WizardPage3 member initialisation
    m_rbNoneClick = NULL;
    m_rbDwellClick = NULL;
    m_rbGestureClick = NULL;
    m_buttonTest = NULL;
    m_staticTest = NULL;
////@end WizardPage3 member initialisation
}


/*!
 * Control creation for WizardPage3
 */

void WizardPage3::CreateControls()
{    
////@begin WizardPage3 content construction
    WizardPage3* itemWizardPageSimple13 = this;

    wxBoxSizer* itemBoxSizer14 = new wxBoxSizer(wxVERTICAL);
    itemWizardPageSimple13->SetSizer(itemBoxSizer14);

    wxStaticText* itemStaticText15 = new wxStaticText( itemWizardPageSimple13, wxID_STATIC, _("Click mode:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer14->Add(itemStaticText15, 0, wxALIGN_LEFT|wxALL, 5);

    m_rbNoneClick = new wxRadioButton( itemWizardPageSimple13, ID_RADIOBUTTON_NONE_CLICK, _("None: click function is disabled."), wxDefaultPosition, wxDefaultSize, 0 );
    m_rbNoneClick->SetValue(false);
    itemBoxSizer14->Add(m_rbNoneClick, 0, wxALIGN_LEFT|wxALL, 5);

    m_rbDwellClick = new wxRadioButton( itemWizardPageSimple13, ID_RADIOBUTTON_DWELL_CLICK, _("Dwell: the click occurs when the mouse remains\nmotionless for a preset time. Using the click\nwindow, the user can indicate the type of click."), wxDefaultPosition, wxDefaultSize, 0 );
    m_rbDwellClick->SetValue(false);
    itemBoxSizer14->Add(m_rbDwellClick, 0, wxALIGN_LEFT|wxALL, 5);

    m_rbGestureClick = new wxRadioButton( itemWizardPageSimple13, ID_RADIOBUTTON_GESTURE_CLICK, _("Gesture: This method contains two steps.\nThe first one is a dwell time with the mouse\nremaining motionless. After this period, the\nuser should move the head to a direction to\nindicate the type of click."), wxDefaultPosition, wxDefaultSize, 0 );
    m_rbGestureClick->SetValue(false);
    itemBoxSizer14->Add(m_rbGestureClick, 0, wxALIGN_LEFT|wxALL, 5);

    wxBoxSizer* itemBoxSizer19 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer14->Add(itemBoxSizer19, 0, wxALIGN_LEFT|wxALL, 5);

    m_buttonTest = new wxButton( itemWizardPageSimple13, ID_BUTTON4, _("TEST"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer19->Add(m_buttonTest, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_staticTest = new wxStaticText( itemWizardPageSimple13, wxID_STATIC_TEST, _(" "), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer19->Add(m_staticTest, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    // Connect events and objects
    m_buttonTest->Connect(ID_BUTTON4, wxEVT_LEFT_DOWN, wxMouseEventHandler(WizardPage3::OnLeftDown), NULL, this);
    m_buttonTest->Connect(ID_BUTTON4, wxEVT_LEFT_UP, wxMouseEventHandler(WizardPage3::OnLeftUp), NULL, this);
    m_buttonTest->Connect(ID_BUTTON4, wxEVT_LEFT_DCLICK, wxMouseEventHandler(WizardPage3::OnLeftDClick), NULL, this);
    m_buttonTest->Connect(ID_BUTTON4, wxEVT_RIGHT_UP, wxMouseEventHandler(WizardPage3::OnRightUp), NULL, this);
////@end WizardPage3 content construction
}


/*!
 * Should we show tooltips?
 */

bool WizardPage3::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap WizardPage3::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin WizardPage3 bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end WizardPage3 bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon WizardPage3::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin WizardPage3 icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end WizardPage3 icon retrieval
}


/*!
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_NONE_CLICK
 */

void WizardPage3::OnRadiobuttonNoneClickSelected( wxCommandEvent& event )
{
	m_rbDwellClick->SetValue(false);
	m_rbGestureClick->SetValue(false);
	m_wizardParent->SetClickMode(WWizard::NONE);
	m_wizardParent->GetViacamController()->GetClickWindowController()->Show(false);
	m_wizardParent->GetViacamController()->GetMouseOutput()->SetClickMode(CMouseOutput::NONE);
	event.Skip(false);
}


/*!
 * wxEVT_WIZARD_PAGE_CHANGED event handler for ID_WIZARDPAGE3
 */

void WizardPage3::OnWizardpage3Changed( wxWizardEvent& event )
{
	m_wizardParent->GetViacamController()->SetEnabled(true);
	m_rbNoneClick->SetValue(true);
	m_rbDwellClick->SetValue(false);
	m_rbGestureClick->SetValue(false);

	m_wizardParent->SetClickMode(WWizard::NONE);
	m_wizardParent->GetViacamController()->GetMouseOutput()->SetClickMode(CMouseOutput::NONE);
	event.Skip(false);
}


/*!
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_DWELL_CLICK
 */

void WizardPage3::OnRadiobuttonDwellClickSelected( wxCommandEvent& event )
{
	m_rbNoneClick->SetValue(false);
	m_rbGestureClick->SetValue(false);
	m_wizardParent->SetClickMode(WWizard::DWELL);
	m_wizardParent->GetViacamController()->GetClickWindowController()->Show(true);
	m_wizardParent->GetViacamController()->GetMouseOutput()->SetClickMode(CMouseOutput::DWELL);
	event.Skip(false);
}


/*!
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_GESTURE_CLICK
 */

void WizardPage3::OnRadiobuttonGestureClickSelected( wxCommandEvent& event )
{
	m_rbNoneClick->SetValue(false);
	m_rbDwellClick->SetValue(false);
	m_wizardParent->SetClickMode(WWizard::GESTURE);
	m_wizardParent->GetViacamController()->GetClickWindowController()->Show(false);
	m_wizardParent->GetViacamController()->GetMouseOutput()->SetClickMode(CMouseOutput::GESTURE);
	event.Skip(false);
}


/*!
 * wxEVT_WIZARD_PAGE_CHANGING event handler for ID_WIZARDPAGE3
 */

void WizardPage3::OnWizardpage3Changing( wxWizardEvent& event )
{
    m_wizardParent->GetViacamController()->SetEnabled(false, true);
	m_wizardParent->GetViacamController()->GetClickWindowController()->Show(false);
	event.Skip(false);
}



/*!
 * WizardPage4 type definition
 */

IMPLEMENT_DYNAMIC_CLASS( WizardPage4, wxWizardPageSimple )


/*!
 * WizardPage4 event table definition
 */

BEGIN_EVENT_TABLE( WizardPage4, wxWizardPageSimple )

////@begin WizardPage4 event table entries
////@end WizardPage4 event table entries

END_EVENT_TABLE()


/*!
 * WizardPage4 constructors
 */

WizardPage4::WizardPage4()
{
    Init();
}

WizardPage4::WizardPage4( wxWizard* parent )
{
    Init();
    Create( parent );
}


/*!
 * WizardPage4 creator
 */

bool WizardPage4::Create( wxWizard* parent )
{
////@begin WizardPage4 creation
    wxBitmap wizardBitmap(wxNullBitmap);
    wxWizardPageSimple::Create( parent, NULL, NULL, wizardBitmap );

    CreateControls();
    if (GetSizer())
        GetSizer()->Fit(this);
////@end WizardPage4 creation
    return true;
}


/*!
 * WizardPage4 destructor
 */

WizardPage4::~WizardPage4()
{
////@begin WizardPage4 destruction
////@end WizardPage4 destruction
}


/*!
 * Member initialisation
 */

void WizardPage4::Init()
{
////@begin WizardPage4 member initialisation
////@end WizardPage4 member initialisation
}


/*!
 * Control creation for WizardPage4
 */

void WizardPage4::CreateControls()
{    
////@begin WizardPage4 content construction
    WizardPage4* itemWizardPageSimple22 = this;

    wxBoxSizer* itemBoxSizer23 = new wxBoxSizer(wxVERTICAL);
    itemWizardPageSimple22->SetSizer(itemBoxSizer23);

    wxStaticText* itemStaticText24 = new wxStaticText( itemWizardPageSimple22, wxID_STATIC, _("Congratulations!\nYou have completed the configuration wizard.\n\nYou can change the settings through\nConfiguration -> Options"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer23->Add(itemStaticText24, 0, wxALIGN_LEFT|wxALL, 5);

////@end WizardPage4 content construction
}


/*!
 * Should we show tooltips?
 */

bool WizardPage4::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap WizardPage4::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin WizardPage4 bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end WizardPage4 bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon WizardPage4::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin WizardPage4 icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end WizardPage4 icon retrieval
}



void WizardPage3::OnTimer(wxTimerEvent& event)
{
	m_staticTest->SetLabel(wxT(" "));
	m_timer.Stop();
	event.Skip(false);
}


/*!
 * wxEVT_RIGHT_UP event handler for ID_BUTTON4
 */

void WizardPage3::OnRightUp( wxMouseEvent& event )
{
	m_staticTest->SetLabel(wxT("Right click"));
	m_timer.Start(1000);
    event.Skip(false);
}


/*!
 * wxEVT_LEFT_DOWN event handler for ID_BUTTON4
 */

void WizardPage3::OnLeftDown( wxMouseEvent& event )
{
	m_staticTest->SetLabel(wxT("Drag"));
	m_timer.Start(1000);
    event.Skip(false);
}


/*!
 * wxEVT_LEFT_UP event handler for ID_BUTTON4
 */

void WizardPage3::OnLeftUp( wxMouseEvent& event )
{
	if (m_staticTest->GetLabel() != wxT("Double click"))
	{
		if (m_staticTest->GetLabel() == wxT("Drag"))
			m_staticTest->SetLabel(wxT("Left click"));
		else
			m_staticTest->SetLabel(wxT("Drop"));
		m_timer.Start(1000);
		event.Skip(false);
	}	
}


/*!
 * wxEVT_LEFT_DCLICK event handler for ID_BUTTON4
 */

void WizardPage3::OnLeftDClick( wxMouseEvent& event )
{
	m_staticTest->SetLabel(wxT("Double click"));
	m_timer.Start(1000);
    event.Skip(false);
}


/*!
 * wxEVT_WIZARD_FINISHED event handler for ID_WWIZARD
 */

void WWizard::OnWwizardFinished( wxWizardEvent& event )
{
	GetViacamController()->SetRunWizardAtStartup(m_runWizardAtStartup);
	GetViacamController()->GetConfigManager()->WriteAll();
    event.Skip(false);
}


/*!
 * wxEVT_WIZARD_PAGE_CHANGED event handler for ID_WIZARDPAGE
 */

void WizardPage::OnWizardpageChanged( wxWizardEvent& event )
{
    m_chkRunWizardAtStartup->SetValue(m_wizardParent->GetViacamController()->GetRunWizardAtStartup());
	event.Skip(false);
}


/*!
 * wxEVT_WIZARD_PAGE_CHANGED event handler for ID_WIZARDPAGE1
 */

void WizardPage1::OnWizardpage1Changed( wxWizardEvent& event )
{
    m_wizardParent->SetPerformCalibration(true);
	m_chkPerformCalibration->SetValue(true);
	event.Skip(false);
}

