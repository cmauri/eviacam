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


#include "dwellaction.h"
#include "mousecontrol.h"
#include "configbase.h"
#include <math.h>

class CClickWindowController;
class wxSound;

class CMouseOutput : public CMouseControl, public CConfigBase
{
  public:
	enum EClickMode { NONE, DWELL };

	CMouseOutput (CClickWindowController& pClickWindowController);
	~CMouseOutput ();

	// Call from machine vision tracker
	float ProcessRelativePointerMove(float dx, float dy);

	//Reset internal state (dwell click time)
	void Reset();

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

	inline const bool GetConsecutiveClicksAllowed() const;
	inline void SetConsecutiveClicksAllowed(bool value);

	inline const unsigned long GetDwellTime() const;
	inline void SetDwellTime (unsigned long ds);

	inline const unsigned long GetDwellToleranceArea() const;
	inline void SetDwellToleranceArea(unsigned long value);

	inline const bool GetBeepOnClick() const;
	inline void SetBeepOnClick(bool value);  

	inline const bool GetEnabled() const;
	void SetEnabled(bool value);

	// Configuration methods
	virtual void InitDefaults();
	virtual void WriteProfileData(wxConfigBase* pConfObj);
	virtual void ReadProfileData(wxConfigBase* pConfObj);

  private:
	
	const float GetSpeedFactor(unsigned long speed) const;

	CClickWindowController * m_pClickWindowController;

	unsigned long m_xSpeed, m_ySpeed;
	unsigned long m_acceleration;
 
	bool m_enabled;
	CDwellAction m_dwellAction;
	EClickMode m_clickMode;
	bool m_beepOnClick;
	bool m_consecutiveClicksAllowed;
    //Define maximal distance (in pixels) from pointer's starting countdown position
    //where is allowed to move without cancelling current countdown.
    //
    //Try and choose: countdown starts when motion is under tolerance or where zero motion
    //interval is detected.
	float m_dwellToleranceArea;

	wxSound* m_pClickSound;
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

inline const bool CMouseOutput::GetConsecutiveClicksAllowed() const {
	return m_consecutiveClicksAllowed;
}

inline void CMouseOutput::SetConsecutiveClicksAllowed(bool value) {
	m_consecutiveClicksAllowed = value;
}

inline const unsigned long CMouseOutput::GetDwellTime() const {
	return m_dwellAction.GetWaitTimeMs() / 100;
}

inline void CMouseOutput::SetDwellTime (unsigned long ds) 
{
	if (ds> 50) ds= 50;
	m_dwellAction.SetWaitTimeMs (ds * 100);
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

#endif
