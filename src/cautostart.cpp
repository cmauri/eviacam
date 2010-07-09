/////////////////////////////////////////////////////////////////////////////
// Name:        keyboardbitmapcheck.h
// Purpose:  
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by: 
// Created:     07/07/2010
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
#include "cautostart.h"
#include <wx/string.h>
#include <wx/stdpaths.h>

CAutostart::CAutostart(wxString fileName)
{
    m_fileName = fileName;
}

CAutostart::~CAutostart()
{
}

bool CAutostart::IsEnabled()
{
    wxStandardPaths paths;
    char cstring[1024];
                    
    wxString path = paths.GetUserConfigDir() + wxT("/.config/autostart/") + m_fileName;
    strncpy(cstring, (const char*)path.mb_str(wxConvUTF8), 1023);
                
    if (FILE *input = fopen(cstring, "r" )) {
        fclose(input);
        return true;
    }
    
    return false;
}


void CAutostart::Enable(bool value)
{
    wxStandardPaths paths;
    wxString path;
    char cStringInput[1024];
    char cStringOutput[1024];
    char command[1024];
    char ch;
    
    path = paths.GetUserConfigDir() + wxT("/.config/autostart/") + m_fileName;
    strncpy(cStringOutput, (const char*)path.mb_str(wxConvUTF8), 1023);
                
    if (value) {
        path = wxT("mkdir ") + paths.GetUserConfigDir() + wxT("/.config/autostart");
        strncpy(command, (const char*)path.mb_str(wxConvUTF8), 1023);
        system(command);
        path = wxT("/usr/share/applications/") + m_fileName;
        strncpy(cStringInput, (const char*)path.mb_str(wxConvUTF8), 1023);
        if (FILE *input = fopen(cStringInput, "r")) {
            FILE *output = fopen(cStringOutput, "w");
            while(1) {
                ch = getc(input);
                if(ch==EOF) {
                    break;
                } else {
                    putc(ch,output);
                }
            }
        
            fclose (input);
            fclose (output);
        }
    } else {
        path = wxT("rm ") + path;
        strncpy(command, (const char*)path.mb_str(wxConvUTF8), 1023);
        system(command);
    }
}

