/////////////////////////////////////////////////////////////////////////////
// Name:        eviacamapp.h
// Purpose:  
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by: 
// Created:     09/01/2008 21:40:06
// Copyright:   (C) 2008-11 Cesar Mauri Loba - CREA Software Systems
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

#ifndef _EVIACAMAPP_H_
#define _EVIACAMAPP_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/image.h"
#include "wviacam.h"
////@end includes
#include <wx/app.h>
#include <assert.h>
#if !defined(NDEBUG)
#include <wx/log.h>
#endif

/*!
 * Forward declarations
 */
class CViacamController;


////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
////@end control identifiers


/*!
 * EViacamApp class declaration
 */

class EViacamApp: public wxApp
{    
	DECLARE_CLASS( EViacamApp )
	DECLARE_EVENT_TABLE()

public:
	/// Constructor
	EViacamApp();

	// Return controller
	CViacamController& GetController() {
		assert (m_pController);
		return *m_pController;
	}

private:
	void Init();

	/// Initialises the application
	virtual bool OnInit();

	/// Called on exit
	virtual int OnExit();

	virtual void OnInitCmdLine(wxCmdLineParser& parser);

    virtual bool OnCmdLineParsed(wxCmdLineParser& parser);

	void OnEndSession( wxCloseEvent& event );

////@begin EViacamApp event handler declarations

////@end EViacamApp event handler declarations

////@begin EViacamApp member function declarations

////@end EViacamApp member function declarations

////@begin EViacamApp member variables
////@end EViacamApp member variables
	//volatile
	CViacamController* m_pController;
#if !defined(NDEBUG)
	wxLogNull m_disableErrorMessages;
#endif
};

/*!
 * Application instance declaration 
 */

////@begin declare app
DECLARE_APP(EViacamApp)
////@end declare app

#endif
    // _GUIAPP_H_
