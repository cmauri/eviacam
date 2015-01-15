/////////////////////////////////////////////////////////////////////////////
// Name:        crvskindetection.cpp
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
#include "crvmisc.h"
#include "crvskindetection.h"
#include <assert.h>
#include <string.h>

void CSkinRegionModel::crvBinarizeSkin_KToyama (IplImage *pIpl, float &krg_min, 
												  float &krg_max, float &krb_min, 
												  float &krb_max)
{
	int x, y, xIni, yIni, xLim, yLim;
	float krg, krb;
	unsigned char *pSrc;

	assert (pIpl);

	crvGetROILimits (pIpl, xIni, yIni, xLim, yLim);
	
	for (y= yIni; y< yLim; y++) {
		pSrc= (unsigned char *) crvImgOffset (pIpl, xIni, y);		
		for (x= xIni; x< xLim; x++) {
			krg= (float) pSrc[0] / (float) pSrc[1];
			krb= (float) pSrc[0] / (float) pSrc[2];

			if (krg_min<= krg && krg_max>= krg &&
				krb_min<= krb && krb_max>= krb) {
				pSrc[0]= pSrc[1]= pSrc[2]= 255;
			}
			else 
				pSrc[0]= pSrc[1]= pSrc[2]= 0;

			pSrc+= 4;
		}
	}
}

// 
// Skin Models Definition
//
#define min(x,y) (x< y? x : y)
#define max(x,y) (x> y? x : y)
#define min3(x,y,z) (min(min(x,y),z))
#define max3(x,y,z) (max(max(x,y),z))


// The skin colour at uniform daylight illumination
#define IS_SKIN_PEER(R,G,B)		(R> 95 && G> 40 && B> 20 && R> G && R> B && \
								(max3(R,G,B) - min3(R,G,B))> 15	&& \
								((R-G)> 15 || (G-R)> 15))

// The skin colour under flashlight or (light) daylight
// lateral illumination
#define IS_SKIN_PEER2(R,G,B)	(R> 220 && G> 210 && B> 170 && \
								((R-G)< 15 || (G-R)< 15) && \
								R> B && G> B)
								
								

inline bool IsSkin_GomezMorales_1 (int R, int G, int B)
{
	float r, g, b, rgb, rgb2;

	r= (float) R / 255.0f;	g= (float) G / 255.0f;	b= (float) B / 255.0f;
	rgb= r + g + b;
	rgb2= rgb * rgb;

	return ( ((r/g)> 1.185) && (((r*b)/rgb2)> 0.107) && (((r*g)/rgb2)> 0.112) );
}

inline bool IsSkin_GomezMorales_2 (int R, int G, int B)
{
	float r, g, b, rgb;

	r= (float) R / 255.0f;	g= (float) G / 255.0f;	b= (float) B / 255.0f;
	rgb= r + g + b;
	
	return ( (b/g)< 1.249 && (rgb / (3*r))> 0.696 && 
			 ((1.0/3.0) - (b/rgb))> 0.014 && (g / (3*rgb))< 0.108 );
}

void CSkinRegionModel::crvBinarizeSkin  (IplImage *pSrc, IplImage *pDst)
{
	int x, y, xIni, yIni, xLim, yLim, srcBytesPPixel, dstBytesPPixel;
	unsigned char *pSrcB, *pDstB;
	bool isBGR= false;
	
	assert (pSrc && pSrc->imageData && pDst && pDst->imageData);
	if (!strncmp (pSrc->channelSeq, "BGR", 4) || !strncmp (pSrc->channelSeq, "BGRA", 4))
		isBGR= true;
	else if (!strncmp (pSrc->channelSeq, "RGB", 4) || !strncmp (pSrc->channelSeq, "RGBA", 4))
		isBGR= false;
	else
		assert (false);
	assert (!strncmp (pDst->channelSeq, "GRAY", 4) || !strncmp (pDst->channelSeq, "BGRA", 4));

	// Get depth
	srcBytesPPixel= pSrc->nChannels;
	dstBytesPPixel= pDst->depth / 8;
	
	cvSetZero(pDst);
	
	crvGetROILimits (pSrc, xIni, yIni, xLim, yLim);

	if (isBGR)
		for (y= yIni; y< yLim; y++) {
			pSrcB= (unsigned char *) crvImgOffset (pSrc, xIni, y);
			pDstB= (unsigned char *) crvImgOffset (pDst, xIni, y);

			for (x= xIni; x< xLim; x++) {

				if (IS_SKIN_PEER(pSrcB[2],pSrcB[1],pSrcB[0]))			
				{
					// Color acceptat
					pDstB[0]= 255;
					if (dstBytesPPixel> 1)
					{
						pDstB[1]= 255;
						if (dstBytesPPixel> 2)
						{
							pDstB[2]= 255;
						}
					}
				}			

				pSrcB+= srcBytesPPixel;
				pDstB+= dstBytesPPixel;
			}
		}
	else
		for (y= yIni; y< yLim; y++) {
			pSrcB= (unsigned char *) crvImgOffset (pSrc, xIni, y);
			pDstB= (unsigned char *) crvImgOffset (pDst, xIni, y);

			for (x= xIni; x< xLim; x++) {

				if (IS_SKIN_PEER(pSrcB[0],pSrcB[1],pSrcB[2]))			
				{
					// Color acceptat
					pDstB[0]= 255;
					if (dstBytesPPixel> 1)
					{
						pDstB[1]= 255;
						if (dstBytesPPixel> 2)
						{
							pDstB[2]= 255;
						}
					}
				}			

				pSrcB+= srcBytesPPixel;
				pDstB+= dstBytesPPixel;
			}
		}
}
