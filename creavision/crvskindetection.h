/////////////////////////////////////////////////////////////////////////////
// Name:        crvskindetection.h
// Purpose:  
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by: 
// Created:     23/02/2008
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
#ifndef CRVSKINDETECTION_H
#define CRVSKINDETECTION_H

#include <cv.h>

class CSkinRegionModel
{
	
public:
	// Binaritza imatge a color segons model de color de la pell
	static void crvBinarizeSkin_KToyama (IplImage *pIpl, float &krg_min, 
									   float &krg_max, float &krb_min, 
									   float &krb_max);

	// Binaritza imatge a color segons model de color de la pel
	//static void IplBinarizeSkin	(IplImage *pIpl);
	static void crvBinarizeSkin  (IplImage *pSrc, IplImage *pDst);
};

#endif	
