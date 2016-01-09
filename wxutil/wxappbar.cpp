/////////////////////////////////////////////////////////////////////////////
// Name:        wxappbar.cpp
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

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include "wxappbar.h"
#include "warnbaroverlap.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#ifdef __WXMSW__
//#include <shellapi.h>
#endif

// X11 includes
#if defined(__WXGTK__) || defined(__WXMOTIF__) || defined(__WXX11__)
#include <X11/Xlib.h>
#include <X11/Xproto.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

#include <gdk/gdkx.h>
#include <gtk/gtkwidget.h>
#include <gtk/gtk.h>

#endif

#include <wx/timer.h>

#define AUTOHIDE_FLANGE 10
#define TIMER_ID 1234

// X11 definitions and structs
#define WIN_STATE_STICKY          (1<<0)	/* everyone knows sticky */
#define WIN_STATE_MINIMIZED       (1<<1)	/* ??? */
#define WIN_STATE_MAXIMIZED_VERT  (1<<2)	/* window in maximized V state */
#define WIN_STATE_MAXIMIZED_HORIZ (1<<3)	/* window in maximized H state */
#define WIN_STATE_HIDDEN          (1<<4)	/* not on taskbar but window visible */
#define WIN_STATE_SHADED          (1<<5)	/* shaded (NeXT style) */
#define WIN_STATE_HID_WORKSPACE   (1<<6)	/* not on current desktop */
#define WIN_STATE_HID_TRANSIENT   (1<<7)	/* owner of transient is hidden */
#define WIN_STATE_FIXED_POSITION  (1<<8)	/* window is fixed in position even */
#define WIN_STATE_ARRANGE_IGNORE  (1<<9)	/* ignore for auto arranging */

#define WIN_HINTS_SKIP_FOCUS      (1<<0)	/* "alt-tab" skips this win */
#define WIN_HINTS_SKIP_WINLIST    (1<<1)	/* not in win list */
#define WIN_HINTS_SKIP_TASKBAR    (1<<2)	/* not on taskbar */
#define WIN_HINTS_GROUP_TRANSIENT (1<<3)	/* ??????? */
#define WIN_HINTS_FOCUS_ON_CLICK  (1<<4)	/* app only accepts focus when clicked */
#define WIN_HINTS_DO_NOT_COVER    (1<<5)	/* attempt to not cover this window */

#define MWM_HINTS_DECORATIONS         (1L << 1)

// Old KDE hints
//_NET_WM_LAYER

#define _NET_WIN_LAYER_DESKTOP                0
#define _NET_WIN_LAYER_BELOW                  2
#define _NET_WIN_LAYER_NORMAL                 4
#define _NET_WIN_LAYER_ONTOP                  6
#define _NET_WIN_LAYER_DOCK                   8
#define _NET_WIN_LAYER_ABOVE_DOCK             10
#define _NET_WIN_LAYER_MENU                   12

// _NET_WM_HINTS
// Additional hints for the window manager or tools like panels or taskbars. Possible values:

#define _NET_WM_HINTS_SKIP_FOCUS (1<<0) /* "alt-tab" skips this win */
#define _NET_WM_HINTS_SKIP_WINLIST (1<<1)  /* do not show in window list */
#define _NET_WM_HINTS_SKIP_TASKBAR (1<<2) /* do not show on taskbar */
#define _NET_WM_HINTS_NO_AUTO_FOCUS (1<<3) /* do not automatically put focus on this window when it pops up */
#define _NET_WM_HINTS_STANDALONE_MENUBAR  (1<<4) /* this window is a standalone menubar */
#define _NET_WM_HINTS_FIXED_POSITION  (1<<5) /* this window has a fixed position (should be excluded from desktop uncluttering etc.) */
#define _NET_WM_HINTS_DO_NOT_COVER (1<<6) /* attempt to never cover up this window if possible (placement policy priority hint)*/

#define _NET_WM_STATE_REMOVE        0    /* remove/unset property */
#define _NET_WM_STATE_ADD           1    /* add/set property */
#define _NET_WM_STATE_TOGGLE        2    /* toggle property  */

// Declare/define an event type for the appbar callback 
// used only under MSW
BEGIN_DECLARE_EVENT_TYPES()
	DECLARE_EVENT_TYPE(WX_APPBAR_CALLBACK, -1)
END_DECLARE_EVENT_TYPES()

DEFINE_EVENT_TYPE(WX_APPBAR_CALLBACK)


/*!
 * WXAppBar type definition
 */

IMPLEMENT_DYNAMIC_CLASS( WXAppBar, wxDialog )


/*!
 * WXAppBar event table definition
 */

