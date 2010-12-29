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
#include "wgetkey.h"
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
	wxLANGUAGE_ARABIC,
	wxLANGUAGE_HEBREW
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

#if defined(__WXGTK__)
    EVT_CHECKBOX( ID_CHECKBOX_ALLOW_VISUAL_ALERTS, WConfiguration::OnCheckboxAllowVisualAlertsClick )
#endif

    EVT_CHOICE( ID_CHOICE_DESIGN, WConfiguration::OnChoiceDesignSelected )

    EVT_CHOICE( ID_CHOICE_BEHAVIOUR, WConfiguration::OnChoiceBehaviourSelected )

    EVT_CHECKBOX( ID_CHECKBOX_CLICKWIN_AT_STARTUP, WConfiguration::OnCheckboxClickwinAtStartupClick )

    EVT_CHECKBOX( ID_CHECKBOX_SHOW_CLICKWIN, WConfiguration::OnCheckboxShowClickwinClick )

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

#if defined(__WXGTK__)
    EVT_CHECKBOX( ID_CHECKBOX_STARTUP, WConfiguration::OnCheckboxStartupClick )
#endif

    EVT_CHECKBOX( ID_CHECKBOX_ENABLE_AT_STARTUP, WConfiguration::OnCheckboxEnableAtStartupClick )

    EVT_TEXT( ID_TEXTCTRL_ONSCREENKEYBOARDCOMMAND, WConfiguration::OnTextctrlOnscreenkeyboardcommandTextUpdated )

    EVT_BUTTON( ID_BUTTON_ONSCREENKEYBOARDCOMMAND, WConfiguration::OnButtonOnscreenkeyboardcommandClick )

#if defined(__WXGTK__)
    EVT_CHECKBOX( ID_CHECKBOX_ACTIVATION_KEY, WConfiguration::OnCheckboxActivationKeyClick )
#endif

#if defined(__WXGTK__)
    EVT_BUTTON( ID_BUTTON_ACTIVATION_KEY, WConfiguration::OnButtonActivationKeyClick )
#endif

    EVT_CHECKBOX( ID_CHECKBOX_AUTO_LOCATE_FACE, WConfiguration::OnCheckboxAutoLocateFaceClick )

    EVT_CHECKBOX( ID_CHECKBOX_SHOW_LOCATE_FACE_FILTER, WConfiguration::OnCheckboxShowLocateFaceFilterClick )

    EVT_CHOICE( ID_CHOICE_PROFILE, WConfiguration::OnChoiceProfileSelected )

    EVT_BUTTON( ID_BUTTON_ADD_PROFILE, WConfiguration::OnButtonAddProfileClick )

    EVT_BUTTON( ID_BUTTON_DEL_PROFILE, WConfiguration::OnButtonDelProfileClick )

    EVT_BUTTON( ID_BUTTON_CAMERA_SETTINGS, WConfiguration::OnButtonCameraSettingsClick )

    EVT_BUTTON( ID_BUTTON_CHANGE_CAMERA, WConfiguration::OnButtonChangeCameraClick )

    EVT_CHOICE( ID_CHOICE_LANGUAGE, WConfiguration::OnChoiceLanguageSelected )

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
    m_buttonCalibrateMotion = NULL;
    m_chkEnabledWorkspace = NULL;
    m_spin_top_workspace = NULL;
    m_spin_left_workspace = NULL;
    m_spin_right_workspace = NULL;
    m_spin_bottom_workspace = NULL;
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
    m_stDesign = NULL;
    m_choClickWindowDesign = NULL;
    m_stBehaviour = NULL;
    m_choClickWindowBehaviour = NULL;
    m_chkOpenClickWinAtStartup = NULL;
    m_chkShowClickWin = NULL;
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
#if defined(__WXGTK__)
    m_chkStartup = NULL;
#endif
    m_chkEnabledAtStartup = NULL;
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
    m_chkAutoLocateFace = NULL;
    m_chkShowAutoLocateFaceFilter = NULL;
    m_choProfile = NULL;
    m_btnAddProfile = NULL;
    m_btnDeleteProfile = NULL;
    m_txtSelectedCamera = NULL;
    m_btnCameraSettings = NULL;
    m_choLanguage = NULL;
    m_btnCancel = NULL;
