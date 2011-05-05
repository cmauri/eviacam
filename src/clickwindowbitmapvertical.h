/////////////////////////////////////////////////////////////////////////////
// Name:        clickwindowbitmapvertical.h
// Purpose:     
// Author:      César Mauri Loba
// Modified by: 
// Created:     Tue 22 Feb 2011 15:35:49 CET
// RCS-ID:      
// Copyright:   (C) 2008 Cesar Mauri from CREA Sistemes Informatics
// License:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _CLICKWINDOWBITMAPVERTICAL_H_
#define _CLICKWINDOWBITMAPVERTICAL_H_


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
#define ID_CLICKWINDOWBITMAPVERTICAL 10069
#define ID_BITMAPBUTTON_NOCLICK 10055
#define ID_BITMAPBUTTON_LEFT 10056
#define ID_BITMAPBUTTON_MIDDLE 10257
#define ID_BITMAPBUTTON_RIGHT 10057
#define ID_BITMAPBUTTON_DRAG 10058
#define ID_BITMAPBUTTON_DBLCLICK 10059
#define ID_BITMAPBUTTON 10060
#define SYMBOL_CCLICKWINDOWBITMAPVERTICAL_STYLE wxCAPTION|wxSYSTEM_MENU|wxSTAY_ON_TOP
#define SYMBOL_CCLICKWINDOWBITMAPVERTICAL_TITLE _("eViacam")
#define SYMBOL_CCLICKWINDOWBITMAPVERTICAL_IDNAME ID_CLICKWINDOWBITMAPVERTICAL
#define SYMBOL_CCLICKWINDOWBITMAPVERTICAL_SIZE wxDefaultSize
#define SYMBOL_CCLICKWINDOWBITMAPVERTICAL_POSITION wxPoint(0, 0)
////@end control identifiers


/*!
 * CClickWindowBitmapVertical class declaration
 */

class CClickWindowBitmapVertical: public CClickWindow
{    
    DECLARE_DYNAMIC_CLASS( CClickWindowBitmapVertical )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    CClickWindowBitmapVertical();
    CClickWindowBitmapVertical( wxWindow* parent, wxWindowID id = SYMBOL_CCLICKWINDOWBITMAPVERTICAL_IDNAME, const wxString& caption = SYMBOL_CCLICKWINDOWBITMAPVERTICAL_TITLE, const wxPoint& pos = SYMBOL_CCLICKWINDOWBITMAPVERTICAL_POSITION, const wxSize& size = SYMBOL_CCLICKWINDOWBITMAPVERTICAL_SIZE, long style = SYMBOL_CCLICKWINDOWBITMAPVERTICAL_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_CCLICKWINDOWBITMAPVERTICAL_IDNAME, const wxString& caption = SYMBOL_CCLICKWINDOWBITMAPVERTICAL_TITLE, const wxPoint& pos = SYMBOL_CCLICKWINDOWBITMAPVERTICAL_POSITION, const wxSize& size = SYMBOL_CCLICKWINDOWBITMAPVERTICAL_SIZE, long style = SYMBOL_CCLICKWINDOWBITMAPVERTICAL_STYLE );

    /// Destructor
    ~CClickWindowBitmapVertical();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin CClickWindowBitmapVertical event handler declarations

////@end CClickWindowBitmapVertical event handler declarations

////@begin CClickWindowBitmapVertical member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end CClickWindowBitmapVertical member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();
virtual void UpdateButtons (bool noClickStatus, CClickWindowController::EButton selected, CClickWindowController::EButton locked);

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
	
////@begin CClickWindowBitmapVertical member variables
    wxBitmapButton* m_btnNoClick;
    wxBitmapButton* m_btnLeft;
    wxBitmapButton* m_btnMiddle;
    wxBitmapButton* m_btnRight;
    wxBitmapButton* m_btnDrag;
    wxBitmapButton* m_btnDblLeft;
    wxBitmapButton* m_btnShowFrame;
////@end CClickWindowBitmapVertical member variables
};

#endif
    // _CLICKWINDOWBITMAPVERTICAL_H_
