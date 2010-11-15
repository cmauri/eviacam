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
#include <wx/msgdlg.h>
#if defined(__WXGTK__)
#include <X11/extensions/XTest.h>
#endif

#define DISABLE 0
#define SINGLE 1
#define SECONDARY 2
#define DOUBLE 3
#define DRAG 4

#define MOUSE_EVENTS_COUNT 5

//#define ENABLE_EXPERIMENTAL_KEY_ACTIONS 1

CMouseOutput::CMouseOutput(CClickWindowController& pClickWindowController) 
#if defined(__WXGTK__)
:	CMouseControl ((void *) wxGetDisplay())
#endif
{
	// General attributes
	m_enabled= false;
	m_pClickSound= new wxSound (wxStandardPaths::Get().GetDataDir() + _T("/click.wav"));
	
	// DWell click
	m_pClickWindowController= &pClickWindowController;
	
	// Gesture click
	m_state = DWELL_TIME;	
	m_isLeftPressed = false;
	m_xIniGesture = 0;
	m_yIniGesture = 0;	
	
	InitKeyboardCodes();
	
	InitDefaults ();
}

// Configuration methods
void CMouseOutput::InitDefaults()
{
	// General attributes
	SetXSpeed (12);
	SetYSpeed (10);
	SetAcceleration (2);
	SetClickMode (CMouseOutput::DWELL);
	SetBeepOnClick (true);
	SetConsecutiveClicksAllowed(false);
	SetVisualAlerts (true);
	SetDwellToleranceArea (3);
	SetSmoothness (5);
	SetEasyStopValue (1);	
	
	// Dwell click specific
	SetDwellTime (10);
	
	// Workspace limits
	SetRestrictedWorkingArea (false);
	SetTopWorkspace(1);
	SetLeftWorkspace(1);
	SetRightWorkspace(1);
	SetBottomWorkspace(1);
	
	// Gesture click
	SetActionLeft(SINGLE);
	SetActionRight(SECONDARY);
	SetActionTop(DOUBLE);
	SetActionBottom(DRAG);
	SetFastGestureAction(false);
}

