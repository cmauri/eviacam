/////////////////////////////////////////////////////////////////////////////
// Name:        mouseoutput.cpp
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

#include "mouseoutput.h"
#include "clickwindowcontroller.h"
#include <wx/stdpaths.h>
#include <wx/sound.h>
#include <wx/utils.h>
#include <math.h>

CMouseOutput::CMouseOutput(CClickWindowController& pClickWindowController)
#if defined(__WXGTK__)
	: CMouseControl ((void *) wxGetDisplay())
#endif
{
	m_pClickWindowController= &pClickWindowController;
	
	m_enabled= false;
        m_restrictedWorkingArea = false;
	m_waitingGesture = false;
	m_isLeftPressed = false;

	InitDefaults ();
	   
	m_dwellCountdown.Reset();  
	m_pClickSound= new wxSound (wxStandardPaths::Get().GetDataDir() + _T("/click.wav"));

}

CMouseOutput::~CMouseOutput ()
{
	delete m_pClickSound;
}

const float CMouseOutput::GetSpeedFactor(unsigned long speed) const
{
	return (float) pow (2.718281828459045235, speed / 6.0);	
}

void CMouseOutput::SetAcceleration(unsigned long acceleration)
{
	if (acceleration> 5) acceleration= 5;

	switch (acceleration) {
	case 0: SetRelAcceleration2 (); break;
	case 1: SetRelAcceleration2 (7, 1.5f); break;
	case 2: SetRelAcceleration2 (7, 2.0f); break;
	case 3: SetRelAcceleration2 (7, 1.5f, 14, 2.0f); break;
	case 4: SetRelAcceleration2 (7, 2.0f, 14, 1.5f); break;
	case 5: SetRelAcceleration2 (7, 2.0f, 14, 2.0f); break;
	default: assert (0);
	}

	m_acceleration= acceleration;
}

float CMouseOutput::ProcessRelativePointerMove(float dx, float dy) 
{
	float despl;
	
	if (!m_enabled) return 0.0f;

	// Do move.
	despl= MovePointerRel (dx, dy);

	if (m_clickMode== CMouseOutput::DWELL)
	{
		// DWell click
		
		// TODO: check relative from dwell start position
		if (despl> m_dwellToleranceArea) 
		{
			// Pointer moving
			m_dwellCountdown.Reset();
		}
		else 
		{
			// Pointer static
			long x, y;
			CClickWindowController::EAction action;

			GetPointerLocation (x, y);
			action= m_pClickWindowController->GetAction (x, y);

			if (action== CClickWindowController::ACT_NO_CLICK)
				m_dwellCountdown.Reset();
			else
			{
				if (m_dwellCountdown.Update())
				{
					// Send action
					switch (action)
					{
					case CClickWindowController::ACT_LEFT_CLICK:
						LeftClick();
						break;
					case CClickWindowController::ACT_RIGHT_CLICK:
						RightClick();
						break;
					case CClickWindowController::ACT_LEFT_DOWN:
						LeftDown();
						break;
					case CClickWindowController::ACT_LEFT_UP:
						LeftUp();
						break;
					case CClickWindowController::ACT_DOUBLE_CLICK:
						LeftDblClick();
						break;
					default:
						assert (false);
					}

					m_pClickWindowController->ActionDone(x, y);
					if (m_consecutiveClicksAllowed) m_dwellCountdown.Reset();

					if (m_beepOnClick) m_pClickSound->Play (wxSOUND_ASYNC);

				}
			}						
		}
	} else if (m_clickMode== CMouseOutput::GESTURE)
	{
		// Gesture click
		
		if (!m_waitingGesture)
		{
			if (despl> m_dwellToleranceArea) 
			{
				// Pointer moving
				m_preGestureCountdown.Reset();
			}
			else 
			{
				if (m_preGestureCountdown.Update())
				{
					m_gestureCountdown.Reset();
					m_waitingGesture = true;
					printf("START\n");
				}
			}
		}
		else
		{
			if (m_gestureCountdown.Update())
			{
				m_preGestureCountdown.Reset();
				m_waitingGesture = false;
				printf("STOP\n");
			}
			else
			{
				if (despl> m_dwellToleranceArea)
				{
					// Compute direction
					if (fabs(dx) > fabs(dy))
					{
						if (dx < 0) DoAction(m_actionLeft);
						else DoAction(m_actionRight);
					}
					else
					{
						if (dy < 0) DoAction(m_actionTop);
						else DoAction(m_actionBottom);
					}
					
					if (m_consecutiveClicksAllowed)
					{
						m_gestureCountdown.Reset();
					}
					else
					{
						m_preGestureCountdown.Reset();
						m_waitingGesture = false;
					}
					
					if (m_beepOnClick) m_pClickSound->Play (wxSOUND_ASYNC);
					printf("STOP\n");
				}
			}
			
		}
	}

	return despl;
}

