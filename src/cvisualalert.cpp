/////////////////////////////////////////////////////////////////////////////
// Name:        cvisualalert.h
// Purpose:  
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by: 
// Created:     21/09/2010
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
#include "cvisualalert.h"
#include <wx/utils.h>
#if defined(__WXGTK__)
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <gtk/gtk.h>
#endif


// class CVisualAlertProgress
#define PROGRESS_SIZE 30
#define PROGRESS_OFFSET 8

#define LINE_WIDTH 2
//#define START_RADIUS 30
//#define RADIUS_STEP 1
//#define MSECOND 1000

#define CROSS_SIZE 8


CVisualAlertProgress::CVisualAlertProgress()
{
	m_oldSize= 0;
	m_oldX= 0;
	m_oldY= 0;
}

CVisualAlertProgress::~CVisualAlertProgress()
{
//	End();
}

#if defined(__WXGTK__)
static void
draw_line (gint x1, gint y1, gint x2, gint y2)
{
    GdkScreen *screen;
    GdkWindow *root;
    GdkGC *gc;

    screen = gdk_display_get_default_screen (gdk_display_get_default ());
    root = gdk_screen_get_root_window (screen);

    gc = gdk_gc_new (root);
    gdk_gc_set_subwindow (gc, GDK_INCLUDE_INFERIORS);
    gdk_gc_set_function (gc, GDK_INVERT);
    gdk_gc_set_line_attributes (gc, 1,
				GDK_LINE_SOLID,
				GDK_CAP_ROUND,
				GDK_JOIN_ROUND);
    gdk_draw_arc (root, gc, TRUE,
		  x1 - 4, y1 - 4, 8, 8, 0, 23040);
    gdk_draw_line (root, gc, x1, y1, x2, y2);
    g_object_unref (gc);
}
#endif

void CVisualAlertProgress::Update(int x, int y, int percent)
{
#if defined(__WXGTK__)
	int size = (PROGRESS_SIZE * (100 - percent)) / 100;
	
	if (size || m_oldSize) {
		XGrabServer(m_display);
	
		if (m_oldSize!= 0) 
			// Need to clear previos drawing
			XDrawLine(m_display, m_window, m_gc, m_oldX + PROGRESS_OFFSET, m_oldY + PROGRESS_OFFSET,  
				m_oldX + PROGRESS_OFFSET, m_oldY + PROGRESS_OFFSET - m_oldSize);
		if (size> 0)
			// Draw progress bar
			XDrawLine(m_display, m_window, m_gc, x+PROGRESS_OFFSET, y+PROGRESS_OFFSET,  x+PROGRESS_OFFSET, y+PROGRESS_OFFSET - size);
		
		XUngrabServer(m_display);
		XFlush(m_display);
	}
	
	m_oldSize = size;
	m_oldX = x;
	m_oldY = y;
#endif
}

void CVisualAlertProgress::End()
{
	if(m_oldSize!= 0) {
		// Need to clear previos drawing	
		XGrabServer(m_display);
		XDrawLine(m_display, m_window, m_gc, m_oldX + PROGRESS_OFFSET, m_oldY + PROGRESS_OFFSET,  
			  m_oldX + PROGRESS_OFFSET, m_oldY + PROGRESS_OFFSET - m_oldSize);
		XUngrabServer(m_display);
		XFlush(m_display);
		m_oldSize= 0;
	}
}

CVisualAlertDirection::CVisualAlertDirection()
{
	m_running= false;
	m_xOrigin= 0;
	m_yOrigin= 0;
	m_xOldDest= 0;
	m_yOldDest= 0;
}

CVisualAlertDirection::~CVisualAlertDirection()
{
//	End();
}

