/////////////////////////////////////////////////////////////////////////////
// Name:        gestureclick.h
// Purpose:
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by:
// Created:
// Copyright:   (C) 2008-12 Cesar Mauri Loba - CREA Software Systems
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
#ifndef GESTURECLICK_H
#define GESTURECLICK_H

#include "waittime.h"
#include "configbase.h"
#include "cvisualalert.h"
#include "mousecommand.h"
#include <math.h>
#include <vector>
#include "keyboardcode.h"

class CMouseControl;

class CGestureClick : public CConfigBase
{
public:
	CGestureClick (CMouseControl& mc);
	~CGestureClick ();

	// Main entry point to signal that the pointer has been moved.
	// Expect motion performed by the pointer in pixels and current
	// pointer location in absolute coordinates
	// Return the mouse command generated
	mousecmd::mousecmd ProcessMotion (int dxPix, int dyPix, unsigned int xCurr, unsigned int yCurr);
	
	//bool GetEnabled() const { return m_enabled; }
	void SetEnabled(bool value);

	// Reset internal state. Useful before start calling ProcessMotion
	void Reset();
	
	bool AreVisualAlertsEnabled() const { return m_visualAlertsEnabled; }
	void EnableVisualAlerts(bool value);

	unsigned int GetDwellTime() const {
		return (unsigned int) (m_dwellCountdown.GetWaitTimeMs() / 100);
	}
	void SetDwellTime (unsigned int ds) {
		if (ds> 50) ds= 50;
		m_dwellCountdown.SetWaitTimeMs (ds * 100);
	}

	unsigned int GetDwellToleranceArea() const {
		return (unsigned int) m_dwellToleranceArea; 
	}
	void SetDwellToleranceArea(unsigned int value) {
		if (value> 8) value= 8;
		m_dwellToleranceArea= (float) value;
	}

	bool GetFastGestureAction() const { return m_fastGestureAction; }
	void SetFastGestureAction(bool value) { m_fastGestureAction= value; }

	// Possible actions that can be generated. Values equal or greater than
	// EActionLast are keystrokes picked from  m_keyboardCodes
	enum EAction { DISABLE= 0, SINGLE, THIRD, SECONDARY, DOUBLE, DRAG, EActionLast };
	enum { MOUSE_EVENTS_COUNT= EActionLast };

	int GetPossibleActionsCount() const {
		return MOUSE_EVENTS_COUNT + (int) m_keyboardCodes.size();
	}

	EAction GetActionLeft() const {	return m_actionLeft; }
	void SetActionLeft(EAction action) {
		assert (GetPossibleActionsCount()> action);
		if (GetPossibleActionsCount()<= action) action= DISABLE;
		m_actionLeft = action; 
	}

	EAction GetActionRight() const { return m_actionRight; }
	void SetActionRight(EAction action) {
		assert (GetPossibleActionsCount()> action);
		if (GetPossibleActionsCount()<= action) action= DISABLE;
		m_actionRight = action; 
	}

	EAction GetActionTop() const { return m_actionTop; }
	void SetActionTop(EAction action) {
		assert (GetPossibleActionsCount()> action);
		if (GetPossibleActionsCount()<= action) action= DISABLE;
		m_actionTop = action; 
	}

	EAction GetActionBottom() const { return m_actionBottom; }
	void SetActionBottom(EAction action) {
		assert (GetPossibleActionsCount()> action);
		if (GetPossibleActionsCount()<= action) action= DISABLE;
		m_actionBottom = action; 
	}

	KeyboardCode GetKeyboardCode(unsigned int position) {
		assert (position< m_keyboardCodes.size());
		return m_keyboardCodes[position];
	}
	unsigned int GetKeyEventsCount() const {
		return (unsigned int) m_keyboardCodes.size();
	}

	// Configuration methods
	virtual void InitDefaults();
	virtual void WriteProfileData(wxConfigBase* pConfObj);
	virtual void ReadProfileData(wxConfigBase* pConfObj);

private:
	
	enum EState { DWELL_TIME, COMPUTE_DIRECTION, WAIT_DWELL };

	void InitKeyboardCodes();
	mousecmd::mousecmd DoAction (EAction action);

	// Working attributes
	volatile bool m_enabled;
	CWaitTime m_dwellCountdown;
	CVisualAlertProgress m_progressVisualAlert;
	CVisualAlertDirection m_gestureVisualAlert;
	bool m_isLeftPressed;
	int m_xIniGesture, m_yIniGesture;
	EState m_state;

	// Associations
	CMouseControl* m_mouseControl;

	// State
	//bool m_consecutiveClicksAllowed;
	bool m_visualAlertsEnabled;	
	//Define maximum distance (in pixels) from pointer's starting countdown position
	//where is allowed to move without cancelling current countdown.
	float m_dwellToleranceArea;	
	EAction m_actionLeft;
	EAction m_actionRight;
	EAction m_actionTop;
	EAction m_actionBottom;
	bool m_fastGestureAction;	
	std::vector<KeyboardCode> m_keyboardCodes;
};

#endif
