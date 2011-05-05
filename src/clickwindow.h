/////////////////////////////////////////////////////////////////////////////
// Name:        clickwindow.h
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

#ifndef _CLICKWINDOW_H_
#define _CLICKWINDOW_H_


#include "wxappbar.h"
#include "clickwindowcontroller.h"

#define SYMBOL_CCLICKWINDOW_STYLE wxSTAY_ON_TOP|wxSIMPLE_BORDER
#define SYMBOL_CCLICKWINDOW_TITLE _("Click Window")
#define SYMBOL_CCLICKWINDOW_IDNAME ID_CCLICKWINDOW
#define SYMBOL_CCLICKWINDOW_SIZE wxDefaultSize
#define SYMBOL_CCLICKWINDOW_POSITION wxPoint(0, 0)

class CClickWindow: public WXAppBar
{    
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    CClickWindow();
    CClickWindow( wxWindow* parent, wxWindowID id, const wxString& caption = SYMBOL_CCLICKWINDOW_TITLE, const wxPoint& pos = SYMBOL_CCLICKWINDOW_POSITION, const wxSize& size = SYMBOL_CCLICKWINDOW_SIZE, long style = SYMBOL_CCLICKWINDOW_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id, const wxString& caption = SYMBOL_CCLICKWINDOW_TITLE, const wxPoint& pos = SYMBOL_CCLICKWINDOW_POSITION, const wxSize& size = SYMBOL_CCLICKWINDOW_SIZE, long style = SYMBOL_CCLICKWINDOW_STYLE );

    /// Destructor
    ~CClickWindow();

    /// Initialises member variables
    void Init();

    void ConnectEvents();
	
	void SetController (CClickWindowController& controller);

	virtual void UpdateButtons (bool noClickStatus, CClickWindowController::EButton selected, CClickWindowController::EButton locked)= 0;

	virtual void OnMainWindowShow ( wxShowEvent& event );
	virtual wxControl* GetNoClickButton()= 0;
protected:
    virtual wxControl* GetLeftButton()= 0;
    virtual wxControl* GetMiddleButton()= 0;
    virtual wxControl* GetRightButton()= 0;
    virtual wxControl* GetDragButton()= 0;
    virtual wxControl* GetDblClickButton()= 0;
    virtual wxControl* GetShowFrame()= 0;
	  
	void OnCloseWindow( wxCloseEvent& event );

	void OnLeftUp( wxMouseEvent& event );

	void OnEnterWindow( wxMouseEvent& event );

    void OnEventSkipperMouse( wxMouseEvent& event );
	
	void OnEventSkipperFocus( wxFocusEvent& event );
	
	CClickWindowController::EButton ButtonId2EButton (int id);
	
	CClickWindowController* m_pController;
};

inline void CClickWindow::SetController (CClickWindowController& controller)
{
	assert (m_pController== NULL);
	m_pController= &controller;
}

#endif
    // _CLICKWINDOW_H_
