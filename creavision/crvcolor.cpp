/////////////////////////////////////////////////////////////////////////////
// Name:        crvcolor.cpp
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

#include <string.h>
#include <assert.h>
#include "crvcolor.h"
#include "crvmisc.h"
#include "crvgeomtry.h"

void CRGBCubicColor::Clear()
{
	memset (m_ColorLookup, 0, 256*256*256);
}

void CRGBCubicColor::TrainColor (unsigned char r, unsigned char g, unsigned char b, int radius, double dDivisor)
{
	int rRadius, gRadius, bRadius, iR, iG, iB;
	t3DRPoint p1, p2;
	double val;

	if (r< radius) rRadius= r;
	else if ((255 - r)< radius) rRadius= (255 - r);
	else rRadius= radius;

	if (g< radius) gRadius= g;
	else if ((255 - g)< radius) gRadius= (255 - g);
	else gRadius= radius;

	if (b< radius) bRadius= b;
	else if ((255 - b)< radius) bRadius= (255 - b);
	else bRadius= radius;

	p1.x= r; p1.y= g; p1.z= b;

	for (iR= (int) r - rRadius; iR<= (int) r + rRadius; iR++) {
		for (iG= (int) g - gRadius; iG<= (int) g + gRadius; iG++) {
			for (iB= (int) b - bRadius; iB<= (int) b + bRadius; iB++) {
				p2.x= iR; p2.y= iG; p2.z= iB;
				val= 255.0 / (points3d_distance (&p1, &p2) / dDivisor + 1.0);
				val+= m_ColorLookup[iR][iG][iB];
				if (val>= 255.0) m_ColorLookup[iR][iG][iB]= 255;
				else m_ColorLookup[iR][iG][iB]= (unsigned char) val;
			}
		}
	}
}

// Realitza entrenament per una imatge IPL tenint en compte ROI
void CRGBCubicColor::TrainImage (const IplImage *img, int radius, double dDivisor)
{	
	int x, y, xIni, yIni, xLim, yLim;
	unsigned char *pImg;

	assert (img && img->imageData);
	assert (!strncmp (img->colorModel, "RGBA", 4) &&
			!strncmp (img->channelSeq, "BGRA", 4));

	// Límits on s'ha d'aplicar la funció
	crvGetROILimits (img, xIni, yIni, xLim, yLim);
	
	// Aplica tractament a la ROI
	for (y= yIni; y< yLim; y++) {
		// Apuntador inici fila component R
		pImg= (unsigned char *) crvImgOffset (img, xIni, y);		
		for (x= xIni; x< xLim; x++) {
			TrainColor (pImg[2], pImg[1], pImg[0], radius, dDivisor);
			pImg+= 4;
		}
	}
}

// Genera imatge resultant d'indexar la taula 
// per tots els píxels
// De moment només vàlid per imatges RGBA
void CRGBCubicColor::LookupImage (const IplImage *pSrc, IplImage *pDst)
{
	int x, y, xIni, yIni, xLim, yLim;
	unsigned char *pSrcB, *pDstB;

	assert (pSrc && pSrc->imageData &&
			pDst && pDst->imageData);
	assert (!strncmp (pSrc->colorModel, "RGBA", 4) &&
			!strncmp (pSrc->channelSeq, "BGRA", 4) &&
			!strncmp (pDst->colorModel, "GRAY", 4) &&
			!strncmp (pDst->channelSeq, "GRAY", 4) );

	// Límits on s'ha d'aplicar la funció
	crvGetROILimits (pSrc, xIni, yIni, xLim, yLim);
	
	// Aplica tractament a la ROI
	for (y= yIni; y< yLim; y++) {
		// Apuntador inici fila component R
		pSrcB= (unsigned char *) crvImgOffset (pSrc, xIni, y);
		pDstB= (unsigned char *) crvImgOffset (pDst, xIni, y);
		for (x= xIni; x< xLim; x++) {
			*pDstB= m_ColorLookup[pSrcB[2]][pSrcB[1]][pSrcB[0]];
			//pDstB[1]= pDstB[0];
			//pDstB[2]= pDstB[0];
			//pDstB[3]= pSrcB[3];

			pSrcB+= 4;
			pDstB++;
		}
	}
}