BEGIN_EVENT_TABLE( WXAppBar, wxDialog )
	EVT_SIZE( WXAppBar::OnSize )
	EVT_MOVE( WXAppBar::OnMove )
	EVT_ENTER_WINDOW( WXAppBar::OnEnterWindow )
	EVT_LEAVE_WINDOW( WXAppBar::OnLeaveWindow )
	EVT_TIMER( TIMER_ID, WXAppBar::OnTimer )
END_EVENT_TABLE()

/*
typedef struct _mwmhints
{
        unsigned long flags;
        unsigned long functions;
        unsigned long decorations;
        long inputMode;
        unsigned long status;
}
MWMHints;
*/
#if defined(__WXGTK__) || defined(__WXMOTIF__) || defined(__WXX11__)
static 
bool IsMappedWindow(Display *dd, Window w)
{
	XWindowAttributes attr;

	XGetWindowAttributes(dd, w, &attr);
	
	return (attr.map_state != IsUnmapped);
}

#if 0
static 
void wxWMspecSetState(Display *dd, Window w, int operation, Atom state)
{
	Atom atom= XInternAtom (dd, "_NET_WM_STATE", False);
	assert (atom!= BadAlloc);
	assert (atom!= BadAtom);
	assert (atom!= BadValue);
	
	int screen = DefaultScreen (dd);
	Window rootWin= RootWindow (dd, screen);
	XEvent xev;

	assert(IsMappedWindow(dd, w));

	xev.type = ClientMessage;
	xev.xclient.type = ClientMessage;
	xev.xclient.serial = 0;
	xev.xclient.send_event = True;
	xev.xclient.display = dd;
	xev.xclient.window = w;
	xev.xclient.message_type = atom;
	xev.xclient.format = 32;
	xev.xclient.data.l[0] = operation;
	xev.xclient.data.l[1] = state;
	xev.xclient.data.l[2] = None;
	
	XSendEvent(dd, rootWin, False, SubstructureRedirectMask | SubstructureNotifyMask, &xev);
}
#endif
#endif



/*!
 * WXAppBar constructors
 */

WXAppBar::WXAppBar() : m_timer(this, TIMER_ID)
{
	Init();
}

WXAppBar::WXAppBar( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style ) : m_timer(this, TIMER_ID)
{
	Init();
	Create(parent, id, caption, pos, size, style);	
}


/*!
 * WXAppBar creator
 */

bool WXAppBar::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
	bool retval= wxDialog::Create( parent, id, caption, pos, size, style );
	// Sticky window by default
	SetSticky(true);
	return retval;
}


/*!
 * WXAppBar destructor
 */

WXAppBar::~WXAppBar()
{
	delete m_pMouseControl;
	delete m_pDialog;
}


/*!
 * Member initialisation
 */

void WXAppBar::Init()
{
	m_X= 0;
	m_Y= 0;
	m_Width= -1;
	m_Height= -1;
	m_currentDockingMode= NON_DOCKED;
	m_dialogHadBorderDecorations= GetBorderDecorations();
	m_firstTime= true;
	m_autohide=  false;
	m_isAutohideWindowShown= false;
	m_warnBarOverlap= true;
	m_pDialog = new WarnBarOverlap(NULL);
#if defined(__WXGTK__)
	m_pMouseControl = new CMouseControl ((void *) wxGetDisplay());
#else
	m_pMouseControl= new CMouseControl ();
#endif
}


// Get whether the dialog has borders or not
bool WXAppBar::GetBorderDecorations () const
{
	// Honours the wxNO_BORDER flag
	return (GetWindowStyleFlag() & wxNO_BORDER? false : true);	
}

// Set whether the dialog has borders or not
void WXAppBar::SetBorderDecorations (bool enable, bool apply)
{
	if (enable == GetBorderDecorations()) return;
	
	// Changes the flag
	long style= GetWindowStyleFlag();
	if (enable) {
		// Enable decorations
		style= style & ~wxNO_BORDER;
//		style= style | wxCAPTION;
	}
	else {
		// Disable decorations
		style= style | wxNO_BORDER;
//		style= style & ~wxCAPTION;
	}
	SetWindowStyleFlag(style);
	// According to the manual, after changing flags a Refresh is needed
	Refresh();

#if defined(__WXMSW__)
	// TODO
	(void)(apply); // Remove warning
	assert (false);
#elif defined(__WXGTK__)
	//
	// On WXGTK the above code is not enough, so we change this property
	// using gtk+ directly
	//
	
	// Get X11 handle for our window
	GtkWindow *gtkWindow= (GtkWindow *) GetHandle();
	assert (gtkWindow);
	if (!gtkWindow) return;

	bool isShown= IsShown();
	if (apply && isShown) wxDialog::Show(false);
	
	gtk_window_set_decorated ((GtkWindow *) GetHandle(), (enable? TRUE : FALSE));
	if (apply && isShown) {
		wxDialog::Show(true);
		Refresh();
		Update();
	}
#else
	assert (false);
#endif
}

