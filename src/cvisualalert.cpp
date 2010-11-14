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

#define PROGRESS_SIZE 30
#define PROGRESS_OFFSET 8
#define LINE_WIDTH 2
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
#if defined(__WXGTK__)
	if(m_oldSize!= 0) {
		// Need to clear previos drawing	
		XGrabServer(m_display);
		XDrawLine(m_display, m_window, m_gc, m_oldX + PROGRESS_OFFSET, m_oldY + PROGRESS_OFFSET,  
			  m_oldX + PROGRESS_OFFSET, m_oldY + PROGRESS_OFFSET - m_oldSize);
		XUngrabServer(m_display);
		XFlush(m_display);
		m_oldSize= 0;
	}
#endif
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
#if defined(__WXGTK__)
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
#endif
}

void CVisualAlertDirection::End()
{
#if defined(__WXGTK__)
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
#endif
}

CVisualAlert::CVisualAlert()
{
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
