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

// X11 includes
#if defined(__WXGTK__) || defined(__WXMOTIF__) || defined(__WXX11__)

#include <X11/Xlib.h>
#include <X11/Xproto.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

#include <gdk/gdkx.h>
#include <gtk/gtkwidget.h>

#endif

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
//    EVT_SIZE( WXAppBar::OnSize )
	EVT_MOVE( WXAppBar::OnMove )
END_EVENT_TABLE()

// X11 definitions and structs
#if defined(__WXGTK__) || defined(__WXMOTIF__) || defined(__WXX11__)

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

static bool IsMappedWindow(Display *dd, Window w)
{
	XWindowAttributes attr;

	XGetWindowAttributes(dd, w, &attr);
	
	return (attr.map_state != IsUnmapped);
}

static void wxWMspecSetState(Display *dd, Window w, int operation, Atom state)
{
	Atom atom= XInternAtom (dd, "_NET_WM_STATE", False);
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


/*!
 * WXAppBar constructors
 */

WXAppBar::WXAppBar()
{
    Init();
}

WXAppBar::WXAppBar( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
	Init();
	Create(parent, id, caption, pos, size, SYMBOL_CWXAPPBAR_STYLE);	// Force window style
	wxUnusedVar(style);
}


/*!
 * WXAppBar creator
 */

bool WXAppBar::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    return wxDialog::Create( parent, id, caption, pos, size, style );
}


/*!
 * WXAppBar destructor
 */

