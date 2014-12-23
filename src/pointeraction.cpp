/////////////////////////////////////////////////////////////////////////////
// Name:        pointeraction.cpp
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
#include "pointeraction.h"
#include "dwellclick.h"
#include "gestureclick.h"
#include "paths.h"

#include <wx/stdpaths.h>
#include <wx/sound.h>
#include <wx/msgdlg.h>


CPointerAction::CPointerAction() 
: m_enabled(false)
{
	m_pClickSound= new wxSound (eviacam::GetDataDir() + _T("/click.wav"));
	m_pLeftUpClickSound= new wxSound (eviacam::GetDataDir() + _T("/click2.wav"));

#if defined(__WXGTK__)
	m_pMouseControl= new CMouseControl ((void *) wxGetDisplay());
#else
	m_pMouseControl= new CMouseControl ();
#endif
	m_pDwellClick= new CDwellClick (*m_pMouseControl);
	m_pGestureClick= new CGestureClick (*m_pMouseControl);
	
	InitDefaults ();
}

CPointerAction::~CPointerAction ()
{
	delete m_pGestureClick;
	m_pGestureClick= NULL;
	delete m_pDwellClick;
	m_pDwellClick= NULL;
	delete m_pMouseControl;
	m_pMouseControl= NULL;
	delete m_pClickSound;
	m_pClickSound= NULL;
	delete m_pLeftUpClickSound;
	m_pLeftUpClickSound= NULL;
}

// Configuration methods
void CPointerAction::InitDefaults()
{
	// General attributes
	SetXSpeed (10);
	SetYSpeed (10);
	SetAcceleration (2);
	SetClickMode (CPointerAction::DWELL);
	SetBeepOnClick (true);
	SetSmoothness (2);
	SetEasyStopValue (1); 
	SetWrapPointer(false);
	SetSendActionWait(0);
	
	// Workspace limits
	SetRestrictedWorkingArea (false);
	SetTopWorkspace(1);
	SetLeftWorkspace(1);
	SetRightWorkspace(1);
	SetBottomWorkspace(1);
}

void CPointerAction::WriteProfileData(wxConfigBase* pConfObj)
{
	pConfObj->SetPath (_T("pointerAction"));

	pConfObj->Write(_T("xSpeed"), (int) GetXSpeed());
	pConfObj->Write(_T("ySpeed"), (int) GetYSpeed());
	pConfObj->Write(_T("acceleration"), (int) GetAcceleration());
	pConfObj->Write(_T("smoothness"), (int) GetSmoothness());
	pConfObj->Write(_T("easyStop"), (int) GetEasyStopValue());
	pConfObj->Write(_T("enabledWorkspace"), (bool) GetRestrictedWorkingArea());
	pConfObj->Write(_T("topWorkspace"), (int) GetTopWorkspace());
	pConfObj->Write(_T("leftWorkspace"), (int) GetLeftWorkspace());
	pConfObj->Write(_T("rightWorkspace"), (int) GetRightWorkspace());
	pConfObj->Write(_T("bottomWorkspace"), (int) GetBottomWorkspace());	
	pConfObj->Write(_T("enabledWrapPointer"), (bool) GetWrapPointer());
	pConfObj->Write(_T("clickMode"), (int) GetClickMode());
	pConfObj->Write(_T("beepOnClick"), (bool) GetBeepOnClick());
	pConfObj->Write(_T("sendActionWait"), (int) GetSendActionWait());

	m_pDwellClick->WriteProfileData(pConfObj);
	m_pGestureClick->WriteProfileData(pConfObj);

	pConfObj->SetPath (_T(".."));
}

void CPointerAction::ReadProfileData(wxConfigBase* pConfObj)
{
	int val;
	bool valb;

	pConfObj->SetPath (_T("pointerAction"));

	if (pConfObj->Read(_T("xSpeed"), &val))	SetXSpeed(val);
	if (pConfObj->Read(_T("ySpeed"), &val))	SetYSpeed(val);
	if (pConfObj->Read(_T("acceleration"), &val)) SetAcceleration(val);
	if (pConfObj->Read(_T("smoothness"), &val)) SetSmoothness(val);
	if (pConfObj->Read(_T("easyStop"), &val)) SetEasyStopValue(val);
	if (pConfObj->Read(_T("enabledWorkspace"), &valb)) SetRestrictedWorkingArea(valb);
	if (pConfObj->Read(_T("enabledWrapPointer"), &valb)) SetWrapPointer(valb);
	if (pConfObj->Read(_T("topWorkspace"), &val)) SetTopWorkspace(val);
	if (pConfObj->Read(_T("leftWorkspace"), &val)) SetLeftWorkspace(val);
	if (pConfObj->Read(_T("rightWorkspace"), &val)) SetRightWorkspace(val);
	if (pConfObj->Read(_T("bottomWorkspace"), &val)) SetBottomWorkspace(val);
	if (pConfObj->Read(_T("clickMode"), &val)) SetClickMode((CPointerAction::EClickMode) val);
	pConfObj->Read(_T("beepOnClick"), &m_beepOnClick);	
	if (pConfObj->Read(_T("sendActionWait"), &val)) SetSendActionWait(val);

	m_pDwellClick->ReadProfileData(pConfObj);
	m_pGestureClick->ReadProfileData(pConfObj);

	pConfObj->SetPath (_T(".."));
}

bool CPointerAction::GetVisualAlerts() const
{
	// This setting affects both dwell and gesture click
	return m_pDwellClick->AreVisualAlertsEnabled() ||
		m_pGestureClick->AreVisualAlertsEnabled();	
}

void CPointerAction::SetVisualAlerts(bool value)
{
	// This setting affects both dwell and gesture click
	m_pDwellClick->EnableVisualAlerts(value);
	m_pGestureClick->EnableVisualAlerts(value);	
}
	
