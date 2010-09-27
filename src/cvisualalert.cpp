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

#define LINE_WIDTH 1
#define START_RADIUS 30
#define RADIUS_STEP 1
#define MSECOND 1000
#define VISUAL_OFFSET 8

CVisualAlert::CVisualAlert()
{
	m_waitTime = NULL;
	m_display = NULL;
	m_window = NULL;
	m_screen = NULL;
	m_gc = NULL;
	m_oldRadius = 0;
	m_oldX = 0;
	m_oldY = 0;
}

CVisualAlert::~CVisualAlert()
{
}

CWaitTime* CVisualAlert::StartDwell(long x, long y, unsigned long time)
{
	m_waitTime = new CWaitTime (time * MSECOND);

#if defined(__WXGTK__) 
	m_oldRadius = 0;
	
	m_display = XOpenDisplay(0);
	if(!m_display)
		return NULL;
	
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

	return m_waitTime;
#endif
}

void CVisualAlert::UpdateDwell(long x, long y)
{
#if defined(__WXGTK__) 
	int radius = (START_RADIUS * (100 - m_waitTime->PercentagePassed())) / 100;
	
	if(m_oldRadius != 0)
	{
		XDrawLine(m_display, m_window, m_gc, m_oldX+VISUAL_OFFSET, m_oldY+VISUAL_OFFSET,  m_oldX+VISUAL_OFFSET, m_oldY+VISUAL_OFFSET - m_oldRadius);
		XFlush(m_display);
	}
	
	if(radius > 0)
	{
		XDrawLine(m_display, m_window, m_gc, x+VISUAL_OFFSET, y+VISUAL_OFFSET,  x+VISUAL_OFFSET, y+VISUAL_OFFSET - radius);
		XFlush(m_display);
	}
	
	m_oldRadius = radius;
	m_oldX = x;
	m_oldY = y;
#endif
}

void CVisualAlert::EndDwell()
{
#if defined(__WXGTK__) 
	if(m_oldRadius != 0)
	{
		XDrawLine(m_display, m_window, m_gc, m_oldX+VISUAL_OFFSET, m_oldY+VISUAL_OFFSET,  m_oldX+VISUAL_OFFSET, m_oldY+VISUAL_OFFSET - m_oldRadius);
		XFlush(m_display);
	}
	m_oldRadius = 0;
#endif
}

CWaitTime* CVisualAlert::StartGestureClick (long x, long y, unsigned long time)
{
	m_waitTime = new CWaitTime (time * MSECOND);

#if defined(__WXGTK__) 
	m_oldRadius = 0;
	
	m_display = XOpenDisplay(0);
	if(!m_display)
		return NULL;
	
	m_screen = DefaultScreen(m_display);
	m_window =  XRootWindow(m_display, m_screen);

	// Create GCs
	m_gc = XCreateGC(m_display, m_window, 0, 0);

	// White pixel
	//XSetForeground(m_display, m_gc, WhitePixel(m_display, m_screen));
	XSetForeground(m_display, m_gc, 0xff0000);

	// Draw on root draws on everything
	XSetSubwindowMode(m_display, m_gc, IncludeInferiors);

	// Nice fat lines
	XSetLineAttributes(m_display, m_gc, LINE_WIDTH, LineSolid, CapButt, JoinRound);

	// Use XOR, so drawind twice removes it
	XSetFunction(m_display, m_gc, GXxor);

#endif
	
	return m_waitTime;

}

void CVisualAlert::UpdateGestureClick(long x, long y)
{
#if defined(__WXGTK__) 
	int radius = (START_RADIUS * (100 - m_waitTime->PercentagePassed())) / 100;
	
	if(m_oldRadius != 0)
	{
		XDrawArc(m_display, m_window, m_gc, m_oldX -  m_oldRadius/2 , m_oldY - m_oldRadius/2,  m_oldRadius, m_oldRadius, 0, 360*64);
		XFlush(m_display);
	}
	
	if(radius > 0)
	{
		XDrawArc(m_display, m_window, m_gc, x -  radius/2 , y - radius/2,  radius, radius, 0, 360*64);
		XFlush(m_display);
	}
	
	m_oldRadius = radius;
	m_oldX = x;
	m_oldY = y;
#endif
}

void CVisualAlert::EndGestureClick()
{
#if defined(__WXGTK__) 
	if(m_oldRadius != 0)
	{
		XDrawArc(m_display, m_window, m_gc, m_oldX -  m_oldRadius/2 , m_oldY - m_oldRadius/2,  m_oldRadius, m_oldRadius, 0, 360*64);
		XFlush(m_display);
	}
	m_oldRadius = 0;
#endif
}