void WXAppBar::SetSticky (bool stick)
{
#if defined(__WXMSW__)
	// TODO
	(void)(stick); // Avoid compilation warning

#elif defined(__WXGTK__)	
	// Get X11 handle for our window
	GtkWindow *gtkWindow= (GtkWindow *) GetHandle();
	assert (gtkWindow);
	if (!gtkWindow) return;

	if (stick) 
		gtk_window_stick (gtkWindow);
	else 
		gtk_window_unstick (gtkWindow);	
#endif
}

void WXAppBar::SetEntryInTaskBar (bool v)
{
#if defined(__WXMSW__)
	// TODO
	(void)(v); // Remove warning
	//assert (false);
#elif defined(__WXGTK__)	
	// Get X11 handle for our window
	GtkWindow *gtkWindow= (GtkWindow *) GetHandle();
	assert (gtkWindow);
	if (!gtkWindow) return;

	gtk_window_set_skip_taskbar_hint (gtkWindow, (v? FALSE : TRUE));
#else
	assert (false);
#endif
}
	
void WXAppBar::SetEntryInPager (bool v)
{
#if defined(__WXMSW__)
	// TODO
	(void)(v);	// Remove warning
	//assert (false);
#elif defined(__WXGTK__)	
	// Get X11 handle for our window
	GtkWindow *gtkWindow= (GtkWindow *) GetHandle();
	assert (gtkWindow);
	if (!gtkWindow) return;
	
	gtk_window_set_skip_pager_hint (gtkWindow, (v? FALSE : TRUE));
#else
	assert (false);
#endif
}

void WXAppBar::SetAcceptFocus (bool accept)
{
#if defined(__WXMSW__)
	// TODO
	(void)(accept);	// Remove warning
	//assert (false);
#elif defined(__WXGTK__)	
	// Get X11 handle for our window
	GtkWindow *gtkWindow= (GtkWindow *) GetHandle();
	assert (gtkWindow);
	if (!gtkWindow) return;
	
	gtk_window_set_accept_focus (gtkWindow, (accept? TRUE : FALSE));
#else
	assert (false);
#endif
}

#if defined(__WXMSW__)
//void WXAppBar::AppBarCallback (UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
	/*
    APPBARDATA abd;
 	static HWND hwndZOrder = NULL;
	POPTIONS pOpt = GetAppbarData ();
    
    abd.cbSize = sizeof(abd);
    abd.hWnd = m_hWnd;
    
    switch (wParam) 
    {
        // Notifies the appbar that the taskbar's autohide or always-on-top 
        // state has changed.  The appbar can use this to conform to the settings
        // of the system taskbar.
	    case ABN_STATECHANGE:
	        break;

        // Notifies the appbar when a full screen application is opening or 
        // closing.  When a full screen app is opening, the appbar must drop
        // to the bottom of the Z-Order.  When the app is closing, we should 
        // restore our Z-order position.
	    case ABN_FULLSCREENAPP:
	        if (lParam) 
	        {
                // A full screen app is opening.  Move us to the bottom of the 
                // Z-Order.  

				// First get the window that we're underneath so we can correctly
				// restore our position
				hwndZOrder = GetWindow (m_hWnd, GW_HWNDPREV);
				
				// Now move ourselves to the bottom of the Z-Order
	            SetWindowPos(m_hWnd, HWND_BOTTOM, 0, 0, 0, 0, 
	            			 SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);            
	        } 
	        else 
	        {
                // The app is closing.  Restore the Z-order			   
                SetWindowPos(m_hWnd, pOpt->fOnTop ? HWND_TOPMOST : hwndZOrder,
	                         0, 0, 0, 0, 
	                         SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
				
				hwndZOrder = NULL;
	        }
			break;
        
        // Notifies the appbar when an event has occured that may effect the 
        // appbar's size and position.  These events include changes in the 
        // taskbar's size, position, and visiblity as well as adding, removing,
        // or resizing another appbar on the same side of the screen.
    	case ABN_POSCHANGED:
            // Update our position in response to the system change
        	PosChanged (&abd);
        	break;
    }
	*/
//}

#endif


