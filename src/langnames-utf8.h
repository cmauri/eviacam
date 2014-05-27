/////////////////////////////////////////////////////////////////////////////
// Name:		langnames-utf8.h
// Purpose:     This file is encoded using UTF-8 with BOM in order to compile
//				native language names
// Author:      César Mauri Loba
// Created:     05/03/2009
// Copyright:   (C) 2009-13 Cesar Mauri from CREA Sistemes Informatics
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

struct lang_t {
	wxString name;
	wxLanguage lang;
};

static const struct lang_t g_languages[] = {
	{ _("System default"),	wxLANGUAGE_DEFAULT },
	{ _T("العربية"), wxLANGUAGE_ARABIC },
#ifdef ENABLE_ASTURIAN	
	{ _T("Asturianu"),	wxLANGUAGE_ASTURIAN },
#endif
	{ _T("Català"),	wxLANGUAGE_CATALAN },
	{ _T("Deutsch"), wxLANGUAGE_GERMAN },
	{ _T("Ελληνικά"), wxLANGUAGE_GREEK },
	{ _T("English"), wxLANGUAGE_ENGLISH },
	{ _T("Español"), wxLANGUAGE_SPANISH },
	{ _T("Français"), wxLANGUAGE_FRENCH },
	{ _T("Galego"), wxLANGUAGE_GALICIAN },
	{ _T("עִבְרִית"), wxLANGUAGE_HEBREW },
	{ _T("Italiano"), wxLANGUAGE_ITALIAN },
    { _T("Lietuvių"), wxLANGUAGE_LITHUANIAN },
	{ _T("日本語"), wxLANGUAGE_JAPANESE },
	{ _T("Occitan"), wxLANGUAGE_OCCITAN },
	{ _T("Português"), wxLANGUAGE_PORTUGUESE_BRAZILIAN },
	{ _T("Русский"), wxLANGUAGE_RUSSIAN },
	{ _T("Türkçe"),	wxLANGUAGE_TURKISH }
};

