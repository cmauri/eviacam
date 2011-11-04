/////////////////////////////////////////////////////////////////////////////
// Name:        mousecontrol.cpp
// Purpose: cross platform mouse control api
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by: 
// Created:     
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

// TODO: under X11. Test XTest extensions enabled
//if(!XTestQueryExtension(display, &ev, &er, &ma, &mi))
//{
//fprintf(stderr, "XTest extension not supported on server.\n");
//exit(1);
//}
#include <math.h>
#include <stdio.h>

#if defined(WIN32)
//
// Windows
//
#define _WIN32_WINNT 0x0500
#include <windows.h>
#define MOUSE_EXTRA_INFO 69

inline float roundf(float x) { return (x-floorf(x))>0.5 ? ceilf(x) : floorf(x); }

#else	// WIN32
//
// Linux
//
#include <X11/extensions/XTest.h>
#include <stdexcept>

#endif // WIN32

#include <assert.h>
#include <string.h>
#include "mousecontrol.h"

// Mouse actions
#define MOUSE_MOVE_ABS	0x8000		// Absolute motion
#define MOUSE_MOVE_REL	0x0000		// Relative motion
#define MOUSE_LEFTDOWN  0x0002		// Left button down
#define MOUSE_LEFTUP    0x0004		// Left button up
#define MOUSE_RIGHTDOWN   0x0008	// Right button down
#define MOUSE_RIGHTUP     0x0010	// Right button up 
#define MOUSE_MIDDLEDOWN  0x0020	// Middle button down
#define MOUSE_MIDDLEUP    0x0040	// Middle button up 

CMouseControl::CMouseControl (void* pDisplay)
: m_enabledRestrictedWorkingArea(false)
, m_enabledWrapPointer(false)
#if !defined(WIN32)
, m_closeDisplay(false)
#endif
{
	m_leftPercent= m_rightPercent= m_topPercent= m_bottomPercent= 1.0f;
        
	// Under Windows display parameter is ignored
	#if defined(WIN32)
	assert (pDisplay== NULL);
	#else
	if (pDisplay) m_pDisplay= (Display *) pDisplay;
	else {
		m_pDisplay= XOpenDisplay(NULL);
		m_closeDisplay= true;
		assert (m_pDisplay);
		if (!m_pDisplay) throw std::runtime_error("mousecontrol: cannot open display");
	}	
	#endif	

	// Read screen size
	OnDisplayChanged ();

	m_VirtualXIni= m_VirtualYIni= 0;
	m_VirtualHeight= (float) m_ScreenHeight; 
	m_VirtualWidth= (float) m_ScreenWidth;

	ResetClickArea ();

	m_fDx= m_fDy= 1.0f;
	m_minDeltaThreshold= 0.0f;
	m_actualMotionWeight= 1.0f;
	m_dxant= 0.0f; m_dyant= 0.0f;

	for (int i= 0; i< ACCEL_ARRAY_SIZE; i++) m_accelArray[i]= (float) 1;
}

CMouseControl::~CMouseControl()
{
#if !defined(WIN32)
	if (m_closeDisplay) XCloseDisplay(m_pDisplay);
#endif
}

void CMouseControl::GetScreenSize()
{
#if defined(WIN32)
	DEVMODE devMode;
	// Get screen size. TODO: test with multiple displays
	BOOL retval= EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode);
	if (!retval) { exit (-1); }

	m_ScreenHeight= devMode.dmPelsHeight;
	m_ScreenWidth= devMode.dmPelsWidth;
#else // Linux
//	BEGIN_GUI_CALL_MUTEX();
	m_ScreenHeight= DisplayHeight (m_pDisplay, DefaultScreen (m_pDisplay));
	m_ScreenWidth= DisplayWidth (m_pDisplay, DefaultScreen (m_pDisplay));
//	END_GUI_CALL_MUTEX();	
#endif

}

void CMouseControl::GetPointerLocation (long& x, long& y)
{
#if defined(WIN32)

	CURSORINFO pci;

	pci.cbSize= sizeof(CURSORINFO);
	pci.ptScreenPos.x= 0;
	pci.ptScreenPos.y= 0;

	GetCursorInfo(&pci);

	x= pci.ptScreenPos.x;
	y= pci.ptScreenPos.y;

#else // Linux
	
	Window root, child;
	int rootX, rootY, winX, winY;
	unsigned int xstate;

//	BEGIN_GUI_CALL_MUTEX();
	Window rootWin= RootWindow (m_pDisplay, DefaultScreen (m_pDisplay));
	XQueryPointer( m_pDisplay, rootWin, &root, &child, &rootX, &rootY, &winX, &winY, &xstate );
//	END_GUI_CALL_MUTEX();
	x= winX;
	y= winY;
#endif
}

