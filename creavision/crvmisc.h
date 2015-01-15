/////////////////////////////////////////////////////////////////////////////
// Name:        crvmisc.h
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
#ifndef CRVMISC_H
#define CRVMISC_H

#include <cv.h>

/*
typedef struct {
	double x, y;
} RPoint;
*/

// Dona l'adre�a de mem�ria del p�xel a la posici� X, Y
void *crvImgOffset (const IplImage *img, int x, int y);

// Dona l'adre�a de mem�ria del p�xel a la posici� X, Y (DEPRECAT)
char *crvImgByteOffset (const IplImage *img, int x, int y);

// Calcula limits en X, Y segons el ROI d'una imatge
void crvGetROILimits (const IplImage *img, int &xIni, int &yIni, 
				   int &xLim, int &yLim);

// Dona la intersecci� entre les ROI de 2 imatges
void crvGetROIIntersectionLimits (IplImage* img1, IplImage* img2,
								  int &xIni1, int &yIni1, int &xLim1, int &yLim1,
								  int &xIni2, int &yIni2, int &xLim2, int &yLim2);


// Ajusta un rectangle per que quedi dins uns limits establerts
// Retorna true si s'ha modificat el rectangle
bool crvFitRect (CvRect *rect, int width, int height);
bool crvFitROI (IplImage *img, IplROI *roi);

// Pinta en ASCII una imatge binaria
//void IplAsciiPaint (IplImage *img, unsigned short thres);

// Copia una ROI d'una imatge sobre una nova imatge aplicant
// l'escalat que s'indica per par�metre. La ROI de la imatge
// dest� ha de ser suficientment gran com per encabir
// la ROI origen un cop escalada.
//void IplCopyScaleImage (IplImage *src, IplImage *dst, float scale);

// Semi-binaritza una imatge de manera que per sobre o per sota
// del llindar indicat no es modifiquen els valors dels p�xels
//void crvSemiBinarize (IplImage *img, unsigned char thres, unsigned char fillVal, bool fillLower);

// Calcula la diferencia de 2 imatges en valor absolut
void crvAbsSubstract (IplImage *srcA, IplImage *srcB, IplImage *dst);

void crvConvert16BinToRGBA (IplImage *pSrcImg, IplImage *pDstImg);

static inline
void crvInvalidateCvPoint (CvPoint *p) 
{
	p->x= -1;
}

static inline 
int crvCvPointIsValid (CvPoint *p)
{
	return (p->x> -1);
}

static inline
void crvInvalidateCvPoint2D32f (CvPoint2D32f *p) 
{
	p->x= 999999999.0f;
}

static inline 
int crvCvPoint2D32fIsValid (CvPoint2D32f *p)
{
	return (p->x<= 999999998.0f);
}

static inline
void crvInvalidateCvRect (CvRect *p) 
{
	p->x= -1;
}

static inline 
int crvCvRectIsValid (CvRect *p)
{
	return (p->x> -1);
}



void crvConvert32FPToGray (IplImage *pSrcImg, IplImage *pDstImg,
						   float min, float max);

void crvThresholdFP (IplImage *pSrcImg, IplImage *pDstImg, 
					 float threshold, float zero= 0.0f);


//void IplGradientModulus (IplImage *pSrcImg, IplImage *pDstImg);

//void IplGradientModulus (IplImage *pSrcImg, IplImage *pDstImg, 
//						 IplImage *pTmpImg1, int *maxX, int *maxY);



#endif
