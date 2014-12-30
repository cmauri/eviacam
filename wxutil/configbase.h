/////////////////////////////////////////////////////////////////////////////
// Name:        configbase.h
// Purpose:     
// Author:      Cesar Mauri Loba
// Modified by: 
// Created:     16/01/2009
// Copyright:   (C) 2009 Cesar Mauri (cesar at crea-si dot com)
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
#ifndef CONFIGBASE_H
#define CONFIGBASE_H

#include <wx/config.h> 

class CConfigBase {
  public:
    virtual void InitDefaults() = 0;

	virtual void ReadAppData(wxConfigBase* pConfObj);
	virtual void ReadProfileData(wxConfigBase* pConfObj) = 0;

	virtual void WriteAppData(wxConfigBase* pConfObj);
    virtual void WriteProfileData(wxConfigBase* pConfObj) = 0;    

	virtual ~CConfigBase();
};
#endif
