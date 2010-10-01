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
#include <wx/cursor.h>
#include <wx/window.h>
#if defined(__WXGTK__)
#include <X11/extensions/XTest.h>
#endif

CMouseOutput::CMouseOutput(CClickWindowController& pClickWindowController)
#if defined(__WXGTK__)
	: CMouseControl ((void *) wxGetDisplay())
#endif
{
	long x, y;
	m_pClickWindowController= &pClickWindowController;

	m_enabled= false;
        m_restrictedWorkingArea = false;
	m_isLeftPressed = false;
	m_visualAlerts = false;
	m_dwellCountdown = new CWaitTime();
	m_gestureCountdown = new CWaitTime();

	GetPointerLocation(x, y);
	InitDefaults ();

	m_dwellVisualAlert.Start(CVisualAlert::DWELL);
	m_gestureVisualAlert.Start(CVisualAlert::GESTURE);
	
	m_pClickSound= new wxSound (wxStandardPaths::Get().GetDataDir() + _T("/click.wav"));
}

CMouseOutput::~CMouseOutput ()
{
	delete m_pClickSound;
	delete m_dwellCountdown;
	delete m_gestureCountdown;
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

	extern wxWindow* globalWindow;

float CMouseOutput::ProcessRelativePointerMove(float dx, float dy) 
{
	float despl;
	long x, y;
	EAction action;

	if (!m_enabled) return 0.0f;

	// Do move.
	despl= MovePointerRel (dx, dy);
	GetPointerLocation (x, y);

	if (m_clickMode== CMouseOutput::DWELL)
	{
		// DWell click
		
		// TODO: check relative from dwell start position
		if (despl> m_dwellToleranceArea) 
		{
			// Pointer moving
			if (m_visualAlerts)
				m_dwellVisualAlert.End(m_xIni, m_yIni);
			
			m_dwellCountdown->Reset();
		}
		else
		{
			if (!m_dwellCountdown->IsExpired())
				if (m_visualAlerts)
					m_dwellVisualAlert.Update(m_xIni, m_yIni, x, y, m_dwellCountdown->PercentagePassed());

			// Pointer static
			CClickWindowController::EAction action;

			action= m_pClickWindowController->GetAction (x, y);

			if (action== CClickWindowController::ACT_NO_CLICK)
			{
				if (m_visualAlerts)
					m_dwellVisualAlert.End(m_xIni, m_yIni);
				
				m_dwellCountdown->Reset();
			}
			else
			{
				if (m_dwellCountdown->Update())
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
					
					if (m_visualAlerts)
						m_dwellVisualAlert.End(m_xIni, m_yIni);
					
					if (m_consecutiveClicksAllowed)
						m_dwellCountdown->Reset();

					if (m_beepOnClick) m_pClickSound->Play (wxSOUND_ASYNC);

				}
			}
		}
	} else if (m_clickMode== CMouseOutput::GESTURE)
	{
		// Gesture click

		switch (m_state)
		{
			case DWELL_TIME:
				if (despl> m_dwellToleranceArea) 
				{
					// Pointer moving
					if (m_visualAlerts)
						m_dwellVisualAlert.End(m_xIni, m_yIni);
					
					m_dwellCountdown->Reset();
				}
				else
				{
					if (!m_dwellCountdown->IsExpired())
						if (m_visualAlerts)
							m_dwellVisualAlert.Update(m_xIni, m_yIni, x, y, m_dwellCountdown->PercentagePassed());

					if (m_dwellCountdown->Update())
					{
						m_state = COMPUTE_DIRECTION;
						
						if (m_visualAlerts)
							m_dwellVisualAlert.End(m_xIni, m_yIni);
						
						m_gestureCountdown->Reset();
						m_xIni = x;
						m_yIni = y;
					}
				}
				break;

			case COMPUTE_DIRECTION:
				if (!m_gestureCountdown->IsExpired())
				{
					if (m_visualAlerts)
						m_gestureVisualAlert.Update(m_xIni, m_yIni, x, y, m_gestureCountdown->PercentagePassed());
					
					m_sumDx += dx;
					m_sumDy += dy;
					
					if (fabs(m_sumDx) > m_dwellToleranceArea or fabs(m_sumDy) > m_dwellToleranceArea)
					{
						// Compute direction
						if (fabs(dx) > fabs(dy))
						{
							if (dx < 0) m_direction = LEFT;
							else m_direction = RIGHT;
						}
						else
						{
							if (dy < 0) m_direction = TOP;
							else m_direction = BOTTOM;
						}
						m_state = WAITING_RETURN;
					}
				}
				else
				{
					m_state = DWELL_TIME;
				}
				break;

			case WAITING_RETURN:
				if (!m_gestureCountdown->IsExpired())
				{
					if (m_visualAlerts)
						m_gestureVisualAlert.Update(m_xIni, m_yIni, x, y, m_gestureCountdown->PercentagePassed());
					
					m_sumDx += dx;
					m_sumDy += dy;
					
					if (((m_direction == LEFT or m_direction == RIGHT) and fabs(m_sumDx) < m_dwellToleranceArea) or ((m_direction == TOP or m_direction == BOTTOM) and fabs(m_sumDy) < m_dwellToleranceArea))
					{
						DoAction();
						m_sumDx = 0;
						m_sumDy = 0;
						
						if (m_visualAlerts)
							m_gestureVisualAlert.End(m_xIni, m_yIni);
						
						m_state = DWELL_TIME;
					}
				}
				else
				{
					DoAction();
					m_sumDx = 0;
					m_sumDy = 0;
					
					if (m_visualAlerts)
						m_gestureVisualAlert.End(m_xIni, m_yIni);
					
					m_state = DWELL_TIME;
				}
				break;
		}
	}
	return despl;
}

