/////////////////////////////////////////////////////////////////////////////
// Name:        crvhistogram.h
// Purpose:  
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by: 
// Created:     30/05/2008
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
#ifndef CRVHISTOGRAM_H
#define CRVHISTOGRAM_H

#include <cv.h>

typedef unsigned char TCrvLookupTable[256];
typedef int TCrvHistogram[256];

// Calcula l'histograma d'una imatge
void crvHistogram (IplImage *srcImg, TCrvHistogram &his);

// Calcula on comença i acaba un histograma. Retorna
// false si l'histograma és buit (tot zeros)
bool crvHistogramBeginEnd (TCrvHistogram &his, unsigned char &begin, unsigned char &end);

// Aplica a una imatge de grisos la transformació que imposa la
// lookup-table passada com a paràmetre (té en compte ROI)
void crvLUTTransform (IplImage *srcImg, IplImage *dstImg, TCrvLookupTable &lut);

// Genera la LUT per normalitzar una imatge a partir del seu
// histograma. Retorna la mida de l'histograma
int crvNormalizeHistogram (TCrvHistogram &his, TCrvLookupTable &lut, unsigned char minRange= 0);

// Normalitza una imatge en escala de grisos (impl. lenta)
// Retorna mida histograma original
int crvNormalize (IplImage *src, IplImage *dst);

#endif
