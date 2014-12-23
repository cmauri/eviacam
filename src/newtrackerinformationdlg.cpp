/////////////////////////////////////////////////////////////////////////////
// Name:        newtrackerinformationdlg.cpp
// Purpose:     
// Author:      C�sar Mauri Loba
// Modified by: 
// Created:     21/12/2014 14:03:05
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

#include "eviacamapp.h"
#include "viacamcontroller.h"
#include "newtrackerinformationdlg.h"
#include "configmanager.h"
#include "pointeraction.h"

////@begin XPM images
////@end XPM images


/*!
 * NewTrackerInformationDlg type definition
 */

IMPLEMENT_DYNAMIC_CLASS( NewTrackerInformationDlg, wxDialog )


/*!
 * NewTrackerInformationDlg event table definition
 */

BEGIN_EVENT_TABLE( NewTrackerInformationDlg, wxDialog )

////@begin NewTrackerInformationDlg event table entries
    EVT_RADIOBUTTON( ID_RADIOBUTTON_LEGACY_TRACKER, NewTrackerInformationDlg::OnRadiobuttonLegacyTrackerSelected )
    EVT_CHECKBOX( ID_CHECKBOX_SHOW_ON_STARTUP, NewTrackerInformationDlg::OnCheckboxShowOnStartupClick )
    EVT_BUTTON( wxID_OK, NewTrackerInformationDlg::OnOkClick )
////@end NewTrackerInformationDlg event table entries

END_EVENT_TABLE()


/*!
 * NewTrackerInformationDlg constructors
 */

NewTrackerInformationDlg::NewTrackerInformationDlg()
{
    Init();
}

NewTrackerInformationDlg::NewTrackerInformationDlg( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*!
 * NewTrackerInformationDlg creator
 */

bool NewTrackerInformationDlg::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin NewTrackerInformationDlg creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end NewTrackerInformationDlg creation
    return true;
}


/*!
 * NewTrackerInformationDlg destructor
 */

NewTrackerInformationDlg::~NewTrackerInformationDlg()
{
////@begin NewTrackerInformationDlg destruction
////@end NewTrackerInformationDlg destruction
}


/*!
 * Member initialisation
 */

void NewTrackerInformationDlg::Init()
{
	m_useLegacyTracker = false;
	m_showOnStartup = false;
////@begin NewTrackerInformationDlg member initialisation
    m_chkShowOnStartup = NULL;
////@end NewTrackerInformationDlg member initialisation
}


/*!
 * Control creation for NewTrackerInformationDlg
 */

