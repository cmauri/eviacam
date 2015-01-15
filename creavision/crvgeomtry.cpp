/////////////////////////////////////////////////////////////////////////////
// Name:        crvgeomtry.cpp
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

#include "crvgeomtry.h"
#include <math.h>

/* troba el punt mig d'un segment */
inline
void segment_mean_point (tRPoint *p1, tRPoint *p2, tRPoint *mp)
{
    mp->x= (p1->x + p2->x) / 2.0;
    mp->y= (p1->y + p2->y) / 2.0;
}

/* distancia entre dos punts */
inline
Real points_distance (tRPoint *p1, tRPoint *p2)
{
    Real xd= p2->x - p1->x, yd= p2->y - p1->y;

    return sqrt(xd*xd + yd*yd);
}

/* distància entre 2 punts 3D */
Real points3d_distance (t3DRPoint *p1, t3DRPoint * p2)
{
	Real xd, yd, zd;

	xd= p1->x - p2->x;
	yd= p1->y - p2->y;
	zd= p1->z - p2->z;

	return sqrt (xd*xd + yd*yd + zd*zd);
}

/* equació de la recta que passa per dos punts  y= mx + n */
void points2rect (tRPoint *p1, tRPoint *p2, tRect *r)
{
    Real v1, v2;

    v1= p2->y - p1->y;
    v2= p2->x - p1->x;
    if (v2!= 0.0) {
        r->m= v1 / v2;
        r->n= p1->y - (r->m * p1->x);
    }
    else {
        r->m= INF_M;
        r->n= p1->x;
    }
}

/* distància d'una recta a un punt */
Real point_rect_distance (tRect *r, tRPoint *p)
{
    tRect nr;
    tRPoint np;

    if (r->m>= INF_M) return fabs(r->n - p->x);     /* pendent infinita */
    if (r->m== 0.0) return fabs(p->y - r->n);       /* pendent 0 */

    /* perpendicular a r que passa per p */
    nr.m = -1.0 / r->m;
    nr.n = p->y - (nr.m * p->x);

    /* punt de tall */
    np.x = (r->n - nr.n) / (nr.m - r->m);
    np.y = r->m * np.x + r->n;

    return points_distance (p, &np);
}

/* calcula l'angle del vector que va de p1 a p2 */
Real segment_angle (tRPoint *p1, tRPoint *p2)
{
    Real dx, dy;

    dx= p2->x - p1->x;
    dy= p2->y - p1->y;

    if (dx== 0.0) {
        if (dy>= 0.0) return (M_PI / 2.0);
        else return (3.0 * M_PI / 2.0);
    }
    if (dx> 0.0) {
        if (dy< 0.0) return (2.0 * M_PI) + atan(dy / dx);
        else return atan(dy / dx);
    }
    else return atan(dy / dx) + M_PI;

//    return 0.0;     /* evitar warning */
}

/* donada una recta diu si un punt es troba sobre la recta (0)
   o a la banda positiva (1) o negativa de la recta (-1) */
int point_rect_where (tRect *r, tRPoint *p, Real error)
{
    Real result;

    /* si la recta es vertical només cal mirar la coordenada X */
    if (r->m>= INF_M) result= p->x - r->n;
    /* sinó s'ha de substituir el punt a l'equació de la recta
       i avaluar-ne el resultat */
    else result= r->m * p->x + r->n - p->y;

    if (fabs(result)<= error) return 0;
    else if (result> 0.0) return 1;
    else return -1;
}

/* donat un segment retorna si un determinat punt es troba a la dreta (1),
sobre el segment (0) o a l'esquerra (-1). La posició es mira respecte
la direcció del vector entre p1 i p2 */
int point_segment_where (tRPoint *p1, tRPoint *p2, tRPoint *p, Real error)
{
	Real vx= p2->x - p1->x, vy= p2->y - p1->y;
	
	Real ang= atan2 (vx, vy); 

	tRect r;
	points2rect (p1, p2, &r);
	int rectWhere= point_rect_where (&r, p, error);

	if (r.m>= INF_M) 
	{
		if (vy<= 0) return rectWhere;
		else return -rectWhere;
	}

	if (rectWhere== 0) return 0;
	if (ang> 0) return -rectWhere;
	else return rectWhere;	
}

/* calcula la recta perpendicular a una donada que passa per un punt */
void perpendicular_rect (tRect *r, tRPoint *p, tRect *perp)
{
    if (r->m>= INF_M) {
        /* pendent infinita */
        perp->m= 0.0;
        perp->n= p->y;
    }
    else if (r->m== 0.0) {
        /* pendent 0 */
        perp->m= INF_M;
        perp->n= p->x;
    }
    else {
        /* perpendicular a r que passa per p */
        perp->m = -1.0 / r->m;
        perp->n = p->y - (perp->m * p->x);
    }
}

/* troba el punt de tall entre dues rectes. retorna 0 si no hi ha
   punt de tall, 1 altrament */
int rects_cutting_point (tRect *r1, tRect *r2, tRPoint *p)
{
    if (r1->m== r2->m) return 0;
    if (r1->m>= INF_M && r2->m>= INF_M) return 0;

    if (r1->m>= INF_M) {
        p->x= r1->n;
        p->y = r2->m * p->x + r2->n;
        return 1;
    }
    if (r2->m>= INF_M) {
        p->x= r2->n;
        p->y = r1->m * p->x + r1->n;
        return 1;
    }

    p->x = (r1->n - r2->n) / (r2->m - r1->m);
    p->y = r1->m * p->x + r1->n;

    return 1;
}

