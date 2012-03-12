/////////////////////////////////////////////////////////////////////////////
// Name:		langnames-utf8.h
// Purpose:     This file is encoded using UTF-8 in order to compile
//				native language names under Linux platforms
// Author:      César Mauri Loba
// Modified by: 
// Created:     05/03/2009
// RCS-ID:      
// Copyright:   (C) 2009 Cesar Mauri from CREA Sistemes Informatics
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

// Uncomment to enable Asturian language support (see README first)
// #define ENABLE_ASTURIAN

const wxString s_langNames[]	= {
	_("System default"),
#ifdef ENABLE_ASTURIAN	
	_T("Asturian"),
#endif
	_T("Català"), 
	_T("Deutsch"),
	_T("English"), 
	_T("Español"), 
	_T("Galego"), 
	_T("Italiano"),
	_T("Русский"),
	_T("Türkçe"),
	_T("Occitan"),
	_T("Français"),
	_T("العربية"),
	_T("עִבְרִית")	
};
