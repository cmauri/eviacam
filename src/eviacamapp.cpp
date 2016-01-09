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
#include <wx/socket.h>
#include <wx/stdpaths.h>
#include <wx/cmdline.h>
#include <wx/xrc/xmlres.h>

#include "eviacamapp.h"
#include "paths.h"
#include "viacamcontroller.h"
#include "simplelog.h"

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
	// These functions are useful to pin down memory leaks on WIN32
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(489103);

	// Call default behaviour
	if (!wxApp::OnInit()) return false;

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

	// Set up globals (TODO: remove paths.cpp)
	eviacam::SetDataDir(wxStandardPaths::Get().GetDataDir());
	
	// Initialize sockets support
	// Note: (Workaround for implementation limitation for wxWidgets up to 2.5.x) 
	// If you want to use sockets or derived classes such as wxFTP in a secondary 
	// thread, call wxSocketBase::Initialize() (undocumented) from the main thread 
	// before creating any sockets - in wxApp::OnInit for example. 
	// See http://wiki.wxwidgets.org/wiki.pl?WxSocket or 
	// http://www.litwindow.com/knowhow/knowhow.html for more details.
	wxSocketBase::Initialize();

	// Initialize resources
	wxXmlResource::Get()->InitAllHandlers();
	wxXmlResource::Get()->LoadAllFiles(eviacam::GetDataDir() + wxT("/resources"));

	// Start main controller
	m_pController= new CViacamController();
	if (!m_pController->Initialize()) {
		OnExit();
		return false;
	}
	else
		return true;
}

static const wxCmdLineEntryDesc g_cmdLineDesc [] =
{
     { wxCMD_LINE_SWITCH, wxT_2("h"), wxT_2("help"), wxT_2("displays help on the command line parameters."),
          wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP },
     { wxCMD_LINE_SWITCH, wxT_2("d"), wxT_2("debug"), wxT_2("debug mode. Print debug messages to the console."),
          wxCMD_LINE_VAL_NONE, wxCMD_LINE_PARAM_OPTIONAL },
      
     { wxCMD_LINE_NONE }
};

void EViacamApp::OnInitCmdLine(wxCmdLineParser& parser)
{
    parser.SetDesc (g_cmdLineDesc);
    // must refuse '/' as parameter starter or cannot use "/path" style paths
    parser.SetSwitchChars (wxT("-"));
}
 
bool EViacamApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
	bool debug_mode= parser.Found(wxT("d"));
#ifndef NDEBUG
	debug_mode = true;
#endif

	if (debug_mode) {
		// Set log priority level
		slog_set_priority (SLOG_PRIO_DEBUG);

#if defined(WIN32)
		AllocConsole();
		freopen("CONOUT$", "wb", stdout);
		freopen("CONOUT$", "wb", stderr);
		slog_write (SLOG_PRIO_INFO, "debug mode enabled");
#endif
	}	

	return true;
}


/*!
 * Cleanup for EViacamApp
 */

int EViacamApp::OnExit()
{    
	m_pController->Finalize();
	delete m_pController;
	m_pController= NULL;

	wxSocketBase::Shutdown();

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

