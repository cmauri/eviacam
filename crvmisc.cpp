/////////////////////////////////////////////////////////////////////////////
// Name:        crvmisc.cpp
// Purpose:  
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by: 
// Created:     23/05/2008
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
#include <float.h>
#include <assert.h>
#include <math.h>

// uintptr_t type
#if defined(WIN32)
#include <stddef.h>
#else
#include <stdint.h>
#endif


// Dona l'adre�a de mem�ria del p�xel a la posici� X, Y (DEPRECAT)
char *crvImgByteOffset (const IplImage *img, int x, int y)
{
	assert (img->nChannels== 1);

	switch (img->depth) {
	case IPL_DEPTH_8U:
		return (char *)((uintptr_t) img->imageData + x + (y * img->widthStep));
	case IPL_DEPTH_16U:
		return (char *)((uintptr_t) img->imageData + x * 2 + (y * img->widthStep));
	default: assert (0);
	}

	return NULL;	// Evitar Warning
}

// Dona l'adre�a de mem�ria del p�xel a la posici� X, Y
void *crvImgOffset (const IplImage *img, int x, int y)
{
	switch (img->depth) {
	case IPL_DEPTH_8U:
	case IPL_DEPTH_8S:
		return (void *)((uintptr_t) img->imageData + x * img->nChannels + (y * img->widthStep));
	case IPL_DEPTH_16U:
	case IPL_DEPTH_16S:
		return (void *)((uintptr_t) img->imageData + x * 2 + (y * img->widthStep));
	case IPL_DEPTH_32S:
	case IPL_DEPTH_32F:
		return (void *)((uintptr_t) img->imageData + x * 4 + (y * img->widthStep));
	default: assert (0);
	}

	return NULL;	// Evitar Warning
}

// Calcula limits en X, Y segons el ROI d'una imatge
void crvGetROILimits (const IplImage *img, int &xIni, int &yIni, int &xLim, int &yLim)
{
	if (img->roi== NULL) {
		// No s'ha indicat regi� d'inter�s -> tota la imatge
		xIni= 0; yIni= 0; xLim= img->width; yLim= img->height;
	}
	else {
		// S'ha indicat regi� d'inter�s
		xIni= img->roi->xOffset;
		yIni= img->roi->yOffset;
		xLim= xIni + img->roi->width;
		yLim= yIni + img->roi->height;

		// Trunca dins els l�mits reals de la imatge
		if (xIni< 0) xIni= 0;
		if (yIni< 0) yIni= 0;
		if (xLim> img->width) xLim= img->width;
		if (yLim> img->height) yLim= img->height;

		//assert (xLim<= img->width && yLim<= img->height);
	}
}

// Dona la intersecci� entre les ROI de 2 imatges
void crvGetROIIntersectionLimits (IplImage *img1, IplImage *img2,
								  int &xIni1, int &yIni1, int &xLim1, int &yLim1,
								  int &xIni2, int &yIni2, int &xLim2, int &yLim2)
{
	int width, height;

	if (img1->roi== NULL) {
		xIni1= 0;
		yIni1= 0;
		width= img1->width;
		height= img1->height;
	}
	else {
		xIni1= img1->roi->xOffset;
		yIni1= img1->roi->yOffset;
		width= img1->roi->width;
		height= img1->roi->height;
	}

	if (img2->roi== NULL) {
		xIni2= 0;
		yIni2= 0;
		width= (width< img2->width ? width : img2->width);
		height= (height< img2->height ? height : img2->height);
	}
	else {
		xIni2= img2->roi->xOffset;
		yIni2= img2->roi->yOffset;
		width= (width< img2->roi->width ? width : img2->roi->width);
		height= (height< img2->roi->height ? height : img2->roi->height);
	}

	// Limits
	xLim1= xIni1 + width;
	yLim1= yIni1 + height;
	
	xLim2= xIni2 + width;
	yLim2= yIni2 + height;
}



// Pinta en ASCII una imatge binaria
/*
void crvAsciiPaint (IplImage *img, unsigned short thres)
{
	int x, y;
	unsigned short pix;

	for (y= 0; y< img->height; y++) {
		for (x= 0; x< img->width; x++) {
			pix= 0;
			iplGetPixel (img, x, y, &pix);
			printf ("%c", (pix< thres ? '.' : '#'));
		}
		printf ("\n");
	}
}*/

// Copia una ROI d'una imatge sobre una nova imatge aplicant
// l'escalat que s'indica per par�metre. La ROI de la imatge
// dest� ha de ser suficientment gran com per encabir
// la ROI origen un cop escalada.
/*
void crvCopyScaleImage (IplImage *src, IplImage *dst, float scale)
{
	int xDst, xSrc, yDst, ySrc;

	assert (src && dst && scale> 0.0f); 
	//assert (src->roi!= NULL && dst->roi!= NULL);

	// Redimensiona imatge
	if (scale== 1.0) {
		xSrc= ySrc= xDst= yDst= 1;
	}
	else {
		xSrc= 10000;
		ySrc= xSrc;
		xDst= (int) ((float) xSrc * scale);
		yDst= xDst;
	}
	
	iplResize(src, dst, xDst, xSrc, yDst, ySrc, IPL_INTER_CUBIC);
}*/



