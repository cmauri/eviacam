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
#ifndef CVISUALALERT_H
#define CVISUALALERT_H

#include "waittime.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

class CVisualAlert{
public:
	CVisualAlert();
	~CVisualAlert();

	CWaitTime* StartDwell (long x, long y, unsigned long time);
	void UpdateDwell (long x, long y);
	void EndDwell ();
	CWaitTime* StartGestureClick (long x, long y, unsigned long time);
	void UpdateGestureClick (long x, long y);
	void EndGestureClick ();

private:
	CWaitTime* m_waitTime;
	Display* m_display;
	Window m_window;
	int m_screen;
	GC m_gc;
	int m_oldRadius;
	int m_oldX;
	int m_oldY;
};


#endif
