/////////////////////////////////////////////////////////////////////////////
// Name:        gestureclick.cpp
// Purpose:  
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by:
// Created:
// Copyright:   (C) 2008-12 Cesar Mauri Loba - CREA Software Systems
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
#include "gestureclick.h"
#include "mousecontrol.h"
#include <math.h>

#undef ENABLE_EXPERIMENTAL_KEY_ACTIONS

CGestureClick::CGestureClick(CMouseControl& mc)
: m_enabled(false)
, m_isLeftPressed(false)
, m_xIniGesture(0)
, m_yIniGesture(0)
, m_state(DWELL_TIME)
, m_mouseControl(&mc)
, m_visualAlertsEnabled(false)
, m_dwellToleranceArea(0)
, m_actionLeft(DISABLE)
, m_actionRight(DISABLE)
, m_actionTop(DISABLE)
, m_actionBottom(DISABLE)
, m_fastGestureAction(false)
{
	InitKeyboardCodes();
	
	InitDefaults ();
}

CGestureClick::~CGestureClick ()
{
	SetEnabled(false);
}


// Configuration methods
void CGestureClick::InitDefaults()
{
	// General attributes
	//SetConsecutiveClicksAllowed(false);
	EnableVisualAlerts(true);
	SetDwellTime (10);
	SetDwellToleranceArea (3);
		
	// Gesture click
	SetActionLeft(SINGLE);
	SetActionRight(SECONDARY);
	SetActionTop(DOUBLE);
	SetActionBottom(DRAG);
	SetFastGestureAction(false);
}


void CGestureClick::WriteProfileData(wxConfigBase* pConfObj)
{
	pConfObj->SetPath (_T("gestureClick"));	

	pConfObj->Write(_T("dwellTime"), (int) GetDwellTime());
	pConfObj->Write(_T("dwellToleranceArea"), (int) GetDwellToleranceArea());
	pConfObj->Write(_T("actionTop"), (int) GetActionTop());
	pConfObj->Write(_T("actionLeft"), (int) GetActionLeft());
	pConfObj->Write(_T("actionRight"), (int) GetActionRight());
	pConfObj->Write(_T("actionBottom"), (int) GetActionBottom());
	pConfObj->Write(_T("visualAlertsEnabled"), (bool) AreVisualAlertsEnabled());
	pConfObj->Write(_T("fastAction"), (bool) GetFastGestureAction());

	pConfObj->SetPath (_T(".."));
}

void CGestureClick::ReadProfileData(wxConfigBase* pConfObj)
{
	int vall;
	bool valb;
	
	pConfObj->SetPath (_T("gestureClick"));

	if (pConfObj->Read(_T("dwellTime"), &vall)) SetDwellTime(vall);	
	if (pConfObj->Read(_T("dwellToleranceArea"), &vall)) 
		SetDwellToleranceArea((unsigned int) vall);		
	if (pConfObj->Read(_T("actionTop"), &vall)) SetActionTop((EAction) vall);
	if (pConfObj->Read(_T("actionLeft"), &vall)) SetActionLeft((EAction) vall);
	if (pConfObj->Read(_T("actionRight"), &vall)) SetActionRight((EAction) vall);
	if (pConfObj->Read(_T("actionBottom"), &vall)) SetActionBottom((EAction) vall);
	if (pConfObj->Read(_T("visualAlertsEnabled"), &valb)) EnableVisualAlerts(valb);
	if (pConfObj->Read(_T("fastAction"), &valb)) SetFastGestureAction(valb);

	pConfObj->SetPath (_T(".."));
}

void CGestureClick::InitKeyboardCodes()
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