void CMouseOutput::InitKeyboardCodes()
{
#if defined(ENABLE_EXPERIMENTAL_KEY_ACTIONS)
	// TODO: add useful entries here
	m_keyboardCodes.push_back(CKeyboardCode('a'));
	m_keyboardCodes.push_back(CKeyboardCode('b'));
	m_keyboardCodes.push_back(CKeyboardCode('c'));
	m_keyboardCodes.push_back(CKeyboardCode('d'));
	m_keyboardCodes.push_back(CKeyboardCode('e'));
	m_keyboardCodes.push_back(CKeyboardCode('f'));
	m_keyboardCodes.push_back(CKeyboardCode('g'));
	m_keyboardCodes.push_back(CKeyboardCode('h'));
	m_keyboardCodes.push_back(CKeyboardCode('i'));
	m_keyboardCodes.push_back(CKeyboardCode('j'));
	m_keyboardCodes.push_back(CKeyboardCode('K'));
	m_keyboardCodes.push_back(CKeyboardCode('L'));
	m_keyboardCodes.push_back(CKeyboardCode('M'));
	m_keyboardCodes.push_back(CKeyboardCode('N'));
	m_keyboardCodes.push_back(CKeyboardCode('O'));
	m_keyboardCodes.push_back(CKeyboardCode('P'));
	m_keyboardCodes.push_back(CKeyboardCode('Q'));
	m_keyboardCodes.push_back(CKeyboardCode('R'));
	m_keyboardCodes.push_back(CKeyboardCode('S'));
	m_keyboardCodes.push_back(CKeyboardCode('T'));
	m_keyboardCodes.push_back(CKeyboardCode('U'));
	m_keyboardCodes.push_back(CKeyboardCode('V'));
	m_keyboardCodes.push_back(CKeyboardCode('W'));
	m_keyboardCodes.push_back(CKeyboardCode('X'));
	m_keyboardCodes.push_back(CKeyboardCode('Y'));
	m_keyboardCodes.push_back(CKeyboardCode('Z'));
#endif
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

	extern wxWindow* globalWindow;

float CMouseOutput::ProcessRelativePointerMove(float dx, float dy) 
{
	float despl;
	long x, y;

	if (!m_enabled) return 0.0f;

	// Do move.
	despl= MovePointerRel (dx, dy);
	GetPointerLocation (x, y);

	if (m_clickMode== CMouseOutput::DWELL) {
		// DWell click

		// TODO: check relative from dwell start position
		if (despl> m_dwellToleranceArea) {
			// Pointer moving
			if (m_visualAlerts) m_progressVisualAlert.End();
			m_dwellCountdown.Reset();
		}
		else {
			// Pointer static
			if (!m_dwellCountdown.HasExpired())
				if (m_visualAlerts)
					m_progressVisualAlert.Update(x, y, m_dwellCountdown.PercentagePassed());

			CClickWindowController::EAction action;

			action= m_pClickWindowController->GetAction (x, y);

			if (action== CClickWindowController::ACT_NO_CLICK)
			{
				if (m_visualAlerts)
					m_progressVisualAlert.End();
				
				m_dwellCountdown.Reset();
			}
			else
			{
				if (m_dwellCountdown.OneShootAction())
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
						m_progressVisualAlert.End();
					
					if (m_consecutiveClicksAllowed)
						m_dwellCountdown.Reset();

					if (m_beepOnClick) m_pClickSound->Play (wxSOUND_ASYNC);
				}
			}
		}
	} 
	else if (m_clickMode== CMouseOutput::GESTURE) {
		// Gesture click
		switch (m_state) {
		case DWELL_TIME:
			if (despl> m_dwellToleranceArea) {
				// Pointer moving
				if (m_visualAlerts)
					m_progressVisualAlert.End();
				m_dwellCountdown.Reset();
			}
			else {
				// Pointer stopped
				if (m_dwellCountdown.HasExpired()) {
					if (m_visualAlerts)
						m_progressVisualAlert.End();

					m_dwellCountdown.Reset();
					m_xIniGesture = x;
					m_yIniGesture = y;
					m_state = COMPUTE_DIRECTION;
				}
				else if (m_visualAlerts)
					m_progressVisualAlert.Update(x, y, m_dwellCountdown.PercentagePassed());
			}
			break;
		case COMPUTE_DIRECTION:
			if (despl> m_dwellToleranceArea) {
				// Pointer moving
				if (m_visualAlerts) {
					m_progressVisualAlert.End();
					m_gestureVisualAlert.Update(x, y);
				}
				if (!m_fastGestureAction) m_dwellCountdown.Reset();
			}
			else {
				// Pointer static
				if (m_dwellCountdown.HasExpired()) {
					// Countdown expired
					
					// Remove visual alerts
					if (m_visualAlerts) {
						m_gestureVisualAlert.End();
						m_progressVisualAlert.End();
					}
					
					// Compute motion from iniGesture point
					int dxPixels= x - m_xIniGesture;
					int dyPixels= y - m_yIniGesture;
					
					if (sqrt((float)(dxPixels*dxPixels + dyPixels*dyPixels))> m_dwellToleranceArea) {
						// Is far enough from iniGeture, do action
						if (abs(dxPixels)> abs(dyPixels)) {
							if (dxPixels < 0) DoActionGesture(LEFT);
							else DoActionGesture(RIGHT);
						} 
						else {
							if (dyPixels < 0) DoActionGesture(TOP);
							else DoActionGesture(BOTTOM);
						}
						
						if (m_beepOnClick) m_pClickSound->Play (wxSOUND_ASYNC);
					}
					
					// Next state. Ignores consecutive clicks allowed because for
					// gesture click mode it is difficult to control
					//if (m_consecutiveClicksAllowed) m_state = DWELL_TIME;
					//else m_state = WAIT_DWELL;
					m_state = WAIT_DWELL;
				}
				else {
					if (m_visualAlerts) {
						m_progressVisualAlert.Update(x, y, m_dwellCountdown.PercentagePassed());
						m_gestureVisualAlert.Update(x, y);
					}
				}
			}
			break;
		case WAIT_DWELL:
			// Wait to restart geture action until pointer is moved
			if (despl> m_dwellToleranceArea) {
				// Pointer moving. Next state
				m_dwellCountdown.Reset();
				m_state= DWELL_TIME;
			}
			break;
		default: assert (false); break;
		}
	}
	return despl;
}

void CMouseOutput::DoActionGesture (EDirection direction)
{
	int action= m_actionLeft;	// Avoid warning
	
	// Set pointer to the point where gesture started
	DoMovePointerAbs(m_xIniGesture, m_yIniGesture);

	switch (direction)
	{
		case LEFT:	action = m_actionLeft;	break;
		case RIGHT:	action = m_actionRight;	break;
		case TOP:	action = m_actionTop;	break;
		case BOTTOM:	action = m_actionBottom; break;
		default: assert (false);
	}
	
	if (action != DRAG && m_isLeftPressed)
	{
		m_isLeftPressed = false;
		LeftUp();
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
		default:
			m_keyboardCodes[action - MOUSE_EVENTS_COUNT].SendKey();
			break;
		
	}
}

