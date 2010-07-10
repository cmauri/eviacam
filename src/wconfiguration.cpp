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

// Trick to properly compile & display native language names
#if defined(__WXMSW__)
#include "langnames-utf16.h"
#else
#include "langnames-utf8.h"
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
	wxLANGUAGE_FRENCH
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

    EVT_CHECKBOX( ID_CHECKBOX_WORKSPACE_LIMIT, WConfiguration::OnCheckboxWorkspaceLimitClick )

    EVT_SPINCTRL( ID_SPINCTRL_TOP_WORKSPACE, WConfiguration::OnSpinctrlTopWorkspaceUpdated )

    EVT_SPINCTRL( ID_SPINCTRL_LEFT_WORKSPACE, WConfiguration::OnSpinctrlLeftWorkspaceUpdated )

    EVT_SPINCTRL( ID_SPINCTRL_RIGHT_WORKSPACE, WConfiguration::OnSpinctrlRightWorkspaceUpdated )

    EVT_SPINCTRL( ID_SPINCTRL_BOTTOM_WORKSPACE, WConfiguration::OnSpinctrlBottomWorkspaceUpdated )

    EVT_SPINCTRL( ID_SPINCTRL_DWELL_TIME, WConfiguration::OnSpinctrlDwellTimeUpdated )

    EVT_SPINCTRL( ID_SPINCTRL_DWELL_AREA, WConfiguration::OnSpinctrlDwellAreaUpdated )

    EVT_CHECKBOX( ID_CHECKBOX_ALLOW_CONSECUTIVE, WConfiguration::OnCheckboxAllowConsecutiveClick )

    EVT_CHECKBOX( ID_CHECKBOX_BEEP_ON_CLICK, WConfiguration::OnCheckboxBeepOnClickClick )

    EVT_CHECKBOX( ID_CHECKBOX_SHOW_CLICK_COUNTDOWN, WConfiguration::OnCheckboxShowClickCountdownClick )

    EVT_CHECKBOX( ID_CHECKBOX_ENABLE_DWELL, WConfiguration::OnCheckboxEnableDwellClick )

    EVT_CHECKBOX( ID_CHECKBOX_CLICKWIN_AT_STARTUP, WConfiguration::OnCheckboxClickwinAtStartupClick )

    EVT_CHECKBOX( ID_CHECKBOX_SHOW_CLICKWIN, WConfiguration::OnCheckboxShowClickwinClick )

    EVT_COMBOBOX( ID_COMBOBOX_DESIGN, WConfiguration::OnComboboxDesignSelected )

    EVT_CHOICE( ID_CHOICE_BEHAVIOUR, WConfiguration::OnChoiceBehaviourSelected )

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
	Init();
    Create(parent, id, caption, pos, size, style);
}

