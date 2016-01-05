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
#include "wx/xrc/xmlres.h"
#include "wx/tglbtn.h"
////@end includes
#include <wx/button.h>
//#include "clickwindowcontroller.h"

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
#define SYMBOL_CCLICKWINDOWTEXT_STYLE wxSTAY_ON_TOP
#define SYMBOL_CCLICKWINDOWTEXT_TITLE _("eViacam")
#define SYMBOL_CCLICKWINDOWTEXT_IDNAME ID_CCLICKWINDOW_TEXT
#define SYMBOL_CCLICKWINDOWTEXT_SIZE wxDefaultSize
#define SYMBOL_CCLICKWINDOWTEXT_POSITION wxPoint(0, 0)
////@end control identifiers

#define CLICK_WINDOW_TEXT wxT("ID_CCLICKWINDOW_TEXT")
#define CLICK_WINDOW_TEXT_VERTICAL wxT("ID_CLICKWINDOW_TEXT_VERTICAL")

/*!
 * CClickWindowText class declaration
 */

class CClickWindowText: public CClickWindow
{    
public:
    /// Constructors
    CClickWindowText( wxWindow* parent, const wxString& name);

    /// Destructor
    ~CClickWindowText();

private:
    /// Creation
    bool Create( wxWindow* parent, const wxString& name);

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls(const wxString& name);

////@begin CClickWindowText event handler declarations

////@end CClickWindowText event handler declarations

////@begin CClickWindowText member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end CClickWindowText member function declarations

	virtual void UpdateButtons (bool noClickStatus, CClickWindowController::EButton selected,
			CClickWindowController::EButton locked);
protected:

	virtual void OnMainWindowShow ( wxShowEvent& event );

	virtual wxControl* GetNoClickButton();
    virtual wxControl* GetLeftButton();
    virtual wxControl* GetMiddleButton();
    virtual wxControl* GetRightButton();
    virtual wxControl* GetDragButton();
    virtual wxControl* GetDblClickButton();
    virtual wxControl* GetShowFrame();

	void UpdateSelectedButtons ( wxControl* btnSelected, wxControl* btnNoSelected1, 
								 wxControl* btnNoSelected2, wxControl* btnNoSelected3, wxControl* btnNoSelected4);

	void UpdateLockedButtons ( wxControl* btnLocked, wxControl* btnNoLocked1, 
							   wxControl* btnNoLocked2, wxControl* btnNoLocked3, wxControl* btnNoLocked4);

////@begin CClickWindowText member variables
    wxToggleButton* m_noClickButton;
    wxToggleButton* m_leftButton;
    wxToggleButton* m_middleButton;
    wxToggleButton* m_rightButton;
    wxToggleButton* m_dragButton;
    wxToggleButton* m_dblClickButton;
    wxButton* m_btnShowFrame;
////@end CClickWindowText member variables

};

#endif
    // _CLICKWINDOWTEXT_H_
