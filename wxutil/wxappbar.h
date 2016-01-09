/////////////////////////////////////////////////////////////////////////////
// Name:        wxappbar.h
// Purpose:  
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by: 
// Created:     18/10/2008
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

#ifndef _WXAPPBAR_H_
#define _WXAPPBAR_H_

/*!
 * Includes
 */
#include "mousecontrol.h"
#include <wx/timer.h>
#include <wx/dialog.h>


/*!
 * Control identifiers
 */
//#define SYMBOL_CWXAPPBAR_STYLE wxSTAY_ON_TOP|wxNO_BORDER

/*!
 * WXAppBar class declaration
 */

class WXAppBar: public wxDialog
{    
	DECLARE_DYNAMIC_CLASS( WXAppBar )
	DECLARE_EVENT_TABLE()

public:
	//enum EClickWindowStatus { VISIBLE= 0, HIDDEN, DOCKED };
	enum EDocking { NON_DOCKED= 0, TOP_DOCKED, BOTTOM_DOCKED, LEFT_DOCKED, RIGHT_DOCKED };
	
	/// Constructors
	WXAppBar();
	WXAppBar( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style= 0); // = SYMBOL_CWXAPPBAR_STYLE );
	
	/// Creation
	bool Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style= 0); // = SYMBOL_CWXAPPBAR_STYLE );
	
	/// Destructor
	~WXAppBar();
	
	/// Initialises member variables
	void Init();
	void OnSize( wxSizeEvent& event );

	void OnMove( wxMoveEvent& event );
	void OnEnterWindow( wxMouseEvent& event );
	void OnLeaveWindow( wxMouseEvent& event );
	void OnTimer(wxTimerEvent& event);

	EDocking GetDockingMode () const { return m_currentDockingMode; }
	void SetDockingMode (EDocking dockingMode);
	
	bool GetAutohideMode () const { return m_autohide; }
	void SetAutohideMode (bool autohide);
	void SetAutohideModeStep ();
	
	bool GetWarnBarOverlap () const { return m_warnBarOverlap; }
	void SetWarnBarOverlap (bool value);
	bool CheckForBar ();
		
	bool GetBorderDecorations () const;
	void SetBorderDecorations (bool enable, bool apply= false);
	
	void SetEntryInTaskBar (bool v);
	
	void SetEntryInPager (bool v);
	
	void SetAcceptFocus (bool accept);
	
	virtual bool Show(bool show = true);

	virtual bool ProcessEvent(wxEvent& event);

protected:
	// Set the stickness of the window, i.e. whether it should
	// appear in the different desktops (GTK+ only)
	void SetSticky(bool stick);

private:
#if defined(__WXMSW__)
//	void AppBarCallback (UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif
	void SetDockedModeStep1();
	void SetDockedModeStep2();
	void UnSetDockedModeStep1();
	void UnSetDockedModeStep2();
	void CheckCreateWindow();
	
	int m_X, m_Y, m_Width, m_Height;
	EDocking m_currentDockingMode;
	bool m_dialogHadBorderDecorations;
	bool m_autohide;
	bool m_isAutohideWindowShown;
	CMouseControl* m_pMouseControl;
	wxTimer m_timer;
	bool m_firstTime;
	bool m_warnBarOverlap;
	wxDialog* m_pDialog;		
};

#endif
    // _WXAPPBAR_H_
