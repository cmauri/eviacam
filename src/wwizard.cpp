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
#define SINGLE 1
#define SECONDARY 2
#define DOUBLE 3
#define DRAG 4

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
	m_pViacamController = pViacamController;
    Create(parent, id, pos);
}


/*!
 * WWizard creator
 */

bool WWizard::Create( wxWindow* parent, wxWindowID id, const wxPoint& pos )
{
////@begin WWizard creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxBitmap wizardBitmap(wxNullBitmap);
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
	m_isMotionEnabled = false;
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

    WizardPage5* itemWizardPageSimple9 = new WizardPage5( itemWizard1 );
    itemWizard1->GetPageAreaSizer()->Add(itemWizardPageSimple9);

    WizardPage1* itemWizardPageSimple19 = new WizardPage1( itemWizard1 );
    itemWizard1->GetPageAreaSizer()->Add(itemWizardPageSimple19);

    WizardPage2* itemWizardPageSimple26 = new WizardPage2( itemWizard1 );
    itemWizard1->GetPageAreaSizer()->Add(itemWizardPageSimple26);

    WizardPage6* itemWizardPageSimple31 = new WizardPage6( itemWizard1 );
    itemWizard1->GetPageAreaSizer()->Add(itemWizardPageSimple31);

    WizardPage7* itemWizardPageSimple44 = new WizardPage7( itemWizard1 );
    itemWizard1->GetPageAreaSizer()->Add(itemWizardPageSimple44);

    WizardPage3* itemWizardPageSimple50 = new WizardPage3( itemWizard1 );
    itemWizard1->GetPageAreaSizer()->Add(itemWizardPageSimple50);

    WizardPage4* itemWizardPageSimple60 = new WizardPage4( itemWizard1 );
    itemWizard1->GetPageAreaSizer()->Add(itemWizardPageSimple60);

    wxWizardPageSimple* lastPage = NULL;
    if (lastPage)
        wxWizardPageSimple::Chain(lastPage, itemWizardPageSimple2);
    lastPage = itemWizardPageSimple2;
    if (lastPage)
        wxWizardPageSimple::Chain(lastPage, itemWizardPageSimple9);
    lastPage = itemWizardPageSimple9;
    if (lastPage)
        wxWizardPageSimple::Chain(lastPage, itemWizardPageSimple19);
    lastPage = itemWizardPageSimple19;
    if (lastPage)
        wxWizardPageSimple::Chain(lastPage, itemWizardPageSimple26);
    lastPage = itemWizardPageSimple26;
    if (lastPage)
        wxWizardPageSimple::Chain(lastPage, itemWizardPageSimple31);
    lastPage = itemWizardPageSimple31;
    if (lastPage)
        wxWizardPageSimple::Chain(lastPage, itemWizardPageSimple44);
    lastPage = itemWizardPageSimple44;
    if (lastPage)
        wxWizardPageSimple::Chain(lastPage, itemWizardPageSimple50);
    lastPage = itemWizardPageSimple50;
    if (lastPage)
        wxWizardPageSimple::Chain(lastPage, itemWizardPageSimple60);
    lastPage = itemWizardPageSimple60;
////@end WWizard content construction
	m_isMotionEnabled = m_pViacamController->GetEnabled();
	m_wizardPage3 = (WizardPage*) itemWizardPageSimple50;
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
	m_wizardParent = (WWizard*) parent;
    Create( parent );
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

    wxStaticText* itemStaticText4 = new wxStaticText( itemWizardPageSimple2, wxID_STATIC, _("Welcome to the eViacam configuration wizard"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText4->SetFont(wxFont(12, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Sans")));
    itemBoxSizer3->Add(itemStaticText4, 0, wxALIGN_LEFT|wxALL, 5);

    wxStaticLine* itemStaticLine5 = new wxStaticLine( itemWizardPageSimple2, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
    itemBoxSizer3->Add(itemStaticLine5, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText6 = new wxStaticText( itemWizardPageSimple2, wxID_STATIC, _("This wizard will guide you through the basic configuration of eViacam."), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(itemStaticText6, 0, wxALIGN_LEFT|wxALL, 5);

    itemBoxSizer3->Add(5, 230, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText8 = new wxStaticText( itemWizardPageSimple2, wxID_STATIC, _("To continue, click Next."), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(itemStaticText8, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

////@end WizardPage content construction
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
	m_wizardParent = (WWizard*) parent;
	Create( parent );
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
    WizardPage1* itemWizardPageSimple19 = this;

    wxBoxSizer* itemBoxSizer20 = new wxBoxSizer(wxVERTICAL);
    itemWizardPageSimple19->SetSizer(itemBoxSizer20);

    wxStaticText* itemStaticText21 = new wxStaticText( itemWizardPageSimple19, wxID_STATIC, _("Pointer calibration"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText21->SetFont(wxFont(10, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Sans")));
    itemBoxSizer20->Add(itemStaticText21, 0, wxALIGN_LEFT|wxRIGHT|wxTOP|wxBOTTOM, 5);

    wxStaticText* itemStaticText22 = new wxStaticText( itemWizardPageSimple19, wxID_STATIC, _("Set the pointer speed"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer20->Add(itemStaticText22, 0, wxALIGN_LEFT|wxALL, 5);

    wxStaticLine* itemStaticLine23 = new wxStaticLine( itemWizardPageSimple19, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
    itemBoxSizer20->Add(itemStaticLine23, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText24 = new wxStaticText( itemWizardPageSimple19, wxID_STATIC, _("Attention, eViacam will be enabled in the next window to calibrate\nthe speed of the pointer.\n\nIf you perform the automatic calibration, the system will ask you to\nmove your head to calibrate the pointer speed. After this calibration,\nyou may adjust the pointer speed manually."), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer20->Add(itemStaticText24, 0, wxALIGN_LEFT|wxALL, 5);

    m_chkPerformCalibration = new wxCheckBox( itemWizardPageSimple19, ID_CHECKBOX_PERFORM_CALIBRATION, _("Perform automatic calibration"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkPerformCalibration->SetValue(true);
    if (WizardPage1::ShowToolTips())
        m_chkPerformCalibration->SetToolTip(_("If checked the next window allows to calibrate the speed of the pointer."));
    itemBoxSizer20->Add(m_chkPerformCalibration, 0, wxALIGN_LEFT|wxALL, 5);

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
	m_wizardParent = (WWizard*) parent;
    Create( parent );
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
    if (GetSizer())
        GetSizer()->Fit(this);
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
    WizardPage2* itemWizardPageSimple26 = this;

    wxBoxSizer* itemBoxSizer27 = new wxBoxSizer(wxVERTICAL);
    itemWizardPageSimple26->SetSizer(itemBoxSizer27);

    wxStaticText* itemStaticText28 = new wxStaticText( itemWizardPageSimple26, wxID_STATIC, _("Pointer calibration "), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText28->SetFont(wxFont(10, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Sans")));
    itemBoxSizer27->Add(itemStaticText28, 0, wxALIGN_LEFT|wxRIGHT|wxTOP|wxBOTTOM, 5);

    wxStaticText* itemStaticText29 = new wxStaticText( itemWizardPageSimple26, wxID_STATIC, _("Set the pointer speed"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer27->Add(itemStaticText29, 0, wxALIGN_LEFT|wxALL, 5);

    wxStaticLine* itemStaticLine30 = new wxStaticLine( itemWizardPageSimple26, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
    itemBoxSizer27->Add(itemStaticLine30, 0, wxGROW|wxALL, 5);

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
	{
		m_wizardParent->SetIsMotionEnabled(m_wizardParent->GetViacamController()->GetEnabled());
		m_wizardParent->GetViacamController()->StartMotionCalibration();
		m_wizardParent->GetViacamController()->SetEnabled(m_wizardParent->GetIsMotionEnabled());
	}	


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

    EVT_RADIOBUTTON( ID_RADIOBUTTON_DWELL_CLICK, WizardPage3::OnRadiobuttonDwellClickSelected )

    EVT_RADIOBUTTON( ID_RADIOBUTTON_GESTURE_CLICK, WizardPage3::OnRadiobuttonGestureClickSelected )

    EVT_RADIOBUTTON( ID_RADIOBUTTON_NONE_CLICK, WizardPage3::OnRadiobuttonNoneClickSelected )

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
	m_wizardParent = (WWizard*) parent;
    Create( parent );
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
    m_rbDwellClick = NULL;
    m_rbGestureClick = NULL;
    m_rbNoneClick = NULL;
    m_toggleTest = NULL;
////@end WizardPage3 member initialisation
}


/*!
 * Control creation for WizardPage3
 */

void WizardPage3::CreateControls()
{    
////@begin WizardPage3 content construction
    WizardPage3* itemWizardPageSimple50 = this;

    wxBoxSizer* itemBoxSizer51 = new wxBoxSizer(wxVERTICAL);
    itemWizardPageSimple50->SetSizer(itemBoxSizer51);

    wxStaticText* itemStaticText52 = new wxStaticText( itemWizardPageSimple50, wxID_STATIC, _("Click mode"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText52->SetFont(wxFont(10, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Sans")));
    itemBoxSizer51->Add(itemStaticText52, 0, wxALIGN_LEFT|wxRIGHT|wxTOP|wxBOTTOM, 5);

    wxStaticText* itemStaticText53 = new wxStaticText( itemWizardPageSimple50, wxID_STATIC, _("How to make the click"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer51->Add(itemStaticText53, 0, wxALIGN_LEFT|wxALL, 5);

    wxStaticLine* itemStaticLine54 = new wxStaticLine( itemWizardPageSimple50, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
    itemBoxSizer51->Add(itemStaticLine54, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText55 = new wxStaticText( itemWizardPageSimple50, wxID_STATIC, _("Please select the click mode that best suits your needs."), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer51->Add(itemStaticText55, 0, wxALIGN_LEFT|wxALL, 5);

    m_rbDwellClick = new wxRadioButton( itemWizardPageSimple50, ID_RADIOBUTTON_DWELL_CLICK, _("Dwell: the click occurs when the mouse remains motionless for a\npreset time. Using the click window, the user can indicate the\ntype of click."), wxDefaultPosition, wxDefaultSize, 0 );
    m_rbDwellClick->SetValue(true);
    itemBoxSizer51->Add(m_rbDwellClick, 0, wxALIGN_LEFT|wxALL, 5);

    m_rbGestureClick = new wxRadioButton( itemWizardPageSimple50, ID_RADIOBUTTON_GESTURE_CLICK, _("Gesture: this method contains two steps. The first one, consists in\na dwell time with the mouse remaining motionless. After this\nperiod, the user should move the head to a direction to indicate\nthe type of click."), wxDefaultPosition, wxDefaultSize, 0 );
    m_rbGestureClick->SetValue(false);
    itemBoxSizer51->Add(m_rbGestureClick, 0, wxALIGN_LEFT|wxALL, 5);

    m_rbNoneClick = new wxRadioButton( itemWizardPageSimple50, ID_RADIOBUTTON_NONE_CLICK, _("None: click function is disabled."), wxDefaultPosition, wxDefaultSize, 0 );
    m_rbNoneClick->SetValue(false);
    itemBoxSizer51->Add(m_rbNoneClick, 0, wxALIGN_LEFT|wxALL, 5);

    m_toggleTest = new wxToggleButton( itemWizardPageSimple50, ID_TOGGLEBUTTON1, _("Click here to test"), wxDefaultPosition, wxSize(150, 50), 0 );
    m_toggleTest->SetValue(false);
    itemBoxSizer51->Add(m_toggleTest, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    // Connect events and objects
    m_toggleTest->Connect(ID_TOGGLEBUTTON1, wxEVT_LEFT_DOWN, wxMouseEventHandler(WizardPage3::OnLeftDownTest), NULL, this);
    m_toggleTest->Connect(ID_TOGGLEBUTTON1, wxEVT_LEFT_UP, wxMouseEventHandler(WizardPage3::OnLeftUpTest), NULL, this);
    m_toggleTest->Connect(ID_TOGGLEBUTTON1, wxEVT_LEFT_DCLICK, wxMouseEventHandler(WizardPage3::OnLeftDClickTest), NULL, this);
    m_toggleTest->Connect(ID_TOGGLEBUTTON1, wxEVT_RIGHT_UP, wxMouseEventHandler(WizardPage3::OnRightUpTest), NULL, this);
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
	m_wizardParent->GetViacamController()->GetMouseOutput()->SetActionLeft (SINGLE);
	m_wizardParent->GetViacamController()->GetMouseOutput()->SetActionRight (SECONDARY);
	m_wizardParent->GetViacamController()->GetMouseOutput()->SetActionTop (DOUBLE);
	m_wizardParent->GetViacamController()->GetMouseOutput()->SetActionBottom (DRAG);	
	
	m_wizardParent->SetIsMotionEnabled(m_wizardParent->GetViacamController()->GetEnabled());
	m_wizardParent->GetViacamController()->SetEnabled(true);
	m_rbDwellClick->SetValue(true);
	m_rbGestureClick->SetValue(false);
	m_rbNoneClick->SetValue(false);

	m_wizardParent->GetViacamController()->GetClickWindowController()->Show(true);	
	m_wizardParent->SetClickMode(WWizard::DWELL);
	m_wizardParent->GetViacamController()->GetMouseOutput()->SetClickMode(CMouseOutput::DWELL);
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
    m_wizardParent->GetViacamController()->SetEnabled(m_wizardParent->GetIsMotionEnabled(), true);
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
    EVT_WIZARD_PAGE_CHANGED( -1, WizardPage4::OnWizardpage4PageChanged )

    EVT_CHECKBOX( ID_CHECKBOX1, WizardPage4::OnCheckboxRunWizardAtStartupClick )

#if defined(__WXGTK__)
    EVT_CHECKBOX( ID_CHECKBOX3, WizardPage4::OnCheckboxStartupClick )
#endif

    EVT_CHECKBOX( ID_CHECKBOX4, WizardPage4::OnCheckboxEnabledAtStartupClick )

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
	m_wizardParent = (WWizard*) parent;
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
    m_chkRunWizardAtStartup = NULL;
#if defined(__WXGTK__)
    m_chkStartup = NULL;
#endif
    m_chkEnabledAtStartup = NULL;
////@end WizardPage4 member initialisation
}


/*!
 * Control creation for WizardPage4
 */

void WizardPage4::CreateControls()
{    
////@begin WizardPage4 content construction
    WizardPage4* itemWizardPageSimple60 = this;

    wxBoxSizer* itemBoxSizer61 = new wxBoxSizer(wxVERTICAL);
    itemWizardPageSimple60->SetSizer(itemBoxSizer61);

    wxStaticText* itemStaticText62 = new wxStaticText( itemWizardPageSimple60, wxID_STATIC, _("Wizard completed"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText62->SetFont(wxFont(10, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Sans")));
    itemBoxSizer61->Add(itemStaticText62, 0, wxALIGN_LEFT|wxRIGHT|wxTOP|wxBOTTOM, 5);

    wxStaticLine* itemStaticLine63 = new wxStaticLine( itemWizardPageSimple60, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
    itemBoxSizer61->Add(itemStaticLine63, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText64 = new wxStaticText( itemWizardPageSimple60, wxID_STATIC, _("Congratulations, you have completed the configuration wizard. You\nmay change the settings through Configuration -> Options."), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer61->Add(itemStaticText64, 0, wxALIGN_LEFT|wxALL, 5);

    m_chkRunWizardAtStartup = new wxCheckBox( itemWizardPageSimple60, ID_CHECKBOX1, _("Run this wizard at startup"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkRunWizardAtStartup->SetValue(true);
    if (WizardPage4::ShowToolTips())
        m_chkRunWizardAtStartup->SetToolTip(_("If checked the wizard automatically appears at startup."));
    itemBoxSizer61->Add(m_chkRunWizardAtStartup, 0, wxALIGN_LEFT|wxALL, 5);

#if defined(__WXGTK__)
    m_chkStartup = new wxCheckBox( itemWizardPageSimple60, ID_CHECKBOX3, _("Start eViacam at user logon"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkStartup->SetValue(false);
    itemBoxSizer61->Add(m_chkStartup, 0, wxALIGN_LEFT|wxALL, 5);

#endif

    m_chkEnabledAtStartup = new wxCheckBox( itemWizardPageSimple60, ID_CHECKBOX4, _("Enable motion at startup"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkEnabledAtStartup->SetValue(false);
    if (WizardPage4::ShowToolTips())
        m_chkEnabledAtStartup->SetToolTip(_("If checked the program automatically \nenables cursor control at startup."));
    itemBoxSizer61->Add(m_chkEnabledAtStartup, 0, wxALIGN_LEFT|wxALL, 5);

    itemBoxSizer61->Add(5, 120, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText69 = new wxStaticText( itemWizardPageSimple60, wxID_STATIC, _("Click finish to exit the wizard."), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer61->Add(itemStaticText69, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

////@end WizardPage4 content construction
	m_chkEnabledAtStartup->SetValue(m_wizardParent->GetViacamController()->GetEnabledAtStartup());
	m_chkStartup->SetValue(m_wizardParent->GetViacamController()->GetAutostart()->IsEnabled());
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
	m_toggleTest->SetLabel(wxT("Click here to test"));
	m_toggleTest->SetValue(false);
	m_timer.Stop();
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
 * wxEVT_WIZARD_PAGE_CHANGED event handler for ID_WIZARDPAGE1
 */

void WizardPage1::OnWizardpage1Changed( wxWizardEvent& event )
{
    m_wizardParent->SetPerformCalibration(true);
	m_chkPerformCalibration->SetValue(true);
	event.Skip(false);
}



/*!
 * WizardPage5 type definition
 */

IMPLEMENT_DYNAMIC_CLASS( WizardPage5, wxWizardPageSimple )


/*!
 * WizardPage5 event table definition
 */

BEGIN_EVENT_TABLE( WizardPage5, wxWizardPageSimple )

////@begin WizardPage5 event table entries
    EVT_BUTTON( ID_BUTTON5, WizardPage5::OnButtonCameraSettingsClick )

////@end WizardPage5 event table entries

END_EVENT_TABLE()


/*!
 * WizardPage5 constructors
 */

WizardPage5::WizardPage5()
{
    Init();
}

WizardPage5::WizardPage5( wxWizard* parent )
{
    Init();
	m_wizardParent = (WWizard*) parent;
    Create( parent );
	
}


/*!
 * WizardPage5 creator
 */

bool WizardPage5::Create( wxWizard* parent )
{
////@begin WizardPage5 creation
    wxBitmap wizardBitmap(wxNullBitmap);
    wxWizardPageSimple::Create( parent, NULL, NULL, wizardBitmap );

    CreateControls();
    if (GetSizer())
        GetSizer()->Fit(this);
////@end WizardPage5 creation
    return true;
}


/*!
 * WizardPage5 destructor
 */

WizardPage5::~WizardPage5()
{
////@begin WizardPage5 destruction
////@end WizardPage5 destruction
}


/*!
 * Member initialisation
 */

void WizardPage5::Init()
{
////@begin WizardPage5 member initialisation
    m_staticCameraName = NULL;
    m_staticFramerate = NULL;
    m_btnCameraSettings = NULL;
////@end WizardPage5 member initialisation
}


/*!
 * Control creation for WizardPage5
 */

void WizardPage5::CreateControls()
{    
////@begin WizardPage5 content construction
    WizardPage5* itemWizardPageSimple9 = this;

    wxBoxSizer* itemBoxSizer10 = new wxBoxSizer(wxVERTICAL);
    itemWizardPageSimple9->SetSizer(itemBoxSizer10);

    wxStaticText* itemStaticText11 = new wxStaticText( itemWizardPageSimple9, wxID_STATIC, _("Camera setup"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText11->SetFont(wxFont(10, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Sans")));
    itemBoxSizer10->Add(itemStaticText11, 0, wxALIGN_LEFT|wxRIGHT|wxTOP|wxBOTTOM, 5);

    wxStaticText* itemStaticText12 = new wxStaticText( itemWizardPageSimple9, wxID_STATIC, _("Select the camera properties"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer10->Add(itemStaticText12, 0, wxALIGN_LEFT|wxALL, 5);

    wxStaticLine* itemStaticLine13 = new wxStaticLine( itemWizardPageSimple9, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
    itemBoxSizer10->Add(itemStaticLine13, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText14 = new wxStaticText( itemWizardPageSimple9, wxID_STATIC, _("Camera detected:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer10->Add(itemStaticText14, 0, wxALIGN_LEFT|wxALL, 5);

    m_staticCameraName = new wxStaticText( itemWizardPageSimple9, wxID_STATIC, _("camera_name"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticCameraName->SetFont(wxFont(10, wxSWISS, wxITALIC, wxNORMAL, false, wxT("Sans")));
    itemBoxSizer10->Add(m_staticCameraName, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText16 = new wxStaticText( itemWizardPageSimple9, wxID_STATIC, _("Framerate:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer10->Add(itemStaticText16, 0, wxALIGN_LEFT|wxALL, 5);

    m_staticFramerate = new wxStaticText( itemWizardPageSimple9, wxID_STATIC, _("framerate"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticFramerate->SetFont(wxFont(10, wxSWISS, wxITALIC, wxNORMAL, false, wxT("Sans")));
    itemBoxSizer10->Add(m_staticFramerate, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    m_btnCameraSettings = new wxButton( itemWizardPageSimple9, ID_BUTTON5, _("Settings"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer10->Add(m_btnCameraSettings, 0, wxALIGN_LEFT|wxALL, 5);

////@end WizardPage5 content construction
	m_staticCameraName->SetLabel(m_wizardParent->GetViacamController()->GetCameraName());
	m_staticFramerate->SetLabel(wxString::Format(wxT("%f"), m_wizardParent->GetViacamController()->GetCamera()->GetFrameRate()));

}


/*!
 * Should we show tooltips?
 */

bool WizardPage5::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap WizardPage5::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin WizardPage5 bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end WizardPage5 bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon WizardPage5::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin WizardPage5 icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end WizardPage5 icon retrieval
}


/*!
 * WizardPage6 type definition
 */

IMPLEMENT_DYNAMIC_CLASS( WizardPage6, wxWizardPageSimple )


/*!
 * WizardPage6 event table definition
 */

BEGIN_EVENT_TABLE( WizardPage6, wxWizardPageSimple )

////@begin WizardPage6 event table entries
    EVT_WIZARD_PAGE_CHANGED( -1, WizardPage6::OnWizardpage6PageChanged )

    EVT_SPINCTRL( ID_SPINCTRL, WizardPage6::OnSpinctrlXSpeedUpdated )

    EVT_SPINCTRL( ID_SPINCTRL1, WizardPage6::OnSpinctrlYSpeedUpdated )

    EVT_TOGGLEBUTTON( ID_TOGGLEBUTTON, WizardPage6::OnTogglebuttonClick )

////@end WizardPage6 event table entries

END_EVENT_TABLE()


/*!
 * WizardPage6 constructors
 */

WizardPage6::WizardPage6()
{
    Init();
}

WizardPage6::WizardPage6( wxWizard* parent )
{
    Init();
	m_wizardParent = (WWizard*) parent;
    Create( parent );
}


/*!
 * WizardPage6 creator
 */

bool WizardPage6::Create( wxWizard* parent )
{
////@begin WizardPage6 creation
    wxBitmap wizardBitmap(wxNullBitmap);
    wxWizardPageSimple::Create( parent, NULL, NULL, wizardBitmap );

    CreateControls();
    if (GetSizer())
        GetSizer()->Fit(this);
////@end WizardPage6 creation
    return true;
}


/*!
 * WizardPage6 destructor
 */

WizardPage6::~WizardPage6()
{
////@begin WizardPage6 destruction
////@end WizardPage6 destruction
}


/*!
 * Member initialisation
 */

void WizardPage6::Init()
{
////@begin WizardPage6 member initialisation
    m_spinXSpeed = NULL;
    m_spinYSpeed = NULL;
    m_toggleEnableMotion = NULL;
////@end WizardPage6 member initialisation
}


/*!
 * Control creation for WizardPage6
 */

void WizardPage6::CreateControls()
{    
////@begin WizardPage6 content construction
    WizardPage6* itemWizardPageSimple31 = this;

    wxBoxSizer* itemBoxSizer32 = new wxBoxSizer(wxVERTICAL);
    itemWizardPageSimple31->SetSizer(itemBoxSizer32);

    wxStaticText* itemStaticText33 = new wxStaticText( itemWizardPageSimple31, wxID_STATIC, _("Pointer calibration"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText33->SetFont(wxFont(10, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Sans")));
    itemBoxSizer32->Add(itemStaticText33, 0, wxALIGN_LEFT|wxRIGHT|wxTOP|wxBOTTOM, 5);

    wxStaticText* itemStaticText34 = new wxStaticText( itemWizardPageSimple31, wxID_STATIC, _("Set the pointer speed"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer32->Add(itemStaticText34, 0, wxALIGN_LEFT|wxALL, 5);

    wxStaticLine* itemStaticLine35 = new wxStaticLine( itemWizardPageSimple31, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
    itemBoxSizer32->Add(itemStaticLine35, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText36 = new wxStaticText( itemWizardPageSimple31, wxID_STATIC, _("You can enable/disable the mouse motion to test the pointer speed\nand adjust the parameters manually."), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer32->Add(itemStaticText36, 0, wxALIGN_LEFT|wxALL, 5);

    wxBoxSizer* itemBoxSizer37 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer32->Add(itemBoxSizer37, 0, wxALIGN_LEFT|wxALL, 5);

    wxStaticText* itemStaticText38 = new wxStaticText( itemWizardPageSimple31, wxID_STATIC, _("X axis speed"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer37->Add(itemStaticText38, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_spinXSpeed = new wxSpinCtrl( itemWizardPageSimple31, ID_SPINCTRL, _T("0"), wxDefaultPosition, wxSize(45, 25), wxSP_ARROW_KEYS, 0, 30, 0 );
    itemBoxSizer37->Add(m_spinXSpeed, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer40 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer32->Add(itemBoxSizer40, 0, wxALIGN_LEFT|wxALL, 5);

    wxStaticText* itemStaticText41 = new wxStaticText( itemWizardPageSimple31, wxID_STATIC, _("Y axis speed"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer40->Add(itemStaticText41, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_spinYSpeed = new wxSpinCtrl( itemWizardPageSimple31, ID_SPINCTRL1, _T("0"), wxDefaultPosition, wxSize(45, 25), wxSP_ARROW_KEYS, 0, 30, 0 );
    itemBoxSizer40->Add(m_spinYSpeed, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_toggleEnableMotion = new wxToggleButton( itemWizardPageSimple31, ID_TOGGLEBUTTON, _("Enable motion"), wxDefaultPosition, wxSize(150, 50), 0 );
    m_toggleEnableMotion->SetValue(false);
    itemBoxSizer32->Add(m_toggleEnableMotion, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

////@end WizardPage6 content construction
	
}


/*!
 * Should we show tooltips?
 */

bool WizardPage6::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap WizardPage6::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin WizardPage6 bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end WizardPage6 bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon WizardPage6::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin WizardPage6 icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end WizardPage6 icon retrieval
}


/*!
 * wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL
 */

void WizardPage6::OnSpinctrlXSpeedUpdated( wxSpinEvent& event )
{
	m_wizardParent->GetViacamController()->GetMouseOutput()->SetXSpeed(event.GetInt());
    event.Skip(false);
}


/*!
 * wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL1
 */

void WizardPage6::OnSpinctrlYSpeedUpdated( wxSpinEvent& event )
{
	m_wizardParent->GetViacamController()->GetMouseOutput()->SetYSpeed(event.GetInt());
    event.Skip(false);
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_TOGGLEBUTTON
 */

void WizardPage6::OnTogglebuttonClick( wxCommandEvent& event )
{
    m_wizardParent->GetViacamController()->SetEnabled(!m_wizardParent->GetViacamController()->GetEnabled(), true);

	if (m_toggleEnableMotion->GetValue())
		m_toggleEnableMotion->SetLabel(wxT("Disable motion"));
	else	
		m_toggleEnableMotion->SetLabel(wxT("Enable motion"));
	
	event.Skip(false);
}


/*!
 * wxEVT_WIZARD_PAGE_CHANGED event handler for ID_WIZARDPAGE6
 */

void WizardPage6::OnWizardpage6PageChanged( wxWizardEvent& event )
{
	m_spinXSpeed->SetValue(m_wizardParent->GetViacamController()->GetMouseOutput()->GetXSpeed());
	m_spinYSpeed->SetValue(m_wizardParent->GetViacamController()->GetMouseOutput()->GetYSpeed());

	if (m_wizardParent->GetIsMotionEnabled())
	{
		m_toggleEnableMotion->SetValue(true);
		m_toggleEnableMotion->SetLabel(wxT("Disable motion"));
	}	

	event.Skip(false);
}



/*!
 * WizardPage7 type definition
 */

IMPLEMENT_DYNAMIC_CLASS( WizardPage7, wxWizardPageSimple )


/*!
 * WizardPage7 event table definition
 */

BEGIN_EVENT_TABLE( WizardPage7, wxWizardPageSimple )

////@begin WizardPage7 event table entries
////@end WizardPage7 event table entries

END_EVENT_TABLE()


/*!
 * WizardPage7 constructors
 */

WizardPage7::WizardPage7()
{
    Init();
}

WizardPage7::WizardPage7( wxWizard* parent )
{
    Init();
	m_wizardParent = (WWizard*) parent;
    Create( parent );
}


/*!
 * WizardPage7 creator
 */

bool WizardPage7::Create( wxWizard* parent )
{
////@begin WizardPage7 creation
    wxBitmap wizardBitmap(wxNullBitmap);
    wxWizardPageSimple::Create( parent, NULL, NULL, wizardBitmap );

    CreateControls();
    if (GetSizer())
        GetSizer()->Fit(this);
////@end WizardPage7 creation
    return true;
}


/*!
 * WizardPage7 destructor
 */

WizardPage7::~WizardPage7()
{
////@begin WizardPage7 destruction
////@end WizardPage7 destruction
}


/*!
 * Member initialisation
 */

void WizardPage7::Init()
{
////@begin WizardPage7 member initialisation
////@end WizardPage7 member initialisation
}


/*!
 * Control creation for WizardPage7
 */

void WizardPage7::CreateControls()
{    
////@begin WizardPage7 content construction
    WizardPage7* itemWizardPageSimple44 = this;

    wxBoxSizer* itemBoxSizer45 = new wxBoxSizer(wxVERTICAL);
    itemWizardPageSimple44->SetSizer(itemBoxSizer45);

    wxStaticText* itemStaticText46 = new wxStaticText( itemWizardPageSimple44, wxID_STATIC, _("Click mode"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText46->SetFont(wxFont(10, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Sans")));
    itemBoxSizer45->Add(itemStaticText46, 0, wxALIGN_LEFT|wxRIGHT|wxTOP|wxBOTTOM, 5);

    wxStaticText* itemStaticText47 = new wxStaticText( itemWizardPageSimple44, wxID_STATIC, _("How to make the click"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer45->Add(itemStaticText47, 0, wxALIGN_LEFT|wxALL, 5);

    wxStaticLine* itemStaticLine48 = new wxStaticLine( itemWizardPageSimple44, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
    itemBoxSizer45->Add(itemStaticLine48, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText49 = new wxStaticText( itemWizardPageSimple44, wxID_STATIC, _("Attention, eViacam will be enabled in the next window to allow you\nto test different ways of clicking.\n"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer45->Add(itemStaticText49, 0, wxALIGN_LEFT|wxALL, 5);

////@end WizardPage7 content construction
}


/*!
 * Should we show tooltips?
 */

bool WizardPage7::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap WizardPage7::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin WizardPage7 bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end WizardPage7 bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon WizardPage7::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin WizardPage7 icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end WizardPage7 icon retrieval
}


/*!
 * wxEVT_LEFT_DOWN event handler for ID_TOGGLEBUTTON1
 */

void WizardPage3::OnLeftDownTest( wxMouseEvent& event )
{
	m_toggleTest->SetLabel(wxT("Drag"));
	m_toggleTest->SetValue(true);
	m_timer.Start(1000);
    event.Skip(false);
}


/*!
 * wxEVT_LEFT_UP event handler for ID_TOGGLEBUTTON1
 */

void WizardPage3::OnLeftUpTest( wxMouseEvent& event )
{
	if (m_toggleTest->GetLabel() != wxT("Double click"))
	{
		if (m_toggleTest->GetLabel() == wxT("Drag"))
			m_toggleTest->SetLabel(wxT("Left click"));
		else
			m_toggleTest->SetLabel(wxT("Drop"));
		m_toggleTest->SetValue(true);	
		m_timer.Start(1000);
		event.Skip(false);
	}	
}




/*!
 * wxEVT_RIGHT_UP event handler for ID_TOGGLEBUTTON1
 */

void WizardPage3::OnRightUpTest( wxMouseEvent& event )
{
	m_toggleTest->SetValue(true);
	m_toggleTest->SetLabel(wxT("Right click"));
	m_timer.Start(1000);
    event.Skip(false);
}




/*!
 * wxEVT_LEFT_DCLICK event handler for ID_TOGGLEBUTTON1
 */

void WizardPage3::OnLeftDClickTest( wxMouseEvent& event )
{
	m_toggleTest->SetLabel(wxT("Double click"));
	m_toggleTest->SetValue(true);
	m_timer.Start(1000);
    event.Skip(false);
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX1
 */

void WizardPage4::OnCheckboxRunWizardAtStartupClick( wxCommandEvent& event )
{
	m_wizardParent->SetRunAtStartup(m_chkRunWizardAtStartup->GetValue());
	event.Skip(false);
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX3
 */

void WizardPage4::OnCheckboxStartupClick( wxCommandEvent& event )
{
#if defined(__WXGTK__)
	m_wizardParent->GetViacamController()->GetAutostart()->Enable(m_chkStartup->GetValue());
#endif
    event.Skip(false);
}


/*!
 * wxEVT_WIZARD_PAGE_CHANGED event handler for ID_WIZARDPAGE4
 */

void WizardPage4::OnWizardpage4PageChanged( wxWizardEvent& event )
{
    m_chkRunWizardAtStartup->SetValue(m_wizardParent->GetViacamController()->GetRunWizardAtStartup());
	event.Skip(false);
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX4
 */

void WizardPage4::OnCheckboxEnabledAtStartupClick( wxCommandEvent& event )
{
	m_wizardParent->GetViacamController()->SetEnabledAtStartup (m_chkEnabledAtStartup->GetValue());
	event.Skip(false);
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON5
 */

void WizardPage5::OnButtonCameraSettingsClick( wxCommandEvent& event )
{
	m_wizardParent->GetViacamController()->OpenCameraControlDialog();
    event.Skip(false);
}

