/////////////////////////////////////////////////////////////////////////////
// Name:        dwellclick.h
// Purpose:
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by:
// Created:
// Copyright:   (C) 2010-12 Cesar Mauri Loba - CREA Software Systems
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
#ifndef DWELLCLICK_H
#define DWELLCLICK_H

#include "waittime.h"
#include "configbase.h"
#include "cvisualalert.h"
#include "mousecommand.h"

class CMouseControl;
class CClickWindowController;

class CDwellClick : public CConfigBase
{
public:
	CDwellClick (CMouseControl& mc);
	~CDwellClick ();

	// Main entry point to signal that the pointer has been moved.
	// Expect motion performed by the pointer in pixels and current
	// pointer location in absolute coordinates
	// Return the mouse command generated
	mousecmd::mousecmd ProcessMotion (int dxPix, int dyPix, unsigned int xCurr, unsigned int yCurr);
	
	bool GetEnabled() const { return m_enabled; }
	void SetEnabled(bool value);

	// Reset internal state. Useful before start calling ProcessMotion
	void Reset();

	bool GetUseClickWindow() const { return m_useClickWindow; }
	void SetUseClickWindow(bool value);
	
	bool AreVisualAlertsEnabled() const { return m_visualAlertsEnabled; }
	void EnableVisualAlerts(bool value);

	const bool GetConsecutiveClicksAllowed() const { 
		return m_consecutiveClicksAllowed; 
	}
	void SetConsecutiveClicksAllowed(bool value) {
		m_consecutiveClicksAllowed = value;
	}

	// Return dwell time is ds
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

	CClickWindowController& GetClickWindowController() {
		return *m_pClickWindowController;
	}
	
	// Configuration methods
	virtual void InitDefaults();
	virtual void WriteProfileData(wxConfigBase* pConfObj);
	virtual void ReadProfileData(wxConfigBase* pConfObj);

private:
	
	volatile bool m_enabled;
	bool m_consecutiveClicksAllowed;
	bool m_visualAlertsEnabled;
	bool m_useClickWindow;	
	//Define maximum distance (pixels) from pointer's starting countdown position
	//where is allowed to move without cancelling current countdown.
	float m_dwellToleranceArea;
	CClickWindowController* m_pClickWindowController;
	CWaitTime m_dwellCountdown;
	CVisualAlertProgress m_progressVisualAlert;

	CMouseControl* m_mouseControl;
};

#endif
