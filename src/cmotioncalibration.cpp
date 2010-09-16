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
#include "mouseoutput.h"
#include "wmotioncalibrationx.h"
#include "wmotioncalibrationy.h"
#include "wconfirmcalibration.h"
#include "timeutil.h"
#include <wx/app.h>
#include <wx/event.h>
#include <stdlib.h>

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


CMotionCalibration::CMotionCalibration(CViacamController* pViacamController)
{
	m_pViacamController= pViacamController;
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

void CMotionCalibration::InitMotionCalibration()
{
	float newSpeedX, newSpeedY;
	
	m_xSpeedBackup = m_pViacamController->GetMouseOutput()->GetXSpeed();
	m_ySpeedBackup = m_pViacamController->GetMouseOutput()->GetYSpeed();
	m_state = WAITING_X;
	
	while (m_state != FINISHED) {
		InitValues();
		m_pViacamController->SetMotionCalibration(true);

		m_pDialog = new WMotionCalibrationX(NULL);
		m_pDialog->ShowModal();
		m_pDialog->Destroy();

		if (m_state == WAITING_Y) {
			m_pDialog = new WMotionCalibrationY(NULL);
			m_pDialog->ShowModal();
			m_pDialog->Destroy();
		}
		
		m_pViacamController->SetMotionCalibration(false);
		
		if (m_state == CONFIRMATION) {
			newSpeedX = fmax(fmin(fabs(((m_posXVirtMax - m_posXVirtMin) * MULTIPLIER_X) - MAX_THRESHOLD_SPEED), MAX_THRESHOLD_SPEED), MIN_THRESHOLD_SPEED);
			newSpeedY = fmax(fmin(fabs(((m_posYVirtMax - m_posYVirtMin) * MULTIPLIER_Y) - MAX_THRESHOLD_SPEED), MAX_THRESHOLD_SPEED), MIN_THRESHOLD_SPEED);
			m_pViacamController->GetMouseOutput()->SetXSpeed(newSpeedX);
			m_pViacamController->GetMouseOutput()->SetYSpeed(newSpeedY);
			m_pDialog = new WConfirmCalibration(NULL);
			int CONFIRMATIONButton = m_pDialog->ShowModal();
			switch (CONFIRMATIONButton)
			{
				case BUTTON_OK:
					m_state = FINISHED;
					break;
				case BUTTON_CANCEL:
					m_state = FINISHED;
					m_pViacamController->GetMouseOutput()->SetXSpeed(m_xSpeedBackup);
					m_pViacamController->GetMouseOutput()->SetYSpeed(m_ySpeedBackup);	
					break;
				case BUTTON_REPEAT:
					m_state = WAITING_X;
					break;
			}
			m_pDialog->Destroy();
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
}


void CMotionCalibration::ComputeMotionRange (float vx, float vy)
{
	switch (m_state)
	{
		case WAITING_X:
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
