/////////////////////////////////////////////////////////////////////////////
// Name:        keyboardbitmapcheck.cpp
// Purpose:  
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by: 
// Created:     28/06/2010
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
#include "keyboardbitmapcheck.h"
#include <wx/utils.h>
#include <X11/Xlib.h>
#include <cmath>


KeySym CKeyboardBitmapCheck::ReadKeySym()
{    
    char keys_return[32];
    unsigned char keys;
    
    KeySym ks;
    KeyCode kc = 0;
            
    XQueryKeymap(((Display *) wxGetDisplay()), keys_return);
    
    for(int i=0; i<32; i++) {
        keys = (unsigned char) keys_return[i];
        if (keys > 0) {
            switch (keys) {
                case 1:   kc = 0; break;
                case 2:   kc = 1; break;
                case 4:   kc = 2; break;
                case 8:   kc = 3; break;
                case 16:  kc = 4; break;
                case 32:  kc = 5; break;
                case 64:  kc = 6; break;
                case 128: kc = 7; break;
            }
            kc += 8 * i;
        }
    }
        
    ks = XKeycodeToKeysym(((Display *) wxGetDisplay()), kc, 0);
    return ks;
}

wxString CKeyboardBitmapCheck::GetKeyName (KeySym ks) 
{
    return wxString(XKeysymToString(ks), wxConvLocal);
}
