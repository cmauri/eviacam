/////////////////////////////////////////////////////////////////////////////
// Name:        clickwindowbitmap.h
// Purpose:     
// Author:      C�sar Mauri Loba
// Modified by: 
// Created:     16/11/2009 15:10:47
// RCS-ID:      
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

#ifndef _CLICKWINDOWBITMAP_H_
#define _CLICKWINDOWBITMAP_H_


/*!
 * Includes
 */
#include "clickwindow.h"
////@begin includes
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_CCLICKWINDOWBITMAP 10020
#define ID_BITMAPBUTTON_NOCLICK 10055
#define ID_BITMAPBUTTON_LEFT 10056
#define ID_BITMAPBUTTON_RIGHT 10057
#define ID_BITMAPBUTTON_DRAG 10058
#define ID_BITMAPBUTTON_DBLCLICK 10059
#define ID_BITMAPBUTTON 10060
#define SYMBOL_CCLICKWINDOWBITMAP_STYLE wxCAPTION|wxSYSTEM_MENU|wxSTAY_ON_TOP
#define SYMBOL_CCLICKWINDOWBITMAP_TITLE _("eViacam")
#define SYMBOL_CCLICKWINDOWBITMAP_IDNAME ID_CCLICKWINDOWBITMAP
#define SYMBOL_CCLICKWINDOWBITMAP_SIZE wxDefaultSize
#define SYMBOL_CCLICKWINDOWBITMAP_POSITION wxPoint(0, 0)
////@end control identifiers

/*!
 * CClickWindowBitmap class declaration
 */

class CClickWindowBitmap: public CClickWindow
{    
    DECLARE_DYNAMIC_CLASS( CClickWindowBitmap )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    CClickWindowBitmap();
    CClickWindowBitmap( wxWindow* parent, wxWindowID id = SYMBOL_CCLICKWINDOWBITMAP_IDNAME, const wxString& caption = SYMBOL_CCLICKWINDOWBITMAP_TITLE, const wxPoint& pos = SYMBOL_CCLICKWINDOWBITMAP_POSITION, const wxSize& size = SYMBOL_CCLICKWINDOWBITMAP_SIZE, long style = SYMBOL_CCLICKWINDOWBITMAP_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_CCLICKWINDOWBITMAP_IDNAME, const wxString& caption = SYMBOL_CCLICKWINDOWBITMAP_TITLE, const wxPoint& pos = SYMBOL_CCLICKWINDOWBITMAP_POSITION, const wxSize& size = SYMBOL_CCLICKWINDOWBITMAP_SIZE, long style = SYMBOL_CCLICKWINDOWBITMAP_STYLE );

    /// Destructor
    ~CClickWindowBitmap();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin CClickWindowBitmap event handler declarations

////@end CClickWindowBitmap event handler declarations

////@begin CClickWindowBitmap member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end CClickWindowBitmap member function declarations

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
	wxBitmap m_bmpButtonRightLocked;
	wxBitmap m_bmpButtonRightSelectedLocked;
	wxBitmap m_bmpButtonRightSelected;
	wxBitmap m_bmpButtonRight;
	wxBitmap m_bmpButtonShowMainWindow;
	
////@begin CClickWindowBitmap member variables
    wxBitmapButton* m_btnNoClick;
    wxBitmapButton* m_btnLeft;
    wxBitmapButton* m_btnRight;
    wxBitmapButton* m_btnDrag;
    wxBitmapButton* m_btnDblLeft;
    wxBitmapButton* m_btnShowFrame;
////@end CClickWindowBitmap member variables
};

#endif
    // _CLICKWINDOWBITMAP_H_
