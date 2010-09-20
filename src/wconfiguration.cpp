/////////////////////////////////////////////////////////////////////////////
// Name:        wconfiguration.cpp
// Purpose:     
// Author:      César Mauri Loba
// Modified by: 
// Created:     29/12/2008 16:35:20
// RCS-ID:      
// Copyright:   (C) 2008 Cesar Mauri from CREA Sistemes Informatics
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

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/imaglist.h>
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
#include "wx/imaglist.h"
////@end includes

#include "wconfiguration.h"

////@begin XPM images
#include "icons/eviacam.xpm"
#include "icons/user.xpm"
////@end XPM images

#include "viacamcontroller.h"
#include "mouseoutput.h"
#include "motiontracker.h"
#include "clickwindowcontroller.h"
#include "cautostart.h"
//#include "cmotioncalibration.h"

// Trick to properly compile & display native language names
#if defined(__WXMSW__)
#include "langnames-utf16.h"
#else
#include "langnames-utf8.h"
#include "activationkey.h"
#include "cautostart.h"
#include <wx/stdpaths.h>
#endif

const wxLanguage s_langIds[] = {
	wxLANGUAGE_DEFAULT,
#ifdef ENABLE_ASTURIAN
	wxLANGUAGE_ASTURIAN,
#endif
	wxLANGUAGE_CATALAN,
	wxLANGUAGE_GERMAN,
	wxLANGUAGE_ENGLISH,
	wxLANGUAGE_SPANISH,
	wxLANGUAGE_GALICIAN,
	wxLANGUAGE_ITALIAN,
	wxLANGUAGE_TURKISH,
	wxLANGUAGE_OCCITAN,
	wxLANGUAGE_FRENCH,
	wxLANGUAGE_ARABIC
};

wxCOMPILE_TIME_ASSERT( WXSIZEOF(s_langNames) == WXSIZEOF(s_langIds), LangArraysMismatch );


/*!
 * WConfiguration type definition
 */

IMPLEMENT_DYNAMIC_CLASS( WConfiguration, wxDialog )


/*!
 * WConfiguration event table definition
 */

BEGIN_EVENT_TABLE( WConfiguration, wxDialog )

////@begin WConfiguration event table entries
    EVT_SPINCTRL( ID_SPINCTRL_XSPEED, WConfiguration::OnSpinctrlXspeedUpdated )

    EVT_SPINCTRL( ID_SPINCTRL_YSPEED, WConfiguration::OnSpinctrlYspeedUpdated )

    EVT_SPINCTRL( ID_SPINCTRL_ACCELERATION, WConfiguration::OnSpinctrlAccelerationUpdated )

    EVT_SPINCTRL( ID_SPINCTRL_SMOOTHNESS, WConfiguration::OnSpinctrlSmoothnessUpdated )

    EVT_SPINCTRL( ID_SPINCTRL_EASYSTOP, WConfiguration::OnSpinctrlEasystopUpdated )

    EVT_CHECKBOX( ID_CHECKBOX_ENABLE_AT_STARTUP, WConfiguration::OnCheckboxEnableAtStartupClick )

    EVT_BUTTON( ID_BUTTON, WConfiguration::OnButtonClick )

    EVT_CHECKBOX( ID_CHECKBOX_WORKSPACE_LIMIT, WConfiguration::OnCheckboxWorkspaceLimitClick )

    EVT_SPINCTRL( ID_SPINCTRL_TOP_WORKSPACE, WConfiguration::OnSpinctrlTopWorkspaceUpdated )

    EVT_SPINCTRL( ID_SPINCTRL_LEFT_WORKSPACE, WConfiguration::OnSpinctrlLeftWorkspaceUpdated )

    EVT_SPINCTRL( ID_SPINCTRL_RIGHT_WORKSPACE, WConfiguration::OnSpinctrlRightWorkspaceUpdated )

    EVT_SPINCTRL( ID_SPINCTRL_BOTTOM_WORKSPACE, WConfiguration::OnSpinctrlBottomWorkspaceUpdated )

    EVT_CHECKBOX( ID_CHECKBOX_ENABLE_DWELL, WConfiguration::OnCheckboxEnableDwellClick )

    EVT_SPINCTRL( ID_SPINCTRL_DWELL_TIME, WConfiguration::OnSpinctrlDwellTimeUpdated )

    EVT_SPINCTRL( ID_SPINCTRL_DWELL_AREA, WConfiguration::OnSpinctrlDwellAreaUpdated )

    EVT_CHECKBOX( ID_CHECKBOX_ALLOW_CONSECUTIVE, WConfiguration::OnCheckboxAllowConsecutiveClick )

    EVT_CHECKBOX( ID_CHECKBOX_BEEP_ON_CLICK, WConfiguration::OnCheckboxBeepOnClickClick )

    EVT_CHECKBOX( ID_CHECKBOX_CLICKWIN_AT_STARTUP, WConfiguration::OnCheckboxClickwinAtStartupClick )

    EVT_CHECKBOX( ID_CHECKBOX_SHOW_CLICKWIN, WConfiguration::OnCheckboxShowClickwinClick )

    EVT_COMBOBOX( ID_COMBOBOX_DESIGN, WConfiguration::OnComboboxDesignSelected )

    EVT_CHOICE( ID_CHOICE_BEHAVIOUR, WConfiguration::OnChoiceBehaviourSelected )

    EVT_CHECKBOX( ID_CHECKBOX, WConfiguration::OnCheckboxEnableGestureClick )

    EVT_COMBOBOX( ID_COMBOBOX_LEFT, WConfiguration::OnComboboxLeftSelected )

    EVT_COMBOBOX( ID_COMBOBOX_RIGHT, WConfiguration::OnComboboxRightSelected )

    EVT_COMBOBOX( ID_COMBOBOX_TOP, WConfiguration::OnComboboxTopSelected )

    EVT_COMBOBOX( ID_COMBOBOX_BOTTOM, WConfiguration::OnComboboxBottomSelected )

    EVT_CHECKBOX( ID_CHECKBOX_AUTO_LOCATE_FACE, WConfiguration::OnCheckboxAutoLocateFaceClick )

    EVT_CHECKBOX( ID_CHECKBOX_SHOW_LOCATE_FACE_FILTER, WConfiguration::OnCheckboxShowLocateFaceFilterClick )

    EVT_TEXT( ID_TEXTCTRL_ONSCREENKEYBOARDCOMMAND, WConfiguration::OnTextctrlOnscreenkeyboardcommandTextUpdated )

    EVT_BUTTON( ID_BUTTON_ONSCREENKEYBOARDCOMMAND, WConfiguration::OnButtonOnscreenkeyboardcommandClick )

#if defined(__WXGTK__)
    EVT_CHECKBOX( ID_CHECKBOX_ACTIVATION_KEY, WConfiguration::OnCheckboxActivationKeyClick )
#endif

#if defined(__WXGTK__)
    EVT_BUTTON( ID_BUTTON_ACTIVATION_KEY, WConfiguration::OnButtonActivationKeyClick )
#endif

#if defined(__WXGTK__)
    EVT_CHECKBOX( ID_CHECKBOX_STARTUP, WConfiguration::OnCheckboxStartupClick )
#endif

    EVT_CHOICE( ID_CHOICE_PROFILE, WConfiguration::OnChoiceProfileSelected )

    EVT_BUTTON( ID_BUTTON_ADD_PROFILE, WConfiguration::OnButtonAddProfileClick )

    EVT_BUTTON( ID_BUTTON_DEL_PROFILE, WConfiguration::OnButtonDelProfileClick )

    EVT_BUTTON( ID_BUTTON_CAMERA_SETTINGS, WConfiguration::OnButtonCameraSettingsClick )

    EVT_BUTTON( ID_BUTTON_CHANGE_CAMERA, WConfiguration::OnButtonChangeCameraClick )

    EVT_CHOICE( ID_CHOICE_LANGUAGE, WConfiguration::OnChoiceLanguageSelected )

    EVT_BUTTON( ID_BUTTON_OK, WConfiguration::OnButtonOkClick )

    EVT_BUTTON( ID_BUTTON_CANCEL, WConfiguration::OnButtonCancelClick )

////@end WConfiguration event table entries

END_EVENT_TABLE()


/*!
 * WConfiguration constructors
 */

WConfiguration::WConfiguration()
{
    Init();
}

WConfiguration::WConfiguration( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}

WConfiguration::WConfiguration( wxWindow* parent, CViacamController* pViacamController, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
	m_pViacamController= pViacamController;
	#if defined(__WXGTK__)
		m_pAutostart = m_pViacamController->GetAutostart();
	#endif

	Init();
    Create(parent, id, caption, pos, size, style);
}


/*!
 * WConfiguration creator
 */

bool WConfiguration::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin WConfiguration creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    SetIcon(GetIconResource(wxT("icons/eviacam.xpm")));
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
////@end WConfiguration creation

	InitializeData ();

    return true;
}


/*!
 * WConfiguration destructor
 */

WConfiguration::~WConfiguration()
{
////@begin WConfiguration destruction
////@end WConfiguration destruction
}


/*!
 * Member initialisation
 */

