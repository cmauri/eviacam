/////////////////////////////////////////////////////////////////////////////
// Name:        dwellaction.cpp
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
#include "waittime.h"
#include "timeutil.h"

CWaitTime::CWaitTime(unsigned long waitTimeMs)
{
	m_waitTimeMs= waitTimeMs;
	Reset ();
}

CWaitTime::~CWaitTime(void)
{
}

void CWaitTime::Reset ()
{
	m_timeCountMs= CTimeUtil::GetMiliCount();
	m_actionDone= false;
}

// If countdown expired returns true once and false for all
// subsequent calls until a Reset
bool CWaitTime::OneShootAction()
{
	if (!m_actionDone)
	{
		unsigned long now= CTimeUtil::GetMiliCount();
		if ((now - m_timeCountMs)> m_waitTimeMs)
		{
			m_actionDone= true;
			return true;
		}
	}	
	return false;
}

bool CWaitTime::HasExpired() const
{
	unsigned long now= CTimeUtil::GetMiliCount();
	return ((now - m_timeCountMs)> m_waitTimeMs);
}

int CWaitTime::PercentagePassed() const
{
	unsigned long now= CTimeUtil::GetMiliCount();
	if ((now - m_timeCountMs)> m_waitTimeMs) return 100;
	float percentage = 100.0f * (float) (now - m_timeCountMs) / (float) m_waitTimeMs;
	return (int) percentage;
}

