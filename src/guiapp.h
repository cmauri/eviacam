/////////////////////////////////////////////////////////////////////////////
// Name:        guiapp.h
// Purpose:  
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by: 
// Created:     09/01/2008 21:40:06
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

#ifndef _GUIAPP_H_
#define _GUIAPP_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/image.h"
#include "wviacam.h"
////@end includes

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
 * GuiApp class declaration
 */

class GuiApp: public wxApp
{    
    DECLARE_CLASS( GuiApp )
    DECLARE_EVENT_TABLE()

public:

    /// Constructor
    GuiApp();

    void Init();

    /// Initialises the application
    virtual bool OnInit();

    /// Called on exit
    virtual int OnExit();

////@begin GuiApp event handler declarations

////@end GuiApp event handler declarations

////@begin GuiApp member function declarations

////@end GuiApp member function declarations

////@begin GuiApp member variables
////@end GuiApp member variables

private:
	
	CViacamController* m_pController;
#if defined(NDEBUG)
	wxLogNull m_disableErrorMessages;
#endif
};

/*!
 * Application instance declaration 
 */

////@begin declare app
DECLARE_APP(GuiApp)
////@end declare app

#endif
    // _GUIAPP_H_