// Semi-binaritza una imatge de manera que per sobre o per sota
// del llindar indicat no es modifiquen els valors dels p�xels
/*
void crvSemiBinarize (IplImage *img, unsigned char thres, unsigned char fillVal, bool fillLower)
{
	int xIni, yIni, xLim, yLim, x, y;
	unsigned char *pBuff, min= 255;

	assert (img->depth== IPL_DEPTH_8U);

	crvGetROILimits (img, xIni, yIni, xLim, yLim);

	if (fillLower) {
		for (y= yIni; y< yLim; y++) {
			pBuff= (unsigned char *) crvImgByteOffset (img, xIni, y);
			for (x= xIni; x< xLim; x++) {
				if (*pBuff< thres) *pBuff= fillVal;
				pBuff++;
			}
		}
	}
	else {
		for (y= yIni; y< yLim; y++) {
			pBuff= (unsigned char *) crvImgByteOffset (img, xIni, y);
			for (x= xIni; x< xLim; x++) {
				if (*pBuff>= thres) *pBuff= fillVal;
				//else *pBuff= MapRange (0, thres, 0, 40, *pBuff);
				//if (*pBuff< 255 && *pBuff> 35) *pBuff= 35;
				pBuff++;
			}
		}
	}
}
*/

// Calcula la diferencia de 2 imatges en valor absolut
void crvAbsSubstract (IplImage *srcA, IplImage *srcB, IplImage *dst)
{
	int x, y, xIni, yIni, xLim, yLim;
	unsigned char *pBuffA, *pBuffB, *pBuffRes;
	
	assert (srcA->depth== IPL_DEPTH_8U && 
			srcB->depth== IPL_DEPTH_8U &&
			dst->depth== IPL_DEPTH_8U);
	assert (srcA->width== srcB->width &&
			srcA->width== dst->width &&
			srcA->height== srcB->height &&
			srcA->height== dst->height);
	assert (!srcA->roi && !srcB->roi && !dst->roi);

	pBuffA= (unsigned char *) srcA->imageData;
	pBuffB= (unsigned char *) srcB->imageData;
	pBuffRes= (unsigned char *) dst->imageData;

	crvGetROILimits (srcA, xIni, yIni, xLim, yLim);

	for (y= yIni; y< yLim; y++) {
		pBuffA= (unsigned char *) crvImgByteOffset (srcA, xIni, y);
		pBuffB= (unsigned char *) crvImgByteOffset (srcB, xIni, y);
		pBuffRes= (unsigned char *) crvImgByteOffset (dst, xIni, y);
		for (x= xIni; x< xLim; x++) {
			*pBuffRes = (unsigned char) abs ((int) *pBuffA - (int) *pBuffB);
			pBuffA++;
			pBuffB++;
			pBuffRes++;
		}
	}
}

void crvConvert16BinToRGBA (IplImage *pSrcImg, IplImage *pDstImg)
{
	int i;	
	unsigned short *pWordSrc;
	unsigned char *pByteDst;

	assert (pSrcImg && pDstImg);

	pWordSrc= (unsigned short *) pSrcImg->imageData;
	pByteDst= (unsigned char *) pDstImg->imageData;
	int bytesPerPixel= pDstImg->depth / 8;

	if (bytesPerPixel== 4)
	{
		for (i= 0; i< pDstImg->imageSize; i+= 4) {
			if (*pWordSrc) {
				*pByteDst++= 255;
				*pByteDst++= 255;
				*pByteDst++= 255;
				pByteDst++;
			}
			else {
				*pByteDst++= 0;
				*pByteDst++= 0;
				*pByteDst++= 0;
				pByteDst++;
			}
			pWordSrc++;	
		}
	}
	else
	{
		for (i= 0; i< pDstImg->imageSize; i+= 3) {
			if (*pWordSrc) {
				*pByteDst++= 255;
				*pByteDst++= 255;
				*pByteDst++= 255;				
			}
			else {
				*pByteDst++= 0;
				*pByteDst++= 0;
				*pByteDst++= 0;				
			}
			pWordSrc++;	
		}
	}
}

// Ajusta un rectangle per que quedi dins uns limits establerts
// Retorna true si s'ha modificat el rectangle
// mode:
bool crvFitRect (CvRect *rect, int width, int height)
{
	bool modif= false;

	assert (rect->x<= width);
	assert (rect->y<= height);

	if (rect->x < 0) {
		rect->width+= rect->x;
		rect->x= 0;
		assert (rect->width> 0);
		modif= true;
	}

	if (rect->x + rect->width> width) {
		rect->width= width - rect->x;
		modif= true;
	}

	if (rect->y < 0) {
		rect->height+= rect->y;
		rect->y= 0;
		assert (rect->height> 0);
		modif= true;
	}

	if (rect->y + rect->height> height) {
		rect->height= height - rect->y;
		modif= true;
	}

	assert ((rect->x + width)>= rect->width);
	assert ((rect->y + height)>= rect->height);

	return (modif);
}