WConfiguration::WConfiguration( wxWindow* parent, CViacamController* pViacamController, CAutostart* pAutostart, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
	m_pViacamController= pViacamController;
    m_pAutostart= pAutostart;
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
    m_chkEnabledWorkspace = NULL;
    m_spin_top_workspace = NULL;
    m_spin_left_workspace = NULL;
    m_spin_right_workspace = NULL;
    m_spin_bottom_workspace = NULL;
    m_spinDwellTime = NULL;
    m_spinDwellArea = NULL;
    m_chkAllowConsecutiveClick = NULL;
    m_chkBeepOnClick = NULL;
    m_chkShowClickCountdown = NULL;
    m_chkDwellClickEnabled = NULL;
    m_chkOpenClickWinAtStartup = NULL;
    m_chkShowClickWin = NULL;
    m_cmbClickWindowDesign = NULL;
    m_choClickWindowBehaviour = NULL;
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

    itemNotebook4->AddPage(itemPanel5, _("Motion"));

    wxPanel* itemPanel19 = new wxPanel( itemNotebook4, ID_PANEL_WORKSPACE, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer20 = new wxBoxSizer(wxVERTICAL);
    itemPanel19->SetSizer(itemBoxSizer20);

    m_chkEnabledWorkspace = new wxCheckBox( itemPanel19, ID_CHECKBOX_WORKSPACE_LIMIT, _("Workspace limit"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkEnabledWorkspace->SetValue(false);
    if (WConfiguration::ShowToolTips())
        m_chkEnabledWorkspace->SetToolTip(_("If checked the mouse workspace will be limited."));
    itemBoxSizer20->Add(m_chkEnabledWorkspace, 0, wxALIGN_LEFT|wxALL, 5);

    wxBoxSizer* itemBoxSizer22 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer20->Add(itemBoxSizer22, 0, wxGROW|wxALL, 5);
    wxGridSizer* itemGridSizer23 = new wxGridSizer(0, 3, 0, 0);
    itemBoxSizer22->Add(itemGridSizer23, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticText* itemStaticText24 = new wxStaticText( itemPanel19, wxID_STATIC, _("Top"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridSizer23->Add(itemStaticText24, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_spin_top_workspace = new wxSpinCtrl( itemPanel19, ID_SPINCTRL_TOP_WORKSPACE, _T("1"), wxDefaultPosition, wxSize(45, 25), wxSP_ARROW_KEYS, 0, 50, 1 );
    if (WConfiguration::ShowToolTips())
        m_spin_top_workspace->SetToolTip(_("Top limit workspace."));
    m_spin_top_workspace->Enable(false);
    itemGridSizer23->Add(m_spin_top_workspace, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText26 = new wxStaticText( itemPanel19, wxID_STATIC, _("%"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridSizer23->Add(itemStaticText26, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxGridSizer* itemGridSizer27 = new wxGridSizer(0, 2, 0, 0);
    itemBoxSizer22->Add(itemGridSizer27, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxGridSizer* itemGridSizer28 = new wxGridSizer(0, 3, 0, 0);
    itemGridSizer27->Add(itemGridSizer28, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxStaticText* itemStaticText29 = new wxStaticText( itemPanel19, wxID_STATIC, _("Left"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridSizer28->Add(itemStaticText29, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_spin_left_workspace = new wxSpinCtrl( itemPanel19, ID_SPINCTRL_LEFT_WORKSPACE, _T("1"), wxDefaultPosition, wxSize(45, 25), wxSP_ARROW_KEYS, 0, 50, 1 );
    if (WConfiguration::ShowToolTips())
        m_spin_left_workspace->SetToolTip(_("Left limit workspace."));
    m_spin_left_workspace->Enable(false);
    itemGridSizer28->Add(m_spin_left_workspace, 0, wxALIGN_CENTER_HORIZONTAL|wxGROW|wxALL, 5);

    wxStaticText* itemStaticText31 = new wxStaticText( itemPanel19, wxID_STATIC, _("%"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridSizer28->Add(itemStaticText31, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxGridSizer* itemGridSizer32 = new wxGridSizer(0, 3, 0, 0);
    itemGridSizer27->Add(itemGridSizer32, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxStaticText* itemStaticText33 = new wxStaticText( itemPanel19, wxID_STATIC, _("Right"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridSizer32->Add(itemStaticText33, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_spin_right_workspace = new wxSpinCtrl( itemPanel19, ID_SPINCTRL_RIGHT_WORKSPACE, _T("1"), wxDefaultPosition, wxSize(45, 25), wxSP_ARROW_KEYS, 0, 50, 1 );
    if (WConfiguration::ShowToolTips())
        m_spin_right_workspace->SetToolTip(_("Right limit workspace."));
    m_spin_right_workspace->Enable(false);
    itemGridSizer32->Add(m_spin_right_workspace, 0, wxALIGN_CENTER_HORIZONTAL|wxGROW|wxALL, 5);

    wxStaticText* itemStaticText35 = new wxStaticText( itemPanel19, wxID_STATIC, _("%"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridSizer32->Add(itemStaticText35, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxGridSizer* itemGridSizer36 = new wxGridSizer(0, 3, 0, 0);
    itemBoxSizer22->Add(itemGridSizer36, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticText* itemStaticText37 = new wxStaticText( itemPanel19, wxID_STATIC, _("Bottom"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridSizer36->Add(itemStaticText37, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_spin_bottom_workspace = new wxSpinCtrl( itemPanel19, ID_SPINCTRL_BOTTOM_WORKSPACE, _T("1"), wxDefaultPosition, wxSize(45, 25), wxSP_ARROW_KEYS, 0, 50, 1 );
    if (WConfiguration::ShowToolTips())
        m_spin_bottom_workspace->SetToolTip(_("Bottom limit workspace."));
    m_spin_bottom_workspace->Enable(false);
    itemGridSizer36->Add(m_spin_bottom_workspace, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText39 = new wxStaticText( itemPanel19, wxID_STATIC, _("%"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridSizer36->Add(itemStaticText39, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemNotebook4->AddPage(itemPanel19, _("Workspace"));

    wxPanel* itemPanel40 = new wxPanel( itemNotebook4, ID_PANEL_CLICK, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer41 = new wxBoxSizer(wxVERTICAL);
    itemPanel40->SetSizer(itemBoxSizer41);

    wxStaticBox* itemStaticBoxSizer42Static = new wxStaticBox(itemPanel40, wxID_ANY, _("Dwell click"));
    wxStaticBoxSizer* itemStaticBoxSizer42 = new wxStaticBoxSizer(itemStaticBoxSizer42Static, wxVERTICAL);
    itemBoxSizer41->Add(itemStaticBoxSizer42, 0, wxGROW|wxALL, 0);
    wxFlexGridSizer* itemFlexGridSizer43 = new wxFlexGridSizer(0, 2, 0, 0);
    itemStaticBoxSizer42->Add(itemFlexGridSizer43, 0, wxALIGN_LEFT|wxALL, 0);
    wxStaticText* itemStaticText44 = new wxStaticText( itemPanel40, wxID_STATIC, _("Dwell time (ds)"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer43->Add(itemStaticText44, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_spinDwellTime = new wxSpinCtrl( itemPanel40, ID_SPINCTRL_DWELL_TIME, _T("2"), wxDefaultPosition, wxSize(45, 25), wxSP_ARROW_KEYS, 2, 50, 2 );
    if (WConfiguration::ShowToolTips())
        m_spinDwellTime->SetToolTip(_("Time to wait (deciseconds) \nbefore sending a click."));
    itemFlexGridSizer43->Add(m_spinDwellTime, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText46 = new wxStaticText( itemPanel40, wxID_STATIC, _("Dwell area"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer43->Add(itemStaticText46, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_spinDwellArea = new wxSpinCtrl( itemPanel40, ID_SPINCTRL_DWELL_AREA, _T("0"), wxDefaultPosition, wxSize(45, 25), wxSP_ARROW_KEYS, 0, 5, 0 );
    if (WConfiguration::ShowToolTips())
        m_spinDwellArea->SetToolTip(_("Maximum allowed displacement\nbefore restarting the dwell time\ncountdown."));
    itemFlexGridSizer43->Add(m_spinDwellArea, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_chkAllowConsecutiveClick = new wxCheckBox( itemPanel40, ID_CHECKBOX_ALLOW_CONSECUTIVE, _("Allow consecutive clicks"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkAllowConsecutiveClick->SetValue(false);
    if (WConfiguration::ShowToolTips())
        m_chkAllowConsecutiveClick->SetToolTip(_("Allows to send multiple clicks\nwhen the pointer is stopped."));
    itemStaticBoxSizer42->Add(m_chkAllowConsecutiveClick, 0, wxALIGN_LEFT|wxALL, 5);

    m_chkBeepOnClick = new wxCheckBox( itemPanel40, ID_CHECKBOX_BEEP_ON_CLICK, _("Beep on click"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkBeepOnClick->SetValue(false);
    if (WConfiguration::ShowToolTips())
        m_chkBeepOnClick->SetToolTip(_("Play sound when click generated."));
    itemStaticBoxSizer42->Add(m_chkBeepOnClick, 0, wxALIGN_LEFT|wxALL, 5);

    m_chkShowClickCountdown = new wxCheckBox( itemPanel40, ID_CHECKBOX_SHOW_CLICK_COUNTDOWN, _("Show click countdown"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkShowClickCountdown->SetValue(false);
    m_chkShowClickCountdown->Enable(false);
    itemStaticBoxSizer42->Add(m_chkShowClickCountdown, 0, wxALIGN_LEFT|wxALL, 5);

    m_chkDwellClickEnabled = new wxCheckBox( itemPanel40, ID_CHECKBOX_ENABLE_DWELL, _("Dwell click enabled"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkDwellClickEnabled->SetValue(false);
    if (WConfiguration::ShowToolTips())
        m_chkDwellClickEnabled->SetToolTip(_("Enable/Disable automatic (dwell)\nclick generation mechanism."));
    itemStaticBoxSizer42->Add(m_chkDwellClickEnabled, 0, wxALIGN_LEFT|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer52Static = new wxStaticBox(itemPanel40, wxID_ANY, _("Click window"));
    wxStaticBoxSizer* itemStaticBoxSizer52 = new wxStaticBoxSizer(itemStaticBoxSizer52Static, wxVERTICAL);
    itemBoxSizer41->Add(itemStaticBoxSizer52, 0, wxGROW|wxALL, 0);
    m_chkOpenClickWinAtStartup = new wxCheckBox( itemPanel40, ID_CHECKBOX_CLICKWIN_AT_STARTUP, _("Open Click Window at startup"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkOpenClickWinAtStartup->SetValue(false);
    if (WConfiguration::ShowToolTips())
        m_chkOpenClickWinAtStartup->SetToolTip(_("If checked the Click Window is automatically\nopened at program startup."));
    itemStaticBoxSizer52->Add(m_chkOpenClickWinAtStartup, 0, wxALIGN_LEFT|wxALL, 5);

    m_chkShowClickWin = new wxCheckBox( itemPanel40, ID_CHECKBOX_SHOW_CLICKWIN, _("Show Click Window"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkShowClickWin->SetValue(false);
    itemStaticBoxSizer52->Add(m_chkShowClickWin, 0, wxALIGN_LEFT|wxALL, 5);

    wxFlexGridSizer* itemFlexGridSizer55 = new wxFlexGridSizer(2, 2, 0, 0);
    itemStaticBoxSizer52->Add(itemFlexGridSizer55, 0, wxALIGN_LEFT|wxALL, 0);
    wxStaticText* itemStaticText56 = new wxStaticText( itemPanel40, wxID_STATIC, _("Design:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer55->Add(itemStaticText56, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString m_cmbClickWindowDesignStrings;
    m_cmbClickWindowDesignStrings.Add(_("Normal"));
    m_cmbClickWindowDesignStrings.Add(_("Thin"));
    m_cmbClickWindowDesign = new wxComboBox( itemPanel40, ID_COMBOBOX_DESIGN, wxEmptyString, wxDefaultPosition, wxDefaultSize, m_cmbClickWindowDesignStrings, wxCB_READONLY );
    itemFlexGridSizer55->Add(m_cmbClickWindowDesign, 0, wxALIGN_LEFT|wxGROW|wxALL, 5);

    wxStaticText* itemStaticText58 = new wxStaticText( itemPanel40, wxID_STATIC, _("Behaviour:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer55->Add(itemStaticText58, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString m_choClickWindowBehaviourStrings;
    m_choClickWindowBehaviourStrings.Add(_("Normal mode"));
    m_choClickWindowBehaviourStrings.Add(_("Fast mode"));
    m_choClickWindowBehaviour = new wxChoice( itemPanel40, ID_CHOICE_BEHAVIOUR, wxDefaultPosition, wxDefaultSize, m_choClickWindowBehaviourStrings, 0 );
    if (WConfiguration::ShowToolTips())
        m_choClickWindowBehaviour->SetToolTip(_("Fast mode enables click type\nselection by hovering the mouse\npointer over the click window\nbuttons."));
    itemFlexGridSizer55->Add(m_choClickWindowBehaviour, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemNotebook4->AddPage(itemPanel40, _("Click"));

    wxPanel* itemPanel60 = new wxPanel( itemNotebook4, ID_PANEL_ADVANCED, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer61 = new wxBoxSizer(wxVERTICAL);
    itemPanel60->SetSizer(itemBoxSizer61);

    wxStaticBox* itemStaticBoxSizer62Static = new wxStaticBox(itemPanel60, wxID_ANY, _("Face localization"));
    wxStaticBoxSizer* itemStaticBoxSizer62 = new wxStaticBoxSizer(itemStaticBoxSizer62Static, wxVERTICAL);
    itemBoxSizer61->Add(itemStaticBoxSizer62, 0, wxGROW|wxALL, 5);
    m_chkAutoLocateFace = new wxCheckBox( itemPanel60, ID_CHECKBOX_AUTO_LOCATE_FACE, _("Locate face automatically"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkAutoLocateFace->SetValue(false);
    if (WConfiguration::ShowToolTips())
        m_chkAutoLocateFace->SetToolTip(_("When enabled tries to automatically\ndetect your face and center the\ntracking area."));
    itemStaticBoxSizer62->Add(m_chkAutoLocateFace, 0, wxALIGN_LEFT|wxALL, 5);

    m_chkShowAutoLocateFaceFilter = new wxCheckBox( itemPanel60, ID_CHECKBOX_SHOW_LOCATE_FACE_FILTER, _("Show locate face filter"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkShowAutoLocateFaceFilter->SetValue(false);
    m_chkShowAutoLocateFaceFilter->Enable(false);
    itemStaticBoxSizer62->Add(m_chkShowAutoLocateFaceFilter, 0, wxALIGN_LEFT|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer65Static = new wxStaticBox(itemPanel60, wxID_ANY, _("On-screen keyboard command"));
    wxStaticBoxSizer* itemStaticBoxSizer65 = new wxStaticBoxSizer(itemStaticBoxSizer65Static, wxVERTICAL);
    itemBoxSizer61->Add(itemStaticBoxSizer65, 0, wxGROW|wxALL, 5);
    m_txtOnScreenKeyboardCommand = new wxTextCtrl( itemPanel60, ID_TEXTCTRL_ONSCREENKEYBOARDCOMMAND, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    if (WConfiguration::ShowToolTips())
        m_txtOnScreenKeyboardCommand->SetToolTip(_("Command to run an external on-screen keyboard application."));
    itemStaticBoxSizer65->Add(m_txtOnScreenKeyboardCommand, 0, wxGROW|wxALL, 5);

    m_btntOnScreenKeyboardCommand = new wxButton( itemPanel60, ID_BUTTON_ONSCREENKEYBOARDCOMMAND, _("Browse..."), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer65->Add(m_btntOnScreenKeyboardCommand, 0, wxALIGN_LEFT|wxALL, 5);

#if defined(__WXGTK__)
    wxStaticBox* itemStaticBoxSizer68Static = new wxStaticBox(itemPanel60, wxID_ANY, _("Activation key"));
    wxStaticBoxSizer* itemStaticBoxSizer68 = new wxStaticBoxSizer(itemStaticBoxSizer68Static, wxVERTICAL);
    itemBoxSizer61->Add(itemStaticBoxSizer68, 0, wxGROW|wxALL, 5);
#if defined(__WXGTK__)
    m_chkActivationKey = new wxCheckBox( itemPanel60, ID_CHECKBOX_ACTIVATION_KEY, _("Enabled activation key"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkActivationKey->SetValue(false);
    if (WConfiguration::ShowToolTips())
        m_chkActivationKey->SetToolTip(_("When enabled allows activate or deactivate the facial mouse pressing a key."));
    itemStaticBoxSizer68->Add(m_chkActivationKey, 0, wxALIGN_LEFT|wxALL, 5);
#endif

#if defined(__WXGTK__)
    m_txtActivationKey = new wxTextCtrl( itemPanel60, ID_TEXTCTRL_ACTIVATION_KEY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    if (WConfiguration::ShowToolTips())
        m_txtActivationKey->SetToolTip(_("Shows the selected key."));
    m_txtActivationKey->Enable(false);
    itemStaticBoxSizer68->Add(m_txtActivationKey, 0, wxGROW|wxALL, 5);
#endif

#if defined(__WXGTK__)
    m_buttonActivationKey = new wxButton( itemPanel60, ID_BUTTON_ACTIVATION_KEY, _("Set key"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer68->Add(m_buttonActivationKey, 0, wxALIGN_RIGHT|wxALL, 5);
#endif

#endif

    itemNotebook4->AddPage(itemPanel60, _("Advanced"));

    itemListbook3->AddPage(itemNotebook4, _("Profile options"), false, 0);

    wxPanel* itemPanel72 = new wxPanel( itemListbook3, ID_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer73 = new wxBoxSizer(wxVERTICAL);
    itemPanel72->SetSizer(itemBoxSizer73);

#if defined(__WXGTK__)
    wxStaticBox* itemStaticBoxSizer74Static = new wxStaticBox(itemPanel72, wxID_ANY, _("Run on startup"));
    wxStaticBoxSizer* itemStaticBoxSizer74 = new wxStaticBoxSizer(itemStaticBoxSizer74Static, wxHORIZONTAL);
    itemBoxSizer73->Add(itemStaticBoxSizer74, 0, wxGROW|wxALL, 5);
    m_chkStartup = new wxCheckBox( itemPanel72, ID_CHECKBOX_STARTUP, _("Enabled run on startup"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkStartup->SetValue(false);
    if (WConfiguration::ShowToolTips())
        m_chkStartup->SetToolTip(_("If checked the program will run on startup."));
    itemStaticBoxSizer74->Add(m_chkStartup, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

#endif

    wxStaticBox* itemStaticBoxSizer76Static = new wxStaticBox(itemPanel72, wxID_ANY, _("Profile management"));
    wxStaticBoxSizer* itemStaticBoxSizer76 = new wxStaticBoxSizer(itemStaticBoxSizer76Static, wxVERTICAL);
    itemBoxSizer73->Add(itemStaticBoxSizer76, 0, wxGROW|wxALL, 5);
    wxBoxSizer* itemBoxSizer77 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer76->Add(itemBoxSizer77, 0, wxGROW|wxALL, 0);
    wxStaticText* itemStaticText78 = new wxStaticText( itemPanel72, wxID_STATIC, _("Select profile:"), wxDefaultPosition, wxDefaultSize, 0 );
    if (WConfiguration::ShowToolTips())
        itemStaticText78->SetToolTip(_("Chooses desired active profile"));
    itemBoxSizer77->Add(itemStaticText78, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString m_choProfileStrings;
    m_choProfile = new wxChoice( itemPanel72, ID_CHOICE_PROFILE, wxDefaultPosition, wxDefaultSize, m_choProfileStrings, 0 );
    if (WConfiguration::ShowToolTips())
        m_choProfile->SetToolTip(_("Chooses desired active profile"));
    itemBoxSizer77->Add(m_choProfile, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer80 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer76->Add(itemBoxSizer80, 0, wxALIGN_RIGHT|wxALL, 0);
    m_btnAddProfile = new wxButton( itemPanel72, ID_BUTTON_ADD_PROFILE, _("Add profile"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer80->Add(m_btnAddProfile, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_btnDeleteProfile = new wxButton( itemPanel72, ID_BUTTON_DEL_PROFILE, _("Delete profile"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer80->Add(m_btnDeleteProfile, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer83Static = new wxStaticBox(itemPanel72, wxID_ANY, _("Selected camera"));
    wxStaticBoxSizer* itemStaticBoxSizer83 = new wxStaticBoxSizer(itemStaticBoxSizer83Static, wxVERTICAL);
    itemBoxSizer73->Add(itemStaticBoxSizer83, 0, wxGROW|wxALL, 5);
    wxBoxSizer* itemBoxSizer84 = new wxBoxSizer(wxVERTICAL);
    itemStaticBoxSizer83->Add(itemBoxSizer84, 0, wxGROW|wxALL, 0);
    m_txtSelectedCamera = new wxTextCtrl( itemPanel72, ID_TEXTCTRL_SELECTED_CAMERA, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
    m_txtSelectedCamera->Enable(false);
    itemBoxSizer84->Add(m_txtSelectedCamera, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer86 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer83->Add(itemBoxSizer86, 0, wxALIGN_RIGHT|wxALL, 0);
    m_btnCameraSettings = new wxButton( itemPanel72, ID_BUTTON_CAMERA_SETTINGS, _("Settings..."), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer86->Add(m_btnCameraSettings, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton88 = new wxButton( itemPanel72, ID_BUTTON_CHANGE_CAMERA, _("Change"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer86->Add(itemButton88, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxFlexGridSizer* itemFlexGridSizer89 = new wxFlexGridSizer(0, 2, 0, 0);
    itemBoxSizer73->Add(itemFlexGridSizer89, 0, wxALIGN_LEFT|wxALL, 5);
    wxStaticText* itemStaticText90 = new wxStaticText( itemPanel72, wxID_STATIC, _("Language:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer89->Add(itemStaticText90, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString m_choLanguageStrings;
    m_choLanguage = new wxChoice( itemPanel72, ID_CHOICE_LANGUAGE, wxDefaultPosition, wxDefaultSize, m_choLanguageStrings, 0 );
    itemFlexGridSizer89->Add(m_choLanguage, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxCheckBox* itemCheckBox92 = new wxCheckBox( itemPanel72, ID_CHECKBOX_RUN_STARTUP, _("Run at system startup"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox92->SetValue(false);
    itemCheckBox92->Show(false);
    itemFlexGridSizer89->Add(itemCheckBox92, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemListbook3->AddPage(itemPanel72, _("General options"), false, 1);

    itemBoxSizer2->Add(itemListbook3, 0, wxALIGN_LEFT|wxALL, 5);

    wxBoxSizer* itemBoxSizer93 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer93, 0, wxALIGN_RIGHT|wxALL, 5);

    wxButton* itemButton94 = new wxButton( itemDialog1, ID_BUTTON_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer93->Add(itemButton94, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_btnCancel = new wxButton( itemDialog1, ID_BUTTON_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    m_btnCancel->Enable(false);
    itemBoxSizer93->Add(m_btnCancel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end WConfiguration content construction
}

void WConfiguration::SetActivationKey(wxString keyName)
{
#if defined(__WXGTK__)   
    m_txtActivationKey->SetValue(m_pViacamController->GetActivationKeyName());
    //m_txtActivationKey->SetValue(keyName);
#endif    
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
	m_chkDwellClickEnabled->SetValue (m_pViacamController->GetMouseOutput()->GetClickMode()== CMouseOutput::DWELL);
	m_chkAllowConsecutiveClick->SetValue (m_pViacamController->GetMouseOutput()->GetConsecutiveClicksAllowed());
	m_chkBeepOnClick->SetValue (m_pViacamController->GetMouseOutput()->GetBeepOnClick());
	//m_chkShowClickCountdown->SetValue (m_pViacamController->GetMouseOutput()->GetShowClickCountdown());
	m_spinDwellTime->SetValue (m_pViacamController->GetMouseOutput()->GetDwellTime());
	m_spinDwellArea->SetValue ( m_pViacamController->GetMouseOutput()->GetDwellToleranceArea () );
	m_chkShowClickWin->SetValue ( m_pViacamController->GetClickWindowController()->IsShown() );
	m_choClickWindowBehaviour->Select (m_pViacamController->GetClickWindowController()->GetFastMode() ? 1 : 0);
	m_cmbClickWindowDesign->Select (m_pViacamController->GetClickWindowController()->GetDesign());
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
			Changed ();
		}
	}
	else
	{
		m_pViacamController->GetMouseOutput()->SetClickMode(CMouseOutput::DWELL);
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
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_SHOW_CLICK_COUNTDOWN
 */

void WConfiguration::OnCheckboxShowClickCountdownClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_SHOW_CLICK_COUNTDOWN in WConfiguration.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_SHOW_CLICK_COUNTDOWN in WConfiguration. 
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
        wxMessageDialog dlg (NULL, _("This action could make you lose control of the program. Are you sure?"), _("eViacam warning"), wxICON_EXCLAMATION | wxYES_NO );
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
    m_pViacamController->OpenActivationKey();
    m_txtActivationKey->SetValue(m_pViacamController->GetActivationKeyName());
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
