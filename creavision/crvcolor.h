/////////////////////////////////////////////////////////////////////////////
// Name:        crvcolor.h
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
#ifndef CRVCOLOR_H
#define CRVCOLOR_H

#include <cv.h>

class CRGBCubicColor
{
private:
	unsigned char m_ColorLookup[256][256][256];
	enum { DEFAULT_RADIUS= 2 };	
public:
	void Clear();
	void TrainColor (unsigned char r, unsigned char g, unsigned char b, 
					int radius= DEFAULT_RADIUS,
					double dDivisor= 1.0);
	void TrainImage (const IplImage *img, int radius= DEFAULT_RADIUS,
				double dDivisor= 1.0);
	void LookupImage (const IplImage *pSrc, IplImage *pDst);
	unsigned char LookupColor (unsigned char r, unsigned char g, unsigned char b) {
		return m_ColorLookup[r][g][b];
	}
};

// Genera constants model de color a partir de la ROI
// d'una imatge RGBA
void crvExtractSkinColorModel (IplImage *pIpl, float &krg_min, float &krg_max,
							   float &krb_min, float &krb_max);

// Binaritza imatge a color segons model de color de la pell
void crvBinarizeSkinColorModel (IplImage *pIpl, float &krg_min, float &krg_max,
							   float &krb_min, float &krb_max);

// Converteix una imatge a color a escala de grisos
void crvColorToGray (const IplImage *pImgSrc, IplImage *pIplDst);

#endif