mousecmd::mousecmd CGestureClick::ProcessMotion (int dxPix, int dyPix, unsigned int xCurr, unsigned int yCurr)
{
	if (!m_enabled) return mousecmd::CMD_NO_CLICK;

	mousecmd::mousecmd retval= mousecmd::CMD_NO_CLICK;
	
	// Do move.
	float despl= sqrtf ((float) (dxPix*dxPix + dyPix*dyPix));
	
	// Gesture click
	switch (m_state) {
	case DWELL_TIME:
		// This state waits until the dwell time has expired
		if (despl> m_dwellToleranceArea) {
			// Pointer moving. Reset countdown and remove
			// visual alerts
			if (m_visualAlertsEnabled)	m_progressVisualAlert.End();
			m_dwellCountdown.Reset();
		}
		else {
			// Pointer stopped
			if (m_dwellCountdown.HasExpired()) {
				if (m_visualAlertsEnabled)	m_progressVisualAlert.End();

				m_dwellCountdown.Reset();
				m_xIniGesture = xCurr;
				m_yIniGesture = yCurr;
				m_state = COMPUTE_DIRECTION;
			}
			else if (m_visualAlertsEnabled)
				m_progressVisualAlert.Update
					(xCurr, yCurr, m_dwellCountdown.PercentagePassed());
		}
		break;
	case COMPUTE_DIRECTION:
		if (despl> m_dwellToleranceArea) {
			// Pointer moving
			if (m_visualAlertsEnabled) {
				m_progressVisualAlert.End();
				m_gestureVisualAlert.Update(xCurr, yCurr);
			}
			if (!m_fastGestureAction) m_dwellCountdown.Reset();
		}
		else {
			// Pointer static
			if (m_dwellCountdown.HasExpired()) {
				// Countdown expired
				
				// Remove visual alerts
				if (m_visualAlertsEnabled) {
					m_gestureVisualAlert.End();
					m_progressVisualAlert.End();
				}
				
				// Compute motion from iniGesture point
				int dxPixels= xCurr - m_xIniGesture;
				int dyPixels= yCurr - m_yIniGesture;
				
				if (sqrtf((float)(dxPixels*dxPixels + dyPixels*dyPixels))> m_dwellToleranceArea) {
					// Is far enough from iniGeture, do action
					if (abs(dxPixels)> abs(dyPixels)) {
						if (dxPixels < 0) retval= DoAction(m_actionLeft);
						else retval= DoAction(m_actionRight);
					} 
					else {
						if (dyPixels < 0) retval= DoAction(m_actionTop);
						else retval= DoAction(m_actionBottom);
					}
				}
				
				// Next state. Ignores consecutive clicks allowed because for
				// gesture click mode it is difficult to control
				//if (m_consecutiveClicksAllowed) m_state = DWELL_TIME;
				//else m_state = WAIT_DWELL;
				m_state = WAIT_DWELL;
			}
			else {
				if (m_visualAlertsEnabled) {
					m_progressVisualAlert.Update(xCurr, yCurr, m_dwellCountdown.PercentagePassed());
					m_gestureVisualAlert.Update(xCurr, yCurr);
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
	default: 
		assert (false); 
		break;
	}

	return retval;
}

mousecmd::mousecmd CGestureClick::DoAction (EAction action)
{
	mousecmd::mousecmd retval= mousecmd::CMD_NO_CLICK;

	// Return pointer to the position where gesture started
	m_mouseControl->DoMovePointerAbs(m_xIniGesture, m_yIniGesture);

	if (m_isLeftPressed) {
		// Previous action started a drag & drop operation,
		// the new action finishes it 
		m_isLeftPressed = false;
		m_mouseControl->LeftUp();
		retval= mousecmd::CMD_LEFT_UP;

		if (action== DRAG) return retval;
	}

	switch (action)	{
		case DISABLE:
			break;
		case SINGLE:
			m_mouseControl->LeftClick();
			retval= mousecmd::CMD_LEFT_CLICK;
			break;
		case DOUBLE:
			m_mouseControl->LeftDblClick();
			retval= mousecmd::CMD_DOUBLE_CLICK;
			break;
		case SECONDARY:
			m_mouseControl->RightClick();
			retval= mousecmd::CMD_RIGHT_CLICK;
			break;
		case THIRD:
			m_mouseControl->MiddleClick();
			retval= mousecmd::CMD_MIDDLE_CLICK;
			break;
		case DRAG:
			m_mouseControl->LeftDown();
			retval= mousecmd::CMD_LEFT_DOWN;
			m_isLeftPressed = true;
			break;
		default:
			// Keyboard event
			m_keyboardCodes[action - MOUSE_EVENTS_COUNT].SendKey();
			break;
	}

	return retval;
}

void CGestureClick::Reset()
{
	m_state = DWELL_TIME;
	m_dwellCountdown.Reset();
	if (m_visualAlertsEnabled) {
		m_progressVisualAlert.End();
		m_gestureVisualAlert.End();		
	}

	if (m_isLeftPressed) {
		m_mouseControl->LeftUp();
		m_isLeftPressed = false;
	}
	// Unnecessary
	//m_xIniGesture = 0;
	//m_yIniGesture = 0;
}

void CGestureClick::SetEnabled(bool value)
{
	// TODO: sync
	if (value!= m_enabled) {
		if (value) {
			// Enable
			Reset();
			m_enabled= true;
		}
		else {
			// Disable
			m_enabled= false;
			Reset();
		}
	}
}

void CGestureClick::EnableVisualAlerts(bool value)
{
	// TODO: sync
	if (value!= m_visualAlertsEnabled) {
		if (value) {
			m_progressVisualAlert.End();
			m_gestureVisualAlert.End();	
			m_visualAlertsEnabled= true;
		}
		else {
			m_visualAlertsEnabled= false;
			m_progressVisualAlert.End();
			m_gestureVisualAlert.End();	
		}
	}
}