void WXAppBar::OnSize( wxSizeEvent& event )
{
	// TODO: delete
//	event.Skip(false);
	
#if defined(__WXMSW__)
//	SetSize(m_X, m_Y, m_Width, m_Height, 0);
//	Raise ();	
#else
	//wxSize s= event.GetSize();
	//printf ("onSize: %d, %d, orig: %d, %d\n", s.GetWidth(), s.GetHeight(), m_Width, m_Height);
	/*
	if (s.GetWidth()!= m_Width || s.GetHeight()!= m_Height) {
		SetSize (m_Width, m_Height);
		event.Skip(false);
	}
	else
		wxDialog::OnSize (event);
	*/
#endif
	wxDialog::OnSize (event);
}

void WXAppBar::OnMove( wxMoveEvent& event )
{
#if defined(__WXMSW__)
	SetSize(m_X, m_Y, m_Width, m_Height, 0);
	Raise ();
#endif
	//wxPoint p= event.GetPosition();
	
//	printf ("onMove: %d, %d, orig: %d, %d\n", p.x, p.y, m_X, m_Y);
//	wxDialog::OnMove (event);
	event.Skip();
}

bool WXAppBar::ProcessEvent(wxEvent& event)
{
	return wxDialog::ProcessEvent (event);
}

void WXAppBar::SetDockingMode (EDocking dockingMode)
{
	if (m_currentDockingMode== dockingMode) return;

	// If is shown, hide
	bool isShown= IsShown();

	if (isShown) Show(false);

	m_currentDockingMode= dockingMode;
	
	if (isShown) Show(true);
}

void WXAppBar::SetAutohideMode (bool autohide)
{
	if (m_autohide == autohide) return;

	// TODO: check this
	
	// If is shown, hide
	bool isShown= IsShown();
	if (isShown) Show(false);
	
	m_autohide= autohide;

	if (isShown) Show(true);
}

void WXAppBar::CheckCreateWindow()
{
#if defined(__WXGTK__)
	//
	// Show & update the window to make sure that is actually created the first time
	//
	if (m_firstTime) {
		wxDialog::Show(true);
		Refresh();
		Update();
		
		wxDialog::Show(false);
		Refresh();
		Update();
		m_firstTime= false;
	}
#endif
	// TODO: check if it is needed for Windows
}

#if defined(__WXGTK__)
static
void GetDesktopDimensions (Display* d, int& x, int& y, int& width, int& height, int& screenWidth, int& screenHeight)
{
	// Try to get real work area to set metrics
	Atom atomTmp = XInternAtom(d, "_NET_WORKAREA", False);
	Atom actual_type;
	int actual_format;
	unsigned long nitems;
	unsigned long bytes_after;
	unsigned long *prop;
	int screen = DefaultScreen (d);
		
	// Get desktop size
	XGetWindowProperty (d, DefaultRootWindow(d), atomTmp, 0, 4, False, AnyPropertyType, &actual_type, &actual_format, &nitems, &bytes_after, (unsigned char **) &prop);

	screenWidth= DisplayWidth (d, screen);
	screenHeight= DisplayHeight (d, screen);
	
	if (prop!= NULL) {
		x= (int) prop[0];
		y= (int) prop[1];
		width= (int)prop[2];
		height= (int)prop[3];
		XFree (prop);
	}
	else {
		// Fallback, just in case. Assert in debug mode
		assert (false);
		
		x= 0;
		y= 0;
		width = screenWidth;
		height = screenHeight;
	}
}