void CMouseOutput::DoAction(EAction action) {
	switch (action)
	{
		case DISABLE:
			break;
		case SINGLE:
			LeftClick();
			break;
		case DOUBLE:
			LeftDblClick();
			break;
		case SECONDARY:
			RightClick();
			break;
		case DRAG:
			if (m_isLeftPressed) {
				LeftUp();
				m_isLeftPressed = false;
				printf("LeftUp\n");
			}
			else
			{
				LeftDown();
				m_isLeftPressed = true;
				printf("LeftDown\n");
			}
			break;
			
	}

}

//Reset internal state (dwell click time)
void CMouseOutput::Reset(CWaitTime countdown) 
{
	countdown.Reset();
}

void CMouseOutput::SetClickMode(CMouseOutput::EClickMode mode)
{
	if (mode!= m_clickMode)
	{
		if (mode== CMouseOutput::DWELL) 
		{
			// Enable dwell click
			m_dwellCountdown.Reset();
			m_clickMode= mode;
		}
		else if (mode== CMouseOutput::GESTURE)
		{
			// Enable gesture click
			m_preGestureCountdown.Reset();
			m_clickMode= mode;			
		}		
		else if (mode== CMouseOutput::NONE)
		{
			// Disable dwell and gesture click
			m_clickMode= mode;			
		}		
	}
}

void CMouseOutput::SetEnabled(bool value)
{
	if (value!= m_enabled)
	{
		if (value)
		{
			if (m_clickMode == DWELL)
				m_dwellCountdown.Reset ();
			
			if (m_clickMode == GESTURE)
				m_preGestureCountdown.Reset ();
		}
		m_enabled= value;
	}
}

// Configuration methods
void CMouseOutput::InitDefaults()
{
	SetXSpeed (12);
	SetYSpeed (10);
	SetAcceleration (2);
	SetSmoothness (5);
	SetEasyStopValue (1);
        SetTopWorkspace(1);
        SetLeftWorkspace(1);
        SetRightWorkspace(1);
        SetBottomWorkspace(1);
	SetClickMode (CMouseOutput::DWELL);
	SetBeepOnClick (true);
	SetConsecutiveClicksAllowed(false);
	SetDwellTime (10);
	SetPreGestureTime (10);
	SetGestureTime (10);
	SetDwellToleranceArea (3); //.0f);	
}

