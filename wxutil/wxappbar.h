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
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#ifdef __WXMSW__
#include <shellapi.h>
#endif

/*!
 * Control identifiers
 */
#define SYMBOL_CWXAPPBAR_STYLE wxSTAY_ON_TOP|wxNO_BORDER

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
	WXAppBar( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = SYMBOL_CWXAPPBAR_STYLE );
	
	/// Creation
	bool Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = SYMBOL_CWXAPPBAR_STYLE );
	
	/// Destructor
	~WXAppBar();
	
	/// Initialises member variables
	void Init();
	//void OnSize( wxSizeEvent& event );

	void OnMove( wxMoveEvent& event );

	//bool SetClickWindowStyle (EClickWindowStatus winStatus, EDocking dockingMode, bool show = true);
	void SetDockingMode (EDocking dockingMode);

	virtual bool Show(bool show = true);

	virtual bool ProcessEvent(wxEvent& event);

private:
#if defined(__WXMSW__)
//	void AppBarCallback (UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif
	int m_X, m_Y, m_Width, m_Height;
//	int m_barX, m_barY, m_barWidth, m_barHeight;
	EDocking m_currentDockingMode; //, m_prevDockingMode;
};

#endif
    // _WXAPPBAR_H_
