/////////////////////////////////////////////////////////////////////////////
// Name:        mousecontrol.h
// Purpose: cross platform mouse control api
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by: 
// Created:     
// Copyright:   (C) 2008-14 Cesar Mauri Loba - CREA Software Systems
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

#include <stdlib.h>		// NULL
#include <assert.h>		

class CMouseControl
{
public:
	CMouseControl(void* pDisplay= NULL);
	virtual ~CMouseControl();

	void SetRestrictedWorkingArea (bool value) { m_enabledRestrictedWorkingArea = value; }
	bool GetRestrictedWorkingArea() const { return m_enabledRestrictedWorkingArea; }
	void SetWorkingArea (float leftPercent, float rightPercent, float topPercent, float bottomPercent);
	
	void SetWrapPointer (bool value) { m_enabledWrapPointer = value; }
	bool GetWrapPointer() const { return m_enabledWrapPointer; }

	float GetTopPercent () const { return m_topPercent; }
	void SetTopPercent (float topPercent) { 
		//assert (0.009f < topPercent && topPercent <= 1.0f);
		assert (0 <= topPercent && topPercent <= 1.0f);
		m_topPercent = topPercent; 
	}
	float GetLeftPercent () const { return m_leftPercent; }
	void SetLeftPercent (float leftPercent) { 
		assert (0 <=  leftPercent && leftPercent <= 1.0f);
		m_leftPercent = leftPercent; 
	}
	float GetRightPercent () const { return m_rightPercent; }
	void SetRightPercent (float rightPercent) { 
		assert (0 <=  rightPercent && rightPercent <= 1.0f);
		m_rightPercent = rightPercent; 
	}
	float GetBottomPercent () const { return m_bottomPercent; }
	void SetBottomPercent (float bottomPercent) { 
		assert (0 <=  bottomPercent && bottomPercent <= 1.0f);
		m_bottomPercent = bottomPercent;
	}
	
	void SetAbsVirtualResolution (float xIni, float yIni, 
								  float width, float height);
	void SetRelFactorX (float fDx) { m_fDx= fDx; }
	void SetRelFactorY (float fDy) { m_fDy= fDy; }
        
	void SetRelFactors (float fDx, float fDy) {	SetRelFactorX (fDx); SetRelFactorY (fDy); }
	void SetRelAcceleration2 (long delta0= 9999, float factor0= 1.0f,
					long delta1= 9999, float factor1= 1.0f);

	inline float GetRelDeltaThreshold () const;
	inline void SetRelDeltaThreshold (float deltaThreshold);
	
	inline float GetLowPassFilterWeight () const;
	inline void SetLowPassFilterWeight (float w);
	
	void MovePointerAbs (float x, float y);
	void DoMovePointerAbs (long x, long y);
	float MovePointerRel (float dx, float dy, int* dxRes= NULL, int* dyRes= NULL);
	
	void CenterPointer ();

	// Click actions
	void LeftDown ();
	void LeftUp ();
	void MiddleDown ();
	void MiddleUp ();
	void RightDown ();
	void RightUp ();
	void LeftClick ();
	void MiddleClick ();
	void RightClick ();
	void LeftDblClick ();

	void GetPointerLocation (int& x, int& y);

	// Display properties change (resolution)
	void OnDisplayChanged ();

	void SetSendActionWait(unsigned int ms) { m_sendActionWait= ms; }
	unsigned int GetSendActionWait() const { return m_sendActionWait; }

protected:

	void Virt2Fis (float virtX, float virtY, float &fisX, float &fisY);
	bool EnforceWorkingAreaLimits (int &x, int &y);
	void SendMouseCommand (long x, long y, int flags);
	
	void DoMovePointerRel (long dx, long dy);
	void RecomputeWorkingArea ();

	void GetScreenSize();

private:	
	int m_ScreenWidth, m_ScreenHeight;
	int m_minScreenX, m_minScreenY, m_maxScreenX, m_maxScreenY;
	float m_leftPercent, m_rightPercent, m_topPercent, m_bottomPercent;
	float m_VirtualXIni, m_VirtualYIni;
	float m_VirtualWidth, m_VirtualHeight;
	float m_fDx, m_fDy;
	float m_minDeltaThreshold;
	float m_dxant, m_dyant;
	// Motion low pass filter (relative motion)
	float m_actualMotionWeight;
	bool m_enabledRestrictedWorkingArea;
	bool m_enabledWrapPointer;
	
#if !defined(WIN32)
	bool m_closeDisplay;	// Destructor should close display connection?
	void* m_pDisplay;
#endif
	enum { ACCEL_ARRAY_SIZE= 30 };
	float m_accelArray[ACCEL_ARRAY_SIZE];

	unsigned int m_sendActionWait;
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
