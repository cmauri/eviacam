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
#if defined(__WXGTK__)
#include <X11/Xlib.h>
#endif

class CVisualAlert {
public:
	// Non virtual destructor is OK
	~CVisualAlert();
protected:
	CVisualAlert();
#if defined(__WXGTK__)
	Display* m_display;
	Window m_window;
	int m_screen;
	GC m_gc;
#endif
};

// Specialization for progress indicator
class CVisualAlertProgress : public CVisualAlert {
public:
	CVisualAlertProgress();
	~CVisualAlertProgress();
	void Update(int x, int y, int percent);
	void End();
	
private:
	int m_oldSize;
	int m_oldX;
	int m_oldY;
};

// Specialization for direction indicator
class CVisualAlertDirection : public CVisualAlert {
public:
	CVisualAlertDirection();
	~CVisualAlertDirection();
	void Update(int x, int y);
	void End();
private:
	bool m_running;
	int m_xOrigin;
	int m_yOrigin;
	int m_xOldDest;
	int m_yOldDest;
};

#endif
