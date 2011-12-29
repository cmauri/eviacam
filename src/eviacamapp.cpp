/////////////////////////////////////////////////////////////////////////////
// Name:        eviacamapp.cpp
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

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes
#include <wx/tooltip.h>
#include "eviacamapp.h"
#include "viacamcontroller.h"

////@begin XPM images
////@end XPM images

#if !wxUSE_THREADS
     #error "This program requires thread support."
#endif // wxUSE_THREADS

/*!
 * Application instance implementation
 */

/*
////@begin implement app
IMPLEMENT_APP( EViacamApp )
////@end implement app
*/
// Under X11 it's necessary enable threading support (for XTestFakeRelativeMotionEvent call)
#if defined(__WXGTK__) || defined(__WXMOTIF__) || defined(__WXX11__)
IMPLEMENT_APP_NO_MAIN(EViacamApp)
#include <X11/Xlib.h>
#ifndef NDEBUG
#include <mcheck.h>
#endif
int main(int argc, char *argv[]) {
//printf( "XInitThreads - calling\n" );
#ifndef NDEBUG
//mtrace();
#endif
if ( XInitThreads() == 0 ) {
fprintf( stderr, "%s: Unable to initialize multithreaded X11 code (XInitThreads failed).\n", argv[0] );
exit( EXIT_FAILURE );
}
return wxEntry(argc, argv);
}
#else
IMPLEMENT_APP(EViacamApp)
#endif

/*!
 * EViacamApp type definition
 */

IMPLEMENT_CLASS( EViacamApp, wxApp )


/*!
 * EViacamApp event table definition
 */

BEGIN_EVENT_TABLE( EViacamApp, wxApp )

////@begin EViacamApp event table entries
////@end EViacamApp event table entries

	EVT_QUERY_END_SESSION(EViacamApp::OnEndSession)
END_EVENT_TABLE()

/*!
 * Constructor for EViacamApp
 */

EViacamApp::EViacamApp()
{
	Init();
}


/*!
 * Member initialisation
 */
void EViacamApp::Init()
{
	m_pController= NULL;

////@begin EViacamApp member initialisation
////@end EViacamApp member initialisation
}

/*!
 * Initialisation for EViacamApp
 */

bool EViacamApp::OnInit()
{     	
/* ////@begin EViacamApp initialisation */
	// Remove the comment markers above and below this block
	// to make permanent changes to the code.

#if wxUSE_XPM
	wxImage::AddHandler(new wxXPMHandler);
#endif
#if wxUSE_LIBPNG
	wxImage::AddHandler(new wxPNGHandler);
#endif
#if wxUSE_LIBJPEG
	wxImage::AddHandler(new wxJPEGHandler);
#endif
#if wxUSE_GIF
	wxImage::AddHandler(new wxGIFHandler);
#endif		
/* ////@end EViacamApp initialisation */

	// Uncomment this to enable a console in Windows for debug purposes
	//AllocConsole();
	//freopen("CONOUT$", "wb", stdout); 
	//printf ("Hola!\n");
	//fflush (stdout);

	m_pController= new CViacamController();
	assert (m_pController);
	if (!m_pController->Initialize()) 
	{
		OnExit();
		return false;
	}
	else return true;
}


/*!
 * Cleanup for EViacamApp
 */

int EViacamApp::OnExit()
{    
	m_pController->Finalize();
	delete m_pController;
	m_pController= NULL;

////@begin EViacamApp cleanup
	return wxApp::OnExit();
////@end EViacamApp cleanup
}

// 
// This handler terminates eViacam gracefully when the OS session is closed
//
void EViacamApp::OnEndSession( wxCloseEvent& event )
{		
	if (m_pController) m_pController->Finalize();
	event.Skip(false);
}

