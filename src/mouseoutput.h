/////////////////////////////////////////////////////////////////////////////
// Name:        mouseoutput.h
// Purpose:
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by:
// Created:
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
#ifndef MOUSEOUTPUT_H
#define MOUSEOUTPUT_H

#include "waittime.h"
#include "mousecontrol.h"
#include "configbase.h"
#include "cvisualalert.h"
#include "ckeyboardcode.h"
#include <math.h>
#include <vector>

class CClickWindowController;
class wxSound;

class CMouseOutput : public CMouseControl, public CConfigBase
{
public:
	enum EClickMode { NONE, DWELL, GESTURE };
	CMouseOutput (CClickWindowController& pClickWindowController);
	~CMouseOutput ();

	// Call from machine vision tracker
	float ProcessRelativePointerMove(float dx, float dy);
	
	// TODO: check if is really needed
	void EndVisualAlerts();

	inline const unsigned long GetXSpeed() const;
	inline void SetXSpeed(unsigned long value);

	inline const unsigned long GetYSpeed() const;
	inline void SetYSpeed(unsigned long value);

	inline const unsigned long GetAcceleration() const;
	void SetAcceleration(unsigned long acceleration);

	inline const unsigned long GetSmoothness() const;
	inline void SetSmoothness (unsigned long smoothness);

	inline const unsigned long GetEasyStopValue() const;
	inline void SetEasyStopValue (unsigned long value);

	inline const EClickMode GetClickMode() const;
	void SetClickMode(EClickMode mode);

        inline const unsigned long GetTopWorkspace() const;
        inline void SetTopWorkspace(unsigned long value);

        inline const unsigned long GetLeftWorkspace() const;
        inline void SetLeftWorkspace(unsigned long value);

        inline const unsigned long GetRightWorkspace() const;
        inline void SetRightWorkspace(unsigned long value);

        inline const unsigned long GetBottomWorkspace() const;
        inline void SetBottomWorkspace(unsigned long value);

        inline const bool GetRestrictedWorkingArea() const;
        inline void SetRestrictedWorkingArea(bool value);

	inline const bool GetConsecutiveClicksAllowed() const;
	inline void SetConsecutiveClicksAllowed(bool value);

	inline const unsigned long GetDwellTime() const;
	inline void SetDwellTime (unsigned long ds);

	inline const unsigned long GetGestureTime() const;
	inline void SetGestureTime (unsigned long ds);

	inline const unsigned long GetDwellToleranceArea() const;
	inline void SetDwellToleranceArea(unsigned long value);

	inline const bool GetBeepOnClick() const;
	inline void SetBeepOnClick(bool value);  

	inline const bool GetEnabled() const;
	void SetEnabled(bool value);
	
	inline const int GetActionLeft() const;
	inline void SetActionLeft(int);

	inline const int GetActionRight() const;
	inline void SetActionRight(int);

	inline const int GetActionTop() const;
	inline void SetActionTop(int);

	inline const int GetActionBottom() const;
	inline void SetActionBottom(int);

	inline const bool GetVisualAlerts() const;
	inline void SetVisualAlerts(bool value);
	
	inline CKeyboardCode GetKeyboardCode(int position);
	inline const unsigned int GetKeyEventsCount() const;
	inline const unsigned int GetMouseEventsCount() const;

	bool GetFastGestureAction() const { return m_fastGestureAction; }
	void SetFastGestureAction(bool value) { m_fastGestureAction= value; }

	// Configuration methods
	virtual void InitDefaults();
	virtual void WriteProfileData(wxConfigBase* pConfObj);
	virtual void ReadProfileData(wxConfigBase* pConfObj);

private:
	const float GetSpeedFactor(unsigned long speed) const;
	
	// TODO: this codi needs refactoring. Dwell click & gesture click 
	// should be in its own class for each
	
