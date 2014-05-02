/////////////////////////////////////////////////////////////////////////////
// Name:        dwellclick.cpp
// Purpose:  
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by:
// Created:
// Copyright:   (C) 2010-12 Cesar Mauri Loba - CREA Software Systems
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
#include "dwellclick.h"
#include "clickwindowcontroller.h"
#include "eviacamapp.h"
#include "mousecontrol.h"
#include <math.h>

CDwellClick::CDwellClick(CMouseControl& mc)
: m_enabled(false)
, m_consecutiveClicksAllowed(false)
, m_visualAlertsEnabled(false)
, m_useClickWindow(false)
, m_dwellToleranceArea(0)
, m_mouseControl(&mc)
{
	m_pClickWindowController= new CClickWindowController(wxGetApp().GetController());
	InitDefaults ();
}

CDwellClick::~CDwellClick ()
{
	SetEnabled(false);
	delete m_pClickWindowController;
	m_pClickWindowController= NULL;
}

// Configuration methods
void CDwellClick::InitDefaults()
{
	SetConsecutiveClicksAllowed (false);
	EnableVisualAlerts (true);
	SetUseClickWindow (true);
	SetDwellToleranceArea (3);	
	SetDwellTime (10);	
}

void CDwellClick::WriteProfileData(wxConfigBase* pConfObj)
{
	pConfObj->SetPath (_T("dwellclick"));

	pConfObj->Write(_T("consecutiveClicksAllowed"), (bool) GetConsecutiveClicksAllowed());
	pConfObj->Write(_T("visualAlertsEnabled"), (bool) AreVisualAlertsEnabled());
	pConfObj->Write(_T("useClickWindow"), (bool) GetUseClickWindow());
	pConfObj->Write(_T("dwellToleranceArea"), (double) GetDwellToleranceArea());	
	pConfObj->Write(_T("dwellTime"), (int) GetDwellTime());

	m_pClickWindowController->WriteProfileData (pConfObj);
	pConfObj->SetPath (_T(".."));
}

void CDwellClick::ReadProfileData(wxConfigBase* pConfObj)
{
	int val;
	bool valb;
	double vald;

	pConfObj->SetPath (_T("dwellclick"));

	pConfObj->Read(_T("consecutiveClicksAllowed"), &m_consecutiveClicksAllowed);
	if (pConfObj->Read(_T("visualAlertsEnabled"), &valb)) EnableVisualAlerts(valb);
	if (pConfObj->Read(_T("useClickWindow"), &valb)) SetUseClickWindow(valb);
	if (pConfObj->Read(_T("dwellToleranceArea"), &vald)) SetDwellToleranceArea((unsigned int) vald);
	if (pConfObj->Read(_T("dwellTime"), &val)) SetDwellTime(val);	

	m_pClickWindowController->ReadProfileData (pConfObj);

	pConfObj->SetPath (_T(".."));
}

mousecmd::mousecmd CDwellClick::ProcessMotion (int dxPix, int dyPix, unsigned int xCurr, unsigned int yCurr)
{
	if (!m_enabled) return mousecmd::CMD_NO_CLICK;;

	//Check autohide window
	//m_pClickWindowController->AutohideClickWindow(xCurr, yCurr);
	
	// Compute cursor displacement
	float despl= sqrtf ((float) (dxPix*dxPix + dyPix*dyPix));
	
	// TODO: check relative from dwell start position
	if (despl> m_dwellToleranceArea) {
		// Pointer is moving
		if (m_visualAlertsEnabled) m_progressVisualAlert.End();
		m_dwellCountdown.Reset();
	}
	else {
		// Pointer is static
		if (!m_dwellCountdown.HasExpired())
			if (m_visualAlertsEnabled)
				m_progressVisualAlert.Update(xCurr, yCurr, m_dwellCountdown.PercentagePassed());

		mousecmd::mousecmd action= m_pClickWindowController->GetAction (xCurr, yCurr);

		if (action== mousecmd::CMD_NO_CLICK) {
			if (m_visualAlertsEnabled)
				m_progressVisualAlert.End();
			
			m_dwellCountdown.Reset();
		}
		else {
			if (m_dwellCountdown.OneShootAction()) {
				// Send action
				switch (action)
				{
				case mousecmd::CMD_LEFT_CLICK:
					m_mouseControl->LeftClick();
					break;
				case mousecmd::CMD_MIDDLE_CLICK:
					m_mouseControl->MiddleClick();
					break;
				case mousecmd::CMD_RIGHT_CLICK:
					m_mouseControl->RightClick();
					break;
				case mousecmd::CMD_LEFT_DOWN:
					m_mouseControl->LeftDown();
					break;
				case mousecmd::CMD_LEFT_UP:
					m_mouseControl->LeftUp();
					break;
				case mousecmd::CMD_DOUBLE_CLICK:
					m_mouseControl->LeftDblClick();
					break;
				default:
					assert (false);
				}

				m_pClickWindowController->ActionDone(xCurr, yCurr);
				
				if (m_visualAlertsEnabled)
					m_progressVisualAlert.End();
				
				if (m_consecutiveClicksAllowed)
					m_dwellCountdown.Reset();	

				return action;
			}
		}
	}
	
	return mousecmd::CMD_NO_CLICK;
}

void CDwellClick::SetEnabled(bool value)
{
	if (value!= m_enabled) {
		if (value) {
			// Enabling dwell click
			Reset();

			// Show click window when needed
			if (m_useClickWindow)
				m_pClickWindowController->Show(true);
		}
		else {
			// Hide click window
			m_pClickWindowController->Show(false);
		}
		m_enabled= value;
	}
}

void CDwellClick::Reset()
{
	m_dwellCountdown.Reset();
	m_progressVisualAlert.End();
}

void CDwellClick::SetUseClickWindow(bool value)
{
	if (value!= m_useClickWindow) {
		if (m_enabled) {
			if (value)
				m_pClickWindowController->Show(true);
			else
				m_pClickWindowController->Show(false);
		}
		m_useClickWindow= value;
	}
}

void CDwellClick::EnableVisualAlerts(bool value)
{ 
	if (value!= m_visualAlertsEnabled) {
		m_visualAlertsEnabled = value;
		if (!value) m_progressVisualAlert.End();
	}
}
