/////////////////////////////////////////////////////////////////////////////
// Name:        wconfiguration.cpp
// Purpose:     
// Author:      César Mauri Loba
// Modified by: 
// Created:     29/12/2008 16:35:20
// RCS-ID:      
// Copyright:   (C) 2008-14 Cesar Mauri from CREA Sistemes Informatics
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

#include "wconfiguration.h"

////@begin includes
#include "wx/imaglist.h"
////@end includes
#include <wx/sizer.h>
#include <wx/msgdlg.h>
#include <wx/textdlg.h>
#include <wx/filedlg.h>
#include <wx/panel.h>

#include "eviacamapp.h"
#include "viacamcontroller.h"
#include "visionpipeline.h"
#include "clickwindowcontroller.h"
#include "pointeraction.h"

#include "cautostart.h"
#include "configmanager.h"

#include "wgetkey.h"
#include "dwellclick.h"
#include "gestureclick.h"

////@begin XPM images
#include "resources/eviacam_mini.xpm"
#include "resources/user.xpm"
#include "resources/eviacam_small.xpm"
////@end XPM images
#define FIRST_CONTROL_ID 12000

#include "langnames-utf8.h"

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
    EVT_BUTTON( ID_BUTTON, WConfiguration::OnButtonClick )
    EVT_CHECKBOX( ID_CHECKBOX_WORKSPACE_LIMIT, WConfiguration::OnCheckboxWorkspaceLimitClick )
    EVT_SPINCTRL( ID_SPINCTRL_TOP_WORKSPACE, WConfiguration::OnSpinctrlTopWorkspaceUpdated )
    EVT_SPINCTRL( ID_SPINCTRL_LEFT_WORKSPACE, WConfiguration::OnSpinctrlLeftWorkspaceUpdated )
    EVT_SPINCTRL( ID_SPINCTRL_RIGHT_WORKSPACE, WConfiguration::OnSpinctrlRightWorkspaceUpdated )
    EVT_SPINCTRL( ID_SPINCTRL_BOTTOM_WORKSPACE, WConfiguration::OnSpinctrlBottomWorkspaceUpdated )
    EVT_CHECKBOX( ID_CHECKBOX2, WConfiguration::OnCheckboxWrapPointer )
    EVT_CHECKBOX( ID_CHECKBOX_ENABLE_DWELL, WConfiguration::OnCheckboxEnableDwellClick )
    EVT_SPINCTRL( ID_SPINCTRL_DWELL_TIME, WConfiguration::OnSpinctrlDwellTimeUpdated )
    EVT_SPINCTRL( ID_SPINCTRL_DWELL_AREA, WConfiguration::OnSpinctrlDwellAreaUpdated )
    EVT_CHECKBOX( ID_CHECKBOX_ALLOW_CONSECUTIVE, WConfiguration::OnCheckboxAllowConsecutiveClick )
    EVT_CHECKBOX( ID_CHECKBOX_BEEP_ON_CLICK, WConfiguration::OnCheckboxBeepOnClickClick )
#if defined(__WXGTK__)
    EVT_CHECKBOX( ID_CHECKBOX_ALLOW_VISUAL_ALERTS, WConfiguration::OnCheckboxAllowVisualAlertsClick )
#endif

    EVT_CHECKBOX( ID_CHECKBOX_SHOW_CLICKWIN, WConfiguration::OnCheckboxShowClickwinClick )
    EVT_CHOICE( ID_CHOICE_DESIGN, WConfiguration::OnChoiceDesignSelected )
    EVT_CHOICE( ID_CHOICE_BEHAVIOUR, WConfiguration::OnChoiceBehaviourSelected )
#if defined(__WXGTK__)
    EVT_CHOICE( ID_CHOICE5, WConfiguration::OnChoiceClickWindowModeSelected )
#endif

#if defined(__WXGTK__)
    EVT_CHECKBOX( ID_CHECKBOX_AUTOHIDE, WConfiguration::OnCheckboxAutohideClick )
#endif

#if defined(__WXGTK__)
    EVT_CHECKBOX( ID_CHECKBOX, WConfiguration::OnCheckboxEnableGestureClick )
#endif

#if defined(__WXGTK__)
    EVT_CHOICE( ID_CHOICE, WConfiguration::OnLeftGestureChoiceSelected )
#endif

#if defined(__WXGTK__)
    EVT_CHOICE( ID_CHOICE1, WConfiguration::OnRightGestureChoiceSelected )
#endif

#if defined(__WXGTK__)
    EVT_CHOICE( ID_CHOICE2, WConfiguration::OnUpGestureChoiceSelected )
#endif

#if defined(__WXGTK__)
    EVT_CHOICE( ID_CHOICE3, WConfiguration::OnDownGestureChoiceSelected )
#endif

    EVT_CHECKBOX( ID_CHECKBOX_STARTUP, WConfiguration::OnCheckboxStartupClick )
    EVT_CHECKBOX( ID_CHECKBOX_ENABLE_AT_STARTUP, WConfiguration::OnCheckboxEnableAtStartupClick )
    EVT_TEXT( ID_TEXTCTRL_ONSCREENKEYBOARDCOMMAND, WConfiguration::OnTextctrlOnscreenkeyboardcommandTextUpdated )
    EVT_BUTTON( ID_BUTTON_ONSCREENKEYBOARDCOMMAND, WConfiguration::OnButtonOnscreenkeyboardcommandClick )
    EVT_CHECKBOX( ID_CHECKBOX_AUTO_LOCATE_FACE, WConfiguration::OnCheckboxAutoLocateFaceClick )
    EVT_CHOICE( ID_CHOICE4, WConfiguration::OnChoCpuUsageSelected )
    EVT_CHECKBOX( ID_CHECKBOX_ENABLE_WHEN_FACE_DETECTED, WConfiguration::OnCheckboxEnableWhenFaceDetectedClick )
    EVT_SPINCTRL( ID_SPINCTRL2, WConfiguration::OnSpinLocateFaceTimeoutUpdated )
    EVT_SPINCTRL( ID_SPIN_SEND_ACTION_WAIT, WConfiguration::OnSpinSendActionWaitUpdated )
    EVT_CHOICE( ID_CHOICE_PROFILE, WConfiguration::OnChoiceProfileSelected )
    EVT_BUTTON( ID_BUTTON_ADD_PROFILE, WConfiguration::OnButtonAddProfileClick )
    EVT_BUTTON( ID_BUTTON_DEL_PROFILE, WConfiguration::OnButtonDelProfileClick )
    EVT_BUTTON( ID_BUTTON_CAMERA_SETTINGS, WConfiguration::OnButtonCameraSettingsClick )
    EVT_BUTTON( ID_BUTTON_CHANGE_CAMERA, WConfiguration::OnButtonChangeCameraClick )
    EVT_CHOICE( ID_CHOICE_LANGUAGE, WConfiguration::OnChoiceLanguageSelected )
    EVT_CHECKBOX( ID_CHECKBOX_CHECK_UPDATES, WConfiguration::OnCheckboxCheckUpdatesClick )
    EVT_BUTTON( wxID_OK, WConfiguration::OnOkClick )
    EVT_BUTTON( wxID_CANCEL, WConfiguration::OnCancelClick )
////@end WConfiguration event table entries
    EVT_COMBOBOX( ID_COMBOBOX_LEFT, WConfiguration::OnComboboxLeftSelected )

    EVT_COMBOBOX( ID_COMBOBOX_RIGHT, WConfiguration::OnComboboxRightSelected )

    EVT_COMBOBOX( ID_COMBOBOX_TOP, WConfiguration::OnComboboxTopSelected )

    EVT_COMBOBOX( ID_COMBOBOX_BOTTOM, WConfiguration::OnComboboxBottomSelected )


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

/*!
 * WConfiguration creator
 */

bool WConfiguration::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin WConfiguration creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    SetIcon(GetIconResource(wxT("resources/eviacam_mini.xpm")));
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
    m_buttonCalibrateMotion = NULL;
    m_chkEnabledWorkspace = NULL;
    m_spin_top_workspace = NULL;
    m_spin_left_workspace = NULL;
    m_spin_right_workspace = NULL;
    m_spin_bottom_workspace = NULL;
    m_chkWrapPointer = NULL;
    m_panelClick = NULL;
    m_chkDwellClickEnabled = NULL;
    m_stDwellTime = NULL;
    m_spinDwellTime = NULL;
    m_stDwellArea = NULL;
    m_spinDwellArea = NULL;
    m_chkAllowConsecutiveClick = NULL;
    m_chkBeepOnClick = NULL;
#if defined(__WXGTK__)
    m_chkAllowVisualAlerts = NULL;
#endif
    m_sboxClickWin = NULL;
    m_chkShowClickWin = NULL;
    m_stDesign = NULL;
    m_choClickWindowDesign = NULL;
    m_stBehaviour = NULL;
    m_choClickWindowBehaviour = NULL;
#if defined(__WXGTK__)
    m_stDocking = NULL;
#endif
#if defined(__WXGTK__)
    m_choDockingMode = NULL;
#endif
#if defined(__WXGTK__)
    m_chkAutohide = NULL;
#endif
#if defined(__WXGTK__)
    m_sboxGestureClick = NULL;
#endif
#if defined(__WXGTK__)
    m_chkEnableGestureClick = NULL;
#endif
#if defined(__WXGTK__)
    m_stMoveLeft = NULL;
#endif
#if defined(__WXGTK__)
    m_choLeft = NULL;
#endif
#if defined(__WXGTK__)
    m_stMoveRight = NULL;
#endif
#if defined(__WXGTK__)
    m_choRight = NULL;
#endif
#if defined(__WXGTK__)
    m_stMoveUp = NULL;
#endif
#if defined(__WXGTK__)
    m_choUp = NULL;
#endif
#if defined(__WXGTK__)
    m_stMoveDown = NULL;
#endif
#if defined(__WXGTK__)
    m_choDown = NULL;
#endif
    m_panelKeys = NULL;
    m_hotkeysSizer = NULL;
    m_chkStartup = NULL;
    m_chkEnabledAtStartup = NULL;
    m_txtOnScreenKeyboardCommand = NULL;
    m_btntOnScreenKeyboardCommand = NULL;
    m_sizerFaceLocalization = NULL;
    m_chkAutoLocateFace = NULL;
    m_staticCpuUsage = NULL;
    m_choCpuUsage = NULL;
    m_chkEnableWhenFaceDetected = NULL;
    m_staticLocateFaceTimeout2 = NULL;
    m_spinLocateFaceTimeout = NULL;
    m_staticLocateFaceTimeout = NULL;
    m_spinSendActionWait = NULL;
    m_choProfile = NULL;
    m_btnAddProfile = NULL;
    m_btnDeleteProfile = NULL;
    m_txtSelectedCamera = NULL;
    m_btnCameraSettings = NULL;
    m_choLanguage = NULL;
    m_chkCheckUpdatesAtStartup = NULL;
    m_btnCancel = NULL;