// Reserves an area for the X11 window 'w' in one of the sides of the desktop depending
// on the value of 'where'. If 'where== NON_DOCKED' or area== 0 then the area is freed.
static
void SetStrutArea (Window w, WXAppBar::EDocking where, int area)
{
	Display* dd= (Display *) wxGetDisplay();
	
	//
	// Get desktop working area dimensions
	//
	int xDesktop, yDesktop, widthDesktop, heightDesktop, screenWidth, screenHeight;
	GetDesktopDimensions (dd, xDesktop, yDesktop, widthDesktop, heightDesktop, screenWidth, screenHeight);
	
	//
	// Reserves an area in the desktop.
	//
	// It seems that for older GNOME version (ex: 2.22.3 on Debian, using a partial strut 
	// doesn't work properly,  more TESTING NEEDED)
	//
	unsigned long strut[4] = {0, 0, 0, 0};
	//unsigned long strut_p[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	/* left, right, top, bottom, left_start_y, left_end_y, right_start_y, right_end_y, top_start_x, top_end_x, bottom_start_x, bottom_end_x*/
	if (area) {
		switch (where) {
		case WXAppBar::TOP_DOCKED:
			strut[2]=  area + yDesktop;
			//strut_p[2]= strut[2];
			break;
		case WXAppBar::BOTTOM_DOCKED:	
			strut[3]= screenHeight - heightDesktop - yDesktop + area;
			//strut_p[3]= strut[3];
			break;
		case WXAppBar::LEFT_DOCKED:
			strut[0]= area + xDesktop;
			//strut_p[0]= strut[0];
			break; 
		case WXAppBar::RIGHT_DOCKED:
			strut[1]= screenWidth - widthDesktop - xDesktop + area;
			//strut_p[1]= strut[1];
			break;
		case WXAppBar::NON_DOCKED:
			break;
		default:
			assert (false);
		}
	}
	
	//atomTmp = XInternAtom (dd, "_NET_WM_STRUT_PARTIAL", False);	
	//XChangeProperty (dd, w, atomTmp, XA_CARDINAL, 32, PropModeReplace, (unsigned char *) &strut_p, 12);
	
	Atom atomTmp = XInternAtom (dd, "_NET_WM_STRUT", False);
	XChangeProperty (dd, w, atomTmp, XA_CARDINAL, 32, PropModeReplace, (unsigned char *) &strut, 4);
	XSync(dd, False);
	
	// If window mapped, then wait until strut applied
	// TODO: is there a better way to do this?	
	if (IsMappedWindow(dd, w)) {
		int new_xDesktop, new_yDesktop, new_widthDesktop, new_heightDesktop, count= 0;
		for (;;) {
			GetDesktopDimensions (dd, new_xDesktop, new_yDesktop, new_widthDesktop, 
				new_heightDesktop, screenWidth, screenHeight);
			++count;
			if (!(count< 20 && new_xDesktop== xDesktop && new_yDesktop== yDesktop &&
			new_widthDesktop== widthDesktop && new_heightDesktop == heightDesktop)) break;
			usleep (100000);
		}
	}	
}



void WXAppBar::SetDockedModeStep1()
{
	//CheckCreateWindow();

	//
	// Get X11 display
	//
	Display* dd= (Display *) wxGetDisplay(); assert (dd);
	
	//
	// Get desktop working area dimensions
	//
	int xDesktop, yDesktop, widthDesktop, heightDesktop, screenWidth, screenHeight;
	GetDesktopDimensions (dd, xDesktop, yDesktop, widthDesktop, heightDesktop, screenWidth, screenHeight);
	
	//
	// As we need to dock the window disable decorations
	//
	m_dialogHadBorderDecorations= GetBorderDecorations();
	SetBorderDecorations(false);
	
	//
	// Get X11 handle for our window
	//
	GtkWidget *gtkWidget= (GtkWidget *) this->GetHandle();
	Window w= GDK_WINDOW_XWINDOW (gtkWidget->window);
	
	// Get original dimensions of the bar
	wxSize proposedSize= GetBestSize();

	// Compute bar position and size depending on docking mode
	switch (m_currentDockingMode) {
	case TOP_DOCKED:
		m_X= xDesktop;
		m_Y= yDesktop;
		m_Width= widthDesktop;
		m_Height= proposedSize.GetHeight();
		break;
	case BOTTOM_DOCKED:
		m_X= xDesktop;
		m_Y= yDesktop + heightDesktop - proposedSize.GetHeight();
		m_Width= widthDesktop;
		m_Height= proposedSize.GetHeight();
		break;
	case LEFT_DOCKED:
		m_X= xDesktop;
		m_Y= yDesktop;
		m_Width= proposedSize.GetWidth();
		m_Height= heightDesktop;
		break; 
	case RIGHT_DOCKED:
		m_X= xDesktop + widthDesktop - proposedSize.GetWidth();
		m_Y= yDesktop;
		m_Width= proposedSize.GetWidth();
		m_Height= heightDesktop;
		break;
	case NON_DOCKED:
	default:
		assert (false);
	}
	
	//
	// Reserves an area in the desktop.
	//
	switch (m_currentDockingMode) {
	case TOP_DOCKED: 	SetStrutArea (w, TOP_DOCKED, m_Height); break;
	case BOTTOM_DOCKED:	SetStrutArea (w, BOTTOM_DOCKED, m_Height); break;
	case LEFT_DOCKED:	SetStrutArea (w, LEFT_DOCKED, m_Width); break; 	
	case RIGHT_DOCKED:	SetStrutArea (w, RIGHT_DOCKED, m_Width); break;
	case NON_DOCKED:
	default:
		assert (false);
	}

	//
	// Functional type of the window (_NET_WM_WINDOW_TYPE)
	//
	Atom atomTmp= XInternAtom (dd, "_NET_WM_WINDOW_TYPE", False);
	Atom atom_NET_WM_WINDOW_TYPE_DOCK= XInternAtom (dd, "_NET_WM_WINDOW_TYPE_DOCK", False);
	Atom atom_NET_WM_WINDOW_TYPE_NORMAL= XInternAtom (dd, "_NET_WM_WINDOW_TYPE_NORMAL", False);
	unsigned long propInfo[2];
	propInfo[0]= atom_NET_WM_WINDOW_TYPE_DOCK;
	propInfo[1]= atom_NET_WM_WINDOW_TYPE_NORMAL;		
	XChangeProperty (dd, w, atomTmp, XA_ATOM, 32, PropModeReplace, (unsigned char *) &propInfo[0], 2);
	SetSticky(true);
	XSync(dd, False);

	// Set desired location and dimensions
	SetSize(m_X, m_Y, m_Width, m_Height);
}