bool crvFitROI (IplImage *img, IplROI *roi)
{
	CvRect rect;
	bool retval;

	rect.x= roi->xOffset;
	rect.y= roi->yOffset;
	rect.width= roi->width;
	rect.height= roi->height;

	retval= crvFitRect (&rect, img->width, img->height);

	roi->xOffset= rect.x;
	roi->yOffset= rect.y;
	roi->width= rect.width;
	roi->height= rect.height;

	return retval;
}



// Converteix una imatge 
void crvConvert32FPToGray (IplImage *pSrcImg, IplImage *pDstImg,
						   float min, float max)
{
	float *pOrig, factor;
	unsigned char *pDst;
	int x, y, xIni, yIni, xLim, yLim;
	
	assert (pSrcImg->depth== IPL_DEPTH_32F);
	assert (pDstImg->depth== IPL_DEPTH_8U);
	assert (pSrcImg->width== pDstImg->width &&
			pSrcImg->height== pDstImg->height);

	factor= 255.0f / (float)(max - min);

	crvGetROILimits (pSrcImg, xIni, yIni, xLim, yLim);

	for (y= yIni; y< yLim; y++) {
		pOrig= (float *) crvImgOffset (pSrcImg, xIni, y);
		pDst= (unsigned char *) crvImgOffset (pDstImg, xIni, y);
		for (x= xIni; x< xLim; x++) {
			*pDst= (unsigned char) ((*pOrig - min) * factor);
			pOrig++; pDst++;
		}
	}
}

void crvThresholdFP (IplImage *pSrcImg, IplImage *pDstImg, 
					 float threshold, float zero)
{
	int y, x, xIni, yIni, xLim, yLim;
	float *pSrc, *pDst;

	assert (pSrcImg && pDstImg);
	assert (pSrcImg->depth== IPL_DEPTH_32F && 
			pDstImg->depth== IPL_DEPTH_32F);
	assert (pSrcImg->width== pDstImg->width &&
			pSrcImg->height== pDstImg->height);

	crvGetROILimits (pSrcImg, xIni, yIni, xLim, yLim);

	for (y= yIni; y< yLim; y++) {
		pSrc= (float *) crvImgOffset (pSrcImg, xIni, y);
		pDst= (float *) crvImgOffset (pDstImg, xIni, y);
		for (x= xIni; x< xLim; x++) {
			if (*pSrc>= threshold) { *pDst= *pSrc; }
			else { *pDst= zero; }
			pSrc++; pDst++;
		}
	}
}

// Calcula el m�dul del gradient d'una imatge
// Veure: iplConvolve2D
/*
void crvGradientModulus (IplImage *pSrcImg, IplImage *pDstImg)
{

	cvSobel (pSrcImg, pDstImg, 1, 1);

	
	int nShiftR;
	IplConvKernel *kernels[2];

	kernels[0]= iplCreateConvKernel(3, 3, 1, 1, 
								{ -1, 0, 1, -2, 0, 2, -1, 0, 1 }, 
								&nShiftR);
	assert (kernels[0]);
	kernels[1]= iplCreateConvKernel(3, 3, 1, 1, 
								{ 1, 2, 1, 0, 0, 0, -1, -2, -1 }, 
								&nShiftR);
	assert (kernels[1]);

	iplConvolve2D(pSrcImage, pDstImage, &kernels, 2, IPL_SUMSQROOT);

	DeleteConvKernel (kernels[0]);
	DeleteConvKernel (kernels[1]);
	
}
*/

/*
void crvGradientModulus (IplImage *pSrcImg, IplImage *pDstImg, 
						 IplImage *pTmpImg1, int *maxX, int *maxY)
{
	int retval;
	unsigned char *pVVal, *pHVal, *pDst, max= 0;
	int x, y, xIni, yIni, xLim, yLim;

	retval= iplFixedFilter (pSrcImg, pDstImg, IPL_SOBEL_3x3_V);
	assert (retval== 0);
	retval= iplFixedFilter (pSrcImg, pTmpImg1, IPL_SOBEL_3x3_H);
	assert (retval== 0);


	crvGetROILimits (pSrcImg, xIni, yIni, xLim, yLim);

	*maxX= 0;
	*maxY= 0;


	for (y= yIni; y< yLim; y++) {
		pVVal= (unsigned char *) crvImgOffset (pDstImg, xIni, y);
		pHVal= (unsigned char *) crvImgOffset (pTmpImg1, xIni, y);
		pDst= (unsigned char *) crvImgOffset (pDstImg, xIni, y);

		for (x= xIni; x< xLim; x++) {
			*pDst= (unsigned char) (((int) *pVVal + (int) *pHVal) / 2);
			
			if (*pDst> max) {
				max= *pDst;
				*maxX= x;
				*maxY= y;
			}
			//val= ((int) *pVVal) * ((int) *pVVal) + ((int) *pHVal) * ((int) *pHVal);
			//val= (int) (sqrt (val) / 362.1f);
			// *pDst= val;

			pVVal++;
			pHVal++;
			pDst++;
		}
	}
}
*/