// Genera constants model de color a partir de la ROI
// d'una imatge RGBA
void crvExtractSkinColorModel (IplImage *pIpl, float &krg_min, float &krg_max,
							   float &krb_min, float &krb_max)
{
	int x, y, xIni, yIni, xLim, yLim;
	float krg, krb;
	unsigned char *pSrc;

	assert (pIpl);

	// Inicialitzacions
	krg_min= 9999999999.0f;
	krb_min= 9999999999.0f;
	krg_max= -9999999999.0f;
	krb_max= -9999999999.0f;

	// Límits on s'ha d'aplicar la funció
	crvGetROILimits (pIpl, xIni, yIni, xLim, yLim);
	
	// Aplica tractament a la ROI
	for (y= yIni; y< yLim; y++) {
		// Apuntador inici fila component R
		pSrc= (unsigned char *) crvImgOffset (pIpl, xIni, y);		
		for (x= xIni; x< xLim; x++) {
			krg= (float) pSrc[0] / (float) pSrc[1];
			krb= (float) pSrc[0] / (float) pSrc[2];
			pSrc+= 4;

			if (krg< krg_min) krg_min= krg;
			if (krg> krg_max) krg_max= krg;
			if (krb< krb_min) krb_min= krb;
			if (krb> krb_max) krb_max= krb;
		}
	}
}



// Converteix una imatge a color a escala de grisos
void crvColorToGray (const IplImage *pImgSrc, IplImage *pImgDst)
{
	int x, y, xIni, yIni, xLim, yLim;
	int xDstIni, yDstIni, xDstLim, yDstLim;
	unsigned char *pSrcPix, *pDstPix;
	unsigned int sum;

	assert (pImgSrc && pImgDst && pImgSrc!= pImgDst && 
			pImgSrc->imageData!= pImgDst->imageData);
	assert (pImgSrc->depth== IPL_DEPTH_8U && 
			pImgDst->depth== IPL_DEPTH_8U);
	assert (((!strncmp (pImgSrc->channelSeq, "BGRA", 4) && pImgSrc->nChannels== 4) ||
			 (!strncmp (pImgSrc->channelSeq, "BGR", 4) && pImgSrc->nChannels== 3) ||
			 (!strncmp (pImgSrc->channelSeq, "RGB", 4) && pImgSrc->nChannels== 3)) &&
			  !strncmp (pImgDst->channelSeq, "GRAY", 4) );

	// PROV: les ROI han de ser indèntiques
	crvGetROILimits (pImgSrc, xIni, yIni, xLim, yLim);
	crvGetROILimits (pImgDst, xDstIni, yDstIni, xDstLim, yDstLim);

	assert (xIni== xDstIni && yIni== yDstIni &&
			xLim== xDstLim && yLim== yDstLim);

	if (pImgSrc->nChannels== 3)
	{
		// 3 channels
		for (y= yIni; y< yLim; y++) {
			pSrcPix= (unsigned char *) crvImgOffset (pImgSrc, xIni, y);
			pDstPix= (unsigned char *) crvImgOffset (pImgDst, xIni, y);
			for (x= xIni; x< xLim; x++) {
				sum= *pSrcPix; pSrcPix++;	// B
				sum+= *pSrcPix; pSrcPix++;	// G
				sum+= *pSrcPix; pSrcPix++;	// R
				*pDstPix= (unsigned char) (sum / 3);
				pDstPix++;
			}
		}
	}
	else
	{
		// 4 chanels
		for (y= yIni; y< yLim; y++) {
			pSrcPix= (unsigned char *) crvImgOffset (pImgSrc, xIni, y);
			pDstPix= (unsigned char *) crvImgOffset (pImgDst, xIni, y);
			for (x= xIni; x< xLim; x++) {
				sum= *pSrcPix; pSrcPix++;	// B
				sum+= *pSrcPix; pSrcPix++;	// G
				sum+= *pSrcPix; pSrcPix++;	// R
				pSrcPix++;					// A
				*pDstPix= (unsigned char) (sum / 3);
				pDstPix++;
			}
		}
	}
}
