/////////////////////////////////////////////////////////////////////////////
// Name:        clickwindowtext.h
// Purpose:  
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by: 
// Created:     
// Copyright:   (C) 2008-09 Cesar Mauri Loba - CREA Software Systems
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

#ifndef _CLICKWINDOWTEXT_H_
#define _CLICKWINDOWTEXT_H_


/*!
 * Includes
 */

#include "clickwindow.h"
////@begin includes
#include "wx/tglbtn.h"
////@end includes
#include "clickwindowcontroller.h"

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxToggleButton;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_CCLICKWINDOW_TEXT 10003
#define ID_BUTTON_NOCLICK 10004
#define ID_BTN_LEFT 10007
#define ID_BTN_RIGHT 10008
#define ID_BTN_DRAG 10009
#define ID_BTN_DBLCLICK 10010
#define ID_BUTTON_SHOW_FRAME 10013
#define SYMBOL_CCLICKWINDOWTEXT_STYLE wxSYSTEM_MENU|wxSTAY_ON_TOP
#define SYMBOL_CCLICKWINDOWTEXT_TITLE _("eViacam")
#define SYMBOL_CCLICKWINDOWTEXT_IDNAME ID_CCLICKWINDOW_TEXT
#define SYMBOL_CCLICKWINDOWTEXT_SIZE wxDefaultSize
#define SYMBOL_CCLICKWINDOWTEXT_POSITION wxPoint(0, 0)
////@end control identifiers

/*!
 * CClickWindowText class declaration
 */

class CClickWindowText: public CClickWindow
{    
    DECLARE_DYNAMIC_CLASS( CClickWindowText )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    CClickWindowText();
    CClickWindowText( wxWindow* parent, wxWindowID id = SYMBOL_CCLICKWINDOWTEXT_IDNAME, const wxString& caption = SYMBOL_CCLICKWINDOWTEXT_TITLE, const wxPoint& pos = SYMBOL_CCLICKWINDOWTEXT_POSITION, const wxSize& size = SYMBOL_CCLICKWINDOWTEXT_SIZE, long style = SYMBOL_CCLICKWINDOWTEXT_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_CCLICKWINDOWTEXT_IDNAME, const wxString& caption = SYMBOL_CCLICKWINDOWTEXT_TITLE, const wxPoint& pos = SYMBOL_CCLICKWINDOWTEXT_POSITION, const wxSize& size = SYMBOL_CCLICKWINDOWTEXT_SIZE, long style = SYMBOL_CCLICKWINDOWTEXT_STYLE );

    /// Destructor
    ~CClickWindowText();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin CClickWindowText event handler declarations

////@end CClickWindowText event handler declarations

////@begin CClickWindowText member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end CClickWindowText member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

	virtual void UpdateButtons (bool noClickStatus, CClickWindowController::EButton selected, CClickWindowController::EButton locked);
protected:

	virtual void OnMainWindowShow ( wxShowEvent& event );

	virtual wxControl* GetNoClickButton();
    virtual wxControl* GetLeftButton();
    virtual wxControl* GetRightButton();
    virtual wxControl* GetDragButton();
    virtual wxControl* GetDblClickButton();
    virtual wxControl* GetShowFrame();

	void UpdateSelectedButtons ( wxControl* btnSelected, wxControl* btnNoSelected1, 
								 wxControl* btnNoSelected2,wxControl* btnNoSelected3);

	void UpdateLockedButtons ( wxControl* btnLocked, wxControl* btnNoLocked1, 
							   wxControl* btnNoLocked2, wxControl* btnNoLocked3);

////@begin CClickWindowText member variables
    wxToggleButton* m_noClickButton;
    wxToggleButton* m_leftButton;
    wxToggleButton* m_rightButton;
    wxToggleButton* m_dragButton;
    wxToggleButton* m_dblClickButton;
    wxButton* m_btnShowFrame;
////@end CClickWindowText member variables

};

#endif
    // _CLICKWINDOWTEXT_H_
