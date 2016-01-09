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

#include "clickwindow.h"
#include "wx/xrc/xmlres.h"
#include "wx/tglbtn.h"
#include <wx/button.h>

class wxToggleButton;

#define CLICK_WINDOW_TEXT wxT("ID_CCLICKWINDOW_TEXT")
#define CLICK_WINDOW_TEXT_VERTICAL wxT("ID_CLICKWINDOW_TEXT_VERTICAL")

class CClickWindowText: public CClickWindow
{    
public:
    /// Constructors
    CClickWindowText( wxWindow* parent, const wxString& name);

    /// Destructor
    ~CClickWindowText();

private:
    /// Creates the controls and sizers
    void CreateControls(const wxString& name);

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

    wxToggleButton* m_noClickButton;
    wxToggleButton* m_leftButton;
    wxToggleButton* m_middleButton;
    wxToggleButton* m_rightButton;
    wxToggleButton* m_dragButton;
    wxToggleButton* m_dblClickButton;
    wxButton* m_btnShowFrame;
};

#endif
    // _CLICKWINDOWTEXT_H_