	//
	// General attributes
	//
	bool m_enabled;
	unsigned long m_xSpeed, m_ySpeed;
	unsigned long m_acceleration;
	EClickMode m_clickMode;
	bool m_beepOnClick;
	bool m_consecutiveClicksAllowed;
	bool m_visualAlerts;
	wxSound* m_pClickSound;	
	//Define maximal distance (in pixels) from pointer's starting countdown position
	//where is allowed to move without cancelling current countdown. Try and choose: 
	// countdown starts when motion is under tolerance or where zero motion interval is detected.
	float m_dwellToleranceArea;
	
	//
	// Dwell click
	//
	CClickWindowController* m_pClickWindowController;
	CWaitTime m_dwellCountdown;
	//CVisualAlert m_dwellVisualAlert;
	CVisualAlertProgress m_progressVisualAlert;
	
	//
	// Workspace limits
	//
        unsigned long m_topWorkspace, m_leftWorkspace, m_rightWorkspace, m_bottomWorkspace; 
        bool m_restrictedWorkingArea;

	//
	// Gesture click
	//
	void InitKeyboardCodes();
	enum EState { DWELL_TIME, COMPUTE_DIRECTION, WAIT_DWELL }; //, WAITING_RETURN };	
	enum EDirection { LEFT, RIGHT, TOP, BOTTOM };
	void DoActionGesture(EDirection direction);
	EState m_state;
	bool m_waitingGesture;	
	CVisualAlertDirection m_gestureVisualAlert;
	int m_actionLeft;
	int m_actionRight;
	int m_actionTop;
	int m_actionBottom;
	bool m_isLeftPressed;
	bool m_fastGestureAction;
	//float m_sumDx, m_sumDy;	
	//EDirection m_direction;	
	std::vector<CKeyboardCode> m_keyboardCodes;
	long m_xIniGesture, m_yIniGesture;
};

inline const unsigned long CMouseOutput::GetXSpeed() const
{
	return  m_xSpeed;
}

inline void CMouseOutput::SetXSpeed(unsigned long value)
{
	if (value> 30) value= 30;
	m_xSpeed= value;
	SetRelFactorX (GetSpeedFactor(m_xSpeed));
}

inline const unsigned long CMouseOutput::GetYSpeed() const
{
	return m_ySpeed;
}

inline void CMouseOutput::SetYSpeed(unsigned long value)
{
	if (value> 30) value= 30;
	m_ySpeed= value;
	SetRelFactorY (GetSpeedFactor(m_ySpeed));
}

inline const unsigned long CMouseOutput::GetAcceleration() const
{
	return m_acceleration;
}

inline const unsigned long CMouseOutput::GetSmoothness() const
{
	//return (unsigned long) (GetLowPassFilterWeight () * 10.0f);
	return (unsigned long) (pow (10.0f, GetLowPassFilterWeight ()));
}

inline void CMouseOutput::SetSmoothness (unsigned long smoothness)
{
	if (smoothness> 9) smoothness= 9;
	//SetLowPassFilterWeight (((float) smoothness) / 10.0f);
	SetLowPassFilterWeight ((float) log10((double)smoothness));
}

inline const unsigned long CMouseOutput::GetEasyStopValue() const
{
	return (unsigned long) GetRelDeltaThreshold ();
}

inline void CMouseOutput::SetEasyStopValue (unsigned long value)
{
	if (value> 10) value= 10;
	SetRelDeltaThreshold ( (float) value);
}

inline const unsigned long CMouseOutput::GetTopWorkspace() const
{
    return m_topWorkspace;
}

inline void CMouseOutput::SetTopWorkspace(unsigned long value)
{
    if (value > 50) value = 50;
    m_topWorkspace = value;
    SetTopPercent((float) (50 - m_topWorkspace) / 50);
}


inline const unsigned long CMouseOutput::GetLeftWorkspace() const
{
    return m_leftWorkspace;
}

inline void CMouseOutput::SetLeftWorkspace(unsigned long value)
{
    if (value > 50) value = 50;
    m_leftWorkspace = value;
    SetLeftPercent((float) (50 - m_leftWorkspace) / 50);
}


inline const unsigned long CMouseOutput::GetRightWorkspace() const
{
    return m_rightWorkspace;
}