void WConfiguration::Init()
{
////@begin WConfiguration member initialisation
    m_spinXSpeed = NULL;
    m_spinYSpeed = NULL;
    m_spinAcceleration = NULL;
    m_spinSmoothness = NULL;
    m_spinEasyStop = NULL;
    m_chkEnabledAtStartup = NULL;
    m_buttonCalibrateMotion = NULL;
    m_chkEnabledWorkspace = NULL;
    m_spin_top_workspace = NULL;
    m_spin_left_workspace = NULL;
    m_spin_right_workspace = NULL;
    m_spin_bottom_workspace = NULL;
    m_chkDwellClickEnabled = NULL;
    m_stDwellTime = NULL;
    m_spinDwellTime = NULL;
    m_stDwellArea = NULL;
    m_spinDwellArea = NULL;
    m_chkAllowConsecutiveClick = NULL;
    m_chkBeepOnClick = NULL;
    m_chkOpenClickWinAtStartup = NULL;
    m_chkShowClickWin = NULL;
    m_stDesign = NULL;
    m_cmbClickWindowDesign = NULL;
    m_stBehaviour = NULL;
    m_choClickWindowBehaviour = NULL;
    m_chkEnableGestureClick = NULL;
    m_stMoveLeft = NULL;
    m_cmbLeft = NULL;
    m_stMoveRight = NULL;
    m_cmbRight = NULL;
    m_stMoveTop = NULL;
    m_cmbTop = NULL;
    m_stMoveBottom = NULL;
    m_cmbBottom = NULL;
    m_chkAutoLocateFace = NULL;
    m_chkShowAutoLocateFaceFilter = NULL;
    m_txtOnScreenKeyboardCommand = NULL;
    m_btntOnScreenKeyboardCommand = NULL;
#if defined(__WXGTK__)
    m_chkActivationKey = NULL;
#endif
#if defined(__WXGTK__)
    m_txtActivationKey = NULL;
#endif
#if defined(__WXGTK__)
    m_buttonActivationKey = NULL;
#endif
#if defined(__WXGTK__)
    m_chkStartup = NULL;
#endif
    m_choProfile = NULL;
    m_btnAddProfile = NULL;
    m_btnDeleteProfile = NULL;
    m_txtSelectedCamera = NULL;
    m_btnCameraSettings = NULL;
    m_choLanguage = NULL;
    m_btnCancel = NULL;
////@end WConfiguration member initialisation

	m_dirty= false;
}


/*!
 * Control creation for WConfiguration
 */

