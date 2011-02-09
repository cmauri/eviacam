/////////////////////////////////////////////////////////////////////////////
// Name:        cmotioncalibration.h
// Purpose:  
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by: 
// Created:     07/09/2010
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

#include "cmotioncalibration.h"
#include "viacamcontroller.h"
#include "wmotioncalibrationx.h"
#include "wmotioncalibrationy.h"
#include "wconfirmcalibration.h"
#include "timeutil.h"
#include "pointeraction.h"
#include "eviacamapp.h"

#include <stdlib.h>
#include <math.h>
#include <wx/app.h>
#include <wx/event.h>

#if defined(_WIN32) || defined(_WIN64)
#define fmax max
#define fmin min
#endif



#define BUTTON_OK 1
#define BUTTON_CANCEL 2
#define BUTTON_REPEAT 3


#define TIME_LIMIT_NO_MOTION_X 5000
#define MOTION_THRESHOLD_X 0.5f
#define TIME_LIMIT_NO_X_RANGE_EXPANDED 3000
#define TIME_LIMIT_NO_MOTION_Y 5000
#define MOTION_THRESHOLD_Y 0.5f
#define TIME_LIMIT_NO_Y_RANGE_EXPANDED 3000
#define MULTIPLIER_X 0.27f
#define MULTIPLIER_Y 0.20f
#define MAX_THRESHOLD_SPEED 30.0f
#define MIN_THRESHOLD_SPEED 1.0f


CMotionCalibration::CMotionCalibration()
{
	m_pDialog = NULL;		
	m_xSpeedBackup = 0; m_ySpeedBackup = 0;
	InitValues();
}

CMotionCalibration::~CMotionCalibration()
{
}

void CMotionCalibration::InitValues()
{
	m_posXVirt = 0;
	m_posYVirt = 0;
	m_posXVirtMax = 0;
	m_posYVirtMax = 0;
	m_posXVirtMin = 0;
	m_posYVirtMin = 0;
	m_lastTimestamp = CTimeUtil::GetMiliCount();
	m_state = WAITING_X;
}

bool CMotionCalibration::InitMotionCalibration()
{
	bool changes = false;	
	
	m_xSpeedBackup = wxGetApp().GetController().GetPointerAction().GetXSpeed();
	m_ySpeedBackup = wxGetApp().GetController().GetPointerAction().GetYSpeed();
	m_state = WAITING_X;
	
	// Store previous values
	bool isEnabled= wxGetApp().GetController().GetEnabled();
	CPointerAction::EClickMode clickMode= wxGetApp().GetController().GetPointerAction().GetClickMode();
		
	while (m_state != FINISHED) {
		InitValues();
		wxGetApp().GetController().SetMotionCalibrationEnabled(true);

		m_pDialog = new WMotionCalibrationX(NULL);
		m_pDialog->ShowModal();
		m_pDialog->Destroy();
		m_pDialog= NULL;

		if (m_state == WAITING_Y) {
			m_pDialog = new WMotionCalibrationY(NULL);
			m_pDialog->ShowModal();
			m_pDialog->Destroy();
			m_pDialog= NULL;
		}
		
		wxGetApp().GetController().SetMotionCalibrationEnabled(false);
		
		if (m_state == CONFIRMATION) {
			// Compute new speed parameters
			// TODO: improve formula taking into account several proves and adjustements
			float newSpeedX = 600.0f / (m_posXVirtMax - m_posXVirtMin);				
			float newSpeedY = 600.0f / (m_posYVirtMax - m_posYVirtMin);
			
			// Set between reasonable limits
			if (newSpeedX> 22.0f) newSpeedX= 22.0f;
			else if (newSpeedX< 10.0f) newSpeedX= 10.0f;
			if (newSpeedY> 20.0f) newSpeedY= 20.0f;
			else if (newSpeedY< 10.0f) newSpeedY= 10.0f;
			
			// Set new parameters
			wxGetApp().GetController().GetPointerAction().SetXSpeed(newSpeedX);
			wxGetApp().GetController().GetPointerAction().SetYSpeed(newSpeedY);
			
			// Disable click generation & enable motion to test
			wxGetApp().GetController().GetPointerAction().SetClickMode(CPointerAction::NONE);
			wxGetApp().GetController().SetEnabled(true, true);			
						
			// Request user acknowledgment
			m_pDialog = new WConfirmCalibration(NULL);
			int retvalConfirm = m_pDialog->ShowModal();
			if (retvalConfirm== BUTTON_REPEAT) m_state = WAITING_X;
			else {
				if (retvalConfirm== BUTTON_OK) changes = true;
				else if (retvalConfirm== BUTTON_CANCEL) {
					wxGetApp().GetController().GetPointerAction().SetXSpeed(m_xSpeedBackup);
					wxGetApp().GetController().GetPointerAction().SetYSpeed(m_ySpeedBackup);	
				}
				else assert (false);
				// Restore previous settings
				wxGetApp().GetController().GetPointerAction().SetClickMode(clickMode);
				wxGetApp().GetController().SetEnabled(isEnabled, true);
				
				m_state = FINISHED;
			}
			m_pDialog->Destroy();
			m_pDialog= NULL;			
		}
		
		if (m_state == ABORTING) {
			wxMessageDialog dlg (NULL, _("No movement was detected.\nDo you want to repeat the calibration?"), _("eViacam warning"), wxICON_EXCLAMATION | wxYES_NO );
			if (dlg.ShowModal() == wxID_YES) {
				m_state = WAITING_X;
			} else {
				m_state = FINISHED;
			}
		}
	}
	return changes;
}


