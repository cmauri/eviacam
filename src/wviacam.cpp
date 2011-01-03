/////////////////////////////////////////////////////////////////////////////
// Name:        wviacam.cpp
// Purpose:  
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by: 
// Created:     
// Copyright:   (C) 2008 Cesar Mauri Loba - CREA Software Systems
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
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif



////@begin includes
#include "wconfiguration.h"
#include "wabout.h"
////@end includes
#include "wviacam.h"
#include "camwindow.h"
#include "viacamcontroller.h"
#include "configmanager.h"
#include "eviacamapp.h"
#include "pointeraction.h"
#include "dwellclick.h"

////@begin XPM images
#include "icons/eviacam_mini.xpm"
#include "icons/on.xpm"
#include "icons/off.xpm"
#include "icons/clickwindow.xpm"
#include "icons/keyboard.xpm"
#include "icons/preferences.xpm"
#include "icons/help.xpm"
////@end XPM images

#include <wx/html/helpctrl.h>
#include <wx/stdpaths.h>

/*!
 * WViacam type definition
 */

IMPLEMENT_CLASS( WViacam, wxFrame )


/*!
 * Custom events to comunicate worker and GUI. These are needed to avoid calling
 * GUI functions from worker thread
 */

// FPS refresh
DECLARE_EVENT_TYPE(wxEVT_SET_FPS, -1)
DEFINE_EVENT_TYPE(wxEVT_SET_FPS)

/*!
 * WViacam event table definition
 */

BEGIN_EVENT_TABLE( WViacam, wxFrame )

////@begin WViacam event table entries
    EVT_CLOSE( WViacam::OnCloseWindow )
    EVT_ICONIZE( WViacam::OnIconize )

    EVT_MENU( ID_MENUITEM_ENABLE, WViacam::OnMenuitemEnableClick )
    EVT_UPDATE_UI( ID_MENUITEM_ENABLE, WViacam::OnMenuitemEnableUpdate )

    EVT_MENU( ID_MENUITEM_EXIT, WViacam::OnMenuitemExitClick )

    EVT_MENU( ID_MENU_WIZARD, WViacam::OnMenuWizardClick )

    EVT_MENU( ID_MENU_OPTIONS, WViacam::OnMenuOptionsClick )

    EVT_MENU( ID_MENU_HELP_CONTENTS, WViacam::OnToolHelpClick )

    EVT_MENU( ID_MENU_ABOUT, WViacam::OnMenuAboutClick )

    EVT_MENU( ID_TOOL_ENABLE, WViacam::OnToolEnableClick )
    EVT_UPDATE_UI( ID_TOOL_ENABLE, WViacam::OnToolEnableUpdate )

    EVT_MENU( ID_TOOL_DISABLE, WViacam::OnToolDisableClick )
    EVT_UPDATE_UI( ID_TOOL_DISABLE, WViacam::OnToolDisableUpdate )

    EVT_MENU( ID_TOOL_CLICKWIN, WViacam::OnToolClickwinClick )
    EVT_UPDATE_UI( ID_TOOL_CLICKWIN, WViacam::OnToolClickwinUpdate )

    EVT_MENU( ID_TOOL_KEYBOARD, WViacam::OnToolKeyboardClick )

    EVT_MENU( ID_TOOL_OPTIONS, WViacam::OnToolOptionsClick )

    EVT_MENU( ID_TOOL_HELP, WViacam::OnToolHelpClick )

////@end WViacam event table entries

	EVT_COMMAND  (wxID_ANY, wxEVT_SET_FPS, WViacam::OnSetFPS)

END_EVENT_TABLE()


/*!
 * WViacam constructors
 */

WViacam::WViacam()
{
    Init();
}

WViacam::WViacam( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create( parent, id, caption, pos, size, style );	
}


/*!
 * WViacam creator
 */
bool WViacam::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin WViacam creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxFrame::Create( parent, id, caption, pos, size, style );

    CreateControls();
    SetIcon(GetIconResource(wxT("icons/eviacam_mini.xpm")));
////@end WViacam creation
	
    return true;
}


/*!
 * WViacam destructor
 */
WViacam::~WViacam()
{
////@begin WViacam destruction
////@end WViacam destruction
	delete m_taskBarIcon;
	delete m_helpController;
}


/*!
 * Member initialisation
 */

void WViacam::Init()
{
////@begin WViacam member initialisation
    m_statusBar = NULL;
////@end WViacam member initialisation
	m_pCamWindow= NULL;
	m_prevFPS= 0;
	m_helpController= NULL;
}


/*!
 * Control creation for WViacam
 */