void NewTrackerInformationDlg::CreateControls()
{    
////@begin NewTrackerInformationDlg content construction
    NewTrackerInformationDlg* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxStaticText* itemStaticText3 = new wxStaticText( itemDialog1, wxID_STATIC, _("Improved face tracker"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText3->SetFont(wxFont(-1, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Sans")));
    itemBoxSizer2->Add(itemStaticText3, 0, wxALIGN_LEFT|wxALL, 5);

    wxStaticText* itemStaticText4 = new wxStaticText( itemDialog1, wxID_STATIC, _("This version of Enable Viacam includes an improved face tracking algorithm. We still provide the old algorithm, but it will most likely be removed in future versions."), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText4->Wrap(400);
    itemBoxSizer2->Add(itemStaticText4, 0, wxALIGN_LEFT|wxALL, 5);

    wxStaticText* itemStaticText5 = new wxStaticText( itemDialog1, wxID_STATIC, _("Do you want to use the new face tracker?"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText5->Wrap(400);
    itemBoxSizer2->Add(itemStaticText5, 0, wxALIGN_LEFT|wxALL, 5);

    wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer2->Add(itemBoxSizer6, 0, wxALIGN_LEFT|wxALL, 5);

    wxRadioButton* itemRadioButton7 = new wxRadioButton( itemDialog1, ID_RADIOBUTTON_NEW_TRACKER, _("Yes and adjust some settings automatically (recommended)"), wxDefaultPosition, wxDefaultSize, 0 );
    itemRadioButton7->SetValue(true);
    itemBoxSizer6->Add(itemRadioButton7, 0, wxALIGN_LEFT|wxALL, 5);

    wxRadioButton* itemRadioButton8 = new wxRadioButton( itemDialog1, ID_RADIOBUTTON_LEGACY_TRACKER, _("No, use the legacy tracker"), wxDefaultPosition, wxDefaultSize, 0 );
    itemRadioButton8->SetValue(false);
    itemBoxSizer6->Add(itemRadioButton8, 0, wxALIGN_LEFT|wxALL, 5);

    wxStaticText* itemStaticText9 = new wxStaticText( itemDialog1, wxID_STATIC, _("You could also change the tracker configuration under:\nConfiguration -> Options... -> Advanced -> Face localization and tracking"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText9->SetFont(wxFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL, false, wxT("Sans")));
    itemBoxSizer2->Add(itemStaticText9, 0, wxALIGN_LEFT|wxALL, 5);

    itemBoxSizer2->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    m_chkShowOnStartup = new wxCheckBox( itemDialog1, ID_CHECKBOX_SHOW_ON_STARTUP, _("Show this message on startup"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkShowOnStartup->SetValue(false);
    itemBoxSizer2->Add(m_chkShowOnStartup, 0, wxALIGN_LEFT|wxALL, 5);

    wxStaticLine* itemStaticLine12 = new wxStaticLine( itemDialog1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
    itemBoxSizer2->Add(itemStaticLine12, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer13 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer13, 0, wxALIGN_RIGHT|wxALL, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer14 = new wxStdDialogButtonSizer;

    itemBoxSizer13->Add(itemStdDialogButtonSizer14, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxButton* itemButton15 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemButton15->SetDefault();
    itemStdDialogButtonSizer14->AddButton(itemButton15);

    itemStdDialogButtonSizer14->Realize();

////@end NewTrackerInformationDlg content construction
}


/*!
 * Should we show tooltips?
 */

bool NewTrackerInformationDlg::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap NewTrackerInformationDlg::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin NewTrackerInformationDlg bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end NewTrackerInformationDlg bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon NewTrackerInformationDlg::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin NewTrackerInformationDlg icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end NewTrackerInformationDlg icon retrieval
}


/*!
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON_LEGACY_TRACKER
 */

void NewTrackerInformationDlg::OnRadiobuttonLegacyTrackerSelected( wxCommandEvent& event )
{
	m_useLegacyTracker = true;
	m_showOnStartup = true;
	m_chkShowOnStartup->SetValue(true);
    event.Skip(false);
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_SHOW_ON_STARTUP
 */

void NewTrackerInformationDlg::OnCheckboxShowOnStartupClick( wxCommandEvent& event )
{
	m_showOnStartup = event.GetInt() != 0;
    event.Skip(false);
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void NewTrackerInformationDlg::OnOkClick( wxCommandEvent& event )
{
	if (m_useLegacyTracker) {
		wxGetApp().GetController().GetVisionPipeline().SetUseLegacyTracker(true);
	}
	else {
		wxGetApp().GetController().GetVisionPipeline().SetUseLegacyTracker(false);
		wxGetApp().GetController().GetVisionPipeline().SetTrackFace(true);
		wxGetApp().GetController().GetVisionPipeline().SetCpuUsage(CVisionPipeline::CPU_NORMAL);
		wxGetApp().GetController().GetPointerAction().SetXSpeed(10);
		wxGetApp().GetController().GetPointerAction().SetYSpeed(10);
		wxGetApp().GetController().GetPointerAction().SetAcceleration(2);
		wxGetApp().GetController().GetPointerAction().SetSmoothness(2);
	}
	wxGetApp().GetController().SetNewTrackerDialogAtStartup(m_showOnStartup);
	wxGetApp().GetController().GetConfigManager().WriteAll();
	
    event.Skip();
}