void CMouseControl::OnDisplayChanged ()
{
	GetScreenSize();
	RecomputeWorkingArea ();
}

void CMouseControl::SetAbsVirtualResolution (float xIni, float yIni, float width, float height)
{
	assert (xIni>= 0 && yIni>= 0 && width> 0 && height> 0);
	
	m_VirtualXIni= xIni;
	m_VirtualYIni= yIni;
	m_VirtualWidth= width;
	m_VirtualHeight= height;
}

void CMouseControl::SetWorkingArea (float leftPercent, float rightPercent, float topPercent, float bottomPercent)
{
	SetTopPercent (topPercent);		
	SetLeftPercent (leftPercent);
	SetRightPercent (rightPercent);
	SetBottomPercent (bottomPercent);
}

void CMouseControl::RecomputeWorkingArea ()
{
	m_minScreenX=	(m_ScreenWidth - 
					 (int) ((float) m_ScreenWidth * m_leftPercent)) / 2;
	m_minScreenY=	(m_ScreenHeight - 
					 (int) ((float) m_ScreenHeight * m_topPercent)) / 2;
	m_maxScreenX=	(m_ScreenWidth - 1) - (m_ScreenWidth - 
					 (int) ((float) m_ScreenWidth * m_rightPercent)) / 2;
	m_maxScreenY=	(m_ScreenHeight - 1) - (m_ScreenHeight - 
					 (int) ((float) m_ScreenHeight * m_bottomPercent)) / 2;
}

void CMouseControl::SetClickArea (long minX, long minY, long maxX, long maxY)
{
	assert (minX>= 0 && minY>= 0);
	assert (maxX< m_ScreenWidth && maxY< m_ScreenHeight);

	m_minClicAreaX= minX;
	m_minClicAreaY= minY;
	m_maxClicAreaX= maxX;
	m_maxClicAreaY= maxY;
}

void CMouseControl::ResetClickArea ()
{
	m_minClicAreaX= m_minClicAreaY= 0;
	m_maxClicAreaX= m_maxScreenX;
	m_maxClicAreaY= m_maxScreenY;
}

bool CMouseControl::CheckClickArea ()
{
	long x, y;

	GetPointerLocation (x, y);
	
	return (x>= m_minClicAreaX && x<= m_maxClicAreaX &&
			y>= m_minClicAreaY && y<= m_maxClicAreaY );		
}

void CMouseControl::SetRelAcceleration2 (long delta0, float factor0,
						long delta1, float factor1)
{
	assert (delta0> 2 &&delta1> 2); 
	assert (factor0> 0.0f && factor1> 0.0f);
	
	if (delta0>= ACCEL_ARRAY_SIZE) delta0= ACCEL_ARRAY_SIZE;
	if (delta1>= ACCEL_ARRAY_SIZE) delta1= ACCEL_ARRAY_SIZE;

	int i;

	for (i= 0; i< delta0; i++) m_accelArray[i]= 1;
	for (;i< delta1; i++) m_accelArray[i]= factor0;
	float j= 0;
	for (;i< ACCEL_ARRAY_SIZE; i++) {
		m_accelArray[i]= factor0 * factor1 + j;
		j+= 0.1f;
	}
}

bool CMouseControl::EnforceWorkingAreaLimits (long &x, long &y)
{
	bool retval= false;

	if (x< m_minScreenX) { x= m_minScreenX; retval= true; }
	else if (x> m_maxScreenX) { x= m_maxScreenX; retval= true; }
	
	if (y< m_minScreenY) { y= m_minScreenY; retval= true; }
	else if (y> m_maxScreenY) { y= m_maxScreenY; retval= true; }

	return retval;
}

void CMouseControl::Virt2Fis (float virtX, float virtY, float &fisX, float &fisY)
{
	float fFisX, fFisY, fVirtWidth, fVirtHeight;

	fFisX= virtX - m_VirtualXIni;
	fFisY= virtY - m_VirtualYIni;
	fVirtWidth= m_VirtualWidth; 
	fVirtHeight= m_VirtualHeight;

	if (fFisX< 0.0f) fFisX= 0.0f;
	else if (fFisX>= fVirtWidth) fFisX= 1.0f; 
	else fFisX/= fVirtWidth;

	if (fFisY< 0.0f) fFisY= 0.0f;
	else if (fFisY>= fVirtHeight) fFisY= 1.0f; 
	else fFisY/= fVirtHeight;

	// To integer
	fisX= fFisX * (float) (m_ScreenWidth - 1);
	fisY= fFisY * (float) (m_ScreenHeight - 1);
}