void CMouseOutput::WriteChar (char* c)
{
#if defined(__WXGTK__)
	KeyCode kc = XKeysymToKeycode((Display *) wxGetDisplay(), XStringToKeysym(c));
	XTestFakeKeyEvent((Display *) wxGetDisplay(), kc, true, 0);
	XTestFakeKeyEvent((Display *) wxGetDisplay(), kc, false, 0);
#endif
}

void CMouseOutput::DoAction()
{
	EAction action;
	DoMovePointerAbs(m_xIni, m_yIni);

	if (action != DRAG and m_isLeftPressed)
	{
		m_isLeftPressed = false;
		LeftUp();
	}

	switch (m_direction)
	{
		case LEFT:
			action = m_actionLeft;
			break;
		case RIGHT:
			action = m_actionRight;
			break;
		case TOP:
			action = m_actionTop;
			break;
		case BOTTOM:
			action = m_actionBottom;
			break;
	}

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
			}
			else
			{
				LeftDown();
				m_isLeftPressed = true;
			}
			break;
		case A: WriteChar("A"); break;
		case B: WriteChar("B"); break;
		case C: WriteChar("C"); break;
		case D: WriteChar("D"); break;
		case E: WriteChar("E"); break;
		case F: WriteChar("F"); break;
		case G: WriteChar("G"); break;
		case H: WriteChar("H"); break;
		case I: WriteChar("I"); break;
		case J: WriteChar("J"); break;
		case K: WriteChar("K"); break;
		case L: WriteChar("L"); break;
		case M: WriteChar("M"); break;
		case N: WriteChar("N"); break;
		case O: WriteChar("O"); break;
		case P: WriteChar("P"); break;
		case Q: WriteChar("Q"); break;
		case R: WriteChar("R"); break;
		case S: WriteChar("S"); break;
		case T: WriteChar("T"); break;
		case U: WriteChar("U"); break;
		case V: WriteChar("V"); break;
		case W: WriteChar("W"); break;
		case X: WriteChar("X"); break;
		case Y: WriteChar("Y"); break;
		case Z: WriteChar("Z"); break;
	}
}

void CMouseOutput::EndVisualAlerts()
{
	if (m_visualAlerts)
	{
		m_dwellVisualAlert.End(m_xIni, m_yIni);
		m_gestureVisualAlert.End(m_xIni, m_yIni);
		m_dwellVisualAlert.End(m_xIni, m_yIni);
	}
	m_xIni = 0;
	m_yIni = 0;
	m_state = DWELL_TIME;
	m_sumDx = 0;
	m_sumDy = 0;
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
			m_dwellCountdown->Reset();
			m_clickMode= mode;
		}
		else if (mode== CMouseOutput::GESTURE)
		{
			// Enable gesture click
			m_dwellCountdown->Reset();
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
				m_dwellCountdown->Reset ();
			
			if (m_clickMode == GESTURE)
				m_dwellCountdown->Reset ();
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
	SetDwellTime (3);
	SetGestureTime (3);
	SetDwellToleranceArea (3); //.0f);	
	m_xIni = 0;
	m_yIni = 0;
	m_state = DWELL_TIME;
	m_sumDx = 0;
	m_sumDy = 0;
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
	pConfObj->Write(_T("gestureTime"), (long) GetGestureTime());
	pConfObj->Write(_T("dwellToleranceArea"), (double) GetDwellToleranceArea());
	pConfObj->Write(_T("actionTop"), (long) GetActionTop());
	pConfObj->Write(_T("actionLeft"), (long) GetActionLeft());
	pConfObj->Write(_T("actionRight"), (long) GetActionRight());
	pConfObj->Write(_T("actionBottom"), (long) GetActionBottom());
	pConfObj->Write(_T("visualAlerts"), (bool) GetVisualAlerts());


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
	if (pConfObj->Read(_T("gestureTime"), &val)) SetGestureTime(val);	
	if (pConfObj->Read(_T("dwellToleranceArea"), &dwellToleranceArea))
		SetDwellToleranceArea((long unsigned int) dwellToleranceArea);
		//SetDwellToleranceArea((float) dwellToleranceArea);
	if (pConfObj->Read(_T("actionTop"), &val)) SetActionTop((CMouseOutput::EAction) val);
	if (pConfObj->Read(_T("actionLeft"), &val)) SetActionLeft((CMouseOutput::EAction) val);
	if (pConfObj->Read(_T("actionRight"), &val)) SetActionRight((CMouseOutput::EAction) val);
	if (pConfObj->Read(_T("actionBottom"), &val)) SetActionBottom((CMouseOutput::EAction) val);
	if (pConfObj->Read(_T("visualAlerts"), &val)) SetVisualAlerts(val);

	pConfObj->SetPath (_T(".."));
}