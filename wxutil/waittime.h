/////////////////////////////////////////////////////////////////////////////
// Name:        dwellaction.h
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

#ifndef WAITTIME_H
#define WAITTIME_H

class CWaitTime
{
public:
	CWaitTime(unsigned long waitTimeMs= 1000);
	~CWaitTime(void);
	inline const unsigned long GetWaitTimeMs () const;
	inline void SetWaitTimeMs (unsigned long val);
	void Reset ();
	// If countdown expired returns true once and false for all
	// subsequent calls until a Reset
	bool OneShootAction();
	bool HasExpired() const;
	int PercentagePassed() const;

private:
	unsigned long m_timeCountMs, m_waitTimeMs;
	bool m_actionDone;
};

inline const unsigned long CWaitTime::GetWaitTimeMs () const
{
	return m_waitTimeMs; 
}

inline void CWaitTime::SetWaitTimeMs (unsigned long val)
{
	m_waitTimeMs= val;
}

#endif
