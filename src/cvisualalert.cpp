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
#include <stdio.h>
#if defined(__WXGTK__) 
#include <unistd.h>
#include <stdlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#endif

#define LINE_WIDTH 2
#define START_RADIUS 30
#define RADIUS_STEP 1
#define MSECOND 1000
#define VISUAL_OFFSET 8
#define CROSS_SIZE 8

CVisualAlert::CVisualAlert()
{
	m_waitTime = NULL;
	m_oldRadius = 0;
	m_oldX = 0;
	m_oldY = 0;
#if defined(__WXGTK__)
	m_display = NULL;
	m_window = NULL;
	m_screen = NULL;
	m_gc = NULL;
#endif
}

CVisualAlert::~CVisualAlert()
{
}

void CVisualAlert::Start(Stage stg)
{
	m_stage = stg;

#if defined(__WXGTK__) 
	m_oldRadius = 0;
	
	m_display = XOpenDisplay(0);
	
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
				XDrawLine(m_display, m_window, m_gc, m_oldX+VISUAL_OFFSET, m_oldY+VISUAL_OFFSET,  m_oldX+VISUAL_OFFSET, m_oldY+VISUAL_OFFSET - m_oldRadius);
				break;
				
			case GESTURE:
				//Red line.
				XSetForeground(m_display, m_gc, 0xff0000);
				XDrawLine(m_display, m_window, m_gc, xIni, yIni, m_oldX, m_oldY);
				XSetForeground(m_display, m_gc, WhitePixel(m_display, m_screen));
	
				//Progress line.
				XDrawLine(m_display, m_window, m_gc, m_oldX+VISUAL_OFFSET, m_oldY+VISUAL_OFFSET,  m_oldX+VISUAL_OFFSET, m_oldY+VISUAL_OFFSET - m_oldRadius);
				
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
				XDrawLine(m_display, m_window, m_gc, x+VISUAL_OFFSET, y+VISUAL_OFFSET,  x+VISUAL_OFFSET, y+VISUAL_OFFSET - radius);
				break;
				
			case GESTURE:
				//Red line.
				XSetForeground(m_display, m_gc, 0xff0000);
				XDrawLine(m_display, m_window, m_gc, xIni, yIni, x, y);
				XSetForeground(m_display, m_gc, WhitePixel(m_display, m_screen));
				
				//Progress line.
				XDrawLine(m_display, m_window, m_gc, x+VISUAL_OFFSET, y+VISUAL_OFFSET,  x+VISUAL_OFFSET, y+VISUAL_OFFSET - radius);
				
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
				XDrawLine(m_display, m_window, m_gc, m_oldX+VISUAL_OFFSET, m_oldY+VISUAL_OFFSET,  m_oldX+VISUAL_OFFSET, m_oldY+VISUAL_OFFSET - m_oldRadius);
				break;
			
			case GESTURE:
				//Red line.
				XSetForeground(m_display, m_gc, 0xff0000);
				XDrawLine(m_display, m_window, m_gc, xIni, yIni, m_oldX, m_oldY);
				XSetForeground(m_display, m_gc, WhitePixel(m_display, m_screen));
				
				//Progress line.
				XDrawLine(m_display, m_window, m_gc, m_oldX+VISUAL_OFFSET, m_oldY+VISUAL_OFFSET,  m_oldX+VISUAL_OFFSET, m_oldY+VISUAL_OFFSET - m_oldRadius);
				
				//Cross.
				XDrawLine(m_display, m_window, m_gc, xIni-VISUAL_OFFSET, yIni,  xIni+VISUAL_OFFSET, yIni);
				XDrawLine(m_display, m_window, m_gc, xIni, yIni-VISUAL_OFFSET, xIni, yIni+VISUAL_OFFSET);
				break;
		}
		XUngrabServer(m_display);
		XFlush(m_display);
	}
	m_oldRadius = 0;
#endif
}