WXAppBar::~WXAppBar()
{
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

/*
void WXAppBar::OnSize( wxSizeEvent& event )
{
	// TODO: delete
	event.Skip(false);

#if defined(__WXMSW__)
//	SetSize(m_X, m_Y, m_Width, m_Height, 0);
//	Raise ();
#endif
	wxDialog::OnSize (event);
}*/

void WXAppBar::OnMove( wxMoveEvent& WXUNUSED(event) )
{
#if defined(__WXMSW__)
	SetSize(m_X, m_Y, m_Width, m_Height, 0);
	Raise ();
#endif
}

bool WXAppBar::ProcessEvent(wxEvent& event)
{
	return wxDialog::ProcessEvent (event);
}

bool WXAppBar::Show (bool show)
{
	if (show== IsShown ()) return false;
		
#if defined(__WXMSW__)
	if (show)
	{
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
		
		wxDialog::Show (true);
		SetSize(m_X, m_Y, m_Width, m_Height, 0);
		Raise();
	}
	else
	{
		APPBARDATA abd;	
		
		wxDialog::Show (false);

		// Unregister app bar
		abd.cbSize= sizeof(APPBARDATA);
		abd.hWnd= (HWND) GetHandle();		
		SHAppBarMessage (ABM_REMOVE, &abd);		
	}

#elif defined(__WXGTK__) 

	// This code is mostly based on the information found in:
	// http://www.freedesktop.org/wiki/Specifications/wm-spec

	Atom atomTmp= 0;
	int val= 0;

	if (show)
	{
		// X11 low-level information
		Display *dd= (Display *) wxGetDisplay();
		int screen = DefaultScreen (dd);
	
		// Try to get real work area to set metrics
		// TOP location only supported for the moment
		atomTmp = XInternAtom(dd, "_NET_WORKAREA", False);
		
		Atom actual_type;
		int actual_format;
		unsigned long nitems;
		unsigned long bytes_after;
		
		//CARD32 *prop;
		unsigned long *prop;
		
		// Get desktop size
		XGetWindowProperty (dd, DefaultRootWindow(dd), atomTmp, 0, 4, False, AnyPropertyType, &actual_type, &actual_format, &nitems, &bytes_after, (unsigned char **) &prop);
		if (prop!= NULL)
		{			
			m_X= (int) prop[0];
			m_Y= (int) prop[1];
			m_Width= prop[2];
			XFree (prop);
		}
		else
		{
			// Fallback
			m_X= 0;
			m_Y= 0;
			m_Width = DisplayWidth (dd, screen);
		}

		// Set metrics (TOP location only)
		wxSize proposedSize= DoGetBestSize();
		m_Height= proposedSize.GetHeight();	
	
		SetSize(m_X, m_Y, m_Width, m_Height, 0);
		wxDialog::Show (true);	// Do real show. If not, no underlying window is created the first time.
	
		// Window X11 handle
		GtkWidget *gtkWidget= (GtkWidget *) this->GetHandle();
		Window w= GDK_WINDOW_XWINDOW (gtkWidget->window);
	
		// TODO: this is uggly but ensures that the window is really mapped (FIXME)
		while (!IsMappedWindow(dd,w))
		{
			usleep (100000);
		}
	
		// Send root window _NET_WM_STATE properties
		atomTmp= XInternAtom (dd, "_NET_WM_STATE_STICKY", False);
		wxWMspecSetState(dd, w, _NET_WM_STATE_ADD, atomTmp);
		atomTmp= XInternAtom (dd, "_NET_WM_STATE_ABOVE", False);
		wxWMspecSetState(dd, w, _NET_WM_STATE_ADD, atomTmp);
		XSync(dd, False);
		
		// Unmap window before set properties
		XUnmapWindow(dd, w);
		XSync(dd, False);
		assert (!IsMappedWindow(dd,w));
		
		//
		// Reserves an area in the desktop
		//
		atomTmp = XInternAtom (dd, "_NET_WM_STRUT", False);	
		//		CARD32 
		unsigned long strut[] = { 0,0,m_Height + m_Y,0 };	// TOP		
		XChangeProperty (dd, w, atomTmp, XA_CARDINAL, 32, PropModeReplace, (unsigned char *) &strut, 4);
	
		//
		// Set window in WIN_LAYER_ABOVE_DOCK (see GNOME Window Manager Compliance document for details)
		// TODO: deprecated, here for compatibility reasons
		//
		atomTmp= XInternAtom (dd, "_WIN_LAYER", False);
		val= 10;
		XChangeProperty (dd, w, atomTmp, XA_CARDINAL, 32, PropModeReplace, (unsigned char *) &val, 1);
	
		// TODO: the same as above, also deprecated
		atomTmp= XInternAtom (dd, "_NET_WM_LAYER", False);
		val= _NET_WIN_LAYER_ABOVE_DOCK;
		XChangeProperty (dd, w, atomTmp, XA_CARDINAL, 32, PropModeReplace, (unsigned char *) &val, 1);		
	
		//
		// Avoid focus, listed in Alt+TAB, listed in taskbar and so on
		// TODO: deprecated?
		//
		atomTmp= XInternAtom (dd, "_WIN_HINTS", False);
		val= WIN_HINTS_SKIP_FOCUS | WIN_HINTS_SKIP_WINLIST | WIN_HINTS_SKIP_TASKBAR | WIN_HINTS_DO_NOT_COVER;
		XChangeProperty (dd, w, atomTmp, XA_CARDINAL, 32, PropModeReplace, (unsigned char *) &val, 1);
	
		// TODO: the same as above, also deprecated
		atomTmp= XInternAtom (dd, "_NET_WM_HINTS", False);
		val= _NET_WM_HINTS_SKIP_FOCUS | WIN_HINTS_SKIP_WINLIST | _NET_WM_HINTS_SKIP_WINLIST | WIN_HINTS_DO_NOT_COVER |
		_NET_WM_HINTS_NO_AUTO_FOCUS | _NET_WM_HINTS_STANDALONE_MENUBAR | _NET_WM_HINTS_FIXED_POSITION | _NET_WM_HINTS_DO_NOT_COVER;
		XChangeProperty (dd, w, atomTmp, XA_CARDINAL, 32, PropModeReplace, (unsigned char *) &val, 1);
	
		//
		// _NET_WM_WINDOW_TYPE 
		//
		atomTmp= XInternAtom (dd, "_NET_WM_WINDOW_TYPE", False);
		Atom atom_NET_WM_WINDOW_TYPE_DOCK= XInternAtom (dd, "_NET_WM_WINDOW_TYPE_DOCK", False);
		Atom atom_NET_WM_WINDOW_TYPE_NORMAL= XInternAtom (dd, "_NET_WM_WINDOW_TYPE_NORMAL", False);
		//CARD32 propInfo[2];
		unsigned long propInfo[2];
		propInfo[0]= atom_NET_WM_WINDOW_TYPE_DOCK;
		propInfo[1]= atom_NET_WM_WINDOW_TYPE_NORMAL;
		
		XChangeProperty (dd, w, atomTmp, XA_ATOM, 32, PropModeReplace, (unsigned char *) &propInfo[0], 2);	
		
		//
		atomTmp= XInternAtom (dd, "_WIN_STATE", False);
		val= WIN_STATE_STICKY | WIN_STATE_FIXED_POSITION;
		XChangeProperty (dd, w, atomTmp, XA_CARDINAL, 32, PropModeReplace, (unsigned char *) &val, 1);
			
		/* borderless motif hint */
	/*
		MWMHints mwm;
		atomTmp= XInternAtom (dd, "_WIN_HINTS", False);
		memset (&mwm, 0, sizeof (mwm));
		mwm.flags = MWM_HINTS_DECORATIONS;
		XChangeProperty (dd, w, atomTmp, atomTmp, 32, PropModeReplace, (unsigned char *) &mwm, sizeof (MWMHints) / 4);
	*/
	
		// Set in proper location	
	//	XSizeHints sizeHints;
	//	bzero (&sizeHints, sizeof(sizeHints));
	//	sizeHints.flags = PPosition;	
		//retval2= XChangeProperty (dd, w, XA_WM_NORMAL_HINTS, XA_WM_SIZE_HINTS, 32, PropModeReplace, (unsigned char *) &sizeHints, sizeof (XSizeHints) / 4);
	
		// Map window again
		XMapRaised(dd, w);
		XSync(dd, False);
	}
	else
	{
		Display *dd= (Display *) wxGetDisplay();
		GtkWidget *gtkWidget= (GtkWidget *) this->GetHandle();
		Window w= GDK_WINDOW_XWINDOW (gtkWidget->window);

		// Unmap window before set properties
		XUnmapWindow(dd, w);
		XSync(dd, False);
		assert (!IsMappedWindow(dd,w));

		// Reserves an area in the desktop
		atomTmp = XInternAtom (dd, "_NET_WM_STRUT", False);	
		//		CARD32 strut[] = { 0,0,0,0 };
		unsigned long strut[] = { 0,0,0,0 };
		XChangeProperty (dd, w, atomTmp, XA_CARDINAL, 32, PropModeReplace, (unsigned char *) &strut, 4);

		wxDialog::Show (false);
	}
#else
#error "GDK not found"
#endif

	return true;
}