void CMotionCalibration::ComputeMotionRange (float vx, float vy, bool warnFaceNotDetected)
{
	switch (m_state)
	{
		case WAITING_X:
			((WMotionCalibrationX*)m_pDialog)->SetFaceDetected(warnFaceNotDetected);
			m_posXVirt += vx;
			if (fabs(vx) > MOTION_THRESHOLD_X) {
				m_state = MEASURING_X;
				m_lastTimestamp = CTimeUtil::GetMiliCount();
			} else {
				if (CTimeUtil::GetMiliCount() - m_lastTimestamp > TIME_LIMIT_NO_MOTION_X) {
					m_state = ABORTING;
					wxCommandEvent event (wxEVT_CLOSE_WINDOW);
					wxPostEvent(m_pDialog, event);
				}
			}
			break;
			
		case MEASURING_X:
			((WMotionCalibrationX*)m_pDialog)->SetFaceDetected(warnFaceNotDetected);
			
			m_posXVirt += vx;
			if (m_posXVirt > m_posXVirtMax) {
				m_posXVirtMax = m_posXVirt;
				m_lastTimestamp = CTimeUtil::GetMiliCount();
			}
			
			if (m_posXVirt < m_posXVirtMin) {
				m_posXVirtMin = m_posXVirt;
				m_lastTimestamp = CTimeUtil::GetMiliCount();
			}
	
			if (CTimeUtil::GetMiliCount() - m_lastTimestamp > TIME_LIMIT_NO_X_RANGE_EXPANDED) {
				m_state = WAITING_Y;
				m_lastTimestamp = CTimeUtil::GetMiliCount();
				wxCommandEvent event (wxEVT_CLOSE_WINDOW);
				wxPostEvent(m_pDialog, event);
			}
			break;
			
		case WAITING_Y:
			((WMotionCalibrationY*)m_pDialog)->SetFaceDetected(warnFaceNotDetected);
			m_posYVirt += vy;
			if (fabs(vy) > MOTION_THRESHOLD_Y) {
				m_state = MEASURING_Y;
				m_lastTimestamp = CTimeUtil::GetMiliCount();
			} else {
				if (CTimeUtil::GetMiliCount() - m_lastTimestamp > TIME_LIMIT_NO_MOTION_Y) {
					m_state = ABORTING;
					wxCommandEvent event (wxEVT_CLOSE_WINDOW);
					wxPostEvent(m_pDialog, event);
				}
			}
			break;
			
		case MEASURING_Y:
			((WMotionCalibrationY*)m_pDialog)->SetFaceDetected(warnFaceNotDetected);
			m_posYVirt += vy;
			if (m_posYVirt > m_posYVirtMax) {
				m_posYVirtMax = m_posYVirt;
				m_lastTimestamp = CTimeUtil::GetMiliCount();
			}
			
			if (m_posYVirt < m_posYVirtMin) {
				m_posYVirtMin = m_posYVirt;
				m_lastTimestamp = CTimeUtil::GetMiliCount();
			}
	
			if (CTimeUtil::GetMiliCount() - m_lastTimestamp > TIME_LIMIT_NO_Y_RANGE_EXPANDED) {
				m_state = CONFIRMATION;
				m_lastTimestamp = CTimeUtil::GetMiliCount();
				wxCommandEvent event (wxEVT_CLOSE_WINDOW);
				wxPostEvent(m_pDialog, event);
			}
			break;
		default:
			break;	
	}
}
