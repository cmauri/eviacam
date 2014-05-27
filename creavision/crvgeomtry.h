/////////////////////////////////////////////////////////////////////////////
// Name:        crvgeomtry.h
// Purpose:  
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by: 
// Created:     22/02/2008
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

#ifndef CRVGEOMTRY_H
#define CRVGEOMTRY_H

typedef double Real;

#define INF_M  999999999    /* valor pendent 'infinita' */
#define PI	   3.1415926535897932384626433832795
#ifndef M_PI
#define M_PI	PI
#endif

/* TIPUS */

typedef struct tRPoint {
    Real x, y;
} tRPoint;

typedef struct t3DRPoint {
    Real x, y, z;
} t3DRPoint;

/* equació recta en forma y = mx + n */
typedef struct tRect{
    Real m, n;
} tRect;


/* FUNCIONS */

/* copia d'un punt */
static inline
void cp_point (tRPoint *dst, tRPoint *src)
{
    dst->x= src->x;
    dst->y= src->y;
}

/* distància entre 2 punts */
Real points_distance (tRPoint *p1, tRPoint *p2);

/* distància entre 2 punts 3D */
Real points3d_distance (t3DRPoint *p1, t3DRPoint * p2);

/* equació de la recta que passa per dos punts */
void points2rect (tRPoint *p1, tRPoint *p2, tRect *r);

/* distància d'un punt a una recta */
Real point_rect_distance (tRect *r, tRPoint *p);

/* calcula l'angle del vector que va de p1 a p2 */
Real segment_angle (tRPoint *p1, tRPoint *p2);

/* donada una recta diu si un punt es troba sobre la recta (0)
   o a la banda positiva (1) o negativa de la recta (-1) */
int point_rect_where (tRect *r, tRPoint *p, Real error);

/* donat un segment retorna si un determinat punt es troba a la dreta (1),
sobre el segment (0) o a l'esquerra (-1). La posició es mira respecte
la direcció del vector entre p1 i p2 */
int point_segment_where (tRPoint *p1, tRPoint *p2, tRPoint *p, Real error);

/* calcula la recta perpendicular a una donada que passa per un punt */
void perpendicular_rect (tRect *r, tRPoint *p, tRect *perp);

/* troba el punt mig d'un segment */
void segment_mean_point (tRPoint *p1, tRPoint *p2, tRPoint *mp);

/* troba el punt de tall entre dues rectes. retorna 0 si no hi ha
   punt de tall, 1 altrament */
int rects_cutting_point (tRect *r1, tRect *r2, tRPoint *p);

/* calcula el centre d'una circumferència a partir de 3 punts del perímetre.
   retorna 0 si no es possible determinar el center, 1 si tot correcte */
int circle_center (tRPoint  *p1, tRPoint *p2, tRPoint *p3, tRPoint *center);

/* donat un punt el rota l'angle indicat respecte del centre */
void point_rotate (tRPoint *p, tRPoint *center, Real ang);

/* escalat d'un segment. es modifica el punt *target per tal que
   el segment que va de *p a *target tingui la nova distància indicada  */
void scale_segment (tRPoint *target, tRPoint *p, Real real_dist);

/* reescala un segment modificant la posició dels 2 extrems per a
   que tingui la mida desitjada */
void rescale_segment (tRPoint *p1, tRPoint *p2, Real real_dist);

/* donada una llista de punts que formen un polígon tancat
   en calcula la seva caixa englobant */
void calculate_englobing_box (tRPoint *lvertexs, int nvertexs,
                              tRPoint *min_box, tRPoint *max_box);

/* comprova si dos caixes englobants intersecten */
static inline
int englobing_box_instersection (tRPoint *min1, tRPoint *max1,
                                 tRPoint *min2, tRPoint *max2)
{
    return (!(min1->x> max2->x || min1->y> max2->y ||
              min2->x> max1->x || min2->y> max2->y )
           );             
}

/* diu si dos segments es tallen en algun punt */
int segments_cutting (tRPoint *s1p1, tRPoint *s1p2, tRPoint *s2p1, tRPoint *s2p2);

/* comprova si dos polígons tancats tenen alguna àrea solapada. 
   NOTA: no es comprova si un polígon és interior a un altre */
int polys_intersect (tRPoint *v1, int lv1, tRPoint *v2, int lv2);

#endif