inline void CMouseOutput::SetRightWorkspace(unsigned long value)
{
    if (value > 50) value = 50;
    m_rightWorkspace = value;
    SetRightPercent((float) (50 - m_rightWorkspace) / 50);
}


inline const unsigned long CMouseOutput::GetBottomWorkspace() const
{
    return m_bottomWorkspace;
}

inline void CMouseOutput::SetBottomWorkspace(unsigned long value)
{
    if (value > 50) value = 50;
    m_bottomWorkspace = value;
    SetBottomPercent((float) (50 - m_bottomWorkspace) / 50);
}


inline const bool CMouseOutput::GetRestrictedWorkingArea() const
{
    return m_restrictedWorkingArea;
}

inline void CMouseOutput::SetRestrictedWorkingArea(bool value)
{
    m_restrictedWorkingArea = value;
    SetEnabledRestrictedWorkingArea(m_restrictedWorkingArea);
}


inline const bool CMouseOutput::GetConsecutiveClicksAllowed() const {
	return m_consecutiveClicksAllowed;
}

inline void CMouseOutput::SetConsecutiveClicksAllowed(bool value) {
	m_consecutiveClicksAllowed = value;
}

inline const unsigned long CMouseOutput::GetDwellTime() const {
	return m_dwellCountdown.GetWaitTimeMs() / 100;
}

inline void CMouseOutput::SetDwellTime (unsigned long ds) 
{
	if (ds> 50) ds= 50;
	m_dwellCountdown.SetWaitTimeMs (ds * 100);
}

// TODO: remove (check config dialog)
inline const unsigned long CMouseOutput::GetGestureTime() const {
	//return m_gestureCountdown->GetWaitTimeMs() / 100;
	return 0;
}

// TODO: remove (check config dialog)
inline void CMouseOutput::SetGestureTime (unsigned long ds) 
{
	if (ds> 50) ds= 50;
	//m_gestureCountdown->SetWaitTimeMs (ds * 100);
}

inline const unsigned long CMouseOutput::GetDwellToleranceArea() const {
	return (unsigned long) m_dwellToleranceArea;
}

inline void CMouseOutput::SetDwellToleranceArea(unsigned long area) 
{
	if (area> 5) area= 5;
	m_dwellToleranceArea= (float) area;
}

inline const bool CMouseOutput::GetBeepOnClick() const {
	return m_beepOnClick;
}

inline void CMouseOutput::SetBeepOnClick(bool value) {
	m_beepOnClick = value;
}

inline const CMouseOutput::EClickMode CMouseOutput::GetClickMode() const {
	return m_clickMode;
}

inline const bool CMouseOutput::GetEnabled() const
{
	return m_enabled;
}

inline const int CMouseOutput::GetActionLeft() const
{
	return m_actionLeft;
}

inline void CMouseOutput::SetActionLeft(int action)
{
	m_actionLeft = action;
}

inline const int CMouseOutput::GetActionRight() const
{
	return m_actionRight;
}

inline void CMouseOutput::SetActionRight(int action)
{
	m_actionRight = action;
}

inline const int CMouseOutput::GetActionTop() const
{
	return m_actionTop;
}

inline void CMouseOutput::SetActionTop(int action)
{
	m_actionTop = action;
}

inline const int CMouseOutput::GetActionBottom() const
{
	return m_actionBottom;
}

inline void CMouseOutput::SetActionBottom(int action)
{
	m_actionBottom = action;
}

inline const bool CMouseOutput::GetVisualAlerts() const
{
	return m_visualAlerts;
}
		
inline void CMouseOutput::SetVisualAlerts(bool value)
{
	m_visualAlerts = value;
}

inline CKeyboardCode CMouseOutput::GetKeyboardCode(int position)
{
	return m_keyboardCodes[position];
}

inline const unsigned int CMouseOutput::GetKeyEventsCount() const
{
	return m_keyboardCodes.size();
}
/*
inline const unsigned int CMouseOutput::GetMouseEventsCount() const
{
	return MOUSE_EVENTS_COUNT;
}*/

#endif