void WConfiguration::CreateControls()
{    
////@begin WConfiguration content construction
    WConfiguration* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxListbook* itemListbook3 = new wxListbook( itemDialog1, ID_LISTBOOK, wxDefaultPosition, wxDefaultSize, wxLB_DEFAULT );
    wxImageList* itemListbook3ImageList = new wxImageList(32, 32, true, 2);
    {
        wxIcon itemListbook3Icon0(itemDialog1->GetIconResource(wxT("icons/user.xpm")));
        itemListbook3ImageList->Add(itemListbook3Icon0);
        wxIcon itemListbook3Icon1(itemDialog1->GetIconResource(wxT("icons/eviacam.xpm")));
        itemListbook3ImageList->Add(itemListbook3Icon1);
    }
    itemListbook3->AssignImageList(itemListbook3ImageList);

    wxNotebook* itemNotebook4 = new wxNotebook( itemListbook3, ID_NOTEBOOK, wxDefaultPosition, wxDefaultSize, wxNB_DEFAULT );

    wxPanel* itemPanel5 = new wxPanel( itemNotebook4, ID_PANEL_MOTION, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxVERTICAL);
    itemPanel5->SetSizer(itemBoxSizer6);

    wxFlexGridSizer* itemFlexGridSizer7 = new wxFlexGridSizer(0, 2, 0, 0);
    itemBoxSizer6->Add(itemFlexGridSizer7, 0, wxALIGN_LEFT|wxALL, 5);
    wxStaticText* itemStaticText8 = new wxStaticText( itemPanel5, wxID_STATIC, _("X axis speed"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer7->Add(itemStaticText8, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_spinXSpeed = new wxSpinCtrl( itemPanel5, ID_SPINCTRL_XSPEED, _T("0"), wxDefaultPosition, wxSize(45, 25), wxSP_ARROW_KEYS, 0, 30, 0 );
    if (WConfiguration::ShowToolTips())
        m_spinXSpeed->SetToolTip(_("Sets response sensitivity for the X axis."));
    itemFlexGridSizer7->Add(m_spinXSpeed, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText10 = new wxStaticText( itemPanel5, wxID_STATIC, _("Y axis speed"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer7->Add(itemStaticText10, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_spinYSpeed = new wxSpinCtrl( itemPanel5, ID_SPINCTRL_YSPEED, _T("0"), wxDefaultPosition, wxSize(45, 25), wxSP_ARROW_KEYS, 0, 30, 0 );
    if (WConfiguration::ShowToolTips())
        m_spinYSpeed->SetToolTip(_("Sets response sensitivity for the Y axis."));
    itemFlexGridSizer7->Add(m_spinYSpeed, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText12 = new wxStaticText( itemPanel5, wxID_STATIC, _("Acceleration"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer7->Add(itemStaticText12, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_spinAcceleration = new wxSpinCtrl( itemPanel5, ID_SPINCTRL_ACCELERATION, _T("0"), wxDefaultPosition, wxSize(45, 25), wxSP_ARROW_KEYS, 0, 5, 0 );
    if (WConfiguration::ShowToolTips())
        m_spinAcceleration->SetToolTip(_("Sets pointer's acceleration.\n0 means no acceleration and\n5 maximum acceleration."));
    itemFlexGridSizer7->Add(m_spinAcceleration, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText14 = new wxStaticText( itemPanel5, wxID_STATIC, _("Smoothness"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer7->Add(itemStaticText14, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_spinSmoothness = new wxSpinCtrl( itemPanel5, ID_SPINCTRL_SMOOTHNESS, _T("0"), wxDefaultPosition, wxSize(45, 25), wxSP_ARROW_KEYS, 0, 9, 0 );
    if (WConfiguration::ShowToolTips())
        m_spinSmoothness->SetToolTip(_("Tremor reduction filter.\n0 means no filter and \n9 maximum filtering."));
    itemFlexGridSizer7->Add(m_spinSmoothness, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText16 = new wxStaticText( itemPanel5, wxID_STATIC, _("Easy stop"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer7->Add(itemStaticText16, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_spinEasyStop = new wxSpinCtrl( itemPanel5, ID_SPINCTRL_EASYSTOP, _T("0"), wxDefaultPosition, wxSize(45, 25), wxSP_ARROW_KEYS, 0, 10, 0 );
    if (WConfiguration::ShowToolTips())
        m_spinEasyStop->SetToolTip(_("Minimum displacement (in pixels)\nto start moving pointer."));
    itemFlexGridSizer7->Add(m_spinEasyStop, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_chkEnabledAtStartup = new wxCheckBox( itemPanel5, ID_CHECKBOX_ENABLE_AT_STARTUP, _("Enabled at startup"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkEnabledAtStartup->SetValue(false);
    if (WConfiguration::ShowToolTips())
        m_chkEnabledAtStartup->SetToolTip(_("If checked the program automatically \nenables cursor control at startup."));
    itemBoxSizer6->Add(m_chkEnabledAtStartup, 0, wxALIGN_LEFT|wxALL, 10);

    m_buttonCalibrateMotion = new wxButton( itemPanel5, ID_BUTTON, _("Calibrate motion"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer6->Add(m_buttonCalibrateMotion, 0, wxALIGN_LEFT|wxALL, 5);

    itemNotebook4->AddPage(itemPanel5, _("Motion"));

    wxPanel* itemPanel20 = new wxPanel( itemNotebook4, ID_PANEL_WORKSPACE, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer21 = new wxBoxSizer(wxVERTICAL);
    itemPanel20->SetSizer(itemBoxSizer21);

    m_chkEnabledWorkspace = new wxCheckBox( itemPanel20, ID_CHECKBOX_WORKSPACE_LIMIT, _("Use workspace limit"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkEnabledWorkspace->SetValue(false);
    if (WConfiguration::ShowToolTips())
        m_chkEnabledWorkspace->SetToolTip(_("Confines the mouse pointer\nto the selected area."));
    itemBoxSizer21->Add(m_chkEnabledWorkspace, 0, wxALIGN_LEFT|wxALL, 5);

    wxBoxSizer* itemBoxSizer23 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer21->Add(itemBoxSizer23, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxGridSizer* itemGridSizer24 = new wxGridSizer(0, 3, 0, 0);
    itemBoxSizer23->Add(itemGridSizer24, 0, wxALIGN_CENTER_HORIZONTAL|wxRIGHT|wxTOP|wxBOTTOM, 5);
    wxStaticText* itemStaticText25 = new wxStaticText( itemPanel20, wxID_STATIC, _("Top"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridSizer24->Add(itemStaticText25, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_spin_top_workspace = new wxSpinCtrl( itemPanel20, ID_SPINCTRL_TOP_WORKSPACE, _T("1"), wxDefaultPosition, wxSize(45, 25), wxSP_ARROW_KEYS, 0, 50, 1 );
    if (WConfiguration::ShowToolTips())
        m_spin_top_workspace->SetToolTip(_("Top limit workspace."));
    m_spin_top_workspace->Enable(false);
    itemGridSizer24->Add(m_spin_top_workspace, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText27 = new wxStaticText( itemPanel20, wxID_STATIC, _("%"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridSizer24->Add(itemStaticText27, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxGridSizer* itemGridSizer28 = new wxGridSizer(0, 2, 0, 0);
    itemBoxSizer23->Add(itemGridSizer28, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 0);
    wxGridSizer* itemGridSizer29 = new wxGridSizer(0, 3, 0, 0);
    itemGridSizer28->Add(itemGridSizer29, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM, 5);
    wxStaticText* itemStaticText30 = new wxStaticText( itemPanel20, wxID_STATIC, _("Left"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridSizer29->Add(itemStaticText30, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_spin_left_workspace = new wxSpinCtrl( itemPanel20, ID_SPINCTRL_LEFT_WORKSPACE, _T("1"), wxDefaultPosition, wxSize(45, 25), wxSP_ARROW_KEYS, 0, 50, 1 );
    if (WConfiguration::ShowToolTips())
        m_spin_left_workspace->SetToolTip(_("Left limit workspace."));
    m_spin_left_workspace->Enable(false);
    itemGridSizer29->Add(m_spin_left_workspace, 0, wxALIGN_CENTER_HORIZONTAL|wxGROW|wxALL, 5);

    wxStaticText* itemStaticText32 = new wxStaticText( itemPanel20, wxID_STATIC, _("%"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridSizer29->Add(itemStaticText32, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxGridSizer* itemGridSizer33 = new wxGridSizer(0, 3, 0, 0);
    itemGridSizer28->Add(itemGridSizer33, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxStaticText* itemStaticText34 = new wxStaticText( itemPanel20, wxID_STATIC, _("Right"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridSizer33->Add(itemStaticText34, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_spin_right_workspace = new wxSpinCtrl( itemPanel20, ID_SPINCTRL_RIGHT_WORKSPACE, _T("1"), wxDefaultPosition, wxSize(45, 25), wxSP_ARROW_KEYS, 0, 50, 1 );
    if (WConfiguration::ShowToolTips())
        m_spin_right_workspace->SetToolTip(_("Right limit workspace."));
    m_spin_right_workspace->Enable(false);
    itemGridSizer33->Add(m_spin_right_workspace, 0, wxALIGN_CENTER_HORIZONTAL|wxGROW|wxALL, 5);

    wxStaticText* itemStaticText36 = new wxStaticText( itemPanel20, wxID_STATIC, _("%"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridSizer33->Add(itemStaticText36, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxGridSizer* itemGridSizer37 = new wxGridSizer(0, 3, 0, 0);
    itemBoxSizer23->Add(itemGridSizer37, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticText* itemStaticText38 = new wxStaticText( itemPanel20, wxID_STATIC, _("Bottom"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridSizer37->Add(itemStaticText38, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_spin_bottom_workspace = new wxSpinCtrl( itemPanel20, ID_SPINCTRL_BOTTOM_WORKSPACE, _T("1"), wxDefaultPosition, wxSize(45, 25), wxSP_ARROW_KEYS, 0, 50, 1 );
    if (WConfiguration::ShowToolTips())
        m_spin_bottom_workspace->SetToolTip(_("Bottom limit workspace."));
    m_spin_bottom_workspace->Enable(false);
    itemGridSizer37->Add(m_spin_bottom_workspace, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText40 = new wxStaticText( itemPanel20, wxID_STATIC, _("%"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridSizer37->Add(itemStaticText40, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemNotebook4->AddPage(itemPanel20, _("Workspace"));

    wxPanel* itemPanel41 = new wxPanel( itemNotebook4, ID_PANEL_CLICK, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer42 = new wxBoxSizer(wxVERTICAL);
    itemPanel41->SetSizer(itemBoxSizer42);

    m_chkDwellClickEnabled = new wxCheckBox( itemPanel41, ID_CHECKBOX_ENABLE_DWELL, _("Dwell click enabled"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkDwellClickEnabled->SetValue(false);
    if (WConfiguration::ShowToolTips())
        m_chkDwellClickEnabled->SetToolTip(_("Enable/Disable automatic (dwell)\nclick generation mechanism."));
    itemBoxSizer42->Add(m_chkDwellClickEnabled, 0, wxALIGN_LEFT|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer44Static = new wxStaticBox(itemPanel41, wxID_ANY, _("Dwell click"));
    wxStaticBoxSizer* itemStaticBoxSizer44 = new wxStaticBoxSizer(itemStaticBoxSizer44Static, wxVERTICAL);
    itemBoxSizer42->Add(itemStaticBoxSizer44, 0, wxGROW|wxALL, 0);
    wxFlexGridSizer* itemFlexGridSizer45 = new wxFlexGridSizer(0, 2, 0, 0);
    itemStaticBoxSizer44->Add(itemFlexGridSizer45, 0, wxALIGN_LEFT|wxALL, 0);
    m_stDwellTime = new wxStaticText( itemPanel41, ID_STATIC_DWELL_TIME, _("Dwell time (ds)"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer45->Add(m_stDwellTime, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_spinDwellTime = new wxSpinCtrl( itemPanel41, ID_SPINCTRL_DWELL_TIME, _T("2"), wxDefaultPosition, wxSize(45, 25), wxSP_ARROW_KEYS, 2, 50, 2 );
    if (WConfiguration::ShowToolTips())
        m_spinDwellTime->SetToolTip(_("Time to wait (deciseconds) \nbefore sending a click."));
    itemFlexGridSizer45->Add(m_spinDwellTime, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_stDwellArea = new wxStaticText( itemPanel41, ID_STATIC_DWELL_AREA, _("Dwell area"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer45->Add(m_stDwellArea, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_spinDwellArea = new wxSpinCtrl( itemPanel41, ID_SPINCTRL_DWELL_AREA, _T("0"), wxDefaultPosition, wxSize(45, 25), wxSP_ARROW_KEYS, 0, 5, 0 );
    if (WConfiguration::ShowToolTips())
        m_spinDwellArea->SetToolTip(_("Maximum allowed displacement\nbefore restarting the dwell time\ncountdown."));
    itemFlexGridSizer45->Add(m_spinDwellArea, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_chkAllowConsecutiveClick = new wxCheckBox( itemPanel41, ID_CHECKBOX_ALLOW_CONSECUTIVE, _("Allow consecutive clicks"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkAllowConsecutiveClick->SetValue(false);
    if (WConfiguration::ShowToolTips())
        m_chkAllowConsecutiveClick->SetToolTip(_("Allows to send multiple clicks\nwhen the pointer is stopped."));
    itemStaticBoxSizer44->Add(m_chkAllowConsecutiveClick, 0, wxALIGN_LEFT|wxALL, 5);

    m_chkBeepOnClick = new wxCheckBox( itemPanel41, ID_CHECKBOX_BEEP_ON_CLICK, _("Beep on click"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkBeepOnClick->SetValue(false);
    if (WConfiguration::ShowToolTips())
        m_chkBeepOnClick->SetToolTip(_("Play sound when click generated."));
    itemStaticBoxSizer44->Add(m_chkBeepOnClick, 0, wxALIGN_LEFT|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer52Static = new wxStaticBox(itemPanel41, wxID_ANY, _("Click window"));
    wxStaticBoxSizer* itemStaticBoxSizer52 = new wxStaticBoxSizer(itemStaticBoxSizer52Static, wxVERTICAL);
    itemBoxSizer42->Add(itemStaticBoxSizer52, 0, wxGROW|wxALL, 0);
    m_chkOpenClickWinAtStartup = new wxCheckBox( itemPanel41, ID_CHECKBOX_CLICKWIN_AT_STARTUP, _("Open Click Window at startup"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkOpenClickWinAtStartup->SetValue(false);
    if (WConfiguration::ShowToolTips())
        m_chkOpenClickWinAtStartup->SetToolTip(_("If checked the Click Window is automatically\nopened at program startup."));
    itemStaticBoxSizer52->Add(m_chkOpenClickWinAtStartup, 0, wxALIGN_LEFT|wxALL, 5);

    m_chkShowClickWin = new wxCheckBox( itemPanel41, ID_CHECKBOX_SHOW_CLICKWIN, _("Show Click Window"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkShowClickWin->SetValue(false);
    itemStaticBoxSizer52->Add(m_chkShowClickWin, 0, wxALIGN_LEFT|wxALL, 5);

    wxFlexGridSizer* itemFlexGridSizer55 = new wxFlexGridSizer(2, 2, 0, 0);
    itemStaticBoxSizer52->Add(itemFlexGridSizer55, 0, wxALIGN_LEFT|wxALL, 0);
    m_stDesign = new wxStaticText( itemPanel41, ID_STATIC_DESIGN, _("Design:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer55->Add(m_stDesign, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString m_cmbClickWindowDesignStrings;
    m_cmbClickWindowDesignStrings.Add(_("Normal"));
    m_cmbClickWindowDesignStrings.Add(_("Thin"));
    m_cmbClickWindowDesign = new wxComboBox( itemPanel41, ID_COMBOBOX_DESIGN, wxEmptyString, wxDefaultPosition, wxDefaultSize, m_cmbClickWindowDesignStrings, wxCB_READONLY );
    itemFlexGridSizer55->Add(m_cmbClickWindowDesign, 0, wxALIGN_LEFT|wxGROW|wxALL, 5);

    m_stBehaviour = new wxStaticText( itemPanel41, ID_STATIC_BEHAVIOUR, _("Behaviour:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer55->Add(m_stBehaviour, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString m_choClickWindowBehaviourStrings;
    m_choClickWindowBehaviourStrings.Add(_("Normal mode"));
    m_choClickWindowBehaviourStrings.Add(_("Fast mode"));
    m_choClickWindowBehaviour = new wxChoice( itemPanel41, ID_CHOICE_BEHAVIOUR, wxDefaultPosition, wxDefaultSize, m_choClickWindowBehaviourStrings, 0 );
    if (WConfiguration::ShowToolTips())
        m_choClickWindowBehaviour->SetToolTip(_("Fast mode enables click type\nselection by hovering the mouse\npointer over the click window\nbuttons."));
    itemFlexGridSizer55->Add(m_choClickWindowBehaviour, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_chkEnableGestureClick = new wxCheckBox( itemPanel41, ID_CHECKBOX, _("Enable click with gestures"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkEnableGestureClick->SetValue(false);
    itemBoxSizer42->Add(m_chkEnableGestureClick, 0, wxALIGN_LEFT|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer61Static = new wxStaticBox(itemPanel41, wxID_ANY, _("Gesture click"));
    wxStaticBoxSizer* itemStaticBoxSizer61 = new wxStaticBoxSizer(itemStaticBoxSizer61Static, wxVERTICAL);
    itemBoxSizer42->Add(itemStaticBoxSizer61, 0, wxGROW|wxALL, 5);
    wxBoxSizer* itemBoxSizer62 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer61->Add(itemBoxSizer62, 0, wxALIGN_LEFT|wxALL, 5);
    m_stMoveLeft = new wxStaticText( itemPanel41, ID_STATIC_MOVE_LEFT, _("Move left:"), wxDefaultPosition, wxSize(100, -1), 0 );
    itemBoxSizer62->Add(m_stMoveLeft, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString m_cmbLeftStrings;
    m_cmbLeftStrings.Add(_("Disable"));
    m_cmbLeftStrings.Add(_("Single click"));
    m_cmbLeftStrings.Add(_("Secondary click"));
    m_cmbLeftStrings.Add(_("Double click"));
    m_cmbLeftStrings.Add(_("Drag click"));
    m_cmbLeftStrings.Add(_("A"));
    m_cmbLeftStrings.Add(_("B"));
    m_cmbLeftStrings.Add(_("C"));
    m_cmbLeftStrings.Add(_("D"));
    m_cmbLeftStrings.Add(_("E"));
    m_cmbLeftStrings.Add(_("F"));
    m_cmbLeftStrings.Add(_("G"));
    m_cmbLeftStrings.Add(_("H"));
    m_cmbLeftStrings.Add(_("I"));
    m_cmbLeftStrings.Add(_("J"));
    m_cmbLeftStrings.Add(_("K"));
    m_cmbLeftStrings.Add(_("L"));
    m_cmbLeftStrings.Add(_("M"));
    m_cmbLeftStrings.Add(_("N"));
    m_cmbLeftStrings.Add(_("O"));
    m_cmbLeftStrings.Add(_("P"));
    m_cmbLeftStrings.Add(_("Q"));
    m_cmbLeftStrings.Add(_("R"));
    m_cmbLeftStrings.Add(_("S"));
    m_cmbLeftStrings.Add(_("T"));
    m_cmbLeftStrings.Add(_("U"));
    m_cmbLeftStrings.Add(_("V"));
    m_cmbLeftStrings.Add(_("W"));
    m_cmbLeftStrings.Add(_("X"));
    m_cmbLeftStrings.Add(_("Y"));
    m_cmbLeftStrings.Add(_("Z"));
    m_cmbLeftStrings.Add(_("0"));
    m_cmbLeftStrings.Add(_("1"));
    m_cmbLeftStrings.Add(_("2"));
    m_cmbLeftStrings.Add(_("3"));
    m_cmbLeftStrings.Add(_("4"));
    m_cmbLeftStrings.Add(_("5"));
    m_cmbLeftStrings.Add(_("6"));
    m_cmbLeftStrings.Add(_("7"));
    m_cmbLeftStrings.Add(_("8"));
    m_cmbLeftStrings.Add(_("9"));
    m_cmbLeft = new wxComboBox( itemPanel41, ID_COMBOBOX_LEFT, wxEmptyString, wxDefaultPosition, wxDefaultSize, m_cmbLeftStrings, wxCB_READONLY );
    itemBoxSizer62->Add(m_cmbLeft, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer65 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer61->Add(itemBoxSizer65, 0, wxALIGN_LEFT|wxALL, 5);
    m_stMoveRight = new wxStaticText( itemPanel41, ID_STATIC_MOVE_RIGHT, _("Move right:"), wxDefaultPosition, wxSize(100, -1), 0 );
    itemBoxSizer65->Add(m_stMoveRight, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString m_cmbRightStrings;
    m_cmbRightStrings.Add(_("Disable"));
    m_cmbRightStrings.Add(_("Single click"));
    m_cmbRightStrings.Add(_("Secondary click"));
    m_cmbRightStrings.Add(_("Double click"));
    m_cmbRightStrings.Add(_("Drag click"));
    m_cmbRightStrings.Add(_("A"));
    m_cmbRightStrings.Add(_("B"));
    m_cmbRightStrings.Add(_("C"));
    m_cmbRightStrings.Add(_("D"));
    m_cmbRightStrings.Add(_("E"));
    m_cmbRightStrings.Add(_("F"));
    m_cmbRightStrings.Add(_("G"));
    m_cmbRightStrings.Add(_("H"));
    m_cmbRightStrings.Add(_("I"));
    m_cmbRightStrings.Add(_("J"));
    m_cmbRightStrings.Add(_("K"));
    m_cmbRightStrings.Add(_("L"));
    m_cmbRightStrings.Add(_("M"));
    m_cmbRightStrings.Add(_("N"));
    m_cmbRightStrings.Add(_("O"));
    m_cmbRightStrings.Add(_("P"));
    m_cmbRightStrings.Add(_("Q"));
    m_cmbRightStrings.Add(_("R"));
    m_cmbRightStrings.Add(_("S"));
    m_cmbRightStrings.Add(_("T"));
    m_cmbRightStrings.Add(_("U"));
    m_cmbRightStrings.Add(_("V"));
    m_cmbRightStrings.Add(_("W"));
    m_cmbRightStrings.Add(_("X"));
    m_cmbRightStrings.Add(_("Y"));
    m_cmbRightStrings.Add(_("Z"));
    m_cmbRightStrings.Add(_("0"));
    m_cmbRightStrings.Add(_("1"));
    m_cmbRightStrings.Add(_("2"));
    m_cmbRightStrings.Add(_("3"));
    m_cmbRightStrings.Add(_("4"));
    m_cmbRightStrings.Add(_("5"));
    m_cmbRightStrings.Add(_("6"));
    m_cmbRightStrings.Add(_("7"));
    m_cmbRightStrings.Add(_("8"));
    m_cmbRightStrings.Add(_("9"));
    m_cmbRight = new wxComboBox( itemPanel41, ID_COMBOBOX_RIGHT, wxEmptyString, wxDefaultPosition, wxDefaultSize, m_cmbRightStrings, wxCB_READONLY );
    itemBoxSizer65->Add(m_cmbRight, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer68 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer61->Add(itemBoxSizer68, 0, wxALIGN_LEFT|wxALL, 5);
    m_stMoveTop = new wxStaticText( itemPanel41, ID_STATIC_MOVE_TOP, _("Move top:"), wxDefaultPosition, wxSize(100, -1), 0 );
    itemBoxSizer68->Add(m_stMoveTop, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString m_cmbTopStrings;
    m_cmbTopStrings.Add(_("Disable"));
    m_cmbTopStrings.Add(_("Single click"));
    m_cmbTopStrings.Add(_("Secondary click"));
    m_cmbTopStrings.Add(_("Double click"));
    m_cmbTopStrings.Add(_("Drag click"));
    m_cmbTopStrings.Add(_("A"));
    m_cmbTopStrings.Add(_("B"));
    m_cmbTopStrings.Add(_("C"));
    m_cmbTopStrings.Add(_("D"));
    m_cmbTopStrings.Add(_("E"));
    m_cmbTopStrings.Add(_("F"));
    m_cmbTopStrings.Add(_("G"));
    m_cmbTopStrings.Add(_("H"));
    m_cmbTopStrings.Add(_("I"));
    m_cmbTopStrings.Add(_("J"));
    m_cmbTopStrings.Add(_("K"));
    m_cmbTopStrings.Add(_("L"));
    m_cmbTopStrings.Add(_("M"));
    m_cmbTopStrings.Add(_("N"));
    m_cmbTopStrings.Add(_("O"));
    m_cmbTopStrings.Add(_("P"));
    m_cmbTopStrings.Add(_("Q"));
    m_cmbTopStrings.Add(_("R"));
    m_cmbTopStrings.Add(_("S"));
    m_cmbTopStrings.Add(_("T"));
    m_cmbTopStrings.Add(_("U"));
    m_cmbTopStrings.Add(_("V"));
    m_cmbTopStrings.Add(_("W"));
    m_cmbTopStrings.Add(_("X"));
    m_cmbTopStrings.Add(_("Y"));
    m_cmbTopStrings.Add(_("Z"));
    m_cmbTopStrings.Add(_("0"));
    m_cmbTopStrings.Add(_("1"));
    m_cmbTopStrings.Add(_("2"));
    m_cmbTopStrings.Add(_("3"));
    m_cmbTopStrings.Add(_("4"));
    m_cmbTopStrings.Add(_("5"));
    m_cmbTopStrings.Add(_("6"));
    m_cmbTopStrings.Add(_("7"));
    m_cmbTopStrings.Add(_("8"));
    m_cmbTopStrings.Add(_("9"));
    m_cmbTop = new wxComboBox( itemPanel41, ID_COMBOBOX_TOP, wxEmptyString, wxDefaultPosition, wxDefaultSize, m_cmbTopStrings, wxCB_READONLY );
    itemBoxSizer68->Add(m_cmbTop, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer71 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer61->Add(itemBoxSizer71, 0, wxALIGN_LEFT|wxALL, 5);
    m_stMoveBottom = new wxStaticText( itemPanel41, ID_STATIC_MOVE_BOTTOM, _("Move bottom:"), wxDefaultPosition, wxSize(100, -1), 0 );
    itemBoxSizer71->Add(m_stMoveBottom, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString m_cmbBottomStrings;
    m_cmbBottomStrings.Add(_("Disable"));
    m_cmbBottomStrings.Add(_("Single click"));
    m_cmbBottomStrings.Add(_("Secondary click"));
    m_cmbBottomStrings.Add(_("Double click"));
    m_cmbBottomStrings.Add(_("Drag click"));
    m_cmbBottomStrings.Add(_("A"));
    m_cmbBottomStrings.Add(_("B"));
    m_cmbBottomStrings.Add(_("C"));
    m_cmbBottomStrings.Add(_("D"));
    m_cmbBottomStrings.Add(_("E"));
    m_cmbBottomStrings.Add(_("F"));
    m_cmbBottomStrings.Add(_("G"));
    m_cmbBottomStrings.Add(_("H"));
    m_cmbBottomStrings.Add(_("I"));
    m_cmbBottomStrings.Add(_("J"));
    m_cmbBottomStrings.Add(_("K"));
    m_cmbBottomStrings.Add(_("L"));
    m_cmbBottomStrings.Add(_("M"));
    m_cmbBottomStrings.Add(_("N"));
    m_cmbBottomStrings.Add(_("O"));
    m_cmbBottomStrings.Add(_("P"));
    m_cmbBottomStrings.Add(_("Q"));
    m_cmbBottomStrings.Add(_("R"));
    m_cmbBottomStrings.Add(_("S"));
    m_cmbBottomStrings.Add(_("T"));
    m_cmbBottomStrings.Add(_("U"));
    m_cmbBottomStrings.Add(_("V"));
    m_cmbBottomStrings.Add(_("W"));
    m_cmbBottomStrings.Add(_("X"));
    m_cmbBottomStrings.Add(_("Y"));
    m_cmbBottomStrings.Add(_("Z"));
    m_cmbBottomStrings.Add(_("0"));
    m_cmbBottomStrings.Add(_("1"));
    m_cmbBottomStrings.Add(_("2"));
    m_cmbBottomStrings.Add(_("3"));
    m_cmbBottomStrings.Add(_("4"));
    m_cmbBottomStrings.Add(_("5"));
    m_cmbBottomStrings.Add(_("6"));
    m_cmbBottomStrings.Add(_("7"));
    m_cmbBottomStrings.Add(_("8"));
    m_cmbBottomStrings.Add(_("9"));
    m_cmbBottom = new wxComboBox( itemPanel41, ID_COMBOBOX_BOTTOM, wxEmptyString, wxDefaultPosition, wxDefaultSize, m_cmbBottomStrings, wxCB_READONLY );
    itemBoxSizer71->Add(m_cmbBottom, 0, wxGROW|wxALL, 5);

    itemNotebook4->AddPage(itemPanel41, _("Click"));

    wxPanel* itemPanel74 = new wxPanel( itemNotebook4, ID_PANEL_ADVANCED, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer75 = new wxBoxSizer(wxVERTICAL);
    itemPanel74->SetSizer(itemBoxSizer75);

    wxStaticBox* itemStaticBoxSizer76Static = new wxStaticBox(itemPanel74, wxID_ANY, _("Face localization"));
    wxStaticBoxSizer* itemStaticBoxSizer76 = new wxStaticBoxSizer(itemStaticBoxSizer76Static, wxVERTICAL);
    itemBoxSizer75->Add(itemStaticBoxSizer76, 0, wxGROW|wxALL, 5);
    m_chkAutoLocateFace = new wxCheckBox( itemPanel74, ID_CHECKBOX_AUTO_LOCATE_FACE, _("Locate face automatically"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkAutoLocateFace->SetValue(false);
    if (WConfiguration::ShowToolTips())
        m_chkAutoLocateFace->SetToolTip(_("When enabled tries to automatically\ndetect your face and center the\ntracking area."));
    itemStaticBoxSizer76->Add(m_chkAutoLocateFace, 0, wxALIGN_LEFT|wxALL, 5);

    m_chkShowAutoLocateFaceFilter = new wxCheckBox( itemPanel74, ID_CHECKBOX_SHOW_LOCATE_FACE_FILTER, _("Show locate face filter"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkShowAutoLocateFaceFilter->SetValue(false);
    m_chkShowAutoLocateFaceFilter->Enable(false);
    itemStaticBoxSizer76->Add(m_chkShowAutoLocateFaceFilter, 0, wxALIGN_LEFT|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer79Static = new wxStaticBox(itemPanel74, wxID_ANY, _("On-screen keyboard command"));
    wxStaticBoxSizer* itemStaticBoxSizer79 = new wxStaticBoxSizer(itemStaticBoxSizer79Static, wxVERTICAL);
    itemBoxSizer75->Add(itemStaticBoxSizer79, 0, wxGROW|wxALL, 5);
    m_txtOnScreenKeyboardCommand = new wxTextCtrl( itemPanel74, ID_TEXTCTRL_ONSCREENKEYBOARDCOMMAND, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    if (WConfiguration::ShowToolTips())
        m_txtOnScreenKeyboardCommand->SetToolTip(_("Command to run an external on-screen keyboard application."));
    itemStaticBoxSizer79->Add(m_txtOnScreenKeyboardCommand, 0, wxGROW|wxALL, 5);

    m_btntOnScreenKeyboardCommand = new wxButton( itemPanel74, ID_BUTTON_ONSCREENKEYBOARDCOMMAND, _("Browse..."), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer79->Add(m_btntOnScreenKeyboardCommand, 0, wxALIGN_LEFT|wxALL, 5);

#if defined(__WXGTK__)
    wxStaticBox* itemStaticBoxSizer82Static = new wxStaticBox(itemPanel74, wxID_ANY, _("Activation key"));
    wxStaticBoxSizer* itemStaticBoxSizer82 = new wxStaticBoxSizer(itemStaticBoxSizer82Static, wxVERTICAL);
    itemBoxSizer75->Add(itemStaticBoxSizer82, 0, wxGROW|wxALL, 5);
#if defined(__WXGTK__)
    m_chkActivationKey = new wxCheckBox( itemPanel74, ID_CHECKBOX_ACTIVATION_KEY, _("Use activation key"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkActivationKey->SetValue(false);
    if (WConfiguration::ShowToolTips())
        m_chkActivationKey->SetToolTip(_("When checked it allows to enable or \ndisable eViacam by pressing a key."));
    itemStaticBoxSizer82->Add(m_chkActivationKey, 0, wxALIGN_LEFT|wxALL, 5);
#endif

#if defined(__WXGTK__)
    m_txtActivationKey = new wxTextCtrl( itemPanel74, ID_TEXTCTRL_ACTIVATION_KEY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    if (WConfiguration::ShowToolTips())
        m_txtActivationKey->SetToolTip(_("Shows the selected key."));
    m_txtActivationKey->Enable(false);
    itemStaticBoxSizer82->Add(m_txtActivationKey, 0, wxGROW|wxALL, 5);
#endif

#if defined(__WXGTK__)
    m_buttonActivationKey = new wxButton( itemPanel74, ID_BUTTON_ACTIVATION_KEY, _("Set key"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer82->Add(m_buttonActivationKey, 0, wxALIGN_LEFT|wxALL, 5);
#endif

#endif

    itemNotebook4->AddPage(itemPanel74, _("Advanced"));

    itemListbook3->AddPage(itemNotebook4, _("Profile options"), false, 0);

    wxPanel* itemPanel86 = new wxPanel( itemListbook3, ID_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer87 = new wxBoxSizer(wxVERTICAL);
    itemPanel86->SetSizer(itemBoxSizer87);

#if defined(__WXGTK__)
    wxStaticBox* itemStaticBoxSizer88Static = new wxStaticBox(itemPanel86, wxID_ANY, _("Run at startup"));
    wxStaticBoxSizer* itemStaticBoxSizer88 = new wxStaticBoxSizer(itemStaticBoxSizer88Static, wxHORIZONTAL);
    itemBoxSizer87->Add(itemStaticBoxSizer88, 0, wxGROW|wxALL, 5);
    m_chkStartup = new wxCheckBox( itemPanel86, ID_CHECKBOX_STARTUP, _("Start eViacam at user logon"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkStartup->SetValue(false);
    itemStaticBoxSizer88->Add(m_chkStartup, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

#endif

    wxStaticBox* itemStaticBoxSizer90Static = new wxStaticBox(itemPanel86, wxID_ANY, _("Profile management"));
    wxStaticBoxSizer* itemStaticBoxSizer90 = new wxStaticBoxSizer(itemStaticBoxSizer90Static, wxVERTICAL);
    itemBoxSizer87->Add(itemStaticBoxSizer90, 0, wxGROW|wxALL, 5);
    wxBoxSizer* itemBoxSizer91 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer90->Add(itemBoxSizer91, 0, wxGROW|wxALL, 0);
    wxStaticText* itemStaticText92 = new wxStaticText( itemPanel86, wxID_STATIC, _("Select profile:"), wxDefaultPosition, wxDefaultSize, 0 );
    if (WConfiguration::ShowToolTips())
        itemStaticText92->SetToolTip(_("Chooses desired active profile"));
    itemBoxSizer91->Add(itemStaticText92, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString m_choProfileStrings;
    m_choProfile = new wxChoice( itemPanel86, ID_CHOICE_PROFILE, wxDefaultPosition, wxDefaultSize, m_choProfileStrings, 0 );
    if (WConfiguration::ShowToolTips())
        m_choProfile->SetToolTip(_("Chooses desired active profile"));
    itemBoxSizer91->Add(m_choProfile, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer94 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer90->Add(itemBoxSizer94, 0, wxALIGN_LEFT|wxALL, 0);
    m_btnAddProfile = new wxButton( itemPanel86, ID_BUTTON_ADD_PROFILE, _("Add profile"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer94->Add(m_btnAddProfile, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_btnDeleteProfile = new wxButton( itemPanel86, ID_BUTTON_DEL_PROFILE, _("Delete profile"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer94->Add(m_btnDeleteProfile, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer97Static = new wxStaticBox(itemPanel86, wxID_ANY, _("Selected camera"));
    wxStaticBoxSizer* itemStaticBoxSizer97 = new wxStaticBoxSizer(itemStaticBoxSizer97Static, wxVERTICAL);
    itemBoxSizer87->Add(itemStaticBoxSizer97, 0, wxGROW|wxALL, 5);
    wxBoxSizer* itemBoxSizer98 = new wxBoxSizer(wxVERTICAL);
    itemStaticBoxSizer97->Add(itemBoxSizer98, 0, wxGROW|wxALL, 0);
    m_txtSelectedCamera = new wxTextCtrl( itemPanel86, ID_TEXTCTRL_SELECTED_CAMERA, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
    m_txtSelectedCamera->Enable(false);
    itemBoxSizer98->Add(m_txtSelectedCamera, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer100 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer97->Add(itemBoxSizer100, 0, wxALIGN_LEFT|wxALL, 0);
    m_btnCameraSettings = new wxButton( itemPanel86, ID_BUTTON_CAMERA_SETTINGS, _("Settings..."), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer100->Add(m_btnCameraSettings, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton102 = new wxButton( itemPanel86, ID_BUTTON_CHANGE_CAMERA, _("Change"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer100->Add(itemButton102, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxFlexGridSizer* itemFlexGridSizer103 = new wxFlexGridSizer(0, 2, 0, 0);
    itemBoxSizer87->Add(itemFlexGridSizer103, 0, wxALIGN_LEFT|wxALL, 5);
    wxStaticText* itemStaticText104 = new wxStaticText( itemPanel86, wxID_STATIC, _("Language:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer103->Add(itemStaticText104, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString m_choLanguageStrings;
    m_choLanguage = new wxChoice( itemPanel86, ID_CHOICE_LANGUAGE, wxDefaultPosition, wxDefaultSize, m_choLanguageStrings, 0 );
    itemFlexGridSizer103->Add(m_choLanguage, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxCheckBox* itemCheckBox106 = new wxCheckBox( itemPanel86, ID_CHECKBOX_RUN_STARTUP, _("Run at system startup"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox106->SetValue(false);
    itemCheckBox106->Show(false);
    itemFlexGridSizer103->Add(itemCheckBox106, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemListbook3->AddPage(itemPanel86, _("General options"), false, 1);

    itemBoxSizer2->Add(itemListbook3, 0, wxALIGN_LEFT|wxALL, 5);

    wxBoxSizer* itemBoxSizer107 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer107, 0, wxALIGN_RIGHT|wxALL, 5);

    wxButton* itemButton108 = new wxButton( itemDialog1, ID_BUTTON_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer107->Add(itemButton108, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_btnCancel = new wxButton( itemDialog1, ID_BUTTON_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    m_btnCancel->Enable(false);
    itemBoxSizer107->Add(m_btnCancel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end WConfiguration content construction
}


/*!
 * Should we show tooltips?
 */

bool WConfiguration::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap WConfiguration::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin WConfiguration bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end WConfiguration bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon WConfiguration::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin WConfiguration icon retrieval
    wxUnusedVar(name);
    if (name == _T("icons/eviacam.xpm"))
    {
        wxIcon icon(eviacam);
        return icon;
    }
    else if (name == _T("icons/user.xpm"))
    {
        wxIcon icon(user);
        return icon;
    }
    return wxNullIcon;
////@end WConfiguration icon retrieval
}

void WConfiguration::InitializeData ()
{
	// 
	// Profile data
	//

	// Motion options
	m_spinXSpeed->SetValue (m_pViacamController->GetMouseOutput()->GetXSpeed());
	m_spinYSpeed->SetValue (m_pViacamController->GetMouseOutput()->GetYSpeed());
	m_spinAcceleration->SetValue (m_pViacamController->GetMouseOutput()->GetAcceleration());
	m_spinSmoothness->SetValue (m_pViacamController->GetMouseOutput()->GetSmoothness());
	m_spinEasyStop->SetValue (m_pViacamController->GetMouseOutput()->GetEasyStopValue());
    
    // Workspace
    m_chkEnabledWorkspace->SetValue (m_pViacamController->GetMouseOutput()->GetRestrictedWorkingArea());
    if (m_pViacamController->GetMouseOutput()->GetRestrictedWorkingArea()) {
        m_spin_top_workspace->Enable(true);
        m_spin_left_workspace->Enable(true);
        m_spin_right_workspace->Enable(true);
        m_spin_bottom_workspace->Enable(true);
    }
    m_spin_top_workspace->SetValue (m_pViacamController->GetMouseOutput()->GetTopWorkspace());
    m_spin_left_workspace->SetValue (m_pViacamController->GetMouseOutput()->GetLeftWorkspace());
    m_spin_right_workspace->SetValue (m_pViacamController->GetMouseOutput()->GetRightWorkspace());
    m_spin_bottom_workspace->SetValue (m_pViacamController->GetMouseOutput()->GetBottomWorkspace());

	// Clic
	m_chkDwellClickEnabled->SetValue (m_pViacamController->GetMouseOutput()->GetClickMode()!= CMouseOutput::NONE);
	m_chkEnableGestureClick->SetValue (m_pViacamController->GetMouseOutput()->GetClickMode()== CMouseOutput::GESTURE);
	m_chkAllowConsecutiveClick->SetValue (m_pViacamController->GetMouseOutput()->GetConsecutiveClicksAllowed());
	m_chkBeepOnClick->SetValue (m_pViacamController->GetMouseOutput()->GetBeepOnClick());
	//m_chkShowClickCountdown->SetValue (m_pViacamController->GetMouseOutput()->GetShowClickCountdown());
	m_spinDwellTime->SetValue (m_pViacamController->GetMouseOutput()->GetDwellTime());
	m_spinDwellArea->SetValue ( m_pViacamController->GetMouseOutput()->GetDwellToleranceArea () );
	m_chkShowClickWin->SetValue ( m_pViacamController->GetClickWindowController()->IsShown() );
	m_choClickWindowBehaviour->Select (m_pViacamController->GetClickWindowController()->GetFastMode() ? 1 : 0);
	m_cmbClickWindowDesign->Select (m_pViacamController->GetClickWindowController()->GetDesign());
	m_cmbLeft->Select (m_pViacamController->GetMouseOutput()->GetActionLeft());
	m_cmbRight->Select (m_pViacamController->GetMouseOutput()->GetActionRight());
	m_cmbTop->Select (m_pViacamController->GetMouseOutput()->GetActionTop());
	m_cmbBottom->Select (m_pViacamController->GetMouseOutput()->GetActionBottom());
	if (m_chkDwellClickEnabled->IsChecked())
	{
		EnableClickOptions(true);
		if (m_chkEnableGestureClick->IsChecked())
			EnableGestureOptions (true);
		else	
			EnableGestureOptions (false);
	}		
	else
	{
		EnableClickOptions(false);
	}	
	
	

	
	//m_pViacamController->GetClickWindowController()->GetDesign ();
	//m_radNormalDesign->;
	//m_radThinDesign;
	//	m_radNormalBehaviour->SetValue (!m_pViacamController->GetClickWindowController()->GetFastMode());
	//	m_radFastBehaviour->SetValue (m_pViacamController->GetClickWindowController()->GetFastMode());

	// Startup
	m_chkEnabledAtStartup->SetValue (m_pViacamController->GetEnabledAtStartup());
    m_chkOpenClickWinAtStartup->SetValue (
		m_pViacamController->GetClickWindowController()->GetOpenClickWinAtStartup());
	
	// Advanced
	m_chkAutoLocateFace->SetValue (m_pViacamController->GetMotionTracker()->GetTrackFace());
	m_chkShowAutoLocateFaceFilter->Enable (m_chkAutoLocateFace->GetValue());
	m_chkShowAutoLocateFaceFilter->SetValue (m_pViacamController->GetMotionTracker()->GetShowTrackFaceFilter());
	m_txtOnScreenKeyboardCommand->SetValue(m_pViacamController->GetOnScreenKeyboardCommand());
#if defined(__WXGTK__)
    m_chkActivationKey->SetValue(m_pViacamController->GetEnabledActivationKey());
    m_txtActivationKey->SetValue(m_pViacamController->GetActivationKeyName());
	    
	// 
	// App data
	//

    m_chkStartup->SetValue(m_pAutostart->IsEnabled());
#endif
	// Profile combo
	m_choProfile->Clear();
	m_choProfile->Append (m_pViacamController->GetConfigManager()->GetProfileList());
	m_choProfile->Select (m_pViacamController->GetConfigManager()->GetCurrentProfile());

	// Profile buttons
	if (m_pViacamController->GetConfigManager()->GetCurrentProfile()== 0)
		m_btnDeleteProfile->Enable (false);
	else
		m_btnDeleteProfile->Enable (true);

	// Fill and set language combo
	m_choLanguage->Clear();
	m_choLanguage->Append (wxGetTranslation(s_langNames[0]));
	for (unsigned int i= 1; i< WXSIZEOF(s_langNames); i++) {
		//m_choLanguage->Append (wxGetTranslation(s_langNames[i]));
		m_choLanguage->Append (s_langNames[i]);
		
	}		
	// Select current language
	for (unsigned int i= 0; i< WXSIZEOF(s_langNames); i++)
		if (s_langIds[i]== m_pViacamController->GetLanguage())
			m_choLanguage->SetSelection(i);
	
	// Camera
	m_txtSelectedCamera->SetValue (m_pViacamController->GetCameraName());
	if (m_pViacamController->CameraHasSettingsDialog ())
		m_btnCameraSettings->Enable (true);
	else
		m_btnCameraSettings->Enable (false);

//	m_fpickOnScreenKeyboardCommand->SetPath (m_pViacamController->GetOnScreenKeyboardCommand());
}

void WConfiguration::EnableClickOptions (bool enable)
{
	m_spinDwellTime->Enable(enable);
	m_spinDwellArea->Enable(enable);
	m_chkAllowConsecutiveClick->Enable(enable);
	m_chkBeepOnClick->Enable(enable);
	m_chkOpenClickWinAtStartup->Enable(enable);
	m_chkShowClickWin->Enable(enable);
	m_cmbClickWindowDesign->Enable(enable);
	m_choClickWindowBehaviour->Enable(enable);
	m_cmbLeft->Enable(enable);
	m_cmbRight->Enable(enable);			
	m_cmbTop->Enable(enable);			
	m_cmbBottom->Enable(enable);
	m_chkEnableGestureClick->Enable(enable);
	m_stDwellTime->Enable(enable);
	m_stDwellArea->Enable(enable);
	m_stDesign->Enable(enable);
	m_stBehaviour->Enable(enable);
	m_stMoveLeft->Enable(enable);
	m_stMoveRight->Enable(enable);
	m_stMoveTop->Enable(enable);
	m_stMoveBottom->Enable(enable);
}

void WConfiguration::EnableGestureOptions (bool enable)
{
	m_cmbLeft->Enable(enable);
	m_cmbRight->Enable(enable);			
	m_cmbTop->Enable(enable);			
	m_cmbBottom->Enable(enable);
	m_stMoveLeft->Enable(enable);
	m_stMoveRight->Enable(enable);
	m_stMoveTop->Enable(enable);
	m_stMoveBottom->Enable(enable);
	m_chkOpenClickWinAtStartup->Enable(!enable);
	m_chkShowClickWin->Enable(!enable);
	m_cmbClickWindowDesign->Enable(!enable);
	m_choClickWindowBehaviour->Enable(!enable);
	m_stDesign->Enable(!enable);
	m_stBehaviour->Enable(!enable);
}

/*
void WConfiguration::OnFilectrlFilePickerChanged( wxFileDirPickerEvent& event )
{	
	wxString path= event.GetPath();
	m_pViacamController->SetOnScreenKeyboardCommand	(path);
	event.Skip(false);
	Changed ();
}*/

void WConfiguration::Changed ()
{
	if (!m_dirty)
	{
		m_dirty= true;
		m_btnCancel->Enable (true);
	}
}

void WConfiguration::UnChanged ()
{
	if (m_dirty)
	{
		m_dirty= false;
		m_btnCancel->Enable (false);
	}
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_OK
 */

void WConfiguration::OnButtonOkClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_OK in WConfiguration.
    // Before editing this code, remove the block markers.
    EndModal(ID_BUTTON_OK);
////@end wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_OK in WConfiguration. 
	event.Skip(false);
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_CANCEL
 */

void WConfiguration::OnButtonCancelClick( wxCommandEvent& event )
{
	wxMessageDialog dlg (NULL, _("Discard changes?"), _("eViacam warning"), wxICON_EXCLAMATION | wxYES_NO );
	if (dlg.ShowModal()== wxID_YES)
	{
		EndModal(ID_BUTTON_CANCEL);		
	}		
	event.Skip(false);
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_ENABLE_AT_STARTUP
 */

void WConfiguration::OnCheckboxEnableAtStartupClick( wxCommandEvent& event )
{
	m_pViacamController->SetEnabledAtStartup (m_chkEnabledAtStartup->GetValue());
	event.Skip(false);
	Changed ();
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_CLICKWIN_AT_STARTUP
 */

void WConfiguration::OnCheckboxClickwinAtStartupClick( wxCommandEvent& event )
{
	m_pViacamController->GetClickWindowController()->SetOpenClickWinAtStartup (
		m_chkOpenClickWinAtStartup->GetValue());
	event.Skip(false);
	Changed ();
}


/*!
 * wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL_XSPEED
 */

void WConfiguration::OnSpinctrlXspeedUpdated( wxSpinEvent& event )
{
	m_pViacamController->GetMouseOutput()->SetXSpeed(m_spinXSpeed->GetValue());
	event.Skip(false);
	Changed ();
}


/*!
 * wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL_YSPEED
 */

void WConfiguration::OnSpinctrlYspeedUpdated( wxSpinEvent& event )
{
	m_pViacamController->GetMouseOutput()->SetYSpeed(m_spinYSpeed->GetValue());
	event.Skip(false);
	Changed ();
}


/*!
 * wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL_ACCELERATION
 */

void WConfiguration::OnSpinctrlAccelerationUpdated( wxSpinEvent& event )
{
	m_pViacamController->GetMouseOutput()->SetAcceleration(m_spinAcceleration->GetValue());
	event.Skip(false);
	Changed ();
}

/*!
 * wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL_SMOOTHNESS
 */

void WConfiguration::OnSpinctrlSmoothnessUpdated( wxSpinEvent& event )
{
	m_pViacamController->GetMouseOutput()->SetSmoothness(m_spinSmoothness->GetValue());
	event.Skip(false);
	Changed ();
}


/*!
 * wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL_EASYSTOP
 */

void WConfiguration::OnSpinctrlEasystopUpdated( wxSpinEvent& event )
{
	m_pViacamController->GetMouseOutput()->SetEasyStopValue(m_spinEasyStop->GetValue());
	event.Skip(false);
	Changed ();
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_ENABLE_DWELL
 */

void WConfiguration::OnCheckboxEnableDwellClick( wxCommandEvent& event )
{
	if (!m_chkDwellClickEnabled->GetValue())
	{
		wxMessageDialog dlg (NULL, _("This action will disable eViacam click generation.\nAre you sure?"), _T("Enable Viacam"), wxICON_EXCLAMATION | wxYES_NO );
		if (dlg.ShowModal()== wxID_YES)
		{
			m_pViacamController->GetMouseOutput()->SetClickMode(CMouseOutput::NONE);
			EnableClickOptions(false);
			Changed ();
		}
	}
	else
	{
		m_pViacamController->GetMouseOutput()->SetClickMode(CMouseOutput::DWELL);
		EnableClickOptions(true);
		if (m_chkEnableGestureClick->IsChecked())
			EnableGestureOptions (true);
		else	
			EnableGestureOptions (false);
	
		Changed ();
	}

	m_chkDwellClickEnabled->SetValue (m_pViacamController->GetMouseOutput()->GetClickMode()== CMouseOutput::DWELL);
	event.Skip(false);	
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_ALLOW_CONSECUTIVE
 */

void WConfiguration::OnCheckboxAllowConsecutiveClick( wxCommandEvent& event )
{
	m_pViacamController->GetMouseOutput()->SetConsecutiveClicksAllowed (m_chkAllowConsecutiveClick->GetValue());
	event.Skip(false);
	Changed ();
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_BEEP_ON_CLICK
 */

void WConfiguration::OnCheckboxBeepOnClickClick( wxCommandEvent& event )
{
	m_pViacamController->GetMouseOutput()->SetBeepOnClick (m_chkBeepOnClick->GetValue());
	event.Skip(false);
	Changed ();
}




/*!
 * wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL_DWELL_TIME
 */

void WConfiguration::OnSpinctrlDwellTimeUpdated( wxSpinEvent& event )
{
	m_pViacamController->GetMouseOutput()->SetDwellTime (m_spinDwellTime->GetValue());
	event.Skip(false);
	Changed ();
}


/*!
 * wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL_DWELL_AREA
 */

void WConfiguration::OnSpinctrlDwellAreaUpdated( wxSpinEvent& event )
{
	m_pViacamController->GetMouseOutput()->SetDwellToleranceArea (m_spinDwellArea->GetValue());		
	event.Skip(false);
	Changed ();
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_SHOW_CLICKWIN
 */

void WConfiguration::OnCheckboxShowClickwinClick( wxCommandEvent& event )
{
	m_pViacamController->GetClickWindowController()->Show (
		! m_pViacamController->GetClickWindowController()->IsShown() );
	event.Skip(false);
	Changed ();
}

/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_AUTO_LOCATE_FACE
 */

void WConfiguration::OnCheckboxAutoLocateFaceClick( wxCommandEvent& event )
{
	m_pViacamController->GetMotionTracker()->SetTrackFace (m_chkAutoLocateFace->GetValue());
	m_chkShowAutoLocateFaceFilter->Enable (m_chkAutoLocateFace->GetValue());
	event.Skip(false);
	Changed ();
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_SHOW_LOCATE_FACE_FILTER
 */

void WConfiguration::OnCheckboxShowLocateFaceFilterClick( wxCommandEvent& event )
{
	m_pViacamController->GetMotionTracker()->SetShowTrackFaceFilter (m_chkShowAutoLocateFaceFilter->GetValue());
	event.Skip(false);
	Changed ();
}



/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE_LANGUAGE
 */

void WConfiguration::OnChoiceLanguageSelected( wxCommandEvent& event )
{
	int index= m_choLanguage->GetCurrentSelection();
	if (index!= wxNOT_FOUND)
	{
        m_pViacamController->SetLanguage (s_langIds[index]);
		wxMessageDialog dlg (NULL, _("You should restart the application to apply this change"), 
			_T("Enable Viacam"), wxICON_INFORMATION );
		dlg.ShowModal ();
	}
	event.Skip(false);
	Changed ();
}



/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE_PROFILE
 */

void WConfiguration::OnChoiceProfileSelected( wxCommandEvent& event )
{
	bool fail= false;
	wxString msg;

	if (m_dirty)
	{
		wxMessageDialog dlg (NULL, _("Current profile has unsaved changes.\nContinue anyway?"), _T("Enable Viacam"), wxICON_EXCLAMATION | wxYES_NO );
		if (dlg.ShowModal()== wxID_NO) fail= true;			
	}

	if (!fail)
	{
		msg.sprintf (_("This action will load %s profile settings.\nAre you sure you want to continue?"), event.GetString().c_str());
		wxMessageDialog dlg (NULL, msg, _T("Enable Viacam"), wxICON_EXCLAMATION | wxYES_NO );
		if (dlg.ShowModal()== wxID_NO) fail= true;
	}

	if (fail)
		// Restore old selection
		m_choProfile->Select (m_pViacamController->GetConfigManager()->GetCurrentProfile());		
	else
	{
		// Change profile
		m_pViacamController->GetConfigManager()->ChangeCurrentProfile (event.GetSelection());

		// Refresh dialog values
		InitializeData ();		

		UnChanged ();
	}

	event.Skip(false);
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_DEL_PROFILE
 */

void WConfiguration::OnButtonDelProfileClick( wxCommandEvent& event )
{
	wxString msg;

	msg.sprintf (_("This action will delete %s profile.\nAre you sure you want to continue?"), m_choProfile->GetStringSelection().c_str());
	wxMessageDialog dlg (NULL, msg, _T("Enable Viacam"), wxICON_EXCLAMATION | wxYES_NO );
	if (dlg.ShowModal()== wxID_YES)
	{
		m_pViacamController->GetConfigManager()->DeleteCurrentProfile();
		InitializeData ();
		UnChanged ();
	}

    event.Skip(false);
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_ADD_PROFILE
 */

void WConfiguration::OnButtonAddProfileClick( wxCommandEvent& event )
{
	wxTextEntryDialog dlg (this, _("Enter new profile name"), _("Add profile"));
	if (dlg.ShowModal()== wxID_OK)
	{
		switch (m_pViacamController->GetConfigManager()->AddNewProfile (dlg.GetValue()))
		{
		case 0:
			InitializeData ();
			UnChanged ();
			break;
		case 1:
			{
				wxMessageDialog errDlg (NULL, _("Profile already exists."), _T("Enable Viacam"), wxICON_ERROR );
				errDlg.ShowModal ();
			}
			break;
		case 2:
			{
				wxMessageDialog errDlg (NULL, _("Invalid profile name."), _T("Enable Viacam"), wxICON_ERROR );
				errDlg.ShowModal ();
			}
			break;
		default:
			assert (false);
		}		
	}

    event.Skip(false);
}

/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE_BEHAVIOUR
 */

void WConfiguration::OnChoiceBehaviourSelected( wxCommandEvent& event )
{
	m_pViacamController->GetClickWindowController()->SetFastMode ( event.GetSelection()!= 0);
    event.Skip(false);
}

/*!
 * wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_COMBOBOX_DESIGN
 */

void WConfiguration::OnComboboxDesignSelected( wxCommandEvent& event )
{
	CClickWindowController::EDesign design= (CClickWindowController::EDesign) event.GetSelection();
	m_pViacamController->GetClickWindowController()->SetDesign (design);
	event.Skip(false);
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_ONSCREENKEYBOARDCOMMAND
 */

void WConfiguration::OnButtonOnscreenkeyboardcommandClick( wxCommandEvent& event )
{
#if defined(__WXMSW__)
	wxFileDialog dlg (this, _("Choose on-screen keyboard command"), _T(""), _T(""), _("Executable files|*.exe"), wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR);
#else
	wxFileDialog dlg (this, _("Choose on-screen keyboard command"), _T(""), _T(""), _T("*"), wxOPEN | wxFILE_MUST_EXIST | wxCHANGE_DIR);
#endif

	if (dlg.ShowModal()== wxID_OK) m_txtOnScreenKeyboardCommand->SetValue(dlg.GetPath());

	event.Skip(false);
}


/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL_ONSCREENKEYBOARDCOMMAND
 */

void WConfiguration::OnTextctrlOnscreenkeyboardcommandTextUpdated( wxCommandEvent& event )
{
	if (event.GetString().Cmp(m_pViacamController->GetOnScreenKeyboardCommand()))
	{
		m_pViacamController->SetOnScreenKeyboardCommand	(event.GetString());
		Changed ();
	}
	event.Skip(false);
	
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_CAMERA_SETTINGS
 */

void WConfiguration::OnButtonCameraSettingsClick( wxCommandEvent& event )
{
	m_pViacamController->ShowCameraSettingsDialog ();

	event.Skip (false);
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_CHANGE_CAMERA
 */

void WConfiguration::OnButtonChangeCameraClick( wxCommandEvent& event )
{
    m_pViacamController->ChangeCamera ();
	wxMessageDialog dlg (NULL, _("You should restart the application to apply this change"), 
		_T("Enable Viacam"), wxICON_INFORMATION );
	dlg.ShowModal ();

	event.Skip(false);
	Changed ();
}


/*!
 * wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL_TOP_WORKSPACE
 */

void WConfiguration::OnSpinctrlTopWorkspaceUpdated( wxSpinEvent& event )
{
    m_pViacamController->GetMouseOutput()->SetTopWorkspace(m_spin_top_workspace->GetValue());
	event.Skip(false);
	Changed ();
}




/*!
 * wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL_RIGHT_WORKSPACE
 */

void WConfiguration::OnSpinctrlRightWorkspaceUpdated( wxSpinEvent& event )
{
    m_pViacamController->GetMouseOutput()->SetRightWorkspace(m_spin_right_workspace->GetValue());
	event.Skip(false);
	Changed ();
}





/*!
 * wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL_LEFT_WORKSPACE
 */

void WConfiguration::OnSpinctrlLeftWorkspaceUpdated( wxSpinEvent& event )
{
    m_pViacamController->GetMouseOutput()->SetLeftWorkspace(m_spin_left_workspace->GetValue());
	event.Skip(false);
	Changed ();
}


/*!
 * wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL_BOTTOM_WORKSPACE
 */

void WConfiguration::OnSpinctrlBottomWorkspaceUpdated( wxSpinEvent& event )
{
    m_pViacamController->GetMouseOutput()->SetBottomWorkspace(m_spin_bottom_workspace->GetValue());
	event.Skip(false);
	Changed ();
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_WORKSPACE_LIMIT
 */

void WConfiguration::OnCheckboxWorkspaceLimitClick( wxCommandEvent& event )
{
    if(m_chkEnabledWorkspace->GetValue()) {
        m_chkEnabledWorkspace->SetValue(false);
        wxMessageDialog dlg (NULL, _("This action could make you lose the control of the computer.\nDo you want to continue?"), _("eViacam warning"), wxICON_EXCLAMATION | wxYES_NO );
        if (dlg.ShowModal()== wxID_YES)
        {
            m_chkEnabledWorkspace->SetValue(true);
            m_pViacamController->GetMouseOutput()->SetRestrictedWorkingArea(m_chkEnabledWorkspace->GetValue());
            m_spin_top_workspace->Enable(m_chkEnabledWorkspace->GetValue());
            m_spin_left_workspace->Enable(m_chkEnabledWorkspace->GetValue());
            m_spin_right_workspace->Enable(m_chkEnabledWorkspace->GetValue());
            m_spin_bottom_workspace->Enable(m_chkEnabledWorkspace->GetValue());
        }		
	} else {
        m_pViacamController->GetMouseOutput()->SetRestrictedWorkingArea(m_chkEnabledWorkspace->GetValue());
        m_spin_top_workspace->Enable(m_chkEnabledWorkspace->GetValue());
        m_spin_left_workspace->Enable(m_chkEnabledWorkspace->GetValue());
        m_spin_right_workspace->Enable(m_chkEnabledWorkspace->GetValue());
        m_spin_bottom_workspace->Enable(m_chkEnabledWorkspace->GetValue());
    }

    
	event.Skip(false);
	Changed ();
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_ACTIVATION_KEY
 */
#if defined(__WXGTK__)
void WConfiguration::OnButtonActivationKeyClick( wxCommandEvent& event )
{
	bool isEnabled = m_pViacamController->GetEnabled();
	Activationkey* pActivationKey = new Activationkey(this);

	if (pActivationKey->ShowModal()== wxID_YES)
	{
		m_txtActivationKey->SetValue(pActivationKey->GetKeyName());
		m_pViacamController->SetActivationKeyCode(pActivationKey->GetKeyCode());
		m_pViacamController->SetEnabled(isEnabled);
	}
	delete (pActivationKey);
		
	event.Skip(false);
    Changed ();
}
#endif

/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_ACTIVATION_KEY
 */
#if defined(__WXGTK__)
void WConfiguration::OnCheckboxActivationKeyClick( wxCommandEvent& event )
{
    m_pViacamController->SetEnabledActivationKey(m_chkActivationKey->GetValue());
    event.Skip(false);
    Changed ();
}
#endif

/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_STARTUP
 */
#if defined(__WXGTK__)
void WConfiguration::OnCheckboxStartupClick( wxCommandEvent& event )
{
	m_pAutostart->Enable(m_chkStartup->GetValue());
    event.Skip();
    Changed ();
}
#endif


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON
 */

void WConfiguration::OnButtonClick( wxCommandEvent& event )
{
	if (m_pViacamController->StartMotionCalibration()) {
		InitializeData();
		Changed ();	
	}	
	event.Skip();
	
}


/*!
 * wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_COMBOBOX_LEFT
 */

void WConfiguration::OnComboboxLeftSelected( wxCommandEvent& event )
{
	CMouseOutput::EAction action= (CMouseOutput::EAction) event.GetSelection();
	m_pViacamController->GetMouseOutput()->SetActionLeft (action);
	event.Skip(false);
}


/*!
 * wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_COMBOBOX_RIGHT
 */

void WConfiguration::OnComboboxRightSelected( wxCommandEvent& event )
{
	CMouseOutput::EAction action= (CMouseOutput::EAction) event.GetSelection();
	m_pViacamController->GetMouseOutput()->SetActionRight (action);
	event.Skip(false);
}


/*!
 * wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_COMBOBOX_TOP
 */

void WConfiguration::OnComboboxTopSelected( wxCommandEvent& event )
{
	CMouseOutput::EAction action= (CMouseOutput::EAction) event.GetSelection();
	m_pViacamController->GetMouseOutput()->SetActionTop (action);
	event.Skip(false);
}


/*!
 * wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_COMBOBOX_BOTTOM
 */

void WConfiguration::OnComboboxBottomSelected( wxCommandEvent& event )
{
	CMouseOutput::EAction action= (CMouseOutput::EAction) event.GetSelection();
	m_pViacamController->GetMouseOutput()->SetActionBottom (action);
	event.Skip(false);
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX
 */

void WConfiguration::OnCheckboxEnableGestureClick( wxCommandEvent& event )
{
	if (m_chkEnableGestureClick->IsChecked())
	{
		EnableGestureOptions(true);
		m_pViacamController->GetMouseOutput()->SetClickMode(CMouseOutput::GESTURE);	
	}
	else
	{
		EnableGestureOptions(false);
		m_pViacamController->GetMouseOutput()->SetClickMode(CMouseOutput::DWELL);		
	}
}

