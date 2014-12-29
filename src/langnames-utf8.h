/////////////////////////////////////////////////////////////////////////////
// Name:		langnames-utf8.h
// Purpose:     Provide native language names. File encoded using UTF-8
//				
// Author:      César Mauri Loba
// Created:     05/03/2009
// Copyright:   (C) 2009-14 Cesar Mauri from CREA Sistemes Informatics
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
	{ wxString::FromUTF8("العربية"), wxLANGUAGE_ARABIC },
#ifdef ENABLE_ASTURIAN	
	{ wxString::FromUTF8("Asturianu"),	wxLANGUAGE_ASTURIAN },
#endif
	{ wxString::FromUTF8("Català"),	wxLANGUAGE_CATALAN },
	{ wxString::FromUTF8("简体中文"), wxLANGUAGE_CHINESE_SIMPLIFIED },
	{ wxString::FromUTF8("正體中文"), wxLANGUAGE_CHINESE_TAIWAN },
	{ wxString::FromUTF8("Deutsch"), wxLANGUAGE_GERMAN },
	{ wxString::FromUTF8("Ελληνικά"), wxLANGUAGE_GREEK },
	{ wxString::FromUTF8("English"), wxLANGUAGE_ENGLISH },
	{ wxString::FromUTF8("Español"), wxLANGUAGE_SPANISH },
	{ wxString::FromUTF8("Français"), wxLANGUAGE_FRENCH },
	{ wxString::FromUTF8("Galego"), wxLANGUAGE_GALICIAN },
	{ wxString::FromUTF8("עִבְרִית"), wxLANGUAGE_HEBREW },
	{ wxString::FromUTF8("Italiano"), wxLANGUAGE_ITALIAN },
	{ wxString::FromUTF8("Lietuvių"), wxLANGUAGE_LITHUANIAN },
	{ wxString::FromUTF8("日本語"), wxLANGUAGE_JAPANESE },
	{ wxString::FromUTF8("Occitan"), wxLANGUAGE_OCCITAN },
	{ wxString::FromUTF8("Português"), wxLANGUAGE_PORTUGUESE_BRAZILIAN },
	{ wxString::FromUTF8("Русский"), wxLANGUAGE_RUSSIAN },
	{ wxString::FromUTF8("Türkçe"),	wxLANGUAGE_TURKISH }
};