void CVisualAlertDirection::Update(int x, int y)
{
	XGrabServer(m_display);
	if (!m_running) {
		// Not running. Use this point as origin
		m_xOrigin= x;
		m_yOrigin= y;
		m_running= true;
		
		// Draw cross.
		XDrawLine(m_display, m_window, m_gc, m_xOrigin - CROSS_SIZE, m_yOrigin, m_xOrigin + CROSS_SIZE, m_yOrigin);
		XDrawLine(m_display, m_window, m_gc, m_xOrigin, m_yOrigin - CROSS_SIZE, m_xOrigin, m_yOrigin + CROSS_SIZE);
	}
	else {
		XSetForeground(m_display, m_gc, 0xff0000);
		// Clear previous direction line
		XDrawLine(m_display, m_window, m_gc, m_xOrigin, m_yOrigin, m_xOldDest, m_yOldDest);
		// Draw new
		XDrawLine(m_display, m_window, m_gc, m_xOrigin, m_yOrigin, x, y);
		XSetForeground(m_display, m_gc, WhitePixel(m_display, m_screen));
	}
	XUngrabServer(m_display);
	XFlush(m_display);
	
	m_xOldDest= x;
	m_yOldDest= y;
}

void CVisualAlertDirection::End()
{
	if (m_running) {
		XGrabServer(m_display);
		// Clear cross.
		XDrawLine(m_display, m_window, m_gc, m_xOrigin - CROSS_SIZE, m_yOrigin, m_xOrigin + CROSS_SIZE, m_yOrigin);
		XDrawLine(m_display, m_window, m_gc, m_xOrigin, m_yOrigin - CROSS_SIZE, m_xOrigin, m_yOrigin + CROSS_SIZE);
		
		XSetForeground(m_display, m_gc, 0xff0000);
		// Clear previous direction line
		XDrawLine(m_display, m_window, m_gc, m_xOrigin, m_yOrigin, m_xOldDest, m_yOldDest);
		
		XSetForeground(m_display, m_gc, WhitePixel(m_display, m_screen));
		XUngrabServer(m_display);
		XFlush(m_display);
		
		m_running= false;
		m_xOrigin= 0;
		m_yOrigin= 0;
		m_xOldDest= 0;
		m_yOldDest= 0;
	}
}


//CVisualAlert::CVisualAlert(Stage stg)
CVisualAlert::CVisualAlert()
{
//	m_stage = stg;
//	m_oldRadius = 0;
//	m_oldX = 0;
//	m_oldY = 0;
	
#if defined(__WXGTK__)	
	m_display = (Display*) wxGetDisplay(); //XOpenDisplay(0);
	
	m_screen = DefaultScreen(m_display);
	m_window =  XRootWindow(m_display, m_screen);

	// Create GCs
	m_gc = XCreateGC(m_display, m_window, 0, 0);

	// White pixel
	XSetForeground(m_display, m_gc, WhitePixel(m_display, m_screen));

	// Draw on root draws on everything
	XSetSubwindowMode(m_display, m_gc, IncludeInferiors);

	// Nice fat lines
	XSetLineAttributes(m_display, m_gc, LINE_WIDTH, LineSolid, CapButt, JoinRound);

	// Use XOR, so drawind twice removes it
	XSetFunction(m_display, m_gc, GXxor);
#endif
}