void WXAppBar::SetDockedModeStep2()
{
	// Do nothing
}

void WXAppBar::UnSetDockedModeStep1()
{
	// Disable all special features
	//Display *dd= (Display *) wxGetDisplay();

	// Window X11 handle
	GtkWidget *gtkWidget= (GtkWidget *) this->GetHandle();
	Window w= GDK_WINDOW_XWINDOW (gtkWidget->window);
	
	// Disables struts
	SetStrutArea (w, NON_DOCKED, 0);
}

void WXAppBar::UnSetDockedModeStep2()
{
	Display *dd= (Display *) wxGetDisplay();

	// Window X11 handle
	GtkWidget *gtkWidget= (GtkWidget *) this->GetHandle();
	Window w= GDK_WINDOW_XWINDOW (gtkWidget->window);
	
	Refresh();
	Update();
	
	//
	// Set window style back to normal again
	//
	Atom atomTmp= XInternAtom (dd, "_NET_WM_WINDOW_TYPE", False);
	Atom atom_NET_WM_WINDOW_TYPE_NORMAL= XInternAtom (dd, "_NET_WM_WINDOW_TYPE_NORMAL", False);
	unsigned long propInfo= atom_NET_WM_WINDOW_TYPE_NORMAL;
	XChangeProperty (dd, w, atomTmp, XA_ATOM, 32, PropModeReplace, (unsigned char *) &propInfo, 1);
	XSync(dd, False);
	
	// The code above disables the sticky property, so we enable it again
	SetSticky(true);
	
	// Restore decorations when needed
	SetBorderDecorations(m_dialogHadBorderDecorations);
	
	// Restore original size
	wxSize proposedSize= DoGetBestSize();
	//SetSize (0, 0, proposedSize.GetWidth(), proposedSize.GetHeight());
	SetSize (proposedSize.GetWidth(), proposedSize.GetHeight());
}

void WXAppBar::SetAutohideModeStep ()
{
	//CheckCreateWindow();

	//
	// Get X11 display
	//
	Display* dd= (Display *) wxGetDisplay(); assert (dd);
	
	//
	// Get desktop working area dimensions
	//
	int xDesktop, yDesktop, widthDesktop, heightDesktop, screenWidth, screenHeight;
	GetDesktopDimensions (dd, xDesktop, yDesktop, widthDesktop, heightDesktop, screenWidth, screenHeight);
	
	//
	// As we need to dock the window disable decorations
	//
	m_dialogHadBorderDecorations= GetBorderDecorations();
	SetBorderDecorations(false);
	
	//
	// Get X11 handle for our window
	//
	GtkWidget *gtkWidget= (GtkWidget *) this->GetHandle();
	Window w= GDK_WINDOW_XWINDOW (gtkWidget->window);
	
	// Get original dimensions of the bar
	wxSize proposedSize= GetBestSize();

	// Compute bar position and size depending on docking mode
	m_Width= proposedSize.GetWidth();
	m_Height= proposedSize.GetHeight();
	
	switch (m_currentDockingMode) {
		case TOP_DOCKED:
			m_X= (screenWidth - proposedSize.GetWidth())/2;
			m_Y= 0 - proposedSize.GetHeight() + AUTOHIDE_FLANGE;
			break;
		case BOTTOM_DOCKED:
			m_X= (screenWidth - proposedSize.GetWidth())/2;
			m_Y= screenHeight - AUTOHIDE_FLANGE;
			break;
		case LEFT_DOCKED:
			m_X= 0 - proposedSize.GetWidth() + AUTOHIDE_FLANGE;
			m_Y= (screenHeight - proposedSize.GetHeight())/2;
			break; 
		case RIGHT_DOCKED:
			m_X= screenWidth - AUTOHIDE_FLANGE;
			m_Y= (screenHeight - proposedSize.GetHeight())/2;
			break;
		case NON_DOCKED:
		default:
			assert (false);
	}
	
	
	//
	// Functional type of the window (_NET_WM_WINDOW_TYPE)
	//
	Atom atomTmp= XInternAtom (dd, "_NET_WM_WINDOW_TYPE", False);
	Atom atom_NET_WM_WINDOW_TYPE_DOCK= XInternAtom (dd, "_NET_WM_WINDOW_TYPE_DOCK", False);
	Atom atom_NET_WM_WINDOW_TYPE_NORMAL= XInternAtom (dd, "_NET_WM_WINDOW_TYPE_NORMAL", False);
	unsigned long propInfo[2];
	propInfo[0]= atom_NET_WM_WINDOW_TYPE_DOCK;
	propInfo[1]= atom_NET_WM_WINDOW_TYPE_NORMAL;		
	XChangeProperty (dd, w, atomTmp, XA_ATOM, 32, PropModeReplace, (unsigned char *) &propInfo[0], 2);
	SetSticky(true);
	XSync(dd, False);

	// Set desired location and dimensions
	SetSize(m_X, m_Y, m_Width, m_Height);	
}