void CMouseOutput::WriteProfileData(wxConfigBase* pConfObj)
{
	pConfObj->SetPath (_T("mouseOutput"));	

	pConfObj->Write(_T("xSpeed"), (long) GetXSpeed());
	pConfObj->Write(_T("ySpeed"), (long) GetYSpeed());
	pConfObj->Write(_T("acceleration"), (long) GetAcceleration());
	pConfObj->Write(_T("smoothness"), (long) GetSmoothness());
        pConfObj->Write(_T("easyStop"), (long) GetEasyStopValue());
        pConfObj->Write(_T("enabledWorkspace"), (bool) GetRestrictedWorkingArea());
        pConfObj->Write(_T("topWorkspace"), (long) GetTopWorkspace());
        pConfObj->Write(_T("leftWorkspace"), (long) GetLeftWorkspace());
        pConfObj->Write(_T("rightWorkspace"), (long) GetRightWorkspace());
        pConfObj->Write(_T("bottomWorkspace"), (long) GetBottomWorkspace());	
        pConfObj->Write(_T("clickMode"), (long) GetClickMode());
	pConfObj->Write(_T("beepOnClick"), (bool) GetBeepOnClick());
	pConfObj->Write(_T("consecutiveClicksAllowed"), (bool) GetConsecutiveClicksAllowed());
	pConfObj->Write(_T("dwellTime"), (long) GetDwellTime());
	pConfObj->Write(_T("preGestureTime"), (long) GetPreGestureTime());
	pConfObj->Write(_T("gestureTime"), (long) GetGestureTime());
	pConfObj->Write(_T("dwellToleranceArea"), (double) GetDwellToleranceArea());
	pConfObj->Write(_T("actionTop"), (long) GetActionTop());
	pConfObj->Write(_T("actionLeft"), (long) GetActionLeft());
	pConfObj->Write(_T("actionRight"), (long) GetActionRight());
	pConfObj->Write(_T("actionBottom"), (long) GetActionBottom());


	pConfObj->SetPath (_T(".."));
}

void CMouseOutput::ReadProfileData(wxConfigBase* pConfObj)
{
	long val;
	double dwellToleranceArea;

	pConfObj->SetPath (_T("mouseOutput"));

	if (pConfObj->Read(_T("xSpeed"), &val))	SetXSpeed(val);
	if (pConfObj->Read(_T("ySpeed"), &val))	SetYSpeed(val);
	if (pConfObj->Read(_T("acceleration"), &val)) SetAcceleration(val);
	if (pConfObj->Read(_T("smoothness"), &val)) SetSmoothness(val);
	if (pConfObj->Read(_T("easyStop"), &val)) SetEasyStopValue(val);
        if (pConfObj->Read(_T("enabledWorkspace"), &val)) SetRestrictedWorkingArea(val);
        if (pConfObj->Read(_T("topWorkspace"), &val)) SetTopWorkspace(val);
        if (pConfObj->Read(_T("leftWorkspace"), &val)) SetLeftWorkspace(val);
        if (pConfObj->Read(_T("rightWorkspace"), &val)) SetRightWorkspace(val);
        if (pConfObj->Read(_T("bottomWorkspace"), &val)) SetBottomWorkspace(val);
	if (pConfObj->Read(_T("clickMode"), &val)) SetClickMode((CMouseOutput::EClickMode) val);
	pConfObj->Read(_T("beepOnClick"), &m_beepOnClick);	
	pConfObj->Read(_T("consecutiveClicksAllowed"), &m_consecutiveClicksAllowed);
	if (pConfObj->Read(_T("dwellTime"), &val)) SetDwellTime(val);	
	if (pConfObj->Read(_T("preGestureTime"), &val)) SetPreGestureTime(val);	
	if (pConfObj->Read(_T("gestureTime"), &val)) SetGestureTime(val);	
	if (pConfObj->Read(_T("dwellToleranceArea"), &dwellToleranceArea))
		SetDwellToleranceArea((long unsigned int) dwellToleranceArea);
		//SetDwellToleranceArea((float) dwellToleranceArea);
	if (pConfObj->Read(_T("actionTop"), &val)) SetActionTop((CMouseOutput::EAction) val);
	if (pConfObj->Read(_T("actionLeft"), &val)) SetActionLeft((CMouseOutput::EAction) val);
	if (pConfObj->Read(_T("actionRight"), &val)) SetActionRight((CMouseOutput::EAction) val);
	if (pConfObj->Read(_T("actionBottom"), &val)) SetActionBottom((CMouseOutput::EAction) val);

	pConfObj->SetPath (_T(".."));
}