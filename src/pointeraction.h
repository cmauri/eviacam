/////////////////////////////////////////////////////////////////////////////
// Name:        pointeraction.h
// Purpose:
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
#ifndef POINTERACTION_H
#define POINTERACTION_H

#include "mousecontrol.h"
#include "configbase.h"
#include <math.h>

class wxSound;
class wxWindow;
class CMouseControl;
class CDwellClick;
class CGestureClick;

class CPointerAction : public CConfigBase
{
public:
	CPointerAction ();
	~CPointerAction ();

	// Call from machine vision tracker
	void ProcessMotion (float dxSensor, float dySensor);

	// Enable/disable processing motion and generating actions
	bool GetEnabled() const { return m_enabled; }
	void SetEnabled(bool value);

	// Choose among diferent types of click generation modes
	enum EClickMode { NONE, DWELL, GESTURE };
	EClickMode GetClickMode() const { return m_clickMode; }
	bool SetClickMode(EClickMode mode, bool silent= true, wxWindow* parent= NULL);

	unsigned int GetXSpeed() const { return  m_xSpeed; }
	void SetXSpeed(unsigned int value) {
		if (value >= 0 && value <= 30)
		{
			m_xSpeed= value;
			m_pMouseControl->SetRelFactorX (GetSpeedFactor(m_xSpeed));
		}
	}

	unsigned int GetYSpeed() const { return m_ySpeed; }
	void SetYSpeed(unsigned int value) {
		if (value >= 0 && value <= 30)
		{
			m_ySpeed= value;
			m_pMouseControl->SetRelFactorY (GetSpeedFactor(m_ySpeed));
		}
	}

	unsigned int GetAcceleration() const {
		return m_acceleration;
	}
	void SetAcceleration(unsigned int acceleration);

	unsigned int GetSmoothness() const {
		return (unsigned int) (powf (10.0f, m_pMouseControl->GetLowPassFilterWeight ()) + 0.5f) - 1;
	}
	void SetSmoothness (unsigned int smoothness) {
		if (smoothness> 8) smoothness= 8;
		m_pMouseControl->SetLowPassFilterWeight (log10f((float) smoothness + 1));
	}

	unsigned int GetEasyStopValue() const { 
		return (unsigned int) m_pMouseControl->GetRelDeltaThreshold (); 
	}
	void SetEasyStopValue (unsigned int value) {
		if (value> 10) value= 10;
		m_pMouseControl->SetRelDeltaThreshold ( (float) value);
	}

	bool GetBeepOnClick() const { return m_beepOnClick; }
	void SetBeepOnClick(bool value) { m_beepOnClick = value; }
	
	// Common for dwell and gesture click
	bool GetVisualAlerts() const;
	void SetVisualAlerts(bool value);

	// Common for dwell and gesture click
	unsigned int GetDwellTime() const;
	void SetDwellTime (unsigned int ds);

	// Common for dwell and gesture click
	unsigned int GetDwellToleranceArea() const;
	void SetDwellToleranceArea(unsigned int value);

	void SetRestrictedWorkingArea (bool value) { 
		m_pMouseControl->SetRestrictedWorkingArea(value); 
	}
	bool GetRestrictedWorkingArea() const { 
		return m_pMouseControl->GetRestrictedWorkingArea();
	}

	unsigned int GetTopWorkspace() const { 
		return (unsigned int) ((1.0f - m_pMouseControl->GetTopPercent()) * 50.0f + 0.5f);
	}
	void SetTopWorkspace(unsigned int value) {
	    if (value > 50) value = 50;
	    m_pMouseControl->SetTopPercent((float) (50 - value) / 50.0f);
	}

	unsigned int GetLeftWorkspace() const { 
		return (unsigned int) ((1.0f - m_pMouseControl->GetLeftPercent()) * 50.0f + 0.5f);
	}
	void SetLeftWorkspace(unsigned int value) {
		if (value > 50) value = 50;
		m_pMouseControl->SetLeftPercent((float) (50 - value) / 50.0f);
	}
	
	unsigned int GetRightWorkspace() const { 
		return (unsigned int) ((1.0f - m_pMouseControl->GetRightPercent()) * 50.0f + 0.5f);
	}
	void SetRightWorkspace(unsigned int value) {
		if (value > 50) value = 50;
		m_pMouseControl->SetRightPercent((float) (50 - value) / 50.0f);
	}

	unsigned int GetBottomWorkspace() {
		return (unsigned int) ((1.0f - m_pMouseControl->GetBottomPercent()) * 50.0f + 0.5f);
	}
	void SetBottomWorkspace(unsigned int value) {
		if (value > 50) value = 50;
		m_pMouseControl->SetBottomPercent((float) (50 - value) / 50.0f);
	}
	
	void SetWrapPointer (bool value) { 
		m_pMouseControl->SetWrapPointer(value); 
	}
	bool GetWrapPointer() const { 
		return m_pMouseControl->GetWrapPointer();
	}

	void SetSendActionWait(unsigned int ms) {
		if (ms> 50) ms= 50;
		m_pMouseControl->SetSendActionWait(ms);
	}
	unsigned int GetSendActionWait() const { return m_pMouseControl->GetSendActionWait(); }

	void ResetCounters();

	//
	void CenterPointer () { m_pMouseControl->CenterPointer(); };
	CDwellClick& GetDwellClick() { assert (m_pDwellClick); return *m_pDwellClick; }
	CGestureClick& GetGestureClick() { assert (m_pGestureClick); return *m_pGestureClick; }

    // Configuration methods
	virtual void InitDefaults();
	virtual void WriteProfileData(wxConfigBase* pConfObj);
	virtual void ReadProfileData(wxConfigBase* pConfObj);

private:
	float GetSpeedFactor(unsigned int speed) const;
	
	bool m_enabled;
	unsigned int m_xSpeed, m_ySpeed;
	unsigned int m_acceleration;
	EClickMode m_clickMode;
	bool m_beepOnClick;
	wxSound* m_pClickSound;
	wxSound* m_pLeftUpClickSound;
	CMouseControl* m_pMouseControl;
	CDwellClick* m_pDwellClick;
	CGestureClick* m_pGestureClick;
};

#endif