// Common dwell time setting for dwell and gesture click
unsigned int CPointerAction::GetDwellTime() const
{
	// This setting affects both dwell and gesture click
	assert (m_pDwellClick->GetDwellTime()== m_pGestureClick->GetDwellTime());
	return m_pDwellClick->GetDwellTime();		
}

void CPointerAction::SetDwellTime (unsigned int ds)
{
	// This setting affects both dwell and gesture click
	m_pDwellClick->SetDwellTime(ds);
	m_pGestureClick->SetDwellTime(ds);
}

unsigned int CPointerAction::GetDwellToleranceArea() const
{
	// This setting affects both dwell and gesture click
	assert (m_pDwellClick->GetDwellToleranceArea()== 
		m_pGestureClick->GetDwellToleranceArea());
	return m_pDwellClick->GetDwellToleranceArea();
}

void CPointerAction::SetDwellToleranceArea(unsigned int value)
{
	// This setting affects both dwell and gesture click
	m_pDwellClick->SetDwellToleranceArea(value);
	m_pGestureClick->SetDwellToleranceArea(value);
}

float CPointerAction::GetSpeedFactor(unsigned int speed) const
{
	return (float) pow (2.718281828459045235, speed / 6.0);	
}

void CPointerAction::SetAcceleration(unsigned int acceleration)
{
	if (acceleration> 5) acceleration= 5;

	switch (acceleration) {
	case 0: m_pMouseControl->SetRelAcceleration2 (); break;
	case 1: m_pMouseControl->SetRelAcceleration2 (7, 1.5f); break;
	case 2: m_pMouseControl->SetRelAcceleration2 (7, 2.0f); break;
	case 3: m_pMouseControl->SetRelAcceleration2 (7, 1.5f, 14, 2.0f); break;
	case 4: m_pMouseControl->SetRelAcceleration2 (7, 2.0f, 14, 1.5f); break;
	case 5: m_pMouseControl->SetRelAcceleration2 (7, 2.0f, 14, 2.0f); break;
	default: assert (0);
	}

	m_acceleration= acceleration;
}

void CPointerAction::ProcessMotion (float dxSensor, float dySensor)
{
	if (!m_enabled) return;
	
	// Do move.
	int dxPix, dyPix;
	m_pMouseControl->MovePointerRel (dxSensor, dySensor, &dxPix, &dyPix);

	// Get current pointer location
	int xCurr, yCurr;
	m_pMouseControl->GetPointerLocation (xCurr, yCurr);

	mousecmd::mousecmd cmd= mousecmd::CMD_NO_CLICK;

	switch (m_clickMode) {
	case CPointerAction::DWELL:
		// DWell click
		cmd= m_pDwellClick->ProcessMotion
			(dxPix, dyPix, xCurr, yCurr);
		break;
	case CPointerAction::GESTURE:
		// Gesture click
		cmd= m_pGestureClick->ProcessMotion
			(dxPix, dyPix, xCurr, yCurr);
		break;
	case CPointerAction::NONE:
		// Do nothing
		break;
	default:
		assert (false);
	}

	if (cmd!= mousecmd::CMD_NO_CLICK && m_beepOnClick) {
		if (cmd== mousecmd::CMD_LEFT_DOWN)
			m_pLeftUpClickSound->Play (wxSOUND_ASYNC);
		else
			m_pClickSound->Play (wxSOUND_ASYNC);
	}
}

void CPointerAction::SetEnabled(bool value)
{
	if (value!= m_enabled) {
		switch (m_clickMode) {
		case CPointerAction::NONE:
			// Do nothing
			break;
		case CPointerAction::DWELL:
			m_pDwellClick->SetEnabled(value);
			break;
		case CPointerAction::GESTURE:
			m_pGestureClick->SetEnabled(value);
			break;
		default:
			assert (false);
			break;
		}		
		m_enabled= value;
	}
}

// Return true is the change has been applied, or false otherwise
bool CPointerAction::SetClickMode(CPointerAction::EClickMode mode, bool silent, wxWindow* parent)
{
	if (mode!= m_clickMode)	{
		if (!silent) {
			if (mode== CPointerAction::NONE) {
				wxMessageDialog dlg (parent, _("This action will disable eViacam click generation.\nAre you sure?"), _T("Enable Viacam"), wxICON_EXCLAMATION | wxYES_NO );
				if (dlg.ShowModal()!= wxID_YES) return false;
			}
			else {
				wxMessageDialog dlg (parent, _("This action will change the click generation method.\nAre you sure?"), _T("Enable Viacam"), wxICON_EXCLAMATION | wxYES_NO );
				if (dlg.ShowModal()!= wxID_YES) return false;
			}
		}
		
		switch (mode) {
		case CPointerAction::NONE:
			// Disable dwell and gesture click
			m_pGestureClick->SetEnabled(false);
			m_pDwellClick->SetEnabled(false);
			break;
		case CPointerAction::DWELL:
			// disable gesture click if enabled
			m_pGestureClick->SetEnabled(false);			
			// Enable dwell click
			if (m_enabled) m_pDwellClick->SetEnabled(true);
			break;
		case CPointerAction::GESTURE:
			// Enable gesture click
			m_pDwellClick->SetEnabled(false);
			// enable gesture click
			if (m_enabled) m_pGestureClick->SetEnabled(true);
			break;
		default:
			assert (false);
			break;
		}		
		m_clickMode= mode;
	}

	return true;
}

void CPointerAction::ResetCounters()
{
	if (m_pDwellClick) m_pDwellClick->Reset();
	if (m_pGestureClick) m_pGestureClick->Reset();
}
