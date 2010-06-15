/////////////////////////////////////////////////////////////////////////////
// Name:        crvcamera.h
// Purpose:  
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by: 
// Created:     30/05/2008
// Copyright:   (C) 2008-10 Cesar Mauri Loba - CREA Software Systems
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
#ifndef CRVCAMERA_H_
#define CRVCAMERA_H_

#include <cv.h>

class CCamera
{
public:
	CCamera();
	virtual ~CCamera (void);

	// 
	// Pure virtual methods
	//
	virtual bool Open()= 0;
	virtual void Close()= 0;
	virtual IplImage* QueryFrame()= 0;
	
	// 
	// Virtual desired methods
	//
	virtual bool HasSettingsDialog() { return false; }
	virtual void ShowSettingsDialog () {}

	//
	// Implemented methods
	//

	// Should be called in the QueryFrame body
	void OnQueryFrame(IplImage *pImage);

	void ShowLive ();	
	void CloseLive ();
	
	int GetWidth () {return m_Width; }
	int GetRealWidth () {return m_RealWidth; }
	//void SetWidth (int width);

	int GetHeight () {return m_Height; }
	int GetRealHeight () {return m_RealHeight; }
	//void SetHeight (int height);

	float GetFrameRate ();
	float GetRealFrameRate ();
	//void SetFrameRate (double fr);
	
	inline bool GetHorizontalFlip ();
	inline void SetHorizontalFlip (bool value);	
protected:
	int GetTime (void);

	int m_Id;
	int m_Width, m_Height;
	int m_RealWidth, m_RealHeight;
	float m_FrameRate;
	float m_RealFrameRate, m_LastRealFrameRate;
	bool m_showingLive;
	int m_lastTimeStamp;
	int m_elapsedTime;
	bool m_horizontalFlip;
};

inline bool CCamera::GetHorizontalFlip ()
{
	return m_horizontalFlip;
}

inline void CCamera::SetHorizontalFlip (bool value)
{
	m_horizontalFlip= value;
}

#endif
