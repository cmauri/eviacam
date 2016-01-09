/////////////////////////////////////////////////////////////////////////////
// Name:        clickwindowbitmap.h
// Purpose:     
// Author:      Cesar Mauri Loba
// Modified by: 
// Created:     16/11/2009 15:10:47
// RCS-ID:      
// Copyright:   (C) 2008-16 Cesar Mauri Loba - CREA Software Systems
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

#ifndef _CLICKWINDOWBITMAP_H_
#define _CLICKWINDOWBITMAP_H_

#include "clickwindow.h"
#include <wx/bitmap.h>
#include "wx/xrc/xmlres.h"

class wxBitmapButton;

#define CLICK_WINDOW_BITMAP wxT("ID_CCLICKWINDOWBITMAP")
#define CLICK_WINDOW_BITMAP_VERTICAL wxT("ID_CLICKWINDOWBITMAPVERTICAL")


class CClickWindowBitmap: public CClickWindow
{    
public:
    /// Constructors
    CClickWindowBitmap( wxWindow* parent, const wxString& name);

	/// Destructor
	~CClickWindowBitmap();

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

	wxBitmap m_bmpButtonDblLeftLocked;
	wxBitmap m_bmpButtonDblLeftSelectedLocked;
	wxBitmap m_bmpButtonDblLeftSelected;
	wxBitmap m_bmpButtonDblLeft;
	wxBitmap m_bmpButtonDragLocked;
	wxBitmap m_bmpButtonDragSelectedLocked;
	wxBitmap m_bmpButtonDragSelected;
	wxBitmap m_bmpButtonDrag;
	wxBitmap m_bmpButtonHideMainWindow;
	wxBitmap m_bmpButtonLeftLocked;
	wxBitmap m_bmpButtonLeftSelectedLocked;
	wxBitmap m_bmpButtonLeftSelected;
	wxBitmap m_bmpButtonLeft;
	wxBitmap m_bmpButtonNoClickSelected;
	wxBitmap m_bmpButtonNoClick;
	wxBitmap m_bmpButtonMiddleLocked;
	wxBitmap m_bmpButtonMiddleSelectedLocked;
	wxBitmap m_bmpButtonMiddleSelected;
	wxBitmap m_bmpButtonMiddle;
	wxBitmap m_bmpButtonRightLocked;
	wxBitmap m_bmpButtonRightSelectedLocked;
	wxBitmap m_bmpButtonRightSelected;
	wxBitmap m_bmpButtonRight;
	wxBitmap m_bmpButtonShowMainWindow;
	
    wxBitmapButton* m_btnNoClick;
    wxBitmapButton* m_btnLeft;
    wxBitmapButton* m_btnMiddle;
    wxBitmapButton* m_btnRight;
    wxBitmapButton* m_btnDrag;
    wxBitmapButton* m_btnDblLeft;
    wxBitmapButton* m_btnShowFrame;
};

#endif
    // _CLICKWINDOWBITMAP_H_