void CMouseOutput::EndVisualAlerts()
{
	if (m_visualAlerts) {
		m_progressVisualAlert.End();
		m_gestureVisualAlert.End();		
	}
	m_xIniGesture = 0;
	m_yIniGesture = 0;
	m_state = DWELL_TIME;
}

// Return true is the change has been applied, or false otherwise
bool CMouseOutput::SetClickMode(CMouseOutput::EClickMode mode, bool silent, wxWindow* parent)
{
	if (mode!= m_clickMode)
	{
		if (!silent) {
			if (mode== CMouseOutput::NONE) {
				wxMessageDialog dlg (parent, _("This action will disable eViacam click generation.\nAre you sure?"), _T("Enable Viacam"), wxICON_EXCLAMATION | wxYES_NO );
				if (dlg.ShowModal()!= wxID_YES) return false;
			}
			else {
				wxMessageDialog dlg (parent, _("This action will change the click generation method.\nAre you sure?"), _T("Enable Viacam"), wxICON_EXCLAMATION | wxYES_NO );
				if (dlg.ShowModal()!= wxID_YES) return false;
			}
		}
		
		if (mode== CMouseOutput::DWELL) 
		{
			// Enable dwell click
			m_dwellCountdown.Reset();
			// TODO: clickwindowcontroller should store an attribute (useClickWindow) instead of relaying on
			// IsShown and ClickWindowAtStartup
			m_pClickWindowController->Show(true);
			m_clickMode= mode;
		}
		else if (mode== CMouseOutput::GESTURE)
		{
			// Enable gesture click
			m_dwellCountdown.Reset();
			 m_pClickWindowController->Show(false);
			m_clickMode= mode;
		}		
		else if (mode== CMouseOutput::NONE)
		{
			// Disable dwell and gesture click
			 m_pClickWindowController->Show(false);
			m_clickMode= mode;
		}
	}
	return true;
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
				m_dwellCountdown.Reset ();
		}
		m_enabled= value;
	}
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
	//pConfObj->Write(_T("gestureTime"), (long) GetGestureTime());
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
	bool valb;
	double dwellToleranceArea;

	pConfObj->SetPath (_T("mouseOutput"));

	if (pConfObj->Read(_T("xSpeed"), &val))	SetXSpeed(val);
	if (pConfObj->Read(_T("ySpeed"), &val))	SetYSpeed(val);
	if (pConfObj->Read(_T("acceleration"), &val)) SetAcceleration(val);
	if (pConfObj->Read(_T("smoothness"), &val)) SetSmoothness(val);
	if (pConfObj->Read(_T("easyStop"), &valb)) SetEasyStopValue(valb);
	if (pConfObj->Read(_T("enabledWorkspace"), &valb)) SetRestrictedWorkingArea(valb);
	if (pConfObj->Read(_T("topWorkspace"), &val)) SetTopWorkspace(val);
	if (pConfObj->Read(_T("leftWorkspace"), &val)) SetLeftWorkspace(val);
	if (pConfObj->Read(_T("rightWorkspace"), &val)) SetRightWorkspace(val);
	if (pConfObj->Read(_T("bottomWorkspace"), &val)) SetBottomWorkspace(val);
	if (pConfObj->Read(_T("clickMode"), &val)) SetClickMode((CMouseOutput::EClickMode) val);
	pConfObj->Read(_T("beepOnClick"), &m_beepOnClick);	
	pConfObj->Read(_T("consecutiveClicksAllowed"), &m_consecutiveClicksAllowed);
	if (pConfObj->Read(_T("dwellTime"), &val)) SetDwellTime(val);	
	//if (pConfObj->Read(_T("gestureTime"), &val)) SetGestureTime(val);	
	if (pConfObj->Read(_T("dwellToleranceArea"), &dwellToleranceArea))
		SetDwellToleranceArea((unsigned long) dwellToleranceArea);
		//SetDwellToleranceArea((float) dwellToleranceArea);
	if (pConfObj->Read(_T("actionTop"), &val)) SetActionTop(val);
	if (pConfObj->Read(_T("actionLeft"), &val)) SetActionLeft(val);
	if (pConfObj->Read(_T("actionRight"), &val)) SetActionRight(val);
	if (pConfObj->Read(_T("actionBottom"), &val)) SetActionBottom(val);
	if (pConfObj->Read(_T("visualAlerts"), &valb)) SetVisualAlerts(valb);

	pConfObj->SetPath (_T(".."));
}