////@end WConfiguration member initialisation
    m_cmbLeft = NULL;
    m_cmbRight = NULL;
    m_cmbTop = NULL;
    m_cmbBottom = NULL;

	m_lastId = FIRST_CONTROL_ID;
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

    wxListbook* itemListbook3 = new wxListbook( itemDialog1, ID_LISTBOOK, wxDefaultPosition, wxDefaultSize, wxBK_DEFAULT );
    wxImageList* itemListbook3ImageList = new wxImageList(32, 32, true, 2);
    {
        wxIcon itemListbook3Icon0(itemDialog1->GetIconResource(wxT("resources/user.xpm")));
        itemListbook3ImageList->Add(itemListbook3Icon0);
        wxIcon itemListbook3Icon1(itemDialog1->GetIconResource(wxT("resources/eviacam_small.xpm")));
        itemListbook3ImageList->Add(itemListbook3Icon1);
    }
    itemListbook3->AssignImageList(itemListbook3ImageList);

    wxNotebook* itemNotebook4 = new wxNotebook( itemListbook3, ID_NOTEBOOK, wxDefaultPosition, wxDefaultSize, wxBK_DEFAULT );

    wxPanel* itemPanel5 = new wxPanel( itemNotebook4, ID_PANEL_MOTION, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxVERTICAL);
    itemPanel5->SetSizer(itemBoxSizer6);

    wxStaticBox* itemStaticBoxSizer7Static = new wxStaticBox(itemPanel5, wxID_ANY, _("Motion calibration"));
    wxStaticBoxSizer* itemStaticBoxSizer7 = new wxStaticBoxSizer(itemStaticBoxSizer7Static, wxVERTICAL);
    itemBoxSizer6->Add(itemStaticBoxSizer7, 0, wxGROW|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer8 = new wxFlexGridSizer(0, 4, 0, 0);
    itemStaticBoxSizer7->Add(itemFlexGridSizer8, 0, wxGROW, 5);
    wxStaticText* itemStaticText9 = new wxStaticText( itemStaticBoxSizer7->GetStaticBox(), wxID_STATIC, _("X axis speed"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer8->Add(itemStaticText9, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_spinXSpeed = new wxSpinCtrl( itemStaticBoxSizer7->GetStaticBox(), ID_SPINCTRL_XSPEED, wxT("0"), wxDefaultPosition, wxSize(75, -1), wxSP_ARROW_KEYS, 0, 30, 0 );
    if (WConfiguration::ShowToolTips())
        m_spinXSpeed->SetToolTip(_("Sets response sensitivity for the X axis."));
    itemFlexGridSizer8->Add(m_spinXSpeed, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText11 = new wxStaticText( itemStaticBoxSizer7->GetStaticBox(), wxID_STATIC, _("Y axis speed"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer8->Add(itemStaticText11, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_spinYSpeed = new wxSpinCtrl( itemStaticBoxSizer7->GetStaticBox(), ID_SPINCTRL_YSPEED, wxT("0"), wxDefaultPosition, wxSize(75, -1), wxSP_ARROW_KEYS, 0, 30, 0 );
    if (WConfiguration::ShowToolTips())
        m_spinYSpeed->SetToolTip(_("Sets response sensitivity for the Y axis."));
    itemFlexGridSizer8->Add(m_spinYSpeed, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText13 = new wxStaticText( itemStaticBoxSizer7->GetStaticBox(), wxID_STATIC, _("Acceleration"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer8->Add(itemStaticText13, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_spinAcceleration = new wxSpinCtrl( itemStaticBoxSizer7->GetStaticBox(), ID_SPINCTRL_ACCELERATION, wxT("0"), wxDefaultPosition, wxSize(75, -1), wxSP_ARROW_KEYS, 0, 5, 0 );
    if (WConfiguration::ShowToolTips())
        m_spinAcceleration->SetToolTip(_("Sets pointer's acceleration.\n0 means no acceleration and\n5 maximum acceleration."));
    itemFlexGridSizer8->Add(m_spinAcceleration, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText15 = new wxStaticText( itemStaticBoxSizer7->GetStaticBox(), wxID_STATIC, _("Smoothness"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer8->Add(itemStaticText15, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_spinSmoothness = new wxSpinCtrl( itemStaticBoxSizer7->GetStaticBox(), ID_SPINCTRL_SMOOTHNESS, wxT("0"), wxDefaultPosition, wxSize(75, -1), wxSP_ARROW_KEYS, 0, 8, 0 );
    if (WConfiguration::ShowToolTips())
        m_spinSmoothness->SetToolTip(_("Tremor reduction filter.\n0 means no filter and \n8 maximum filtering."));
    itemFlexGridSizer8->Add(m_spinSmoothness, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText17 = new wxStaticText( itemStaticBoxSizer7->GetStaticBox(), wxID_STATIC, _("Motion threshold"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer8->Add(itemStaticText17, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_spinEasyStop = new wxSpinCtrl( itemStaticBoxSizer7->GetStaticBox(), ID_SPINCTRL_EASYSTOP, wxT("0"), wxDefaultPosition, wxSize(75, -1), wxSP_ARROW_KEYS, 0, 10, 0 );
    if (WConfiguration::ShowToolTips())
        m_spinEasyStop->SetToolTip(_("Minimum displacement (in pixels)\nto start moving pointer."));
    itemFlexGridSizer8->Add(m_spinEasyStop, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemFlexGridSizer8->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemFlexGridSizer8->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemFlexGridSizer8->AddGrowableCol(0);
    itemFlexGridSizer8->AddGrowableCol(1);
    itemFlexGridSizer8->AddGrowableCol(2);
    itemFlexGridSizer8->AddGrowableCol(3);

    m_buttonCalibrateMotion = new wxButton( itemStaticBoxSizer7->GetStaticBox(), ID_BUTTON, _("Assisted calibration"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer7->Add(m_buttonCalibrateMotion, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    itemNotebook4->AddPage(itemPanel5, _("Motion"));

    wxPanel* itemPanel22 = new wxPanel( itemNotebook4, ID_PANEL_WORKSPACE, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer23 = new wxBoxSizer(wxVERTICAL);
    itemPanel22->SetSizer(itemBoxSizer23);

    wxStaticBox* itemStaticBoxSizer24Static = new wxStaticBox(itemPanel22, wxID_ANY, _("Workspace limit"));
    wxStaticBoxSizer* itemStaticBoxSizer24 = new wxStaticBoxSizer(itemStaticBoxSizer24Static, wxVERTICAL);
    itemBoxSizer23->Add(itemStaticBoxSizer24, 0, wxGROW|wxALL, 5);
    m_chkEnabledWorkspace = new wxCheckBox( itemStaticBoxSizer24->GetStaticBox(), ID_CHECKBOX_WORKSPACE_LIMIT, _("Enable workspace limit"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkEnabledWorkspace->SetValue(false);
    if (WConfiguration::ShowToolTips())
        m_chkEnabledWorkspace->SetToolTip(_("Confines the mouse pointer\nto the selected area."));
    itemStaticBoxSizer24->Add(m_chkEnabledWorkspace, 0, wxALIGN_LEFT|wxALL, 5);

    wxBoxSizer* itemBoxSizer26 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer24->Add(itemBoxSizer26, 0, wxALIGN_CENTER_HORIZONTAL|wxLEFT|wxRIGHT, 5);
    wxBoxSizer* itemBoxSizer27 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer26->Add(itemBoxSizer27, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);
    wxGridSizer* itemGridSizer28 = new wxGridSizer(0, 3, 0, 0);
    itemBoxSizer27->Add(itemGridSizer28, 0, wxALIGN_CENTER_HORIZONTAL, 5);
    wxStaticText* itemStaticText29 = new wxStaticText( itemStaticBoxSizer24->GetStaticBox(), wxID_STATIC, _("Top"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridSizer28->Add(itemStaticText29, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_spin_top_workspace = new wxSpinCtrl( itemStaticBoxSizer24->GetStaticBox(), ID_SPINCTRL_TOP_WORKSPACE, wxT("1"), wxDefaultPosition, wxSize(75, -1), wxSP_ARROW_KEYS, 0, 50, 1 );
    if (WConfiguration::ShowToolTips())
        m_spin_top_workspace->SetToolTip(_("Top limit workspace."));
    m_spin_top_workspace->Enable(false);
    itemGridSizer28->Add(m_spin_top_workspace, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM, 5);

    wxStaticText* itemStaticText31 = new wxStaticText( itemStaticBoxSizer24->GetStaticBox(), wxID_STATIC, _("%"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridSizer28->Add(itemStaticText31, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

    wxGridSizer* itemGridSizer32 = new wxGridSizer(0, 6, 0, 0);
    itemBoxSizer27->Add(itemGridSizer32, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticText* itemStaticText33 = new wxStaticText( itemStaticBoxSizer24->GetStaticBox(), wxID_STATIC, _("Left"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridSizer32->Add(itemStaticText33, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_spin_left_workspace = new wxSpinCtrl( itemStaticBoxSizer24->GetStaticBox(), ID_SPINCTRL_LEFT_WORKSPACE, wxT("1"), wxDefaultPosition, wxSize(75, -1), wxSP_ARROW_KEYS, 0, 50, 1 );
    if (WConfiguration::ShowToolTips())
        m_spin_left_workspace->SetToolTip(_("Left limit workspace."));
    m_spin_left_workspace->Enable(false);
    itemGridSizer32->Add(m_spin_left_workspace, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM, 5);

    wxStaticText* itemStaticText35 = new wxStaticText( itemStaticBoxSizer24->GetStaticBox(), wxID_STATIC, _("%"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridSizer32->Add(itemStaticText35, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

    wxStaticText* itemStaticText36 = new wxStaticText( itemStaticBoxSizer24->GetStaticBox(), wxID_STATIC, _("Right"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridSizer32->Add(itemStaticText36, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_spin_right_workspace = new wxSpinCtrl( itemStaticBoxSizer24->GetStaticBox(), ID_SPINCTRL_RIGHT_WORKSPACE, wxT("1"), wxDefaultPosition, wxSize(75, -1), wxSP_ARROW_KEYS, 0, 50, 1 );
    if (WConfiguration::ShowToolTips())
        m_spin_right_workspace->SetToolTip(_("Right limit workspace."));
    m_spin_right_workspace->Enable(false);
    itemGridSizer32->Add(m_spin_right_workspace, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM, 5);

    wxStaticText* itemStaticText38 = new wxStaticText( itemStaticBoxSizer24->GetStaticBox(), wxID_STATIC, _("%"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridSizer32->Add(itemStaticText38, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxGridSizer* itemGridSizer39 = new wxGridSizer(0, 3, 0, 0);
    itemBoxSizer27->Add(itemGridSizer39, 0, wxALIGN_CENTER_HORIZONTAL, 5);
    wxStaticText* itemStaticText40 = new wxStaticText( itemStaticBoxSizer24->GetStaticBox(), wxID_STATIC, _("Bottom"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridSizer39->Add(itemStaticText40, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, 5);

    m_spin_bottom_workspace = new wxSpinCtrl( itemStaticBoxSizer24->GetStaticBox(), ID_SPINCTRL_BOTTOM_WORKSPACE, wxT("1"), wxDefaultPosition, wxSize(75, -1), wxSP_ARROW_KEYS, 0, 50, 1 );
    if (WConfiguration::ShowToolTips())
        m_spin_bottom_workspace->SetToolTip(_("Bottom limit workspace."));
    m_spin_bottom_workspace->Enable(false);
    itemGridSizer39->Add(m_spin_bottom_workspace, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM, 5);

    wxStaticText* itemStaticText42 = new wxStaticText( itemStaticBoxSizer24->GetStaticBox(), wxID_STATIC, _("%"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridSizer39->Add(itemStaticText42, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM, 5);

    wxStaticBox* itemStaticBoxSizer43Static = new wxStaticBox(itemPanel22, wxID_ANY, _("Pointer wrapping"));
    wxStaticBoxSizer* itemStaticBoxSizer43 = new wxStaticBoxSizer(itemStaticBoxSizer43Static, wxVERTICAL);
    itemBoxSizer23->Add(itemStaticBoxSizer43, 0, wxGROW|wxALL, 5);
    m_chkWrapPointer = new wxCheckBox( itemStaticBoxSizer43->GetStaticBox(), ID_CHECKBOX2, _("Allow wrapping the pointer around the working area"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkWrapPointer->SetValue(false);
    itemStaticBoxSizer43->Add(m_chkWrapPointer, 0, wxALIGN_LEFT|wxALL, 5);

    itemNotebook4->AddPage(itemPanel22, _("Workspace"));

    m_panelClick = new wxPanel( itemNotebook4, ID_PANEL_CLICK, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer46 = new wxBoxSizer(wxVERTICAL);
    m_panelClick->SetSizer(itemBoxSizer46);

    wxStaticBox* itemStaticBoxSizer47Static = new wxStaticBox(m_panelClick, wxID_ANY, _("Dwell click"));
    wxStaticBoxSizer* itemStaticBoxSizer47 = new wxStaticBoxSizer(itemStaticBoxSizer47Static, wxVERTICAL);
    itemBoxSizer46->Add(itemStaticBoxSizer47, 0, wxGROW|wxALL, 5);
    m_chkDwellClickEnabled = new wxCheckBox( itemStaticBoxSizer47->GetStaticBox(), ID_CHECKBOX_ENABLE_DWELL, _("Enable dwell click"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkDwellClickEnabled->SetValue(false);
    if (WConfiguration::ShowToolTips())
        m_chkDwellClickEnabled->SetToolTip(_("Enable/Disable automatic (dwell)\nclick generation mechanism."));
    itemStaticBoxSizer47->Add(m_chkDwellClickEnabled, 0, wxALIGN_LEFT|wxALL, 5);

    wxGridSizer* itemGridSizer49 = new wxGridSizer(0, 2, 0, 0);
    itemStaticBoxSizer47->Add(itemGridSizer49, 0, wxGROW, 5);
    wxFlexGridSizer* itemFlexGridSizer50 = new wxFlexGridSizer(0, 2, 0, 0);
    itemGridSizer49->Add(itemFlexGridSizer50, 0, wxGROW|wxALIGN_TOP|wxALL, 0);
    m_stDwellTime = new wxStaticText( itemStaticBoxSizer47->GetStaticBox(), ID_STATIC_DWELL_TIME, _("Dwell time (ds)"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer50->Add(m_stDwellTime, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_spinDwellTime = new wxSpinCtrl( itemStaticBoxSizer47->GetStaticBox(), ID_SPINCTRL_DWELL_TIME, wxT("2"), wxDefaultPosition, wxSize(75, -1), wxSP_ARROW_KEYS, 2, 50, 2 );
    if (WConfiguration::ShowToolTips())
        m_spinDwellTime->SetToolTip(_("Time to wait (deciseconds) \nbefore sending a click."));
    itemFlexGridSizer50->Add(m_spinDwellTime, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    m_stDwellArea = new wxStaticText( itemStaticBoxSizer47->GetStaticBox(), ID_STATIC_DWELL_AREA, _("Dwell area"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer50->Add(m_stDwellArea, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_spinDwellArea = new wxSpinCtrl( itemStaticBoxSizer47->GetStaticBox(), ID_SPINCTRL_DWELL_AREA, wxT("0"), wxDefaultPosition, wxSize(75, -1), wxSP_ARROW_KEYS, 0, 5, 0 );
    if (WConfiguration::ShowToolTips())
        m_spinDwellArea->SetToolTip(_("Maximum allowed displacement\nbefore restarting the dwell time\ncountdown."));
    itemFlexGridSizer50->Add(m_spinDwellArea, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer55 = new wxBoxSizer(wxVERTICAL);
    itemGridSizer49->Add(itemBoxSizer55, 0, wxGROW|wxALIGN_TOP|wxLEFT|wxRIGHT, 5);
    m_chkAllowConsecutiveClick = new wxCheckBox( itemStaticBoxSizer47->GetStaticBox(), ID_CHECKBOX_ALLOW_CONSECUTIVE, _("Allow consecutive clicks"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkAllowConsecutiveClick->SetValue(false);
    if (WConfiguration::ShowToolTips())
        m_chkAllowConsecutiveClick->SetToolTip(_("Send multiple clicks\nwhen the pointer is stopped."));
    itemBoxSizer55->Add(m_chkAllowConsecutiveClick, 0, wxALIGN_LEFT|wxALL, 5);

    m_chkBeepOnClick = new wxCheckBox( itemStaticBoxSizer47->GetStaticBox(), ID_CHECKBOX_BEEP_ON_CLICK, _("Beep on click"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkBeepOnClick->SetValue(false);
    if (WConfiguration::ShowToolTips())
        m_chkBeepOnClick->SetToolTip(_("Play sound when click generated."));
    itemBoxSizer55->Add(m_chkBeepOnClick, 0, wxALIGN_LEFT|wxALL, 5);

#if defined(__WXGTK__)
    m_chkAllowVisualAlerts = new wxCheckBox( itemStaticBoxSizer47->GetStaticBox(), ID_CHECKBOX_ALLOW_VISUAL_ALERTS, _("Allow visual alerts"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkAllowVisualAlerts->SetValue(false);
    itemBoxSizer55->Add(m_chkAllowVisualAlerts, 0, wxALIGN_LEFT|wxALL, 5);
#endif

    m_sboxClickWin = new wxStaticBox(itemStaticBoxSizer47->GetStaticBox(), ID_STATICBOX_CLICK_WIN, _("Click window"));
    wxStaticBoxSizer* itemStaticBoxSizer59 = new wxStaticBoxSizer(m_sboxClickWin, wxVERTICAL);
    itemStaticBoxSizer47->Add(itemStaticBoxSizer59, 0, wxGROW|wxALL, 5);
    m_chkShowClickWin = new wxCheckBox( itemStaticBoxSizer59->GetStaticBox(), ID_CHECKBOX_SHOW_CLICKWIN, _("Show Click Window"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkShowClickWin->SetValue(false);
    itemStaticBoxSizer59->Add(m_chkShowClickWin, 0, wxALIGN_LEFT|wxALL, 5);

    wxFlexGridSizer* itemFlexGridSizer61 = new wxFlexGridSizer(0, 4, 0, 0);
    itemStaticBoxSizer59->Add(itemFlexGridSizer61, 0, wxGROW, 5);
    m_stDesign = new wxStaticText( itemStaticBoxSizer59->GetStaticBox(), ID_STATIC_DESIGN, _("Design:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer61->Add(m_stDesign, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxArrayString m_choClickWindowDesignStrings;
    m_choClickWindowDesignStrings.Add(_("Normal"));
    m_choClickWindowDesignStrings.Add(_("Thin"));
    m_choClickWindowDesign = new wxChoice( itemStaticBoxSizer59->GetStaticBox(), ID_CHOICE_DESIGN, wxDefaultPosition, wxDefaultSize, m_choClickWindowDesignStrings, 0 );
    itemFlexGridSizer61->Add(m_choClickWindowDesign, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_stBehaviour = new wxStaticText( itemStaticBoxSizer59->GetStaticBox(), ID_STATIC_BEHAVIOUR, _("Behaviour:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer61->Add(m_stBehaviour, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxArrayString m_choClickWindowBehaviourStrings;
    m_choClickWindowBehaviourStrings.Add(_("Normal mode"));
    m_choClickWindowBehaviourStrings.Add(_("Fast mode"));
    m_choClickWindowBehaviour = new wxChoice( itemStaticBoxSizer59->GetStaticBox(), ID_CHOICE_BEHAVIOUR, wxDefaultPosition, wxDefaultSize, m_choClickWindowBehaviourStrings, 0 );
    if (WConfiguration::ShowToolTips())
        m_choClickWindowBehaviour->SetToolTip(_("Fast mode enables click type\nselection by hovering the mouse\npointer over the click window\nbuttons."));
    itemFlexGridSizer61->Add(m_choClickWindowBehaviour, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

#if defined(__WXGTK__)
    m_stDocking = new wxStaticText( itemStaticBoxSizer59->GetStaticBox(), wxID_STATIC, _("Location:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer61->Add(m_stDocking, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
#endif

#if defined(__WXGTK__)
    wxArrayString m_choDockingModeStrings;
    m_choDockingModeStrings.Add(_("Floating horizontal"));
    m_choDockingModeStrings.Add(_("Floating vertical"));
    m_choDockingModeStrings.Add(_("Top docked"));
    m_choDockingModeStrings.Add(_("Bottom docked"));
    m_choDockingModeStrings.Add(_("Left docked"));
    m_choDockingModeStrings.Add(_("Right docked"));
    m_choDockingMode = new wxChoice( itemStaticBoxSizer59->GetStaticBox(), ID_CHOICE5, wxDefaultPosition, wxDefaultSize, m_choDockingModeStrings, 0 );
    itemFlexGridSizer61->Add(m_choDockingMode, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
#endif

#if defined(__WXGTK__)
    m_chkAutohide = new wxCheckBox( itemStaticBoxSizer59->GetStaticBox(), ID_CHECKBOX_AUTOHIDE, _("Auto-hide"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkAutohide->SetValue(false);
    itemFlexGridSizer61->Add(m_chkAutohide, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
#endif

    itemFlexGridSizer61->AddGrowableCol(1);
    itemFlexGridSizer61->AddGrowableCol(3);

#if defined(__WXGTK__)
    m_sboxGestureClick = new wxStaticBox(itemStaticBoxSizer47->GetStaticBox(), ID_STATICBOX_GESTURE_CLICK, _("Gesture click"));
    wxStaticBoxSizer* itemStaticBoxSizer69 = new wxStaticBoxSizer(m_sboxGestureClick, wxVERTICAL);
    itemStaticBoxSizer47->Add(itemStaticBoxSizer69, 0, wxGROW|wxALL, 5);
    m_chkEnableGestureClick = new wxCheckBox( itemStaticBoxSizer69->GetStaticBox(), ID_CHECKBOX, _("Enable gesture click"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkEnableGestureClick->SetValue(false);
    itemStaticBoxSizer69->Add(m_chkEnableGestureClick, 0, wxALIGN_LEFT|wxALL, 5);

    wxFlexGridSizer* itemFlexGridSizer71 = new wxFlexGridSizer(0, 4, 0, 0);
    itemStaticBoxSizer69->Add(itemFlexGridSizer71, 0, wxGROW, 5);
    m_stMoveLeft = new wxStaticText( itemStaticBoxSizer69->GetStaticBox(), ID_STATIC_MOVE_LEFT, _("Move left:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer71->Add(m_stMoveLeft, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString m_choLeftStrings;
    m_choLeftStrings.Add(_("Disable"));
    m_choLeftStrings.Add(_("Left click"));
    m_choLeftStrings.Add(_("Middle click"));
    m_choLeftStrings.Add(_("Right click"));
    m_choLeftStrings.Add(_("Double click"));
    m_choLeftStrings.Add(_("Drag click"));
    m_choLeft = new wxChoice( itemStaticBoxSizer69->GetStaticBox(), ID_CHOICE, wxDefaultPosition, wxDefaultSize, m_choLeftStrings, 0 );
    itemFlexGridSizer71->Add(m_choLeft, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_stMoveRight = new wxStaticText( itemStaticBoxSizer69->GetStaticBox(), ID_STATIC_MOVE_RIGHT, _("Move right:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer71->Add(m_stMoveRight, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString m_choRightStrings;
    m_choRightStrings.Add(_("Disable"));
    m_choRightStrings.Add(_("Left click"));
    m_choRightStrings.Add(_("Middle click"));
    m_choRightStrings.Add(_("Right click"));
    m_choRightStrings.Add(_("Double click"));
    m_choRightStrings.Add(_("Drag click"));
    m_choRight = new wxChoice( itemStaticBoxSizer69->GetStaticBox(), ID_CHOICE1, wxDefaultPosition, wxDefaultSize, m_choRightStrings, 0 );
    itemFlexGridSizer71->Add(m_choRight, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_stMoveUp = new wxStaticText( itemStaticBoxSizer69->GetStaticBox(), ID_STATIC_MOVE_UP, _("Move up:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer71->Add(m_stMoveUp, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString m_choUpStrings;
    m_choUpStrings.Add(_("Disable"));
    m_choUpStrings.Add(_("Left click"));
    m_choUpStrings.Add(_("Middle click"));
    m_choUpStrings.Add(_("Right click"));
    m_choUpStrings.Add(_("Double click"));
    m_choUpStrings.Add(_("Drag click"));
    m_choUp = new wxChoice( itemStaticBoxSizer69->GetStaticBox(), ID_CHOICE2, wxDefaultPosition, wxDefaultSize, m_choUpStrings, 0 );
    itemFlexGridSizer71->Add(m_choUp, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_stMoveDown = new wxStaticText( itemStaticBoxSizer69->GetStaticBox(), ID_STATIC_MOVE_DOWN, _("Move down:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer71->Add(m_stMoveDown, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString m_choDownStrings;
    m_choDownStrings.Add(_("Disable"));
    m_choDownStrings.Add(_("Left click"));
    m_choDownStrings.Add(_("Middle click"));
    m_choDownStrings.Add(_("Right click"));
    m_choDownStrings.Add(_("Double click"));
    m_choDownStrings.Add(_("Drag click"));
    m_choDown = new wxChoice( itemStaticBoxSizer69->GetStaticBox(), ID_CHOICE3, wxDefaultPosition, wxDefaultSize, m_choDownStrings, 0 );
    itemFlexGridSizer71->Add(m_choDown, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemFlexGridSizer71->AddGrowableCol(1);
    itemFlexGridSizer71->AddGrowableCol(3);

#endif

    itemNotebook4->AddPage(m_panelClick, _("Click"));

    wxPanel* itemPanel80 = new wxPanel( itemNotebook4, ID_PANEL_KEYS, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer81 = new wxBoxSizer(wxVERTICAL);
    itemPanel80->SetSizer(itemBoxSizer81);

    m_panelKeys = new wxStaticBox(itemPanel80, wxID_ANY, _("Hotkeys"));
    wxStaticBoxSizer* itemStaticBoxSizer82 = new wxStaticBoxSizer(m_panelKeys, wxVERTICAL);
    itemBoxSizer81->Add(itemStaticBoxSizer82, 0, wxGROW|wxALL, 5);
    wxStaticText* itemStaticText83 = new wxStaticText( itemStaticBoxSizer82->GetStaticBox(), wxID_STATIC, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer82->Add(itemStaticText83, 0, wxALIGN_LEFT|wxALL, 5);

    m_hotkeysSizer = new wxFlexGridSizer(0, 4, 0, 0);
    itemStaticBoxSizer82->Add(m_hotkeysSizer, 0, wxGROW|wxALL, 5);
    m_hotkeysSizer->AddGrowableCol(0);
    m_hotkeysSizer->AddGrowableCol(1);
    m_hotkeysSizer->AddGrowableCol(2);
    m_hotkeysSizer->AddGrowableCol(3);

    itemNotebook4->AddPage(itemPanel80, _("Hotkeys"));

    wxPanel* itemPanel85 = new wxPanel( itemNotebook4, ID_PANEL_ADVANCED, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer86 = new wxBoxSizer(wxVERTICAL);
    itemPanel85->SetSizer(itemBoxSizer86);

    wxStaticBox* itemStaticBoxSizer87Static = new wxStaticBox(itemPanel85, wxID_ANY, _("Startup"));
    wxStaticBoxSizer* itemStaticBoxSizer87 = new wxStaticBoxSizer(itemStaticBoxSizer87Static, wxVERTICAL);
    itemBoxSizer86->Add(itemStaticBoxSizer87, 0, wxGROW|wxALL, 5);
    m_chkStartup = new wxCheckBox( itemStaticBoxSizer87->GetStaticBox(), ID_CHECKBOX_STARTUP, _("Start eViacam at user logon"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkStartup->SetValue(false);
    itemStaticBoxSizer87->Add(m_chkStartup, 0, wxALIGN_LEFT|wxALL, 5);

    m_chkEnabledAtStartup = new wxCheckBox( itemStaticBoxSizer87->GetStaticBox(), ID_CHECKBOX_ENABLE_AT_STARTUP, _("Enable eViacam at startup"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkEnabledAtStartup->SetValue(false);
    if (WConfiguration::ShowToolTips())
        m_chkEnabledAtStartup->SetToolTip(_("If checked the program automatically \nenables cursor control at startup."));
    itemStaticBoxSizer87->Add(m_chkEnabledAtStartup, 0, wxALIGN_LEFT|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer90Static = new wxStaticBox(itemPanel85, wxID_ANY, _("On-screen keyboard command"));
    wxStaticBoxSizer* itemStaticBoxSizer90 = new wxStaticBoxSizer(itemStaticBoxSizer90Static, wxVERTICAL);
    itemBoxSizer86->Add(itemStaticBoxSizer90, 0, wxGROW|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer91 = new wxFlexGridSizer(0, 2, 0, 0);
    itemStaticBoxSizer90->Add(itemFlexGridSizer91, 0, wxGROW, 5);
    m_txtOnScreenKeyboardCommand = new wxTextCtrl( itemStaticBoxSizer90->GetStaticBox(), ID_TEXTCTRL_ONSCREENKEYBOARDCOMMAND, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    if (WConfiguration::ShowToolTips())
        m_txtOnScreenKeyboardCommand->SetToolTip(_("Command to run an external on-screen keyboard application."));
    itemFlexGridSizer91->Add(m_txtOnScreenKeyboardCommand, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_btntOnScreenKeyboardCommand = new wxButton( itemStaticBoxSizer90->GetStaticBox(), ID_BUTTON_ONSCREENKEYBOARDCOMMAND, _("Browse..."), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer91->Add(m_btntOnScreenKeyboardCommand, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemFlexGridSizer91->AddGrowableCol(0);

    m_sizerFaceLocalization = new wxStaticBox(itemPanel85, wxID_ANY, _("Face localization and tracking"));
    wxStaticBoxSizer* itemStaticBoxSizer94 = new wxStaticBoxSizer(m_sizerFaceLocalization, wxVERTICAL);
    itemBoxSizer86->Add(itemStaticBoxSizer94, 0, wxGROW|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer95 = new wxFlexGridSizer(0, 2, 0, 0);
    itemStaticBoxSizer94->Add(itemFlexGridSizer95, 0, wxGROW|wxALL, 0);
    m_chkAutoLocateFace = new wxCheckBox( itemStaticBoxSizer94->GetStaticBox(), ID_CHECKBOX_AUTO_LOCATE_FACE, _("Locate face automatically"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkAutoLocateFace->SetValue(false);
    if (WConfiguration::ShowToolTips())
        m_chkAutoLocateFace->SetToolTip(_("When enabled tries to automatically\ndetect your face and center the\ntracking area."));
    itemFlexGridSizer95->Add(m_chkAutoLocateFace, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxFlexGridSizer* itemFlexGridSizer97 = new wxFlexGridSizer(0, 2, 0, 0);
    itemFlexGridSizer95->Add(itemFlexGridSizer97, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 0);
    m_staticCpuUsage = new wxStaticText( itemStaticBoxSizer94->GetStaticBox(), wxID_STATIC, _("CPU usage"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer97->Add(m_staticCpuUsage, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString m_choCpuUsageStrings;
    m_choCpuUsageStrings.Add(_("Lowest"));
    m_choCpuUsageStrings.Add(_("Low"));
    m_choCpuUsageStrings.Add(_("Normal"));
    m_choCpuUsageStrings.Add(_("High"));
    m_choCpuUsageStrings.Add(_("Highest"));
    m_choCpuUsage = new wxChoice( itemStaticBoxSizer94->GetStaticBox(), ID_CHOICE4, wxDefaultPosition, wxDefaultSize, m_choCpuUsageStrings, 0 );
    itemFlexGridSizer97->Add(m_choCpuUsage, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_chkEnableWhenFaceDetected = new wxCheckBox( itemStaticBoxSizer94->GetStaticBox(), ID_CHECKBOX_ENABLE_WHEN_FACE_DETECTED, _("Stop pointer when face not detected"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkEnableWhenFaceDetected->SetValue(false);
    if (WConfiguration::ShowToolTips())
        m_chkEnableWhenFaceDetected->SetToolTip(_("eViacam is only enabled when face is detected."));
    m_chkEnableWhenFaceDetected->Enable(false);
    itemFlexGridSizer95->Add(m_chkEnableWhenFaceDetected, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer101 = new wxBoxSizer(wxHORIZONTAL);
    itemFlexGridSizer95->Add(itemBoxSizer101, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 0);
    m_staticLocateFaceTimeout2 = new wxStaticText( itemStaticBoxSizer94->GetStaticBox(), wxID_STATIC, _("After"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer101->Add(m_staticLocateFaceTimeout2, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_spinLocateFaceTimeout = new wxSpinCtrl( itemStaticBoxSizer94->GetStaticBox(), ID_SPINCTRL2, wxT("1"), wxDefaultPosition, wxSize(75, -1), wxSP_ARROW_KEYS, 1, 59, 1 );
    itemBoxSizer101->Add(m_spinLocateFaceTimeout, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_staticLocateFaceTimeout = new wxStaticText( itemStaticBoxSizer94->GetStaticBox(), wxID_STATIC, _("seconds"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer101->Add(m_staticLocateFaceTimeout, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemFlexGridSizer95->AddGrowableRow(0);
    itemFlexGridSizer95->AddGrowableCol(0);
    itemFlexGridSizer95->AddGrowableCol(1);

    wxStaticBox* itemStaticBoxSizer105Static = new wxStaticBox(itemPanel85, wxID_ANY, _("Mouse actions"));
    wxStaticBoxSizer* itemStaticBoxSizer105 = new wxStaticBoxSizer(itemStaticBoxSizer105Static, wxVERTICAL);
    itemBoxSizer86->Add(itemStaticBoxSizer105, 0, wxGROW|wxALL, 5);
    wxBoxSizer* itemBoxSizer106 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer105->Add(itemBoxSizer106, 0, wxALIGN_LEFT|wxALL, 0);
    m_spinSendActionWait = new wxSpinCtrl( itemStaticBoxSizer105->GetStaticBox(), ID_SPIN_SEND_ACTION_WAIT, wxT("0"), wxDefaultPosition, wxSize(75, -1), wxSP_ARROW_KEYS, 0, 50, 0 );
    if (WConfiguration::ShowToolTips())
        m_spinSendActionWait->SetToolTip(_("Increase this value if some application ignores click events."));
    itemBoxSizer106->Add(m_spinSendActionWait, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText108 = new wxStaticText( itemStaticBoxSizer105->GetStaticBox(), wxID_STATIC, _("ms delay between up and down events"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer106->Add(itemStaticText108, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemNotebook4->AddPage(itemPanel85, _("Advanced"));

    itemListbook3->AddPage(itemNotebook4, _("Profile options"), false, 0);

    wxPanel* itemPanel109 = new wxPanel( itemListbook3, ID_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer110 = new wxBoxSizer(wxVERTICAL);
    itemPanel109->SetSizer(itemBoxSizer110);

    wxStaticBox* itemStaticBoxSizer111Static = new wxStaticBox(itemPanel109, wxID_ANY, _("Profile management"));
    wxStaticBoxSizer* itemStaticBoxSizer111 = new wxStaticBoxSizer(itemStaticBoxSizer111Static, wxVERTICAL);
    itemBoxSizer110->Add(itemStaticBoxSizer111, 0, wxGROW|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer112 = new wxFlexGridSizer(0, 4, 0, 0);
    itemStaticBoxSizer111->Add(itemFlexGridSizer112, 0, wxGROW, 5);
    wxStaticText* itemStaticText113 = new wxStaticText( itemStaticBoxSizer111->GetStaticBox(), wxID_STATIC, _("Select profile:"), wxDefaultPosition, wxDefaultSize, 0 );
    if (WConfiguration::ShowToolTips())
        itemStaticText113->SetToolTip(_("Chooses desired active profile"));
    itemFlexGridSizer112->Add(itemStaticText113, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString m_choProfileStrings;
    m_choProfile = new wxChoice( itemStaticBoxSizer111->GetStaticBox(), ID_CHOICE_PROFILE, wxDefaultPosition, wxDefaultSize, m_choProfileStrings, 0 );
    if (WConfiguration::ShowToolTips())
        m_choProfile->SetToolTip(_("Chooses desired active profile"));
    itemFlexGridSizer112->Add(m_choProfile, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_btnAddProfile = new wxButton( itemStaticBoxSizer111->GetStaticBox(), ID_BUTTON_ADD_PROFILE, _("Add profile"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer112->Add(m_btnAddProfile, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_btnDeleteProfile = new wxButton( itemStaticBoxSizer111->GetStaticBox(), ID_BUTTON_DEL_PROFILE, _("Delete profile"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer112->Add(m_btnDeleteProfile, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemFlexGridSizer112->AddGrowableCol(1);

    wxStaticBox* itemStaticBoxSizer117Static = new wxStaticBox(itemPanel109, wxID_ANY, _("Camera"));
    wxStaticBoxSizer* itemStaticBoxSizer117 = new wxStaticBoxSizer(itemStaticBoxSizer117Static, wxVERTICAL);
    itemBoxSizer110->Add(itemStaticBoxSizer117, 0, wxGROW|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer118 = new wxFlexGridSizer(0, 3, 0, 0);
    itemStaticBoxSizer117->Add(itemFlexGridSizer118, 0, wxGROW, 5);
    m_txtSelectedCamera = new wxTextCtrl( itemStaticBoxSizer117->GetStaticBox(), ID_TEXTCTRL_SELECTED_CAMERA, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
    m_txtSelectedCamera->Enable(false);
    itemFlexGridSizer118->Add(m_txtSelectedCamera, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_btnCameraSettings = new wxButton( itemStaticBoxSizer117->GetStaticBox(), ID_BUTTON_CAMERA_SETTINGS, _("Settings..."), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer118->Add(m_btnCameraSettings, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton121 = new wxButton( itemStaticBoxSizer117->GetStaticBox(), ID_BUTTON_CHANGE_CAMERA, _("Change"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer118->Add(itemButton121, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemFlexGridSizer118->AddGrowableCol(0);

    wxStaticBox* itemStaticBoxSizer122Static = new wxStaticBox(itemPanel109, wxID_ANY, _("Language"));
    wxStaticBoxSizer* itemStaticBoxSizer122 = new wxStaticBoxSizer(itemStaticBoxSizer122Static, wxVERTICAL);
    itemBoxSizer110->Add(itemStaticBoxSizer122, 0, wxGROW|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer123 = new wxFlexGridSizer(0, 2, 0, 0);
    itemStaticBoxSizer122->Add(itemFlexGridSizer123, 0, wxALIGN_LEFT, 5);
    wxStaticText* itemStaticText124 = new wxStaticText( itemStaticBoxSizer122->GetStaticBox(), wxID_STATIC, _("Language:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer123->Add(itemStaticText124, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString m_choLanguageStrings;
    m_choLanguage = new wxChoice( itemStaticBoxSizer122->GetStaticBox(), ID_CHOICE_LANGUAGE, wxDefaultPosition, wxDefaultSize, m_choLanguageStrings, 0 );
    itemFlexGridSizer123->Add(m_choLanguage, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer126Static = new wxStaticBox(itemPanel109, wxID_ANY, _("Check for updates"));
    wxStaticBoxSizer* itemStaticBoxSizer126 = new wxStaticBoxSizer(itemStaticBoxSizer126Static, wxVERTICAL);
    itemBoxSizer110->Add(itemStaticBoxSizer126, 0, wxGROW|wxALL, 5);
    m_chkCheckUpdatesAtStartup = new wxCheckBox( itemStaticBoxSizer126->GetStaticBox(), ID_CHECKBOX_CHECK_UPDATES, _("Automatically check for updates at startup"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkCheckUpdatesAtStartup->SetValue(false);
    itemStaticBoxSizer126->Add(m_chkCheckUpdatesAtStartup, 0, wxALIGN_LEFT|wxALL, 5);

    itemListbook3->AddPage(itemPanel109, _("General options"), false, 1);

    itemBoxSizer2->Add(itemListbook3, 0, wxALIGN_LEFT|wxALL, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer128 = new wxStdDialogButtonSizer;

    itemBoxSizer2->Add(itemStdDialogButtonSizer128, 0, wxALIGN_RIGHT|wxALL, 5);
    wxButton* itemButton129 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer128->AddButton(itemButton129);

    m_btnCancel = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    m_btnCancel->Enable(false);
    itemStdDialogButtonSizer128->AddButton(m_btnCancel);

    itemStdDialogButtonSizer128->Realize();

////@end WConfiguration content construction
#if defined(__WXGTK__)
	for (unsigned int i=0; i<wxGetApp().GetController().GetPointerAction().GetGestureClick().GetKeyEventsCount(); i++) {
		m_choLeft->Append(_("Key:") + wxGetApp().GetController().GetPointerAction().GetGestureClick().GetKeyboardCode(i).GetName());
		m_choRight->Append(_("Key:") + wxGetApp().GetController().GetPointerAction().GetGestureClick().GetKeyboardCode(i).GetName());
		m_choUp->Append(_("Key:") + wxGetApp().GetController().GetPointerAction().GetGestureClick().GetKeyboardCode(i).GetName());
		m_choDown->Append(_("Key:") + wxGetApp().GetController().GetPointerAction().GetGestureClick().GetKeyboardCode(i).GetName());
	}
#endif
	//Add hotkeys
	eviacam::HotkeyManager& hkManager= wxGetApp().GetController().getHotkeyManager();
	for (unsigned int j=0; j< hkManager.get_num_hotkeys(); j++) {
		CreateHotkey (hkManager.GetHotKey(j), m_panelKeys, m_hotkeysSizer);
	}
}

void WConfiguration::CreateHotkey (const eviacam::HotKey& kc, wxWindow* parent, wxSizer* sizer)
{
	wxStaticText* itemStaticText = new wxStaticText( parent, m_lastId, wxString((const wxChar*)kc.GetDescription(),wxConvUTF8), wxDefaultPosition, wxDefaultSize, 0 );
	sizer->Add(itemStaticText, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_controlList.push_back((wxControl*) itemStaticText);

	wxTextCtrl* itemTextCtrl = new wxTextCtrl( parent, m_lastId+1, kc.GetKey().GetName(), wxDefaultPosition, wxDefaultSize, 0 );
	itemTextCtrl->SetEditable(false);
	sizer->Add(itemTextCtrl, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_controlList.push_back((wxControl*) itemTextCtrl);

	wxCheckBox* itemCheckBox = new wxCheckBox( parent, m_lastId+2, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	itemCheckBox->SetValue(kc.IsEnabled());
	sizer->Add(itemCheckBox, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_controlList.push_back((wxControl*) itemCheckBox);

	wxButton* itemButton = new wxButton( parent, m_lastId+3, _("Set key"), wxDefaultPosition, wxDefaultSize, 0 );
	sizer->Add(itemButton, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_controlList.push_back((wxControl*) itemButton);
	
	itemCheckBox->Connect(m_lastId+2, wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(WConfiguration::OnHotkeyCheckboxClick), NULL, this);						
	itemButton->Connect(m_lastId+3, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(WConfiguration::OnHotkeyButtonClick), NULL, this);						
	
	m_lastId += 4;
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
    if (name == wxT("resources/eviacam_mini.xpm"))
    {
        wxIcon icon(eviacam_mini);
        return icon;
    }
    else if (name == wxT("resources/user.xpm"))
    {
        wxIcon icon(user);
        return icon;
    }
    else if (name == wxT("resources/eviacam_small.xpm"))
    {
        wxIcon icon(eviacam_small);
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
	m_spinXSpeed->SetValue (
		wxGetApp().GetController().GetPointerAction().GetXSpeed());
	m_spinYSpeed->SetValue (
		wxGetApp().GetController().GetPointerAction().GetYSpeed());
	m_spinAcceleration->SetValue (
		wxGetApp().GetController().GetPointerAction().GetAcceleration());
	m_spinSmoothness->SetValue (
		wxGetApp().GetController().GetPointerAction().GetSmoothness());
	m_spinEasyStop->SetValue (
		wxGetApp().GetController().GetPointerAction().GetEasyStopValue());
		
	// Workspace
	m_chkEnabledWorkspace->SetValue (
		wxGetApp().GetController().GetPointerAction().GetRestrictedWorkingArea());
	if (wxGetApp().GetController().GetPointerAction().GetRestrictedWorkingArea()) {
		m_spin_top_workspace->Enable(true);
		m_spin_left_workspace->Enable(true);
		m_spin_right_workspace->Enable(true);
		m_spin_bottom_workspace->Enable(true);
	}
	m_spin_top_workspace->SetValue (
		wxGetApp().GetController().GetPointerAction().GetTopWorkspace());
	m_spin_left_workspace->SetValue (
		wxGetApp().GetController().GetPointerAction().GetLeftWorkspace());
	m_spin_right_workspace->SetValue (
		wxGetApp().GetController().GetPointerAction().GetRightWorkspace());
	m_spin_bottom_workspace->SetValue (
		wxGetApp().GetController().GetPointerAction().GetBottomWorkspace());
	m_chkWrapPointer->SetValue (
		wxGetApp().GetController().GetPointerAction().GetWrapPointer());	
	
	// Clic
	m_chkDwellClickEnabled->SetValue (
		wxGetApp().GetController().GetPointerAction().GetClickMode()!= CPointerAction::NONE);
#if defined(__WXGTK__)
	m_chkEnableGestureClick->SetValue (
		wxGetApp().GetController().GetPointerAction().GetClickMode()== CPointerAction::GESTURE);
#endif
	m_chkAllowConsecutiveClick->SetValue (
		wxGetApp().GetController().GetPointerAction().GetDwellClick().GetConsecutiveClicksAllowed());
	m_chkBeepOnClick->SetValue (
		wxGetApp().GetController().GetPointerAction().GetBeepOnClick());
	m_spinDwellTime->SetValue (
		wxGetApp().GetController().GetPointerAction().GetDwellTime());
	m_spinDwellArea->SetValue (
		wxGetApp().GetController().GetPointerAction().GetDwellToleranceArea () );
	m_chkShowClickWin->SetValue (
		wxGetApp().GetController().GetPointerAction().GetDwellClick().GetUseClickWindow());		
	m_choClickWindowBehaviour->Select (
		wxGetApp().GetController().GetPointerAction().GetDwellClick().GetClickWindowController().GetFastMode() ? 1 : 0);
	m_choClickWindowDesign->Select (
		wxGetApp().GetController().GetPointerAction().GetDwellClick().GetClickWindowController().GetDesign());
#if defined(__WXGTK__)
	m_choDockingMode->Select (
		wxGetApp().GetController().GetPointerAction().GetDwellClick().GetClickWindowController().GetLocation());
	m_chkAutohide->SetValue (
		wxGetApp().GetController().GetPointerAction().GetDwellClick().GetClickWindowController().GetAutohide() ? 1 : 0);	
	m_choLeft->Select (
		wxGetApp().GetController().GetPointerAction().GetGestureClick().GetActionLeft());
	m_choRight->Select (
		wxGetApp().GetController().GetPointerAction().GetGestureClick().GetActionRight());
	m_choUp->Select (
		wxGetApp().GetController().GetPointerAction().GetGestureClick().GetActionTop());
	m_choDown->Select (
		wxGetApp().GetController().GetPointerAction().GetGestureClick().GetActionBottom());
	m_chkAllowVisualAlerts->SetValue (
		wxGetApp().GetController().GetPointerAction().GetVisualAlerts());
#endif
	UpdateGUIClickOptions();

	// Startup
	m_chkEnabledAtStartup->SetValue (
		wxGetApp().GetController().GetEnabledAtStartup());
	
	// Advanced
	m_chkAutoLocateFace->SetValue (
		wxGetApp().GetController().GetVisionPipeline().GetTrackFace());
	m_chkEnableWhenFaceDetected->Enable (m_chkAutoLocateFace->GetValue());
	m_chkEnableWhenFaceDetected->SetValue (
		wxGetApp().GetController().GetVisionPipeline().GetEnableWhenFaceDetected());
	m_spinLocateFaceTimeout->SetValue(wxGetApp().GetController().GetVisionPipeline().GetTimeout());
	m_spinLocateFaceTimeout->Enable(m_chkAutoLocateFace->GetValue() && m_chkEnableWhenFaceDetected->GetValue());
	m_choCpuUsage->Select(wxGetApp().GetController().GetVisionPipeline().GetCpuUsage());
	m_choCpuUsage->Enable(m_chkAutoLocateFace->GetValue());
	m_txtOnScreenKeyboardCommand->SetValue(
		wxGetApp().GetController().GetOnScreenKeyboardCommand());
	m_spinSendActionWait->SetValue(wxGetApp().GetController().GetPointerAction().GetSendActionWait());


	// 
	// App data
	//

	m_chkStartup->SetValue(wxGetApp().GetController().GetAutostart().IsEnabled());

	// Profile combo
	m_choProfile->Clear();
	m_choProfile->Append (
		wxGetApp().GetController().GetConfigManager().GetProfileList());
	m_choProfile->Select (
		wxGetApp().GetController().GetConfigManager().GetCurrentProfile());

	// Profile buttons
	if (wxGetApp().GetController().GetConfigManager().GetCurrentProfile()== 0)
		m_btnDeleteProfile->Enable (false);
	else
		m_btnDeleteProfile->Enable (true);

	// Fill and set language combo
	m_choLanguage->Clear();
	m_choLanguage->Append (wxGetTranslation(g_languages[0].name));
	for (unsigned int i= 1; i< WXSIZEOF(g_languages); i++) {
		m_choLanguage->Append (g_languages[i].name);
	}		
	// Select current language
	for (unsigned int i= 0; i< WXSIZEOF(g_languages); i++)
		if (g_languages[i].lang== wxGetApp().GetController().GetLanguage())
			m_choLanguage->SetSelection(i);
	
	// Camera
	m_txtSelectedCamera->SetValue (wxGetApp().GetController().GetCameraName());
	if (wxGetApp().GetController().CameraHasSettingsDialog ())
		m_btnCameraSettings->Enable (true);
	else
		m_btnCameraSettings->Enable (false);

	// Check updates at startup
	m_chkCheckUpdatesAtStartup->SetValue(wxGetApp().GetController().GetCheckUpdatesAtStartup());
}

void WConfiguration::UpdateGUIClickOptions()
{
	if (m_chkDwellClickEnabled->IsChecked()) {
		EnableGUIGeneralClickOptions(true);
#if defined(__WXGTK__)
		if (m_chkEnableGestureClick->IsChecked()) {
			EnableGUIGestureOptions (true);
			EnableGUIClickWindowOptions(false);
		}
		else {
			EnableGUIGestureOptions (false);
			EnableGUIClickWindowOptions(true);
		}
#else
		EnableGUIClickWindowOptions(true);	
#endif
	}		
	else {
		EnableGUIGeneralClickOptions(false);
		EnableGUIGestureOptions (false);
		EnableGUIClickWindowOptions(false);
	}
}

void WConfiguration::EnableGUIGeneralClickOptions (bool enable)
{
	m_spinDwellTime->Enable(enable);
	m_spinDwellArea->Enable(enable);
	m_chkAllowConsecutiveClick->Enable(enable);
	m_chkBeepOnClick->Enable(enable);
	m_chkShowClickWin->Enable(enable);
	m_choClickWindowDesign->Enable(enable);
	m_choClickWindowBehaviour->Enable(enable);
	m_stDwellTime->Enable(enable);
	m_stDwellArea->Enable(enable);
	m_stDesign->Enable(enable);
	m_stBehaviour->Enable(enable);
#if defined(__WXGTK__)
	m_chkAllowVisualAlerts->Enable(enable);
	m_chkEnableGestureClick->Enable(enable);
#endif
}

void WConfiguration::EnableGUIGestureOptions (bool enable)
{
#if defined(__WXGTK__)
	m_choLeft->Enable(enable);
	m_choRight->Enable(enable);			
	m_choUp->Enable(enable);			
	m_choDown->Enable(enable);
	m_stMoveLeft->Enable(enable);
	m_stMoveRight->Enable(enable);
	m_stMoveUp->Enable(enable);
	m_stMoveDown->Enable(enable);
#else
	wxUnusedVar(enable);
#endif
}

void WConfiguration::EnableGUIClickWindowOptions(bool enable)
{
	m_chkShowClickWin->Enable(enable);
	m_choClickWindowDesign->Enable(enable);
	m_choClickWindowBehaviour->Enable(enable);
    m_stDesign->Enable(enable);
	m_stBehaviour->Enable(enable);
#if defined(__WXGTK__)
    m_stDocking->Enable(enable);
    m_choDockingMode->Enable(enable);
	m_chkAutohide->Enable(enable);
#endif
	
	if (enable) {
		m_stDesign->Enable(m_chkShowClickWin->IsChecked());
		m_choClickWindowDesign->Enable(m_chkShowClickWin->IsChecked());
		m_stBehaviour->Enable(m_chkShowClickWin->IsChecked());
		m_choClickWindowBehaviour->Enable(m_chkShowClickWin->IsChecked());
#if defined(__WXGTK__)
		m_stDocking->Enable(m_chkShowClickWin->IsChecked());
		m_choDockingMode->Enable(m_chkShowClickWin->IsChecked());
		m_chkAutohide->Enable(m_chkShowClickWin->IsChecked());
#endif        
	}
#if defined(__WXGTK__)	
	if (	m_choDockingMode->GetCurrentSelection()== CClickWindowController::FLOATING_VERTICAL ||
		m_choDockingMode->GetCurrentSelection()== CClickWindowController::FLOATING_HORIZONTAL) {
		
		m_chkAutohide->Enable(false);
	}
#endif
}

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
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_ENABLE_AT_STARTUP
 */

void WConfiguration::OnCheckboxEnableAtStartupClick( wxCommandEvent& event )
{
	wxGetApp().GetController().SetEnabledAtStartup (m_chkEnabledAtStartup->GetValue());
	event.Skip(false);
	Changed ();
}




/*!
 * wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL_XSPEED
 */

void WConfiguration::OnSpinctrlXspeedUpdated( wxSpinEvent& event )
{
	wxGetApp().GetController().GetPointerAction().SetXSpeed(m_spinXSpeed->GetValue());
	event.Skip(false);
	Changed ();
}


/*!
 * wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL_YSPEED
 */

void WConfiguration::OnSpinctrlYspeedUpdated( wxSpinEvent& event )
{
	wxGetApp().GetController().GetPointerAction().SetYSpeed(m_spinYSpeed->GetValue());
	event.Skip(false);
	Changed ();
}


/*!
 * wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL_ACCELERATION
 */

void WConfiguration::OnSpinctrlAccelerationUpdated( wxSpinEvent& event )
{
	wxGetApp().GetController().GetPointerAction().SetAcceleration(m_spinAcceleration->GetValue());
	event.Skip(false);
	Changed ();
}

/*!
 * wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL_SMOOTHNESS
 */

void WConfiguration::OnSpinctrlSmoothnessUpdated( wxSpinEvent& event )
{
	wxGetApp().GetController().GetPointerAction().SetSmoothness(m_spinSmoothness->GetValue());
	event.Skip(false);
	Changed ();
}


/*!
 * wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL_EASYSTOP
 */

void WConfiguration::OnSpinctrlEasystopUpdated( wxSpinEvent& event )
{
	wxGetApp().GetController().GetPointerAction().SetEasyStopValue(m_spinEasyStop->GetValue());
	event.Skip(false);
	Changed ();
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_ENABLE_DWELL
 */

void WConfiguration::OnCheckboxEnableDwellClick( wxCommandEvent& event )
{
	if (!m_chkDwellClickEnabled->GetValue()) {
		if (wxGetApp().GetController().GetPointerAction().SetClickMode(CPointerAction::NONE, false, this)) {
			EnableGUIGeneralClickOptions(false);
			EnableGUIGestureOptions(false);
			EnableGUIClickWindowOptions(false);
			Changed();
		}
	}
	else {
		// Enabling click generation, which modality?
#if defined(__WXGTK__)
		if (m_chkEnableGestureClick->IsChecked()) {
			// Gesture click
			if (wxGetApp().GetController().GetPointerAction().SetClickMode(CPointerAction::GESTURE, false, this)) {
				EnableGUIGeneralClickOptions(true);
				EnableGUIGestureOptions(true);
				EnableGUIClickWindowOptions(false);
				Changed ();
			}
		}
		else
#endif
			if (wxGetApp().GetController().GetPointerAction().SetClickMode(CPointerAction::DWELL, false, this)) {
				EnableGUIGeneralClickOptions(true);
				EnableGUIGestureOptions(false);
				EnableGUIClickWindowOptions(true);
				Changed ();
			}
	}		
	m_chkDwellClickEnabled->SetValue (wxGetApp().GetController().GetPointerAction().GetClickMode()!= CPointerAction::NONE);
	event.Skip(false);	
}

#if defined(__WXGTK__)
/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX
 */

void WConfiguration::OnCheckboxEnableGestureClick( wxCommandEvent& event )
{
	if (m_chkEnableGestureClick->IsChecked())
	{
		if (wxGetApp().GetController().GetPointerAction().SetClickMode(CPointerAction::GESTURE, false, this)) {
			EnableGUIGestureOptions(true);
			EnableGUIClickWindowOptions(false);
		}
		else {
			m_chkEnableGestureClick->SetValue(false);
		}
	}
	else {
		if (wxGetApp().GetController().GetPointerAction().SetClickMode(CPointerAction::DWELL, false, this)) {
			EnableGUIGestureOptions(false);
			EnableGUIClickWindowOptions(true);
		}
		else
			m_chkEnableGestureClick->SetValue(true);
	}
}
#endif

/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_ALLOW_CONSECUTIVE
 */

void WConfiguration::OnCheckboxAllowConsecutiveClick( wxCommandEvent& event )
{
	wxGetApp().GetController().GetPointerAction().GetDwellClick().
		SetConsecutiveClicksAllowed (m_chkAllowConsecutiveClick->GetValue());
	Changed ();
	event.Skip(false);	
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_BEEP_ON_CLICK
 */

void WConfiguration::OnCheckboxBeepOnClickClick( wxCommandEvent& event )
{
	wxGetApp().GetController().GetPointerAction().SetBeepOnClick (m_chkBeepOnClick->GetValue());
	event.Skip(false);
	Changed ();
}


/*!
 * wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL_DWELL_TIME
 */

void WConfiguration::OnSpinctrlDwellTimeUpdated( wxSpinEvent& event )
{
	wxGetApp().GetController().GetPointerAction().SetDwellTime (m_spinDwellTime->GetValue());
	event.Skip(false);
	Changed ();
}


/*!
 * wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL_DWELL_AREA
 */

void WConfiguration::OnSpinctrlDwellAreaUpdated( wxSpinEvent& event )
{
	wxGetApp().GetController().GetPointerAction().SetDwellToleranceArea (m_spinDwellArea->GetValue());		
	event.Skip(false);
	Changed ();
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_SHOW_CLICKWIN
 */

void WConfiguration::OnCheckboxShowClickwinClick( wxCommandEvent& event )
{
	m_stDesign->Enable(m_chkShowClickWin->IsChecked());
	m_choClickWindowDesign->Enable(m_chkShowClickWin->IsChecked());
	m_stBehaviour->Enable(m_chkShowClickWin->IsChecked());
	m_choClickWindowBehaviour->Enable(m_chkShowClickWin->IsChecked());
#if defined(__WXGTK__)
	m_stDocking->Enable(m_chkShowClickWin->IsChecked());
	m_choDockingMode->Enable(m_chkShowClickWin->IsChecked());
	m_chkAutohide->Enable(m_chkShowClickWin->IsChecked());	
	if (	m_choDockingMode->GetCurrentSelection()== CClickWindowController::FLOATING_VERTICAL ||
		m_choDockingMode->GetCurrentSelection()== CClickWindowController::FLOATING_HORIZONTAL)
	{
		m_chkAutohide->Enable(false);
	}
#endif
		
	wxGetApp().GetController().GetPointerAction().GetDwellClick().SetUseClickWindow(
		!wxGetApp().GetController().GetPointerAction().GetDwellClick().GetUseClickWindow()
	);
	event.Skip(false);
	Changed ();
}

/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_AUTO_LOCATE_FACE
 */

void WConfiguration::OnCheckboxAutoLocateFaceClick( wxCommandEvent& event )
{
	wxGetApp().GetController().GetVisionPipeline().SetTrackFace (m_chkAutoLocateFace->GetValue());
	wxGetApp().GetController().GetVisionPipeline().SetDegradation(0);
	m_chkEnableWhenFaceDetected->Enable (m_chkAutoLocateFace->GetValue());
	m_spinLocateFaceTimeout->Enable(m_chkAutoLocateFace->GetValue() && m_chkEnableWhenFaceDetected->GetValue());
	m_staticLocateFaceTimeout->Enable(m_chkAutoLocateFace->GetValue());
	m_staticLocateFaceTimeout2->Enable(m_chkAutoLocateFace->GetValue());
	m_choCpuUsage->Enable(m_chkAutoLocateFace->GetValue());
	m_staticCpuUsage->Enable(m_chkAutoLocateFace->GetValue());
	event.Skip(false);
	Changed ();
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_SHOW_LOCATE_FACE_FILTER
 */

void WConfiguration::OnCheckboxEnableWhenFaceDetectedClick( wxCommandEvent& event )
{
	wxGetApp().GetController().GetVisionPipeline().SetEnableWhenFaceDetected(m_chkEnableWhenFaceDetected->GetValue());
	m_spinLocateFaceTimeout->Enable (m_chkEnableWhenFaceDetected->GetValue());
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
		wxGetApp().GetController().SetLanguage (g_languages[index].lang);
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
		m_choProfile->Select (wxGetApp().GetController().GetConfigManager().GetCurrentProfile());		
	else
	{
		// Change profile
		wxGetApp().GetController().GetConfigManager().ChangeCurrentProfile (event.GetSelection());

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
		wxGetApp().GetController().GetConfigManager().DeleteCurrentProfile();
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
		switch (wxGetApp().GetController().GetConfigManager().AddNewProfile (dlg.GetValue()))
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
	wxGetApp().GetController().GetPointerAction().GetDwellClick().
		GetClickWindowController().SetFastMode ( event.GetSelection()!= 0);
	event.Skip(false);
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE_DESIGN
 */

void WConfiguration::OnChoiceDesignSelected( wxCommandEvent& event )
{
	CClickWindowController::EDesign design= 
		(CClickWindowController::EDesign) event.GetSelection();

	wxGetApp().GetController().GetPointerAction().GetDwellClick().
		GetClickWindowController().SetDesign (design);

	event.Skip(false);
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_ONSCREENKEYBOARDCOMMAND
 */

void WConfiguration::OnButtonOnscreenkeyboardcommandClick( wxCommandEvent& event )
{
#if defined(__WXMSW__)
	wxFileDialog dlg (this, _("Choose on-screen keyboard command"), _T(""), _T(""),
		_("Executable files|*.exe"), wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR);
#else
	wxFileDialog dlg (this, _("Choose on-screen keyboard command"), 
		_T(""), _T(""), _T("*"), wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR);
#endif

	if (dlg.ShowModal()== wxID_OK) m_txtOnScreenKeyboardCommand->SetValue(dlg.GetPath());

	event.Skip(false);
}


/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL_ONSCREENKEYBOARDCOMMAND
 */

void WConfiguration::OnTextctrlOnscreenkeyboardcommandTextUpdated( wxCommandEvent& event )
{
	if (event.GetString().Cmp(wxGetApp().GetController().GetOnScreenKeyboardCommand()))
	{
		wxGetApp().GetController().SetOnScreenKeyboardCommand	(event.GetString());
		Changed ();
	}
	event.Skip(false);
	
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_CAMERA_SETTINGS
 */

void WConfiguration::OnButtonCameraSettingsClick( wxCommandEvent& event )
{
	wxGetApp().GetController().ShowCameraSettingsDialog ();

	event.Skip (false);
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_CHANGE_CAMERA
 */

void WConfiguration::OnButtonChangeCameraClick( wxCommandEvent& event )
{
    wxGetApp().GetController().ChangeCamera ();
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
    wxGetApp().GetController().GetPointerAction().SetTopWorkspace(m_spin_top_workspace->GetValue());
	m_spin_top_workspace->SetValue (wxGetApp().GetController().GetPointerAction().GetTopWorkspace());
	event.Skip(false);
	Changed ();
}


/*!
 * wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL_RIGHT_WORKSPACE
 */

void WConfiguration::OnSpinctrlRightWorkspaceUpdated( wxSpinEvent& event )
{
    wxGetApp().GetController().GetPointerAction().SetRightWorkspace(m_spin_right_workspace->GetValue());
	m_spin_right_workspace->SetValue (wxGetApp().GetController().GetPointerAction().GetRightWorkspace());
	event.Skip(false);
	Changed ();
}


/*!
 * wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL_LEFT_WORKSPACE
 */

void WConfiguration::OnSpinctrlLeftWorkspaceUpdated( wxSpinEvent& event )
{
    wxGetApp().GetController().GetPointerAction().SetLeftWorkspace(m_spin_left_workspace->GetValue());
	m_spin_left_workspace->SetValue (wxGetApp().GetController().GetPointerAction().GetLeftWorkspace());
	event.Skip(false);
	Changed ();
}


/*!
 * wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL_BOTTOM_WORKSPACE
 */

void WConfiguration::OnSpinctrlBottomWorkspaceUpdated( wxSpinEvent& event )
{
    wxGetApp().GetController().GetPointerAction().SetBottomWorkspace(m_spin_bottom_workspace->GetValue());
	m_spin_bottom_workspace->SetValue (wxGetApp().GetController().GetPointerAction().GetBottomWorkspace());
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
            wxGetApp().GetController().GetPointerAction().SetRestrictedWorkingArea(m_chkEnabledWorkspace->GetValue());
            m_spin_top_workspace->Enable(m_chkEnabledWorkspace->GetValue());
            m_spin_left_workspace->Enable(m_chkEnabledWorkspace->GetValue());
            m_spin_right_workspace->Enable(m_chkEnabledWorkspace->GetValue());
            m_spin_bottom_workspace->Enable(m_chkEnabledWorkspace->GetValue());
        }		
	} else {
        wxGetApp().GetController().GetPointerAction().SetRestrictedWorkingArea(m_chkEnabledWorkspace->GetValue());
        m_spin_top_workspace->Enable(m_chkEnabledWorkspace->GetValue());
        m_spin_left_workspace->Enable(m_chkEnabledWorkspace->GetValue());
        m_spin_right_workspace->Enable(m_chkEnabledWorkspace->GetValue());
        m_spin_bottom_workspace->Enable(m_chkEnabledWorkspace->GetValue());
    }

    
	event.Skip(false);
	Changed ();
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_STARTUP
 */

void WConfiguration::OnCheckboxStartupClick( wxCommandEvent& event )
{
	wxGetApp().GetController().GetAutostart().Enable(m_chkStartup->GetValue());
    event.Skip();
    Changed ();
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON
 */

void WConfiguration::OnButtonClick( wxCommandEvent& event )
{
	if (wxGetApp().GetController().StartMotionCalibration()) {
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
	wxGetApp().GetController().GetPointerAction().GetGestureClick().
		SetActionLeft ((CGestureClick::EAction) event.GetInt());
	event.Skip(false);
}


/*!
 * wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_COMBOBOX_RIGHT
 */

void WConfiguration::OnComboboxRightSelected( wxCommandEvent& event )
{
	wxGetApp().GetController().GetPointerAction().GetGestureClick().
		SetActionRight ((CGestureClick::EAction) event.GetInt());
	event.Skip(false);
}


/*!
 * wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_COMBOBOX_TOP
 */

void WConfiguration::OnComboboxTopSelected( wxCommandEvent& event )
{
	wxGetApp().GetController().GetPointerAction().GetGestureClick().
		SetActionTop ((CGestureClick::EAction) event.GetInt());
	event.Skip(false);
}


/*!
 * wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_COMBOBOX_BOTTOM
 */

void WConfiguration::OnComboboxBottomSelected( wxCommandEvent& event )
{
	wxGetApp().GetController().GetPointerAction().GetGestureClick().
		SetActionBottom ((CGestureClick::EAction) event.GetInt());
	event.Skip(false);
}

#if defined(__WXGTK__)
/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_ALLOW_VISUAL_ALERTS
 */

void WConfiguration::OnCheckboxAllowVisualAlertsClick( wxCommandEvent& event )
{
	wxGetApp().GetController().GetPointerAction().SetVisualAlerts(m_chkAllowVisualAlerts->IsChecked());
	Changed ();
	event.Skip(false);	
}
#endif

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void WConfiguration::OnOkClick( wxCommandEvent& event )
{
	// Save changes and destroy window
	wxGetApp().GetController().GetConfigManager().WriteAll();
	Destroy();
	event.Skip(false);
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL
 */

void WConfiguration::OnCancelClick( wxCommandEvent& event )
{
	bool shouldClose= true;

	if (m_dirty) {
		wxMessageDialog dlg (NULL, _("Discard changes?"), _("eViacam warning"), wxICON_EXCLAMATION | wxYES_NO );
		if (dlg.ShowModal()!= wxID_YES) shouldClose= false;
		else 
			// Discard changes
			wxGetApp().GetController().GetConfigManager().ReadAll();	
	}
	
	if (shouldClose) Destroy();

	event.Skip(false);
}



#if defined(__WXGTK__)

/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE
 */

void WConfiguration::OnLeftGestureChoiceSelected( wxCommandEvent& event )
{
	wxGetApp().GetController().GetPointerAction().GetGestureClick().
		SetActionLeft ( (CGestureClick::EAction) event.GetInt());
	Changed();
	event.Skip(false);
}
#endif

#if defined(__WXGTK__)

/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE1
 */

void WConfiguration::OnRightGestureChoiceSelected( wxCommandEvent& event )
{
	wxGetApp().GetController().GetPointerAction().GetGestureClick().
		SetActionRight ((CGestureClick::EAction) event.GetInt());
	Changed();
	event.Skip(false);
}
#endif

#if defined(__WXGTK__)

/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE2
 */

void WConfiguration::OnUpGestureChoiceSelected( wxCommandEvent& event )
{
	wxGetApp().GetController().GetPointerAction().GetGestureClick().
		SetActionTop ((CGestureClick::EAction) event.GetInt());
	Changed();
	event.Skip(false);
}
#endif

#if defined(__WXGTK__)

/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE3
 */

void WConfiguration::OnDownGestureChoiceSelected( wxCommandEvent& event )
{
	wxGetApp().GetController().GetPointerAction().GetGestureClick().
		SetActionBottom ((CGestureClick::EAction) event.GetInt());
	Changed();
	event.Skip(false);
}
#endif


/*!
 * wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL2
 */

void WConfiguration::OnSpinLocateFaceTimeoutUpdated( wxSpinEvent& event )
{
	wxGetApp().GetController().GetVisionPipeline().SetTimeout(m_spinLocateFaceTimeout->GetValue());
    event.Skip();
}




/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE4
 */

void WConfiguration::OnChoCpuUsageSelected( wxCommandEvent& event )
{
	wxGetApp().GetController().GetVisionPipeline().SetCpuUsage(m_choCpuUsage->GetCurrentSelection());
    event.Skip();
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for Hotkeys
 */

void WConfiguration::OnHotkeyCheckboxClick( wxCommandEvent& event )
{
	eviacam::HotkeyManager& hkm= wxGetApp().GetController().getHotkeyManager();
	// TODO: this is ugly...
	eviacam::HotKey& hk= hkm.GetHotKey((event.GetId()-FIRST_CONTROL_ID-2)/4);
	if (event.GetInt()) hkm.EnableHotKey(hk);
	else hkm.DisableHotKey(hk);

	event.Skip(false);
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for Hotkeys
 */

void WConfiguration::OnHotkeyButtonClick( wxCommandEvent& event )
{
	WGetKey dlgGetKey(this);
	eviacam::HotkeyManager& hkm= wxGetApp().GetController().getHotkeyManager();
	eviacam::HotKey& hk= hkm.GetHotKey((event.GetId()-FIRST_CONTROL_ID-3)/4);

	for (;;) {
		if (dlgGetKey.ShowModal()!= wxID_YES) break;
		KeyboardCode kc= dlgGetKey.GetKeyCode();
		if (hkm.SetHotKeyKeyboardCode(hk, kc)) {
			((wxTextCtrl*)m_controlList[event.GetId()-FIRST_CONTROL_ID-2])->SetValue(kc.GetName());
			break;
		}
	}

	event.Skip(false);
}



/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX2
 */

void WConfiguration::OnCheckboxWrapPointer( wxCommandEvent& event )
{
	wxGetApp().GetController().GetPointerAction().SetWrapPointer(m_chkWrapPointer->GetValue());
    event.Skip();
}

#if defined(__WXGTK__)
/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE5
 */

void WConfiguration::OnChoiceClickWindowModeSelected( wxCommandEvent& event )
{
	wxGetApp().GetController().GetPointerAction().GetDwellClick().GetClickWindowController().
		SetLocation( (CClickWindowController::ELocation) event.GetSelection() );
	
	if (	m_choDockingMode->GetCurrentSelection()== CClickWindowController::FLOATING_VERTICAL ||
		m_choDockingMode->GetCurrentSelection()== CClickWindowController::FLOATING_HORIZONTAL)
		
		m_chkAutohide->Enable(false);
	else
		m_chkAutohide->Enable(true);
	
	event.Skip(false);
}
#endif


#if defined(__WXGTK__)
/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_AUTOHIDE
 */

void WConfiguration::OnCheckboxAutohideClick( wxCommandEvent& event )
{
	wxGetApp().GetController().GetPointerAction().GetDwellClick().
	GetClickWindowController().SetAutohide(m_chkAutohide->IsChecked());
	
	event.Skip(false);
}
#endif


/*!
 * wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPIN_SEND_ACTION_WAIT
 */

void WConfiguration::OnSpinSendActionWaitUpdated( wxSpinEvent& event )
{
	wxGetApp().GetController().GetPointerAction().SetSendActionWait(event.GetInt());
	Changed();
    event.Skip(false);
}

/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_CHECK_UPDATES
 */

void WConfiguration::OnCheckboxCheckUpdatesClick(wxCommandEvent& event)
{
	wxGetApp().GetController().SetCheckUpdatesAtStartup(m_chkCheckUpdatesAtStartup->GetValue());
	event.Skip(false);
	Changed();
}