void WViacam::CreateControls()
{    
////@begin WViacam content construction
    WViacam* itemFrame1 = this;

    wxMenuBar* menuBar = new wxMenuBar;
    wxMenu* itemMenu3 = new wxMenu;
    itemMenu3->Append(ID_MENUITEM_ENABLE, _("E&nable / Disable"), wxEmptyString, wxITEM_CHECK);
    itemMenu3->AppendSeparator();
    itemMenu3->Append(ID_MENUITEM_EXIT, _("&Exit"), wxEmptyString, wxITEM_NORMAL);
    menuBar->Append(itemMenu3, _("&File"));
    wxMenu* itemMenu7 = new wxMenu;
    itemMenu7->Append(ID_MENU_WIZARD, _("&Wizard..."), wxEmptyString, wxITEM_NORMAL);
    itemMenu7->AppendSeparator();
    itemMenu7->Append(ID_MENU_OPTIONS, _("&Options..."), wxEmptyString, wxITEM_NORMAL);
    menuBar->Append(itemMenu7, _("&Configuration"));
    wxMenu* itemMenu11 = new wxMenu;
    itemMenu11->Append(ID_MENU_HELP_CONTENTS, _("&Help contents"), wxEmptyString, wxITEM_NORMAL);
    itemMenu11->AppendSeparator();
    itemMenu11->Append(ID_MENU_ABOUT, _("&About..."), wxEmptyString, wxITEM_NORMAL);
    menuBar->Append(itemMenu11, _("&Help"));
    itemFrame1->SetMenuBar(menuBar);

    wxToolBar* itemToolBar15 = CreateToolBar( wxTB_FLAT|wxTB_HORIZONTAL, ID_TOOLBAR );
    itemToolBar15->SetToolBitmapSize(wxSize(32, 32));
    wxBitmap itemtool16Bitmap(itemFrame1->GetBitmapResource(wxT("icons/on.xpm")));
    wxBitmap itemtool16BitmapDisabled;
    itemToolBar15->AddTool(ID_TOOL_ENABLE, _("Enable"), itemtool16Bitmap, itemtool16BitmapDisabled, wxITEM_NORMAL, _("Enable"), wxEmptyString);
    wxBitmap itemtool17Bitmap(itemFrame1->GetBitmapResource(wxT("icons/off.xpm")));
    wxBitmap itemtool17BitmapDisabled;
    itemToolBar15->AddTool(ID_TOOL_DISABLE, _("Disable"), itemtool17Bitmap, itemtool17BitmapDisabled, wxITEM_NORMAL, _("Disable"), wxEmptyString);
    itemToolBar15->EnableTool(ID_TOOL_DISABLE, false);
    wxBitmap itemtool18Bitmap(itemFrame1->GetBitmapResource(wxT("icons/clickwindow.xpm")));
    wxBitmap itemtool18BitmapDisabled;
    itemToolBar15->AddTool(ID_TOOL_CLICKWIN, _("Click Window"), itemtool18Bitmap, itemtool18BitmapDisabled, wxITEM_CHECK, _("Click Window"), wxEmptyString);
    wxBitmap itemtool19Bitmap(itemFrame1->GetBitmapResource(wxT("icons/keyboard.xpm")));
    wxBitmap itemtool19BitmapDisabled;
    itemToolBar15->AddTool(ID_TOOL_KEYBOARD, _("On-screen keyboard"), itemtool19Bitmap, itemtool19BitmapDisabled, wxITEM_NORMAL, _("On-screen keyboard"), wxEmptyString);
    itemToolBar15->AddSeparator();
    wxBitmap itemtool21Bitmap(itemFrame1->GetBitmapResource(wxT("icons/preferences.xpm")));
    wxBitmap itemtool21BitmapDisabled;
    itemToolBar15->AddTool(ID_TOOL_OPTIONS, _("Options"), itemtool21Bitmap, itemtool21BitmapDisabled, wxITEM_NORMAL, _("Options"), wxEmptyString);
    itemToolBar15->AddSeparator();
    wxBitmap itemtool23Bitmap(itemFrame1->GetBitmapResource(wxT("icons/help.xpm")));
    wxBitmap itemtool23BitmapDisabled;
    itemToolBar15->AddTool(ID_TOOL_HELP, _("Help"), itemtool23Bitmap, itemtool23BitmapDisabled, wxITEM_NORMAL, _("Help"), wxEmptyString);
    itemToolBar15->Realize();
    itemFrame1->SetToolBar(itemToolBar15);

    m_statusBar = new wxStatusBar( itemFrame1, ID_STATUSBAR, wxST_SIZEGRIP|wxNO_BORDER );
    if (WViacam::ShowToolTips())
        m_statusBar->SetToolTip(_("Frame rate meter. For proper function\nshould be greater than 25 fps. Reduce\nthe exposure time of your camera when\nnecessary."));
    m_statusBar->SetFieldsCount(1);
    itemFrame1->SetStatusBar(m_statusBar);

////@end WViacam content construction
	// Create taskbar icon
	m_taskBarIcon= new wxTaskBarIcon ();
	m_taskBarIcon->SetIcon (GetIconResource (_T("icons/eviacam_mini.xpm")), _T("Enable Viacam"));
	m_taskBarIcon->Connect 
		(wxID_ANY, wxEVT_TASKBAR_LEFT_UP, wxTaskBarIconEventHandler(WViacam::OnTaskBarIconLeftUp), NULL, this);
	m_pCamWindow = new CCamWindow ( itemFrame1, wxID_ANY, wxPoint(0,0), wxSize(160, 144));
}