CVisualAlert::~CVisualAlert()
{
#if defined(__WXGTK__) 
	XFreeGC(m_display, m_gc);
#endif
}
/*
void CVisualAlert::Update(long xIni, long yIni, long x, long y, int percent)
{
#if defined(__WXGTK__)
	int radius = (START_RADIUS * (100 - percent)) / 100;
	
	if(m_oldRadius != 0)
	{
		XGrabServer(m_display);
		switch(m_stage)
		{
			case DWELL:
				XDrawLine(m_display, m_window, m_gc, m_oldX+PROGRESS_OFFSET, m_oldY+PROGRESS_OFFSET,  m_oldX+PROGRESS_OFFSET, m_oldY+PROGRESS_OFFSET - m_oldRadius);
				break;
				
			case GESTURE:
				//Red line.
				XSetForeground(m_display, m_gc, 0xff0000);
				XDrawLine(m_display, m_window, m_gc, xIni, yIni, m_oldX, m_oldY);
				XSetForeground(m_display, m_gc, WhitePixel(m_display, m_screen));
	
				//Progress line.
				XDrawLine(m_display, m_window, m_gc, m_oldX+PROGRESS_OFFSET, m_oldY+PROGRESS_OFFSET,  m_oldX+PROGRESS_OFFSET, m_oldY+PROGRESS_OFFSET - m_oldRadius);
				
				//Cross.
				XDrawLine(m_display, m_window, m_gc, xIni-CROSS_SIZE, yIni,  xIni+CROSS_SIZE, yIni);
				XDrawLine(m_display, m_window, m_gc, xIni, yIni-CROSS_SIZE,  xIni, yIni+CROSS_SIZE);
				break;
		}
		XUngrabServer(m_display);
		XFlush(m_display);
	}
	
	if(radius > 0)
	{
		XGrabServer(m_display);
		switch(m_stage)
		{
			case DWELL:
				XDrawLine(m_display, m_window, m_gc, x+PROGRESS_OFFSET, y+PROGRESS_OFFSET,  x+PROGRESS_OFFSET, y+PROGRESS_OFFSET - radius);
				break;
				
			case GESTURE:
				//Red line.
				XSetForeground(m_display, m_gc, 0xff0000);
				XDrawLine(m_display, m_window, m_gc, xIni, yIni, x, y);
				XSetForeground(m_display, m_gc, WhitePixel(m_display, m_screen));
				
				//Progress line.
				XDrawLine(m_display, m_window, m_gc, x+PROGRESS_OFFSET, y+PROGRESS_OFFSET,  x+PROGRESS_OFFSET, y+PROGRESS_OFFSET - radius);
				
				//Cross.
				XDrawLine(m_display, m_window, m_gc, xIni-CROSS_SIZE, yIni,  xIni+CROSS_SIZE, yIni);
				XDrawLine(m_display, m_window, m_gc, xIni, yIni-CROSS_SIZE,  xIni, yIni+CROSS_SIZE);
				break;
		}
		XUngrabServer(m_display);
		XFlush(m_display);
	}
	m_oldRadius = radius;
	m_oldX = x;
	m_oldY = y;
#endif
}

void CVisualAlert::End(long xIni, long yIni)
{
#if defined(__WXGTK__) 
	if(m_oldRadius != 0)
	{
		XGrabServer(m_display);
		switch(m_stage)
		{
			case DWELL:
				XDrawLine(m_display, m_window, m_gc, m_oldX+PROGRESS_OFFSET, m_oldY+PROGRESS_OFFSET,  m_oldX+PROGRESS_OFFSET, m_oldY+PROGRESS_OFFSET - m_oldRadius);
				break;
			
			case GESTURE:
				//Red line.
				XSetForeground(m_display, m_gc, 0xff0000);
				XDrawLine(m_display, m_window, m_gc, xIni, yIni, m_oldX, m_oldY);
				XSetForeground(m_display, m_gc, WhitePixel(m_display, m_screen));
				
				//Progress line.
				XDrawLine(m_display, m_window, m_gc, m_oldX+PROGRESS_OFFSET, m_oldY+PROGRESS_OFFSET,  m_oldX+PROGRESS_OFFSET, m_oldY+PROGRESS_OFFSET - m_oldRadius);
				
				//Cross.
				XDrawLine(m_display, m_window, m_gc, xIni-PROGRESS_OFFSET, yIni,  xIni+PROGRESS_OFFSET, yIni);
				XDrawLine(m_display, m_window, m_gc, xIni, yIni-PROGRESS_OFFSET, xIni, yIni+PROGRESS_OFFSET);
				break;
		}
		XUngrabServer(m_display);
		XFlush(m_display);
	}
	m_oldRadius = 0;
#endif
}
*/