void CMouseControl::MovePointerAbs (float x, float y)
{
//	OnDisplayChanged ();

	long iX, iY;
	float fisX, fisY, dx, dy;	

	Virt2Fis (x, y, fisX, fisY);

	GetPointerLocation (iX, iY);	
	dx= fisX - (float) iX;
	dy= fisY - (float) iY;
	
	// Low-pass filter	
	dx= dx * (1.0f - m_actualMotionWeight) + m_dxant * m_actualMotionWeight;
	dy= dy * (1.0f - m_actualMotionWeight) + m_dyant * m_actualMotionWeight;
	m_dxant= dx; m_dyant= dy;

	// Map to screen coordinates
	iX= iX + (long) dx;
	iY= iY + (long) dy;
	
	EnforceWorkingAreaLimits (iX, iY);
	DoMovePointerAbs (iX, iY);
}

float CMouseControl::MovePointerRel (float dx, float dy, int* dxRes, int* dyRes)
{
	OnDisplayChanged ();

	// Apply factors
	dx*= m_fDx;
	dy*= m_fDy;

	// Low-pass filter	
	dx= dx * (1.0f - m_actualMotionWeight) + m_dxant * m_actualMotionWeight;
	dy= dy * (1.0f - m_actualMotionWeight) + m_dyant * m_actualMotionWeight;
	m_dxant= dx; m_dyant= dy;

	// Acceleration
	float distance= (float) ::sqrt (dx * dx + dy * dy);

	unsigned int iAccelArray= (unsigned int) (distance + 0.5f);
	if (iAccelArray>= ACCEL_ARRAY_SIZE) iAccelArray= ACCEL_ARRAY_SIZE - 1;
	dx*= m_accelArray[iAccelArray];
	dy*= m_accelArray[iAccelArray];

	// Apply delta threshold
	if (-m_minDeltaThreshold < dx && dx < m_minDeltaThreshold) dx= 0.0f;
	if (-m_minDeltaThreshold < dy && dy < m_minDeltaThreshold) dy= 0.0f;
	
	int idx= (int) roundf(dx);
	int idy= (int) roundf(dy);
	long mouseX, mouseY;
	if (m_enabledRestrictedWorkingArea && !m_enabledWrapPointer) {
		GetPointerLocation (mouseX, mouseY);
		if (mouseX + idx< m_minScreenX) idx= m_minScreenX - mouseX;
		else if (mouseX + idx > m_maxScreenX) idx= m_maxScreenX - mouseX;
		if (mouseY + idy < m_minScreenY) idy= m_minScreenY - mouseY;
		else if (mouseY + idy > m_maxScreenY) idy= m_maxScreenY - mouseY;
	}
	
	if (m_enabledWrapPointer) {
		int minWrapX= 0;
		int maxWrapX= m_ScreenWidth;
		int minWrapY= 0;
		int maxWrapY= m_ScreenHeight;
				
		if (m_enabledRestrictedWorkingArea) {
			minWrapX= m_minScreenX;
			maxWrapX= m_maxScreenX;
			minWrapY= m_minScreenY;
			maxWrapY= m_maxScreenY;
		}

		GetPointerLocation(mouseX, mouseY);
		if (mouseX + idx < minWrapX) {
			idx -= mouseX - minWrapX;
			DoMovePointerAbs(maxWrapX, mouseY);
		}		
		if (mouseX + idx > maxWrapX) {
			idx -= maxWrapX - mouseX;
			DoMovePointerAbs(minWrapX, mouseY);
		}
		if (mouseY + idy < minWrapY) {
			idy -= mouseY - minWrapY;
			DoMovePointerAbs(mouseX, maxWrapY);
		}
		if (mouseY + idy > maxWrapY) {
			idy -= maxWrapY - mouseY;
			DoMovePointerAbs(mouseX, minWrapY);
		}
	}

	DoMovePointerRel (idx, idy);
	if (dxRes) *dxRes= idx;
	if (dyRes) *dyRes= idy;

	return (float) sqrt((double)(idx * idx + idy * idy));
}