////@end WConfiguration member initialisation
    m_cmbLeft = NULL;
    m_cmbRight = NULL;
    m_cmbTop = NULL;
    m_cmbBottom = NULL;


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

    wxStaticBox* itemStaticBoxSizer7Static = new wxStaticBox(itemPanel5, wxID_ANY, _("Motion calibration"));
    wxStaticBoxSizer* itemStaticBoxSizer7 = new wxStaticBoxSizer(itemStaticBoxSizer7Static, wxVERTICAL);
    itemBoxSizer6->Add(itemStaticBoxSizer7, 0, wxGROW|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer8 = new wxFlexGridSizer(0, 4, 0, 0);
    itemFlexGridSizer8->AddGrowableCol(0);
    itemFlexGridSizer8->AddGrowableCol(1);
    itemFlexGridSizer8->AddGrowableCol(2);
    itemFlexGridSizer8->AddGrowableCol(3);
    itemStaticBoxSizer7->Add(itemFlexGridSizer8, 0, wxGROW, 5);
    wxStaticText* itemStaticText9 = new wxStaticText( itemPanel5, wxID_STATIC, _("X axis speed"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer8->Add(itemStaticText9, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_spinXSpeed = new wxSpinCtrl( itemPanel5, ID_SPINCTRL_XSPEED, _T("0"), wxDefaultPosition, wxSize(45, 25), wxSP_ARROW_KEYS, 0, 30, 0 );
    if (WConfiguration::ShowToolTips())
        m_spinXSpeed->SetToolTip(_("Sets response sensitivity for the X axis."));
    itemFlexGridSizer8->Add(m_spinXSpeed, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText11 = new wxStaticText( itemPanel5, wxID_STATIC, _("Y axis speed"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer8->Add(itemStaticText11, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_spinYSpeed = new wxSpinCtrl( itemPanel5, ID_SPINCTRL_YSPEED, _T("0"), wxDefaultPosition, wxSize(45, 25), wxSP_ARROW_KEYS, 0, 30, 0 );
    if (WConfiguration::ShowToolTips())
        m_spinYSpeed->SetToolTip(_("Sets response sensitivity for the Y axis."));
    itemFlexGridSizer8->Add(m_spinYSpeed, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText13 = new wxStaticText( itemPanel5, wxID_STATIC, _("Acceleration"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer8->Add(itemStaticText13, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_spinAcceleration = new wxSpinCtrl( itemPanel5, ID_SPINCTRL_ACCELERATION, _T("0"), wxDefaultPosition, wxSize(45, 25), wxSP_ARROW_KEYS, 0, 5, 0 );
    if (WConfiguration::ShowToolTips())
        m_spinAcceleration->SetToolTip(_("Sets pointer's acceleration.\n0 means no acceleration and\n5 maximum acceleration."));
    itemFlexGridSizer8->Add(m_spinAcceleration, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText15 = new wxStaticText( itemPanel5, wxID_STATIC, _("Smoothness"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer8->Add(itemStaticText15, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_spinSmoothness = new wxSpinCtrl( itemPanel5, ID_SPINCTRL_SMOOTHNESS, _T("0"), wxDefaultPosition, wxSize(45, 25), wxSP_ARROW_KEYS, 0, 9, 0 );
    if (WConfiguration::ShowToolTips())
        m_spinSmoothness->SetToolTip(_("Tremor reduction filter.\n0 means no filter and \n9 maximum filtering."));
    itemFlexGridSizer8->Add(m_spinSmoothness, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText17 = new wxStaticText( itemPanel5, wxID_STATIC, _("Motion threshold"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer8->Add(itemStaticText17, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_spinEasyStop = new wxSpinCtrl( itemPanel5, ID_SPINCTRL_EASYSTOP, _T("0"), wxDefaultPosition, wxSize(45, 25), wxSP_ARROW_KEYS, 0, 10, 0 );
    if (WConfiguration::ShowToolTips())
        m_spinEasyStop->SetToolTip(_("Minimum displacement (in pixels)\nto start moving pointer."));
    itemFlexGridSizer8->Add(m_spinEasyStop, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemFlexGridSizer8->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemFlexGridSizer8->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_buttonCalibrateMotion = new wxButton( itemPanel5, ID_BUTTON, _("Assisted calibration"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer7->Add(m_buttonCalibrateMotion, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    itemNotebook4->AddPage(itemPanel5, _("Motion"));

    wxPanel* itemPanel22 = new wxPanel( itemNotebook4, ID_PANEL_WORKSPACE, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer23 = new wxBoxSizer(wxVERTICAL);
    itemPanel22->SetSizer(itemBoxSizer23);

    wxStaticBox* itemStaticBoxSizer24Static = new wxStaticBox(itemPanel22, wxID_ANY, _("Workspace limit"));
    wxStaticBoxSizer* itemStaticBoxSizer24 = new wxStaticBoxSizer(itemStaticBoxSizer24Static, wxVERTICAL);
    itemBoxSizer23->Add(itemStaticBoxSizer24, 0, wxGROW|wxALL, 5);
    m_chkEnabledWorkspace = new wxCheckBox( itemPanel22, ID_CHECKBOX_WORKSPACE_LIMIT, _("Enable workspace limit"), wxDefaultPosition, wxDefaultSize, 0 );
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
    wxStaticText* itemStaticText29 = new wxStaticText( itemPanel22, wxID_STATIC, _("Top"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridSizer28->Add(itemStaticText29, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_spin_top_workspace = new wxSpinCtrl( itemPanel22, ID_SPINCTRL_TOP_WORKSPACE, _T("1"), wxDefaultPosition, wxSize(45, 25), wxSP_ARROW_KEYS, 0, 50, 1 );
    if (WConfiguration::ShowToolTips())
        m_spin_top_workspace->SetToolTip(_("Top limit workspace."));
    m_spin_top_workspace->Enable(false);
    itemGridSizer28->Add(m_spin_top_workspace, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM, 5);

    wxStaticText* itemStaticText31 = new wxStaticText( itemPanel22, wxID_STATIC, _("%"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridSizer28->Add(itemStaticText31, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

    wxGridSizer* itemGridSizer32 = new wxGridSizer(0, 6, 0, 0);
    itemBoxSizer27->Add(itemGridSizer32, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticText* itemStaticText33 = new wxStaticText( itemPanel22, wxID_STATIC, _("Left"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridSizer32->Add(itemStaticText33, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_spin_left_workspace = new wxSpinCtrl( itemPanel22, ID_SPINCTRL_LEFT_WORKSPACE, _T("1"), wxDefaultPosition, wxSize(45, 25), wxSP_ARROW_KEYS, 0, 50, 1 );
    if (WConfiguration::ShowToolTips())
        m_spin_left_workspace->SetToolTip(_("Left limit workspace."));
    m_spin_left_workspace->Enable(false);
    itemGridSizer32->Add(m_spin_left_workspace, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM, 5);

    wxStaticText* itemStaticText35 = new wxStaticText( itemPanel22, wxID_STATIC, _("%"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridSizer32->Add(itemStaticText35, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

    wxStaticText* itemStaticText36 = new wxStaticText( itemPanel22, wxID_STATIC, _("Right"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridSizer32->Add(itemStaticText36, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_spin_right_workspace = new wxSpinCtrl( itemPanel22, ID_SPINCTRL_RIGHT_WORKSPACE, _T("1"), wxDefaultPosition, wxSize(45, 25), wxSP_ARROW_KEYS, 0, 50, 1 );
    if (WConfiguration::ShowToolTips())
        m_spin_right_workspace->SetToolTip(_("Right limit workspace."));
    m_spin_right_workspace->Enable(false);
    itemGridSizer32->Add(m_spin_right_workspace, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM, 5);

    wxStaticText* itemStaticText38 = new wxStaticText( itemPanel22, wxID_STATIC, _("%"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridSizer32->Add(itemStaticText38, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxGridSizer* itemGridSizer39 = new wxGridSizer(0, 3, 0, 0);
    itemBoxSizer27->Add(itemGridSizer39, 0, wxALIGN_CENTER_HORIZONTAL, 5);
    wxStaticText* itemStaticText40 = new wxStaticText( itemPanel22, wxID_STATIC, _("Bottom"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridSizer39->Add(itemStaticText40, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, 5);

    m_spin_bottom_workspace = new wxSpinCtrl( itemPanel22, ID_SPINCTRL_BOTTOM_WORKSPACE, _T("1"), wxDefaultPosition, wxSize(45, 25), wxSP_ARROW_KEYS, 0, 50, 1 );
    if (WConfiguration::ShowToolTips())
        m_spin_bottom_workspace->SetToolTip(_("Bottom limit workspace."));
    m_spin_bottom_workspace->Enable(false);
    itemGridSizer39->Add(m_spin_bottom_workspace, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM, 5);

    wxStaticText* itemStaticText42 = new wxStaticText( itemPanel22, wxID_STATIC, _("%"), wxDefaultPosition, wxDefaultSize, 0 );
    itemGridSizer39->Add(itemStaticText42, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM, 5);

    itemNotebook4->AddPage(itemPanel22, _("Workspace"));

    m_panelClick = new wxPanel( itemNotebook4, ID_PANEL_CLICK, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer44 = new wxBoxSizer(wxVERTICAL);
    m_panelClick->SetSizer(itemBoxSizer44);

    wxStaticBox* itemStaticBoxSizer45Static = new wxStaticBox(m_panelClick, wxID_ANY, _("Dwell click"));
    wxStaticBoxSizer* itemStaticBoxSizer45 = new wxStaticBoxSizer(itemStaticBoxSizer45Static, wxVERTICAL);
    itemBoxSizer44->Add(itemStaticBoxSizer45, 0, wxGROW|wxALL, 5);
    m_chkDwellClickEnabled = new wxCheckBox( m_panelClick, ID_CHECKBOX_ENABLE_DWELL, _("Enable dwell click"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkDwellClickEnabled->SetValue(false);
    if (WConfiguration::ShowToolTips())
        m_chkDwellClickEnabled->SetToolTip(_("Enable/Disable automatic (dwell)\nclick generation mechanism."));
    itemStaticBoxSizer45->Add(m_chkDwellClickEnabled, 0, wxALIGN_LEFT|wxALL, 5);

    wxGridSizer* itemGridSizer47 = new wxGridSizer(0, 2, 0, 0);
    itemStaticBoxSizer45->Add(itemGridSizer47, 0, wxGROW|wxLEFT|wxRIGHT, 5);
    wxFlexGridSizer* itemFlexGridSizer48 = new wxFlexGridSizer(0, 2, 0, 0);
    itemGridSizer47->Add(itemFlexGridSizer48, 0, wxGROW|wxALIGN_TOP|wxALL, 0);
    m_stDwellTime = new wxStaticText( m_panelClick, ID_STATIC_DWELL_TIME, _("Dwell time (ds)"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer48->Add(m_stDwellTime, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_spinDwellTime = new wxSpinCtrl( m_panelClick, ID_SPINCTRL_DWELL_TIME, _T("2"), wxDefaultPosition, wxSize(45, 25), wxSP_ARROW_KEYS, 2, 50, 2 );
    if (WConfiguration::ShowToolTips())
        m_spinDwellTime->SetToolTip(_("Time to wait (deciseconds) \nbefore sending a click."));
    itemFlexGridSizer48->Add(m_spinDwellTime, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    m_stDwellArea = new wxStaticText( m_panelClick, ID_STATIC_DWELL_AREA, _("Dwell area"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer48->Add(m_stDwellArea, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_spinDwellArea = new wxSpinCtrl( m_panelClick, ID_SPINCTRL_DWELL_AREA, _T("0"), wxDefaultPosition, wxSize(45, 25), wxSP_ARROW_KEYS, 0, 5, 0 );
    if (WConfiguration::ShowToolTips())
        m_spinDwellArea->SetToolTip(_("Maximum allowed displacement\nbefore restarting the dwell time\ncountdown."));
    itemFlexGridSizer48->Add(m_spinDwellArea, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer53 = new wxBoxSizer(wxVERTICAL);
    itemGridSizer47->Add(itemBoxSizer53, 0, wxGROW|wxALIGN_TOP|wxLEFT|wxRIGHT, 5);
    m_chkAllowConsecutiveClick = new wxCheckBox( m_panelClick, ID_CHECKBOX_ALLOW_CONSECUTIVE, _("Allow consecutive clicks"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkAllowConsecutiveClick->SetValue(false);
    if (WConfiguration::ShowToolTips())
        m_chkAllowConsecutiveClick->SetToolTip(_("Allows to send multiple clicks\nwhen the pointer is stopped."));
    itemBoxSizer53->Add(m_chkAllowConsecutiveClick, 0, wxALIGN_LEFT|wxALL, 5);

    m_chkBeepOnClick = new wxCheckBox( m_panelClick, ID_CHECKBOX_BEEP_ON_CLICK, _("Beep on click"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkBeepOnClick->SetValue(false);
    if (WConfiguration::ShowToolTips())
        m_chkBeepOnClick->SetToolTip(_("Play sound when click generated."));
    itemBoxSizer53->Add(m_chkBeepOnClick, 0, wxALIGN_LEFT|wxALL, 5);

#if defined(__WXGTK__)
    m_chkAllowVisualAlerts = new wxCheckBox( m_panelClick, ID_CHECKBOX_ALLOW_VISUAL_ALERTS, _("Allow visual alerts"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkAllowVisualAlerts->SetValue(false);
    itemBoxSizer53->Add(m_chkAllowVisualAlerts, 0, wxALIGN_LEFT|wxALL, 5);
#endif

    m_sboxClickWin = new wxStaticBox(m_panelClick, ID_STATICBOX_CLICK_WIN, _("Click window"));
    wxStaticBoxSizer* itemStaticBoxSizer57 = new wxStaticBoxSizer(m_sboxClickWin, wxVERTICAL);
    itemStaticBoxSizer45->Add(itemStaticBoxSizer57, 0, wxGROW|wxALL, 5);
    wxGridSizer* itemGridSizer58 = new wxGridSizer(0, 2, 0, 0);
    itemStaticBoxSizer57->Add(itemGridSizer58, 0, wxGROW, 5);
    wxFlexGridSizer* itemFlexGridSizer59 = new wxFlexGridSizer(2, 2, 0, 0);
    itemGridSizer58->Add(itemFlexGridSizer59, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 0);
    m_stDesign = new wxStaticText( m_panelClick, ID_STATIC_DESIGN, _("Design:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer59->Add(m_stDesign, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxArrayString m_choClickWindowDesignStrings;
    m_choClickWindowDesignStrings.Add(_("Normal"));
    m_choClickWindowDesignStrings.Add(_("Thin"));
    m_choClickWindowDesign = new wxChoice( m_panelClick, ID_CHOICE_DESIGN, wxDefaultPosition, wxDefaultSize, m_choClickWindowDesignStrings, 0 );
    itemFlexGridSizer59->Add(m_choClickWindowDesign, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_stBehaviour = new wxStaticText( m_panelClick, ID_STATIC_BEHAVIOUR, _("Behaviour:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer59->Add(m_stBehaviour, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxArrayString m_choClickWindowBehaviourStrings;
    m_choClickWindowBehaviourStrings.Add(_("Normal mode"));
    m_choClickWindowBehaviourStrings.Add(_("Fast mode"));
    m_choClickWindowBehaviour = new wxChoice( m_panelClick, ID_CHOICE_BEHAVIOUR, wxDefaultPosition, wxDefaultSize, m_choClickWindowBehaviourStrings, 0 );
    if (WConfiguration::ShowToolTips())
        m_choClickWindowBehaviour->SetToolTip(_("Fast mode enables click type\nselection by hovering the mouse\npointer over the click window\nbuttons."));
    itemFlexGridSizer59->Add(m_choClickWindowBehaviour, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxBoxSizer* itemBoxSizer64 = new wxBoxSizer(wxVERTICAL);
    itemGridSizer58->Add(itemBoxSizer64, 0, wxGROW|wxALIGN_TOP|wxALL, 5);
    m_chkOpenClickWinAtStartup = new wxCheckBox( m_panelClick, ID_CHECKBOX_CLICKWIN_AT_STARTUP, _("Open Click Window at startup"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkOpenClickWinAtStartup->SetValue(false);
    if (WConfiguration::ShowToolTips())
        m_chkOpenClickWinAtStartup->SetToolTip(_("If checked the Click Window is automatically\nopened at program startup."));
    itemBoxSizer64->Add(m_chkOpenClickWinAtStartup, 0, wxALIGN_LEFT|wxALL, 5);

    m_chkShowClickWin = new wxCheckBox( m_panelClick, ID_CHECKBOX_SHOW_CLICKWIN, _("Show Click Window"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkShowClickWin->SetValue(false);
    itemBoxSizer64->Add(m_chkShowClickWin, 0, wxALIGN_LEFT|wxALL, 5);

#if defined(__WXGTK__)
    m_sboxGestureClick = new wxStaticBox(m_panelClick, ID_STATICBOX_GESTURE_CLICK, _("Gesture click"));
    wxStaticBoxSizer* itemStaticBoxSizer67 = new wxStaticBoxSizer(m_sboxGestureClick, wxVERTICAL);
    itemStaticBoxSizer45->Add(itemStaticBoxSizer67, 0, wxGROW|wxALL, 5);
    m_chkEnableGestureClick = new wxCheckBox( m_panelClick, ID_CHECKBOX, _("Enable gesture click"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkEnableGestureClick->SetValue(false);
    itemStaticBoxSizer67->Add(m_chkEnableGestureClick, 0, wxALIGN_LEFT|wxALL, 5);

    wxFlexGridSizer* itemFlexGridSizer69 = new wxFlexGridSizer(0, 4, 0, 0);
    itemFlexGridSizer69->AddGrowableCol(1);
    itemFlexGridSizer69->AddGrowableCol(3);
    itemStaticBoxSizer67->Add(itemFlexGridSizer69, 0, wxGROW, 5);
    m_stMoveLeft = new wxStaticText( m_panelClick, ID_STATIC_MOVE_LEFT, _("Move left:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer69->Add(m_stMoveLeft, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString m_choLeftStrings;
    m_choLeftStrings.Add(_("Disable"));
    m_choLeftStrings.Add(_("Left click"));
    m_choLeftStrings.Add(_("Right click"));
    m_choLeftStrings.Add(_("Double click"));
    m_choLeftStrings.Add(_("Drag click"));
    m_choLeft = new wxChoice( m_panelClick, ID_CHOICE, wxDefaultPosition, wxDefaultSize, m_choLeftStrings, 0 );
    itemFlexGridSizer69->Add(m_choLeft, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_stMoveRight = new wxStaticText( m_panelClick, ID_STATIC_MOVE_RIGHT, _("Move right:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer69->Add(m_stMoveRight, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString m_choRightStrings;
    m_choRightStrings.Add(_("Disable"));
    m_choRightStrings.Add(_("Left click"));
    m_choRightStrings.Add(_("Right click"));
    m_choRightStrings.Add(_("Double click"));
    m_choRightStrings.Add(_("Drag click"));
    m_choRight = new wxChoice( m_panelClick, ID_CHOICE1, wxDefaultPosition, wxDefaultSize, m_choRightStrings, 0 );
    itemFlexGridSizer69->Add(m_choRight, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_stMoveUp = new wxStaticText( m_panelClick, ID_STATIC_MOVE_UP, _("Move up:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer69->Add(m_stMoveUp, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString m_choUpStrings;
    m_choUpStrings.Add(_("Disable"));
    m_choUpStrings.Add(_("Left click"));
    m_choUpStrings.Add(_("Right click"));
    m_choUpStrings.Add(_("Double click"));
    m_choUpStrings.Add(_("Drag click"));
    m_choUp = new wxChoice( m_panelClick, ID_CHOICE2, wxDefaultPosition, wxDefaultSize, m_choUpStrings, 0 );
    itemFlexGridSizer69->Add(m_choUp, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_stMoveDown = new wxStaticText( m_panelClick, ID_STATIC_MOVE_DOWN, _("Move down:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer69->Add(m_stMoveDown, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString m_choDownStrings;
    m_choDownStrings.Add(_("Disable"));
    m_choDownStrings.Add(_("Left click"));
    m_choDownStrings.Add(_("Right click"));
    m_choDownStrings.Add(_("Double click"));
    m_choDownStrings.Add(_("Drag click"));
    m_choDown = new wxChoice( m_panelClick, ID_CHOICE3, wxDefaultPosition, wxDefaultSize, m_choDownStrings, 0 );
    itemFlexGridSizer69->Add(m_choDown, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

#endif

    itemNotebook4->AddPage(m_panelClick, _("Click"));

    wxPanel* itemPanel78 = new wxPanel( itemNotebook4, ID_PANEL_ADVANCED, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer79 = new wxBoxSizer(wxVERTICAL);
    itemPanel78->SetSizer(itemBoxSizer79);

    wxStaticBox* itemStaticBoxSizer80Static = new wxStaticBox(itemPanel78, wxID_ANY, _("Startup"));
    wxStaticBoxSizer* itemStaticBoxSizer80 = new wxStaticBoxSizer(itemStaticBoxSizer80Static, wxVERTICAL);
    itemBoxSizer79->Add(itemStaticBoxSizer80, 0, wxGROW|wxALL, 5);
#if defined(__WXGTK__)
    m_chkStartup = new wxCheckBox( itemPanel78, ID_CHECKBOX_STARTUP, _("Start eViacam at user logon"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkStartup->SetValue(false);
    itemStaticBoxSizer80->Add(m_chkStartup, 0, wxALIGN_LEFT|wxALL, 5);
#endif

    m_chkEnabledAtStartup = new wxCheckBox( itemPanel78, ID_CHECKBOX_ENABLE_AT_STARTUP, _("Enable eViacam at startup"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkEnabledAtStartup->SetValue(false);
    if (WConfiguration::ShowToolTips())
        m_chkEnabledAtStartup->SetToolTip(_("If checked the program automatically \nenables cursor control at startup."));
    itemStaticBoxSizer80->Add(m_chkEnabledAtStartup, 0, wxALIGN_LEFT|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer83Static = new wxStaticBox(itemPanel78, wxID_ANY, _("On-screen keyboard command"));
    wxStaticBoxSizer* itemStaticBoxSizer83 = new wxStaticBoxSizer(itemStaticBoxSizer83Static, wxVERTICAL);
    itemBoxSizer79->Add(itemStaticBoxSizer83, 0, wxGROW|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer84 = new wxFlexGridSizer(0, 2, 0, 0);
    itemFlexGridSizer84->AddGrowableCol(0);
    itemStaticBoxSizer83->Add(itemFlexGridSizer84, 0, wxGROW, 5);
    m_txtOnScreenKeyboardCommand = new wxTextCtrl( itemPanel78, ID_TEXTCTRL_ONSCREENKEYBOARDCOMMAND, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    if (WConfiguration::ShowToolTips())
        m_txtOnScreenKeyboardCommand->SetToolTip(_("Command to run an external on-screen keyboard application."));
    itemFlexGridSizer84->Add(m_txtOnScreenKeyboardCommand, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_btntOnScreenKeyboardCommand = new wxButton( itemPanel78, ID_BUTTON_ONSCREENKEYBOARDCOMMAND, _("Browse..."), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer84->Add(m_btntOnScreenKeyboardCommand, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

#if defined(__WXGTK__)
    wxStaticBox* itemStaticBoxSizer87Static = new wxStaticBox(itemPanel78, wxID_ANY, _("Activation key"));
    wxStaticBoxSizer* itemStaticBoxSizer87 = new wxStaticBoxSizer(itemStaticBoxSizer87Static, wxVERTICAL);
    itemBoxSizer79->Add(itemStaticBoxSizer87, 0, wxGROW|wxALL, 5);
#if defined(__WXGTK__)
    m_chkActivationKey = new wxCheckBox( itemPanel78, ID_CHECKBOX_ACTIVATION_KEY, _("Use activation key"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkActivationKey->SetValue(false);
    if (WConfiguration::ShowToolTips())
        m_chkActivationKey->SetToolTip(_("When checked it allows to enable or \ndisable eViacam by pressing a key."));
    itemStaticBoxSizer87->Add(m_chkActivationKey, 0, wxALIGN_LEFT|wxALL, 5);
#endif

    wxFlexGridSizer* itemFlexGridSizer89 = new wxFlexGridSizer(0, 2, 0, 0);
    itemFlexGridSizer89->AddGrowableCol(0);
    itemStaticBoxSizer87->Add(itemFlexGridSizer89, 0, wxGROW, 5);
#if defined(__WXGTK__)
    m_txtActivationKey = new wxTextCtrl( itemPanel78, ID_TEXTCTRL_ACTIVATION_KEY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    if (WConfiguration::ShowToolTips())
        m_txtActivationKey->SetToolTip(_("Shows the selected key."));
    m_txtActivationKey->Enable(false);
    itemFlexGridSizer89->Add(m_txtActivationKey, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
#endif

#if defined(__WXGTK__)
    m_buttonActivationKey = new wxButton( itemPanel78, ID_BUTTON_ACTIVATION_KEY, _("Set key..."), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer89->Add(m_buttonActivationKey, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
#endif

#endif

    wxStaticBox* itemStaticBoxSizer92Static = new wxStaticBox(itemPanel78, wxID_ANY, _("Face localization"));
    wxStaticBoxSizer* itemStaticBoxSizer92 = new wxStaticBoxSizer(itemStaticBoxSizer92Static, wxVERTICAL);
    itemBoxSizer79->Add(itemStaticBoxSizer92, 0, wxGROW|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer93 = new wxFlexGridSizer(0, 2, 0, 0);
    itemFlexGridSizer93->AddGrowableCol(0);
    itemFlexGridSizer93->AddGrowableCol(1);
    itemStaticBoxSizer92->Add(itemFlexGridSizer93, 0, wxGROW, 5);
    m_chkAutoLocateFace = new wxCheckBox( itemPanel78, ID_CHECKBOX_AUTO_LOCATE_FACE, _("Locate face automatically"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkAutoLocateFace->SetValue(false);
    if (WConfiguration::ShowToolTips())
        m_chkAutoLocateFace->SetToolTip(_("When enabled tries to automatically\ndetect your face and center the\ntracking area."));
    itemFlexGridSizer93->Add(m_chkAutoLocateFace, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_chkShowAutoLocateFaceFilter = new wxCheckBox( itemPanel78, ID_CHECKBOX_SHOW_LOCATE_FACE_FILTER, _("Show locate face filter"), wxDefaultPosition, wxDefaultSize, 0 );
    m_chkShowAutoLocateFaceFilter->SetValue(false);
    m_chkShowAutoLocateFaceFilter->Enable(false);
    itemFlexGridSizer93->Add(m_chkShowAutoLocateFaceFilter, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemNotebook4->AddPage(itemPanel78, _("Advanced"));

    itemListbook3->AddPage(itemNotebook4, _("Profile options"), false, 0);

    wxPanel* itemPanel96 = new wxPanel( itemListbook3, ID_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer97 = new wxBoxSizer(wxVERTICAL);
    itemPanel96->SetSizer(itemBoxSizer97);

    wxStaticBox* itemStaticBoxSizer98Static = new wxStaticBox(itemPanel96, wxID_ANY, _("Profile management"));
    wxStaticBoxSizer* itemStaticBoxSizer98 = new wxStaticBoxSizer(itemStaticBoxSizer98Static, wxVERTICAL);
    itemBoxSizer97->Add(itemStaticBoxSizer98, 0, wxGROW|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer99 = new wxFlexGridSizer(0, 4, 0, 0);
    itemFlexGridSizer99->AddGrowableCol(1);
    itemStaticBoxSizer98->Add(itemFlexGridSizer99, 0, wxGROW, 5);
    wxStaticText* itemStaticText100 = new wxStaticText( itemPanel96, wxID_STATIC, _("Select profile:"), wxDefaultPosition, wxDefaultSize, 0 );
    if (WConfiguration::ShowToolTips())
        itemStaticText100->SetToolTip(_("Chooses desired active profile"));
    itemFlexGridSizer99->Add(itemStaticText100, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString m_choProfileStrings;
    m_choProfile = new wxChoice( itemPanel96, ID_CHOICE_PROFILE, wxDefaultPosition, wxDefaultSize, m_choProfileStrings, 0 );
    if (WConfiguration::ShowToolTips())
        m_choProfile->SetToolTip(_("Chooses desired active profile"));
    itemFlexGridSizer99->Add(m_choProfile, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_btnAddProfile = new wxButton( itemPanel96, ID_BUTTON_ADD_PROFILE, _("Add profile"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer99->Add(m_btnAddProfile, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_btnDeleteProfile = new wxButton( itemPanel96, ID_BUTTON_DEL_PROFILE, _("Delete profile"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer99->Add(m_btnDeleteProfile, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer104Static = new wxStaticBox(itemPanel96, wxID_ANY, _("Camera"));
    wxStaticBoxSizer* itemStaticBoxSizer104 = new wxStaticBoxSizer(itemStaticBoxSizer104Static, wxVERTICAL);
    itemBoxSizer97->Add(itemStaticBoxSizer104, 0, wxGROW|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer105 = new wxFlexGridSizer(0, 3, 0, 0);
    itemFlexGridSizer105->AddGrowableCol(0);
    itemStaticBoxSizer104->Add(itemFlexGridSizer105, 0, wxGROW, 5);
    m_txtSelectedCamera = new wxTextCtrl( itemPanel96, ID_TEXTCTRL_SELECTED_CAMERA, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
    m_txtSelectedCamera->Enable(false);
    itemFlexGridSizer105->Add(m_txtSelectedCamera, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_btnCameraSettings = new wxButton( itemPanel96, ID_BUTTON_CAMERA_SETTINGS, _("Settings..."), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer105->Add(m_btnCameraSettings, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton108 = new wxButton( itemPanel96, ID_BUTTON_CHANGE_CAMERA, _("Change"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer105->Add(itemButton108, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer109Static = new wxStaticBox(itemPanel96, wxID_ANY, _("Language"));
    wxStaticBoxSizer* itemStaticBoxSizer109 = new wxStaticBoxSizer(itemStaticBoxSizer109Static, wxVERTICAL);
    itemBoxSizer97->Add(itemStaticBoxSizer109, 0, wxGROW|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer110 = new wxFlexGridSizer(0, 2, 0, 0);
    itemStaticBoxSizer109->Add(itemFlexGridSizer110, 0, wxALIGN_LEFT, 5);
    wxStaticText* itemStaticText111 = new wxStaticText( itemPanel96, wxID_STATIC, _("Language:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer110->Add(itemStaticText111, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString m_choLanguageStrings;
    m_choLanguage = new wxChoice( itemPanel96, ID_CHOICE_LANGUAGE, wxDefaultPosition, wxDefaultSize, m_choLanguageStrings, 0 );
    itemFlexGridSizer110->Add(m_choLanguage, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemListbook3->AddPage(itemPanel96, _("General options"), false, 1);

    itemBoxSizer2->Add(itemListbook3, 0, wxALIGN_LEFT|wxALL, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer113 = new wxStdDialogButtonSizer;

    itemBoxSizer2->Add(itemStdDialogButtonSizer113, 0, wxALIGN_RIGHT|wxALL, 5);
    wxButton* itemButton114 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer113->AddButton(itemButton114);

    m_btnCancel = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    m_btnCancel->Enable(false);
    itemStdDialogButtonSizer113->AddButton(m_btnCancel);

    itemStdDialogButtonSizer113->Realize();

////@end WConfiguration content construction
#if defined(__WXGTK__)
	for (unsigned int i=0; i<m_pViacamController->GetMouseOutput()->GetKeyEventsCount(); i++) {
		m_choLeft->Append(_("Key:") + wxString(m_pViacamController->GetMouseOutput()->GetKeyboardCode(i).GetName(), wxConvLocal));
		m_choRight->Append(_("Key:") + wxString(m_pViacamController->GetMouseOutput()->GetKeyboardCode(i).GetName(), wxConvLocal));
		m_choUp->Append(_("Key:") + wxString(m_pViacamController->GetMouseOutput()->GetKeyboardCode(i).GetName(), wxConvLocal));
		m_choDown->Append(_("Key:") + wxString(m_pViacamController->GetMouseOutput()->GetKeyboardCode(i).GetName(), wxConvLocal));
	}
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
	m_chkDwellClickEnabled->SetValue (m_pViacamController->GetMouseOutput()->GetClickMode()!= CMouseOutput::NONE);
#if defined(__WXGTK__)
	m_chkEnableGestureClick->SetValue (m_pViacamController->GetMouseOutput()->GetClickMode()== CMouseOutput::GESTURE);
#endif
	m_chkAllowConsecutiveClick->SetValue (m_pViacamController->GetMouseOutput()->GetConsecutiveClicksAllowed());
	m_chkBeepOnClick->SetValue (m_pViacamController->GetMouseOutput()->GetBeepOnClick());
	//m_chkShowClickCountdown->SetValue (m_pViacamController->GetMouseOutput()->GetShowClickCountdown());
	m_spinDwellTime->SetValue (m_pViacamController->GetMouseOutput()->GetDwellTime());
	m_spinDwellArea->SetValue ( m_pViacamController->GetMouseOutput()->GetDwellToleranceArea () );
	m_chkShowClickWin->SetValue ( m_pViacamController->GetClickWindowController()->IsShown() );
	m_choClickWindowBehaviour->Select (m_pViacamController->GetClickWindowController()->GetFastMode() ? 1 : 0);
	m_choClickWindowDesign->Select (m_pViacamController->GetClickWindowController()->GetDesign());
#if defined(__WXGTK__)
	m_choLeft->Select (m_pViacamController->GetMouseOutput()->GetActionLeft());
	m_choRight->Select (m_pViacamController->GetMouseOutput()->GetActionRight());
	m_choUp->Select (m_pViacamController->GetMouseOutput()->GetActionTop());
	m_choDown->Select (m_pViacamController->GetMouseOutput()->GetActionBottom());
	m_chkAllowVisualAlerts->SetValue (m_pViacamController->GetMouseOutput()->GetVisualAlerts());
#endif
	UpdateGUIClickOptions();

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
	m_txtActivationKey->SetValue(wxString(m_pViacamController->GetActivationKey().GetName(), wxConvLocal));
	    
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
	m_chkOpenClickWinAtStartup->Enable(enable);
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
#endif
}

void WConfiguration::EnableGUIClickWindowOptions(bool enable)
{
	m_chkOpenClickWinAtStartup->Enable(enable);
	m_chkShowClickWin->Enable(enable);
	m_choClickWindowDesign->Enable(enable);
	m_choClickWindowBehaviour->Enable(enable);
	m_stDesign->Enable(enable);
	m_stBehaviour->Enable(enable);
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
	if (!m_chkDwellClickEnabled->GetValue()) {
		if (m_pViacamController->GetMouseOutput()->SetClickMode(CMouseOutput::NONE, false, this)) {
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
			if (m_pViacamController->GetMouseOutput()->SetClickMode(CMouseOutput::GESTURE, false, this)) {
				EnableGUIGeneralClickOptions(true);
				EnableGUIGestureOptions(true);
				EnableGUIClickWindowOptions(false);
				Changed ();
			}
		}
		else
#endif
			if (m_pViacamController->GetMouseOutput()->SetClickMode(CMouseOutput::DWELL, false, this)) {
				EnableGUIGeneralClickOptions(true);
				EnableGUIGestureOptions(false);
				EnableGUIClickWindowOptions(true);
				Changed ();
			}
	}		
	m_chkDwellClickEnabled->SetValue (m_pViacamController->GetMouseOutput()->GetClickMode()!= CMouseOutput::NONE);
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
		if (m_pViacamController->GetMouseOutput()->SetClickMode(CMouseOutput::GESTURE, false, this)) {
			EnableGUIGestureOptions(true);
			EnableGUIClickWindowOptions(false);
		}
		else {
			m_chkEnableGestureClick->SetValue(false);
		}
	}
	else {
		if (m_pViacamController->GetMouseOutput()->SetClickMode(CMouseOutput::DWELL, false, this)) {
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
	m_pViacamController->GetMouseOutput()->SetConsecutiveClicksAllowed (m_chkAllowConsecutiveClick->GetValue());
	Changed ();
	event.Skip(false);	
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
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE_DESIGN
 */

void WConfiguration::OnChoiceDesignSelected( wxCommandEvent& event )
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

#if defined(__WXGTK__)
/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_ACTIVATION_KEY
 */

void WConfiguration::OnButtonActivationKeyClick( wxCommandEvent& event )
{
	bool isEnabled = m_pViacamController->GetEnabled();
	WGetKey dlgGetKey(this);

	if (dlgGetKey.ShowModal()== wxID_YES) {
		CKeyboardCode kc= dlgGetKey.GetKeyCode();
		m_txtActivationKey->SetValue(wxString(kc.GetName(), wxConvLocal));
		m_pViacamController->SetActivationKeyCode(kc);
		Changed ();
	}
	m_pViacamController->SetEnabled(isEnabled);
		
	event.Skip(false);    
}
#endif

#if defined(__WXGTK__)
/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_ACTIVATION_KEY
 */

void WConfiguration::OnCheckboxActivationKeyClick( wxCommandEvent& event )
{
    m_pViacamController->SetEnabledActivationKey(m_chkActivationKey->GetValue());
    event.Skip(false);
    Changed ();
}
#endif

#if defined(__WXGTK__)
/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_STARTUP
 */

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
	int action = event.GetInt();
	m_pViacamController->GetMouseOutput()->SetActionLeft (action);
	event.Skip(false);
}


/*!
 * wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_COMBOBOX_RIGHT
 */

void WConfiguration::OnComboboxRightSelected( wxCommandEvent& event )
{
	int action = event.GetInt();
	m_pViacamController->GetMouseOutput()->SetActionRight (action);
	event.Skip(false);
}


/*!
 * wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_COMBOBOX_TOP
 */

void WConfiguration::OnComboboxTopSelected( wxCommandEvent& event )
{
	int action = event.GetInt();
	m_pViacamController->GetMouseOutput()->SetActionTop (action);
	event.Skip(false);
}


/*!
 * wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_COMBOBOX_BOTTOM
 */

void WConfiguration::OnComboboxBottomSelected( wxCommandEvent& event )
{
	int action = event.GetInt();
	m_pViacamController->GetMouseOutput()->SetActionBottom (action);
	event.Skip(false);
}

#if defined(__WXGTK__)
/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX_ALLOW_VISUAL_ALERTS
 */

void WConfiguration::OnCheckboxAllowVisualAlertsClick( wxCommandEvent& event )
{
	m_pViacamController->GetMouseOutput()->SetVisualAlerts(m_chkAllowVisualAlerts->IsChecked());
	event.Skip(false);
	event.Skip(false);
	Changed ();
}
#endif

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void WConfiguration::OnOkClick( wxCommandEvent& event )
{
	EndModal(wxID_OK);	
	event.Skip(false);
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL
 */

void WConfiguration::OnCancelClick( wxCommandEvent& event )
{
	wxMessageDialog dlg (NULL, _("Discard changes?"), _("eViacam warning"), wxICON_EXCLAMATION | wxYES_NO );
	if (dlg.ShowModal()== wxID_YES)
	{
		EndModal(wxID_CANCEL);		
	}		
	event.Skip(false);
}



#if defined(__WXGTK__)

/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE
 */

void WConfiguration::OnLeftGestureChoiceSelected( wxCommandEvent& event )
{
	m_pViacamController->GetMouseOutput()->SetActionLeft (event.GetInt());
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
	m_pViacamController->GetMouseOutput()->SetActionRight (event.GetInt());
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
	m_pViacamController->GetMouseOutput()->SetActionTop (event.GetInt());
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
	m_pViacamController->GetMouseOutput()->SetActionBottom (event.GetInt());
	Changed();
	event.Skip(false);
}
#endif

