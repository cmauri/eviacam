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

#if !defined(DWELLACTION_H)
#define DWELLACTION_H

class CDwellAction
{
public:
	CDwellAction(unsigned long waitTimeMs= 1000);
	~CDwellAction(void);
	inline const unsigned long GetWaitTimeMs () const;
	inline void SetWaitTimeMs (unsigned long val);
	void Reset ();
	// True if time-out
	bool Update();
		
private:
	unsigned long GetMiliCount();
	unsigned long m_timeCountMs, m_waitTimeMs;
	bool m_actionDone;
};

inline const unsigned long CDwellAction::GetWaitTimeMs () const
{ 
	return m_waitTimeMs; 
}

inline void CDwellAction::SetWaitTimeMs (unsigned long val)
{ 	
	m_waitTimeMs= val; 
}

#endif
