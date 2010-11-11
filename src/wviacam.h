/////////////////////////////////////////////////////////////////////////////
// Name:        wviacam.h
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

#ifndef WVIACAM_H_
#define WVIACAM_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/frame.h"
#include "wx/toolbar.h"
#include "wx/statusbr.h"
////@end includes

#include <wx/taskbar.h>

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxStatusBar;
////@end forward declarations

class CCamWindow;
class CViacamController;

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_WVIACAM 10000
#define ID_MENUITEM_ENABLE 10011
#define ID_MENUITEM_EXIT 10001
#define ID_MENU_WIZARD 10094
#define ID_MENU_OPTIONS 10003
#define ID_MENU_HELP_CONTENTS 10040
#define ID_MENU_ABOUT 10028
#define ID_TOOLBAR 10042
#define ID_TOOL_ENABLE 10043
#define ID_TOOL_DISABLE 10044
#define ID_TOOL_CLICKWIN 10046
#define ID_TOOL_KEYBOARD 10048
#define ID_TOOL_OPTIONS 10045
#define ID_TOOL_HELP 10047
#define ID_STATUSBAR 10002
#define SYMBOL_WVIACAM_STYLE wxDEFAULT_FRAME_STYLE|wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxMINIMIZE|wxMINIMIZE_BOX|wxCLOSE_BOX
#define SYMBOL_WVIACAM_TITLE _("Enable Viacam")
#define SYMBOL_WVIACAM_IDNAME ID_WVIACAM
#define SYMBOL_WVIACAM_SIZE wxSize(300, 300)
#define SYMBOL_WVIACAM_POSITION wxPoint(100, 200)
////@end control identifiers


/*!
 * WViacam class declaration
 */

class WViacam: public wxFrame
{    
    DECLARE_CLASS( WViacam )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    WViacam();

    WViacam( wxWindow* parent, wxWindowID id = SYMBOL_WVIACAM_IDNAME, const wxString& caption = SYMBOL_WVIACAM_TITLE, const wxPoint& pos = SYMBOL_WVIACAM_POSITION, const wxSize& size = SYMBOL_WVIACAM_SIZE, long style = SYMBOL_WVIACAM_STYLE );

    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_WVIACAM_IDNAME, const wxString& caption = SYMBOL_WVIACAM_TITLE, const wxPoint& pos = SYMBOL_WVIACAM_POSITION, const wxSize& size = SYMBOL_WVIACAM_SIZE, long style = SYMBOL_WVIACAM_STYLE );

    /// Destructor
    ~WViacam();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

    CCamWindow* GetCamWindow() { return m_pCamWindow; }
    void SetController (CViacamController* pController) { m_pController= pController; }

	// 
	// Custom events
	//

	// Update FPS meter
	enum EFPSCondition { GOOD, FAIR, POOR };
	void SetFPS (long value, EFPSCondition cond);
	void OnSetFPS( wxCommandEvent &event );

	// For sending mouse system commands from worker thread
//	void SendMouseMotionUpdate (float vx, float vy);
//	void OnSendMouseMotionUpdate ( wxCommandEvent &event );

////@begin WViacam event handler declarations

    /// wxEVT_CLOSE_WINDOW event handler for ID_WVIACAM
    void OnCloseWindow( wxCloseEvent& event );

    /// wxEVT_ICONIZE event handler for ID_WVIACAM
    void OnIconize( wxIconizeEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENUITEM_ENABLE
    void OnMenuitemEnableClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_MENUITEM_ENABLE
    void OnMenuitemEnableUpdate( wxUpdateUIEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENUITEM_EXIT
    void OnMenuitemExitClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENU_WIZARD
    void OnMenuWizardClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENU_OPTIONS
    void OnMenuOptionsClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENU_HELP_CONTENTS
    void OnMenuHelpContentsClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENU_ABOUT
    void OnMenuAboutClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL_ENABLE
    void OnToolEnableClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_TOOL_ENABLE
    void OnToolEnableUpdate( wxUpdateUIEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL_DISABLE
    void OnToolDisableClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_TOOL_DISABLE
    void OnToolDisableUpdate( wxUpdateUIEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL_CLICKWIN
    void OnToolClickwinClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_TOOL_CLICKWIN
    void OnToolClickwinUpdate( wxUpdateUIEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL_KEYBOARD
    void OnToolKeyboardClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL_OPTIONS
    void OnToolOptionsClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_TOOL_HELP
    void OnToolHelpClick( wxCommandEvent& event );

////@end WViacam event handler declarations

////@begin WViacam member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end WViacam member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

	virtual bool Show(bool show);

	void OnTaskBarIconLeftUp ( wxTaskBarIconEvent& event );

////@begin WViacam member variables
    wxStatusBar* m_statusBar;
////@end WViacam member variables	
private:
	CCamWindow* m_pCamWindow;
	CViacamController* m_pController;
	wxTaskBarIcon* m_taskBarIcon;	

	long m_prevFPS;
};

#endif
    // _WVIACAM_H_
