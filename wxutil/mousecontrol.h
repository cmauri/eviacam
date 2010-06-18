/////////////////////////////////////////////////////////////////////////////
// Name:        mousecontrol.h
// Purpose: cross platform mouse control api
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
#if !defined(MOUSECONTROL_H)
#define MOUSECONTROL_H

#if !defined(WIN32)
#include <X11/Xlib.h>		// Display struct
#endif
#include <stdlib.h>		// NULL
#include <assert.h>		

class CMouseControl
{
public:
	CMouseControl(void* pDisplay= NULL);
	virtual ~CMouseControl();

	void SetWorkingArea (float leftPercent, float rightPercent,
						 float topPercent, float bottomPercent);

	void SetClickArea (long minX, long minY, long maxX, long maxY);
	void ResetClickArea ();
	
	void SetAbsVirtualResolution (float xIni, float yIni, 
								  float width, float height);
	void SetRelFactorX (float fDx) { m_fDx= fDx; }
	void SetRelFactorY (float fDy) { m_fDy= fDy; }
	void SetRelFactors (float fDx, float fDy) {	SetRelFactorX (fDx); SetRelFactorY (fDy); }
	void SetRelAcceleration2 (long delta0= 9999, float factor0= 1.0f,
					long delta1= 9999, float factor1= 1.0f);

	bool getEnabledRestrictedWorkingArea () { return m_enabledRestrictedWorkingArea; }
	void setEnabledRestrictedWorkingArea (bool value) { m_enabledRestrictedWorkingArea = value; }


	inline float GetRelDeltaThreshold () const;
	inline void SetRelDeltaThreshold (float deltaThreshold);
	
	inline float GetLowPassFilterWeight () const;
	inline void SetLowPassFilterWeight (float w);
	
	void MovePointerAbs (float x, float y);
	float MovePointerRel (float dx, float dy);

	// Click actions
	bool LeftDown ();
	void LeftUp ();
	bool RightDown ();
	void RightUp ();
	bool LeftClick ();
	bool RightClick ();
	bool LeftDblClick ();
	
	// Display properties change (resolution)
	void OnDisplayChanged ();

protected:

	void Virt2Fis (float virtX, float virtY, float &fisX, float &fisY);
	bool CheckClickArea ();
	bool EnforceWorkingAreaLimits (long &x, long &y);
	void SendMouseCommand (long x, long y, int flags);
	void DoMovePointerAbs (long x, long y);
	void DoMovePointerRel (long dx, long dy);
	void RecomputeWorkingArea ();

	// TODO: derive a X11 file stuff for these
	void GetScreenSize();
	void GetPointerLocation (long& x, long& y);
	
private:	
	int m_ScreenWidth, m_ScreenHeight;
	int m_minScreenX, m_minScreenY, m_maxScreenX, m_maxScreenY;
	float m_leftPercent, m_rightPercent, m_topPercent, m_bottomPercent;
	int m_minClicAreaX, m_minClicAreaY, m_maxClicAreaX, m_maxClicAreaY;
	float m_VirtualXIni, m_VirtualYIni;
	float m_VirtualWidth, m_VirtualHeight;
	float m_fDx, m_fDy;
	float m_minDeltaThreshold;
	float m_dxant, m_dyant;
	bool m_enabledRestrictedWorkingArea;

	// Motion low pass filter (relative motion)
	float m_actualMotionWeight;

	enum { ACCEL_ARRAY_SIZE= 30 };
	float m_accelArray[ACCEL_ARRAY_SIZE];

#if !defined(WIN32)
	Display* m_pDisplay;
#endif

};

inline float CMouseControl::GetLowPassFilterWeight () const
{
	return m_actualMotionWeight;
}

inline void CMouseControl::SetLowPassFilterWeight (float w)
{ 
	assert (0.0f<= w && 1.0f>= w);
	m_actualMotionWeight= w; 
}

inline float CMouseControl::GetRelDeltaThreshold () const	
{
	return m_minDeltaThreshold;
}

inline void CMouseControl::SetRelDeltaThreshold (float deltaThreshold)	
{
	assert (deltaThreshold>= 0);

	m_minDeltaThreshold= deltaThreshold;
}

#endif