/* determina si un punt que pertany a la recta que passa per un
   segment es troba dins els límits del segment */
static inline
int point_rect_on_segment (tRPoint *p1, tRPoint *p2, tRPoint *ptest)
{
    tRPoint smin, smax;

    /* caixa englobant del segment */
    if (p1->x< p2->x) {
        smin.x= p1->x;
        smax.x= p2->x;
    }
    else {
        smin.x= p2->x;
        smax.x= p1->x;
    }
    if (p1->y< p2->y) {
        smin.y= p1->y;
        smax.y= p2->y;
    }
    else {
        smin.y= p2->y;
        smax.y= p1->y;
    }

    if (ptest->x> smax.x || ptest->x< smin.x ||
        ptest->y> smax.y || ptest->y< smin.y)  return 0;

    return 1;
}

/* diu si dos segments es tallen en algun punt */
int segments_cutting (tRPoint *s1p1, tRPoint *s1p2, tRPoint *s2p1, tRPoint *s2p2)
{
    tRect r1, r2;
    tRPoint pcut;

    /* obté equacions de les rectes dels segments */
    points2rect (s1p1, s1p2, &r1);
    points2rect (s2p1, s2p2, &r2);

    /* punt de tall de les dues rectes */
    if (rects_cutting_point (&r1, &r2, &pcut)== 0) return 0;

    /* comprova si el punt de tall pertany als dos segments */
    return (point_rect_on_segment (s1p1, s1p2, &pcut) &&
            point_rect_on_segment (s2p1, s2p2, &pcut) );
}

/* calcula el centre d'una circumferència a partir de 3 punts del perímetre.
   retorna 0 si no es possible determinar el center, 1 si tot correcte */
int circle_center (tRPoint  *p1, tRPoint *p2, tRPoint *p3, tRPoint *center)
{
    tRect r1, r2, pr1, pr2;
    tRPoint mp1, mp2;

    points2rect (p1, p2, &r1);
    points2rect (p2, p3, &r2);

    /* punts mitjos de les rectes */
    segment_mean_point (p1, p2, &mp1);
    segment_mean_point (p2, p3, &mp2);

    /* rectes perpendiculars que passen per aquestos punts */
    perpendicular_rect (&r1, &mp1, &pr1);
    perpendicular_rect (&r2, &mp2, &pr2);

    /* punt de tall de les rectes anteriors */
    return rects_cutting_point (&pr1, &pr2, center);
}

/* donat un punt el rota l'angle indicat respecte del centre */
void point_rotate (tRPoint *p, tRPoint *center, Real ang)
{
    Real beta, dist;

    beta= segment_angle (center, p) + ang;
    dist= points_distance (center, p);

    p->x= center->x + dist * cos (beta);
    p->y= center->y + dist * sin (beta);
}

/* escalat d'un segment. es modifica el punt *target per tal que
   el segment que va de *p a *target tingui la nova distància indicada  */
void scale_segment (tRPoint *target, tRPoint *p, Real real_dist)
{
    Real modul;
    Real dx, dy;

    modul= points_distance (target, p);
    if (modul== 0.0) return;
    dx= (target->x - p->x) / modul;
    dy= (target->y - p->y) / modul;

    target->x= p->x + dx * real_dist;
    target->y= p->y + dy * real_dist;
}

/* reescala un segment modificant la posició dels 2 extrems per a
   que tingui la mida desitjada */
void rescale_segment (tRPoint *p1, tRPoint *p2, Real real_dist)
{
    Real modul;
    Real dx, dy;

    modul= points_distance (p1, p2);
    if (modul== 0.0) return;
    dx= (p2->x - p1->x) / modul;
    dy= (p2->y - p1->y) / modul;

    dx*= (real_dist - modul) / 2.0;
    dy*= (real_dist - modul) / 2.0;

    p1->x-= dx; p1->y-= dy;
    p2->x+= dx; p2->y+= dy;
}

/* donada una llista de punts que formen un polígon tancat en calcula la seva caixa englobant */
void calculate_englobing_box (tRPoint *lvertexs, int nvertexs, tRPoint *min_box, tRPoint *max_box)
{
    int i;
    
    min_box->x= max_box->x= lvertexs[0].x;
    min_box->y= max_box->y= lvertexs[0].y;    
    
    for (i= 1; i< nvertexs; i++) {
        if (lvertexs[i].x> max_box->x) max_box->x= lvertexs[i].x;
        else if (lvertexs[i].x< min_box->x) min_box->x= lvertexs[i].x;
        
        if (lvertexs[i].y> max_box->y) max_box->y= lvertexs[i].y;
        else if (lvertexs[i].y< min_box->y) min_box->y= lvertexs[i].y;
    }
}

/* comprova si dos polígons tancats tenen alguna àrea solapada. 
   NOTA: no es comprova si un polígon és interior a un altre */
int polys_intersect (tRPoint *v1, int lv1, tRPoint *v2, int lv2)
{
    int i, j;
    
    for (i= 0; i< lv1; i++) 
        for (j= 0; j< lv2; j++) {
            if (segments_cutting (&v1[i], &v1[(i + 1) % lv1], 
                                  &v2[j], &v2[(j + 1) % lv2])) return 1;
        }
    return 0;
}