/*!
 * Should we show tooltips?
 */

bool WViacam::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap WViacam::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin WViacam bitmap retrieval
    wxUnusedVar(name);
    if (name == _T("icons/on.xpm"))
    {
        wxBitmap bitmap(on);
        return bitmap;
    }
    else if (name == _T("icons/off.xpm"))
    {
        wxBitmap bitmap(off);
        return bitmap;
    }
    else if (name == _T("icons/clickwindow.xpm"))
    {
        wxBitmap bitmap(clickwindow);
        return bitmap;
    }
    else if (name == _T("icons/keyboard.xpm"))
    {
        wxBitmap bitmap(keyboard);
        return bitmap;
    }
    else if (name == _T("icons/preferences.xpm"))
    {
        wxBitmap bitmap(preferences);
        return bitmap;
    }
    else if (name == _T("icons/help.xpm"))
    {
        wxBitmap bitmap(help);
        return bitmap;
    }
    return wxNullBitmap;
////@end WViacam bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon WViacam::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin WViacam icon retrieval
    wxUnusedVar(name);
    if (name == _T("icons/eviacam_mini.xpm"))
    {
        wxIcon icon(eviacam_mini);
        return icon;
    }
    return wxNullIcon;
////@end WViacam icon retrieval
}

//
//	Custom events
//

// Update FPS meter
//enum EFPSCondition { GOOD, FAIR, POOR };
void WViacam::SetFPS (long value, WViacam::EFPSCondition cond)
{
	if (value!= m_prevFPS)
	{
		wxString text;
		switch (cond)
		{
		case WViacam::GOOD:
			text.Printf (_(" %li fps (good)"), value);
			break;
		case WViacam::FAIR:
			text.Printf (_(" %li fps (could be improved)"), value);
			break;
		case WViacam::POOR:
			text.Printf (_(" %li fps (too low)"), value);
			break;
		default:	
			assert (false);
		}

		// Send by a messege (this method is called from other thread)
		wxCommandEvent event(wxEVT_SET_FPS);
		event.SetString (text);
		
		wxPostEvent(this, event);

		m_prevFPS= value;
	}
}

void WViacam::OnSetFPS( wxCommandEvent &event )
{
	m_statusBar->SetStatusText(event.GetString());	
}

//
// EVENT HANDLERS
// 


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENUITEM_EXIT
 */

void WViacam::OnMenuitemExitClick( wxCommandEvent& event )
{
	event.Skip();	
	Close();	
}


/*!
 * wxEVT_CLOSE_WINDOW event handler for ID_WVIACAM
 */

void WViacam::OnCloseWindow( wxCloseEvent& event )
{
	event.Skip(false);
	if (event.CanVeto())
	{
		wxMessageDialog dlg (NULL, _("This action will close the program.\nAre you sure?"), _T("Enable Viacam"), wxICON_EXCLAMATION | wxYES_NO );
		if (dlg.ShowModal()== wxID_NO) return;
	}
	
	// Shutdown application
	wxGetApp().GetController().Finalize();

	Destroy ();
}

/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENUITEM_ENABLE
 */

