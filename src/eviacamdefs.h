/////////////////////////////////////////////////////////////////////////////
// Name:        eviacamdefs.h
// Purpose:  
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by: Common definitions
// Created:     30/12/2010
// Copyright:   (C) 2010 Cesar Mauri Loba - CREA Software Systems
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
#ifndef EVIACAMDEFS_H
#define EVIACAMDEFS_H

// Under wxGTK we should protect calls to GUI. Under Windows is not needed
#if defined(__WXGTK__) 
#include <wx/thread.h>
#define BEGIN_GUI_CALL_MUTEX() if (!wxIsMainThread()) wxMutexGuiEnter();
#define END_GUI_CALL_MUTEX() if (!wxIsMainThread()) wxMutexGuiLeave();
#else
#define BEGIN_GUI_CALL_MUTEX()
#define END_GUI_CALL_MUTEX()
#endif // __WXGTK___

#endif
