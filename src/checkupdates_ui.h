/////////////////////////////////////////////////////////////////////////////
// Name:        checkupdates_gui.h
// Purpose:     Check updates dialog
// Author:      César Mauri Loba
// Created:     12/10/2012 20:15:41
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

#ifndef _CHECKUPDATES_GUI_H_
#define _CHECKUPDATES_GUI_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/hyperlink.h"
////@end includes
#include <wx/timer.h>
#include <wx/dialog.h>
#include <wx/stattext.h>


/*!
 * Forward declarations
 */

////@begin forward declarations
class wxHyperlinkCtrl;
////@end forward declarations
namespace eviacam {
	class CheckUpdates;
}



/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_CHECKFORUPDATES 10128
#define ID_HYPERLINKCTRL_WEBSITE 10129
#define ID_BUTTON_CHECKUPDATE_CLOSE 10130
#define SYMBOL_CHECKUPDATESUI_STYLE wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_CHECKUPDATESUI_TITLE _("Check for updates")
#define SYMBOL_CHECKUPDATESUI_IDNAME ID_CHECKFORUPDATES
#define SYMBOL_CHECKUPDATESUI_SIZE wxDefaultSize
#define SYMBOL_CHECKUPDATESUI_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * CheckUpdatesUI class declaration
 */

class CheckUpdatesUI: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( CheckUpdatesUI )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    CheckUpdatesUI();
    CheckUpdatesUI( wxWindow* parent, wxWindowID id = SYMBOL_CHECKUPDATESUI_IDNAME, const wxString& caption = SYMBOL_CHECKUPDATESUI_TITLE, const wxPoint& pos = SYMBOL_CHECKUPDATESUI_POSITION, const wxSize& size = SYMBOL_CHECKUPDATESUI_SIZE, long style = SYMBOL_CHECKUPDATESUI_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_CHECKUPDATESUI_IDNAME, const wxString& caption = SYMBOL_CHECKUPDATESUI_TITLE, const wxPoint& pos = SYMBOL_CHECKUPDATESUI_POSITION, const wxSize& size = SYMBOL_CHECKUPDATESUI_SIZE, long style = SYMBOL_CHECKUPDATESUI_STYLE );

	void StartProgress();
	void StopProgress();
	void SetResults(const wxString& txt1, const wxString& txt2, bool showLink);

    /// Destructor
    ~CheckUpdatesUI();

private:
    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin CheckUpdatesUI event handler declarations

    /// wxEVT_COMMAND_HYPERLINK event handler for ID_HYPERLINKCTRL_WEBSITE
    void OnHyperlinkctrlWebsiteHyperlinkClicked( wxHyperlinkEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_CHECKUPDATE_CLOSE
    void OnButtonCheckupdateCloseClick( wxCommandEvent& event );

////@end CheckUpdatesUI event handler declarations

    void OnTimer(wxTimerEvent& event);


////@begin CheckUpdatesUI member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end CheckUpdatesUI member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin CheckUpdatesUI member variables
    wxStaticText* m_msg1;
    wxStaticText* m_msg2;
    wxHyperlinkCtrl* m_link;
////@end CheckUpdatesUI member variables
    wxTimer m_timer;
};

#endif
    // _CHECKUPDATES_GUI_H_