void WViacam::OnMenuitemEnableClick( wxCommandEvent& event )
{
	wxGetApp().GetController().SetEnabled (!wxGetApp().GetController().GetEnabled());
	event.Skip(false);
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENU_ABOUT
 */

void WViacam::OnMenuAboutClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENU_ABOUT in WViacam.
    // Before editing this code, remove the block markers.
    WAbout* window = new WAbout(this);
    int returnValue = window->ShowModal();
    window->Destroy();
////@end wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENU_ABOUT in WViacam. 
	wxUnusedVar(returnValue);
	event.Skip(false);
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_MENUITEM_ENABLE
 */

void WViacam::OnMenuitemEnableUpdate( wxUpdateUIEvent& event )
{
	event.Check (wxGetApp().GetController().GetEnabled());
	event.Skip(false);
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENU_OPTIONS
 */

void WViacam::OnMenuOptionsClick( wxCommandEvent& event )
{
	wxGetApp().GetController().OpenConfiguration();   
	event.Skip(false);
}


/*!
 * wxEVT_ICONIZE event handler for ID_WVIACAM
 */

void WViacam::OnIconize( wxIconizeEvent& event )
{
	if (event.Iconized())
	{
		// Cancel actual iconization and hide window.
		Iconize (false);
		Show(false);
		event.Skip(false);
	}
}

bool WViacam::Show(bool show)
{
	if (show!= IsShown())
	{
		wxFrame::Show (show);
		if (show) Raise ();
		return true;
	}
	else
		return false;
}

void WViacam::OnTaskBarIconLeftUp ( wxTaskBarIconEvent& event )
{
	Show(!IsShown());
	event.Skip (true);
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL_ENABLE
 */

void WViacam::OnToolEnableClick( wxCommandEvent& event )
{
	wxGetApp().GetController().SetEnabled (true);
	event.Skip(false);
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_TOOL_ENABLE
 */

void WViacam::OnToolEnableUpdate( wxUpdateUIEvent& event )
{
	event.Enable (!wxGetApp().GetController().GetEnabled ());
	event.Skip(false);
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL_DISABLE
 */

void WViacam::OnToolDisableClick( wxCommandEvent& event )
{
	wxGetApp().GetController().SetEnabled (false);
	event.Skip(false);
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_TOOL_DISABLE
 */

void WViacam::OnToolDisableUpdate( wxUpdateUIEvent& event )
{
	event.Enable (wxGetApp().GetController().GetEnabled ());
	event.Skip(false);
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL_CLICKWIN
 */

void WViacam::OnToolClickwinClick( wxCommandEvent& event )
{
	wxGetApp().GetController().GetPointerAction().GetDwellClick().SetUseClickWindow(
		!wxGetApp().GetController().GetPointerAction().GetDwellClick().GetUseClickWindow()
	);		
	event.Skip(false);
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_TOOL_CLICKWIN
 */

void WViacam::OnToolClickwinUpdate( wxUpdateUIEvent& event )
{	
	event.Enable (wxGetApp().GetController().GetPointerAction().GetDwellClick().GetEnabled());
	event.Check (wxGetApp().GetController().GetPointerAction().GetDwellClick().GetUseClickWindow());
	event.Skip(false);
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL_PREFERENCES
 */

void WViacam::OnToolOptionsClick( wxCommandEvent& event )
{
	wxGetApp().GetController().OpenConfiguration();   
	event.Skip(false);
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL_HELP
 */

void WViacam::OnToolHelpClick( wxCommandEvent& event )
{
	if (m_helpController== NULL) {
		m_helpController= new wxHtmlHelpController(wxHF_DEFAULT_STYLE, this);
		wxString path;
			
		switch(wxGetApp().GetController().GetLocale()->GetLanguage()) {		
		case (wxLANGUAGE_CATALAN):
			path= wxStandardPaths::Get().GetDataDir().Append(_T("/help/ca/help.hhp"));
			break;
		case (wxLANGUAGE_SPANISH):
			path= wxStandardPaths::Get().GetDataDir().Append(_T("/help/es/help.hhp"));
			break;
		case (wxLANGUAGE_GERMAN):
			path= wxStandardPaths::Get().GetDataDir().Append(_T("/help/de/help.hhp"));
			break;
		case (wxLANGUAGE_GALICIAN):
			path= wxStandardPaths::Get().GetDataDir().Append(_T("/help/gl/help.hhp"));
			break;
		default:
			path= wxStandardPaths::Get().GetDataDir().Append(_T("/help/en/help.hhp"));
			break;
		}		
	
		m_helpController->AddBook(path, false);
	}

	m_helpController->DisplayContents();
	
	event.Skip(false);
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL_KEYBOARD
 */

void WViacam::OnToolKeyboardClick( wxCommandEvent& event )
{
	wxGetApp().GetController().OpenOnScreenKeyboard();
	event.Skip(false);
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENU_WIZARD
 */

void WViacam::OnMenuWizardClick( wxCommandEvent& event )
{
	wxGetApp().GetController().StartWizard();
	event.Skip(false);
}