#elif defined(__WXMSW__)

void WXAppBar::SetDockedModeStep1()
{
	// TODO: manage different docking locations on windows
	APPBARDATA abd;		

	// Set metrics (TOP location only)
	wxSize proposedSize= DoGetBestSize();
	m_X= 0;
	m_Y= 0;
	m_Width= GetSystemMetrics(SM_CXSCREEN);
	m_Height= proposedSize.GetHeight();	

	// Register appbar
	abd.cbSize= sizeof(APPBARDATA);
	abd.hWnd= (HWND) GetHandle();
	abd.uCallbackMessage= 12345; //WX_APPBAR_CALLBACK; // TODO: respond to these callback messages
	SHAppBarMessage (ABM_NEW, &abd);

	// Set size
	abd.uEdge= ABE_TOP;
	abd.rc.left = m_X;
	abd.rc.top = m_Y;
	abd.rc.right = m_X + m_Width;
	abd.rc.bottom = m_Y + m_Height;

	SHAppBarMessage(ABM_QUERYPOS,&abd);	// Correct size if needed

	m_X= abd.rc.left;
	m_Y= abd.rc.top;		
	m_Width= abd.rc.right - m_X;
	// Re-calculate bottom corner
	abd.rc.bottom = m_Y + m_Height;		
	SHAppBarMessage(ABM_SETPOS, &abd);
}

void WXAppBar::SetDockedModeStep2()
{
	SetSize(m_X, m_Y, m_Width, m_Height, 0);
	Raise();
}

void WXAppBar::UnSetDockedModeStep1()
{
	// Do nothing
}

void WXAppBar::UnSetDockedModeStep2()
{
	// Unregister app bar
	APPBARDATA abd;	
	abd.cbSize= sizeof(APPBARDATA);
	abd.hWnd= (HWND) GetHandle();	
	SHAppBarMessage (ABM_REMOVE, &abd);
}

void WXAppBar::SetAutohideModeStep ()
{
	//TODO
}
#endif

bool WXAppBar::Show (bool show)
{
	if (show== IsShown ()) return false;
	
	// If no docking enabled, simply forward the call
	if (m_currentDockingMode == NON_DOCKED) {
		m_firstTime= false;
		return wxDialog::Show (show);
	}
	else {
		CheckCreateWindow();
		
		// "Docking mode" enabled
		if (!m_autohide) {
			// Normal docking
			if (show) {
				SetDockedModeStep1();
				wxDialog::Show(true);
				SetDockedModeStep2();
			}
			else {
				UnSetDockedModeStep1();
				wxDialog::Show(false);
				UnSetDockedModeStep2();
			}
		}
		else {
			if (show) {
				if (CheckForBar() && m_warnBarOverlap)
					SetWarnBarOverlap(!(m_pDialog->ShowModal() ? true : false));
				SetAutohideModeStep();
				wxDialog::Show(true);
			}
			else {
				UnSetDockedModeStep1();
				wxDialog::Show(false);
				UnSetDockedModeStep2();
			}
		}
	}
	
	return true;
}