void CMouseControl::DoMovePointerAbs (long x, long y)
{
	SendMouseCommand (x, y, MOUSE_MOVE_ABS);
}

void CMouseControl::DoMovePointerRel (long dx, long dy)
{
	SendMouseCommand (dx, dy, MOUSE_MOVE_REL);
}

// Send a mouse control system command
void CMouseControl::SendMouseCommand (long x, long y, int flags)
{
#if defined(WIN32)
	INPUT ip;

	// Allways define MOUSEEVENTF_MOVE flag, other flags are coincident
	// with native flags under windows
	flags|= MOUSEEVENTF_MOVE;

	if (flags & MOUSE_MOVE_ABS)
	{
		// Normalize absolute motion
		x= (x * 65535) / (m_ScreenWidth - 1);
		y= (y * 65535) / (m_ScreenHeight - 1);
	}
	ip.type = INPUT_MOUSE;
	ip.mi.dwFlags = flags;
	ip.mi.mouseData = 0;
	ip.mi.dwExtraInfo = MOUSE_EXTRA_INFO;
	ip.mi.time = 0;
	ip.mi.dx= x;
	ip.mi.dy= y;

	SendInput(1, &ip, sizeof(ip));
#else
//	BEGIN_GUI_CALL_MUTEX();
	if (flags== MOUSE_MOVE_ABS) 
		// Absolute motion
		XTestFakeMotionEvent(m_pDisplay, DefaultScreen(m_pDisplay), x, y, CurrentTime);
	else if (flags== MOUSE_MOVE_REL) 
		// Relative motion
		XTestFakeRelativeMotionEvent(m_pDisplay, x, y, CurrentTime);		
	else
	{
		// Button press
		bool is_press= false;
		unsigned int button= 0;;

		switch (flags)
		{		
		case MOUSE_LEFTDOWN:  	// Left button down
			button= 1; is_press= true;
			break;
		case MOUSE_LEFTUP:		// Left button up
			button= 1; is_press= false;
			break;
		case MOUSE_RIGHTDOWN:	// Right button down
			button= 3; is_press= true;
			break;
		case MOUSE_RIGHTUP:		// Right button up
			button= 3; is_press= false;
			break;
		case MOUSE_MIDDLEDOWN:	// Middle button down
			button= 2; is_press= true;
			break;
		case MOUSE_MIDDLEUP:	// Middle button up
			button= 2; is_press= false;
			break;
		default:
			assert (false);
		}

		XTestFakeButtonEvent(m_pDisplay, button, is_press, CurrentTime);
	}
	XFlush (m_pDisplay);
//	END_GUI_CALL_MUTEX();
#endif
}

void CMouseControl::CenterPointer ()
{
	DoMovePointerAbs(m_ScreenWidth/2, m_ScreenHeight/2);
}

bool CMouseControl::LeftDown ()
{
	if (CheckClickArea ()) {
		SendMouseCommand (0, 0,	MOUSE_LEFTDOWN);
		return true;
	}

	return false;
}

void CMouseControl::LeftUp ()
{
	SendMouseCommand (0, 0,	MOUSE_LEFTUP);
}

bool CMouseControl::MiddleDown ()
{
	if (CheckClickArea ()) {
		SendMouseCommand (0, 0,	MOUSE_MIDDLEDOWN);
		return true;
	}

	return false;
}

void CMouseControl::MiddleUp ()
{
	SendMouseCommand (0, 0,	MOUSE_MIDDLEUP);
}

bool CMouseControl::RightDown ()
{
	if (CheckClickArea ()) {
		SendMouseCommand (0, 0,	MOUSE_RIGHTDOWN);
		return true;
	}
	
	return false;
}

void CMouseControl::RightUp ()
{
	SendMouseCommand (0, 0,	MOUSE_RIGHTUP);
}

bool CMouseControl::LeftClick ()
{
	if (CheckClickArea ()) {
		LeftDown ();
		LeftUp ();
		return true;
	}

	return false;
}

bool CMouseControl::MiddleClick ()
{
	if (CheckClickArea ()) {
		MiddleDown ();
		MiddleUp ();
		return true;
	}

	return false;
}

bool CMouseControl::RightClick ()
{	
	if (CheckClickArea ()) {
		RightDown ();
		RightUp ();
		return true;
	}

	return false;
}

bool CMouseControl::LeftDblClick ()
{
	if (CheckClickArea ()) {
		LeftClick ();
		LeftClick ();
		return true;
	}

	return false;
}
