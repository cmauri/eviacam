/////////////////////////////////////////////////////////////////////////////
// Name:        crvhistogram.cpp
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
#include "crvhistogram.h"
#include "crvmisc.h"

// Computes the histogram of a gray image
void crvHistogram (IplImage *srcImg, TCrvHistogram &his)
{
	int x, y, xIni, yIni, xLim, yLim;
	unsigned char *pSrc= (unsigned char *) srcImg->imageData;
	
	assert (srcImg->depth== IPL_DEPTH_8U);

	// ROI
	crvGetROILimits (srcImg, xIni, yIni, xLim, yLim);

	// Init
	for (x= 0; x< 256; x++) his[x]= 0;

	for (y= yIni; y< yLim; y++) {
		// Begining row pointer
		pSrc= (unsigned char *) crvImgByteOffset (srcImg, xIni, y);		
		for (x= xIni; x< xLim; x++) {
			his[*pSrc]++;
			pSrc++;
		}
	}
}

// Computes where the histogram begins and ends (non-zero values)
// Return false if histogram is void
bool crvHistogramBeginEnd (TCrvHistogram &his, unsigned char &begin, unsigned char &end)
{
	int i;
	
	// Begining
	for (i= 0; i< 256; i++) {
		if (his[i]> 0) {
			begin= (unsigned char) i;
			break;
		}
	}

	if (i== 256) return false;

	// End
	for (i= 255; i>= 0; i--) {
		if (his[i]> 0) {
			end= (unsigned char) i;
			break;
		}
	}

	assert (i> -1);

	return true;
}


// Aplica a una imatge de grisos la transformaci� que imposa la
// lookup-table passada com a par�metre (t� en compte ROI)
void crvLUTTransform (IplImage *srcImg, IplImage *dstImg, TCrvLookupTable &lut)
{
	int x, y, xIni, yIni, xLim, yLim;
	unsigned char *pSrc= (unsigned char *) srcImg->imageData,
				  *pDst= (unsigned char *) dstImg->imageData;

	assert (srcImg->depth== IPL_DEPTH_8U && 
			dstImg->depth== IPL_DEPTH_8U);
	
	// L�mits on s'ha d'aplicar la funci�
	crvGetROILimits (srcImg, xIni, yIni, xLim, yLim);

	// Aplica tractament a la ROI
	for (y= yIni; y< yLim; y++) {
		// Apuntador inici fila
		pSrc= (unsigned char *) crvImgByteOffset (srcImg, xIni, y);
		pDst= (unsigned char *) crvImgByteOffset (dstImg, xIni, y);
		for (x= xIni; x< xLim; x++) {
			*pDst= lut[*pSrc];
			pSrc++;
			pDst++;
		}
	}
}

// Genera la LUT per normalitzar una imatge a partir del seu
// histograma. Retorna la mida de l'histograma
int crvNormalizeHistogram (TCrvHistogram &his, TCrvLookupTable &lut, unsigned char minRange)
{
	int i, range;
	unsigned char min, max;

	if (!crvHistogramBeginEnd (his, min, max)) return 0;
	if (min== max) return 0;

	range= max - min;
	if (range< minRange) range= minRange;

	for (i= min; i<= max; i++) {
		lut[i]= (unsigned char) (((int) (i - min) * 255) / range);
	}

	return max - min;
}


// Normalitza una imatge en escala de grisos
int crvNormalize (IplImage *src, IplImage *dst)
{
	TCrvHistogram his;
	TCrvLookupTable lut;
	int range;
	 	
	assert (src->depth== IPL_DEPTH_8U && dst->depth== IPL_DEPTH_8U);

	crvHistogram (src, his);

	range= crvNormalizeHistogram (his, lut);

	crvLUTTransform (src, dst, lut);

	return range;
}