void WXAppBar::OnEnterWindow( wxMouseEvent& event )
{
#if defined(__WXGTK__)
	if (m_autohide && m_currentDockingMode != NON_DOCKED && !m_isAutohideWindowShown)
	{
		// Get X11 display
		Display* dd= (Display *) wxGetDisplay(); assert (dd);
		
		// Get desktop working area dimensions
		int xDesktop, yDesktop, widthDesktop, heightDesktop, screenWidth, screenHeight;
		GetDesktopDimensions (dd, xDesktop, yDesktop, widthDesktop, heightDesktop, screenWidth, screenHeight);

		// Get original dimensions of the bar
		wxSize proposedSize= GetBestSize();

		// Compute bar position and size depending on docking mode
		m_Width= proposedSize.GetWidth();
		m_Height= proposedSize.GetHeight();
	
		switch (m_currentDockingMode) {
			case TOP_DOCKED:
				m_X= (screenWidth - proposedSize.GetWidth())/2;
				m_Y= 0;
				break;
			case BOTTOM_DOCKED:
				m_X= (screenWidth - proposedSize.GetWidth())/2;
				m_Y= screenHeight - proposedSize.GetHeight();
				break;
			case LEFT_DOCKED:
				m_X= 0;
				m_Y= (screenHeight - proposedSize.GetHeight())/2;
				break; 
			case RIGHT_DOCKED:
				m_X= screenWidth - proposedSize.GetWidth();
				m_Y= (screenHeight - proposedSize.GetHeight())/2;
				break;
			case NON_DOCKED:
			default:
				assert (false);
		}
		
		// Set desired location and dimensions
		SetSize(m_X, m_Y, m_Width, m_Height);
		
		m_isAutohideWindowShown= true;
	}
#endif
	event.Skip(true);
}

void WXAppBar::OnLeaveWindow( wxMouseEvent& event )
{
#if defined(__WXGTK__)
	int x, y;
	
	m_pMouseControl->GetPointerLocation (x, y);
	wxRect barRect = GetRect();
	
	if (m_autohide && m_currentDockingMode != NON_DOCKED && m_isAutohideWindowShown && !barRect.Contains(x,y))
	{
		m_timer.Start(1000);
	}
#endif
	event.Skip(true);
}

void WXAppBar::OnTimer(wxTimerEvent& event)
{
#if defined(__WXGTK__)
	int x, y;
	
	m_pMouseControl->GetPointerLocation (x, y);
	wxRect barRect = GetRect();
	
	if (m_autohide && m_currentDockingMode != NON_DOCKED && m_isAutohideWindowShown && !barRect.Contains(x,y))
	{
		// Get X11 display
		Display* dd= (Display *) wxGetDisplay(); assert (dd);
		
		// Get desktop working area dimensions
		int xDesktop, yDesktop, widthDesktop, heightDesktop, screenWidth, screenHeight;
		GetDesktopDimensions (dd, xDesktop, yDesktop, widthDesktop, heightDesktop, screenWidth, screenHeight);

		// Get original dimensions of the bar
		wxSize proposedSize= GetBestSize();

		// Compute bar position and size depending on docking mode
		m_Width= proposedSize.GetWidth();
		m_Height= proposedSize.GetHeight();
	
		switch (m_currentDockingMode) {
			case TOP_DOCKED:
				m_X= (screenWidth - proposedSize.GetWidth())/2;
				m_Y= 0 - proposedSize.GetHeight() + AUTOHIDE_FLANGE;
				break;
			case BOTTOM_DOCKED:
				m_X= (screenWidth - proposedSize.GetWidth())/2;
				m_Y= screenHeight - AUTOHIDE_FLANGE;
				break;
			case LEFT_DOCKED:
				m_X= 0 - proposedSize.GetWidth() + AUTOHIDE_FLANGE;
				m_Y= (screenHeight - proposedSize.GetHeight())/2;
				break; 
			case RIGHT_DOCKED:
				m_X= screenWidth - AUTOHIDE_FLANGE;
				m_Y= (screenHeight - proposedSize.GetHeight())/2;
				break;
			case NON_DOCKED:
			default:
				assert (false);
		}
		
		// Set desired location and dimensions
		SetSize(m_X, m_Y, m_Width, m_Height);
		
		m_isAutohideWindowShown= false;
	}
#endif
	event.Skip(false);
}

void WXAppBar::SetWarnBarOverlap(bool value)
{
	m_warnBarOverlap= value;
}

bool WXAppBar::CheckForBar()
{
#if defined(__WXGTK__)
	// Get X11 display
	Display* dd= (Display *) wxGetDisplay(); assert (dd);
		
	// Get desktop working area dimensions
	int xDesktop, yDesktop, widthDesktop, heightDesktop, screenWidth, screenHeight;
	GetDesktopDimensions (dd, xDesktop, yDesktop, widthDesktop, heightDesktop, screenWidth, screenHeight);
	
	switch (m_currentDockingMode)
	{
		case TOP_DOCKED:
			return (yDesktop > 0);
			break;
			
		case BOTTOM_DOCKED:
			return ((screenHeight - yDesktop - heightDesktop) > 0);
			break;
			
		case LEFT_DOCKED:
			return (xDesktop > 0);
			break;
			
		case RIGHT_DOCKED:
			return ((screenWidth - xDesktop - widthDesktop) > 0);
			break;
			
		case NON_DOCKED:
		default:
			return false;
			break;
	}
#endif
	return false;
}
