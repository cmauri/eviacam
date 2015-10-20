/*******************************************************************************#
#           guvcview              http://guvcview.berlios.de                    #
#                                                                               #
#           Paulo Assis <pj.assis@gmail.com>                                    #
#                                                                               #
# This program is free software; you can redistribute it and/or modify          #
# it under the terms of the GNU General Public License as published by          #
# the Free Software Foundation; either version 2 of the License, or             #
# (at your option) any later version.                                           #
#                                                                               #
# This program is distributed in the hope that it will be useful,               #
# but WITHOUT ANY WARRANTY; without even the implied warranty of                #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                 #
# GNU General Public License for more details.                                  #
#                                                                               #
# You should have received a copy of the GNU General Public License             #
# along with this program; if not, write to the Free Software                   #
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA      #
#                                                                               #
********************************************************************************/

#ifndef COLORSPACES_H
#define COLORSPACES_H

#include "defs.h"

/*convert yuv 420 planar (yu12) to yuv 422
* args: 
*      framebuffer: pointer to frame buffer (yuyv)
*      tmpbuffer: pointer to temp buffer containing yuv420 planar data frame
*      width: picture width
*      height: picture height
*/
void 
yuv420_to_yuyv (BYTE *framebuffer, BYTE *tmpbuffer, int width, int height);

/*convert yvu 420 planar (yv12) to yuv 422 (yuyv)
* args: 
*      framebuffer: pointer to frame buffer (yuyv)
*      tmpbuffer: pointer to temp buffer containing yvu420 planar data frame
*      width: picture width
*      height: picture height
*/
void yvu420_to_yuyv (BYTE *framebuffer, BYTE *tmpbuffer, int width, int height);

/*convert yuv 420 planar (uv interleaved) (nv12) to yuv 422
* args: 
*      framebuffer: pointer to frame buffer (yuyv)
*      tmpbuffer: pointer to temp buffer containing yuv420 (nv12) planar data frame
*      width: picture width
*      height: picture height
*/
void nv12_to_yuyv (BYTE *framebuffer, BYTE *tmpbuffer, int width, int height);

/*convert yuv 420 planar (vu interleaved) (nv21) to yuv 422
* args: 
*      framebuffer: pointer to frame buffer (yuyv)
*      tmpbuffer: pointer to temp buffer containing yuv420 (nv21) planar data frame
*      width: picture width
*      height: picture height
*/
void nv21_to_yuyv (BYTE *framebuffer, BYTE *tmpbuffer, int width, int height);

/*convert yuv 422 planar (uv interleaved) (nv16) to yuv 422
* args: 
*      framebuffer: pointer to frame buffer (yuyv)
*      tmpbuffer: pointer to temp buffer containing yuv422 (nv16) planar data frame
*      width: picture width
*      height: picture height
*/
void nv16_to_yuyv (BYTE *framebuffer, BYTE *tmpbuffer, int width, int height);

/*convert yuv 422 planar (vu interleaved) (nv61) to yuv 422
* args: 
*      framebuffer: pointer to frame buffer (yuyv)
*      tmpbuffer: pointer to temp buffer containing yuv422 (nv61) planar data frame
*      width: picture width
*      height: picture height
*/
void nv61_to_yuyv (BYTE *framebuffer, BYTE *tmpbuffer, int width, int height);

/*convert yyuv to yuyv
* args: 
*      framebuffer: pointer to frame buffer (yuyv)
*      tmpbuffer: pointer to temp buffer containing a yyuv data frame
*      width: picture width
*      height: picture height
*/
void 
yyuv_to_yuyv (BYTE *framebuffer, BYTE *tmpbuffer, int width, int height);

/*convert uyvy (packed) to yuyv (packed)
* args: 
*      framebuffer: pointer to frame buffer (yuyv)
*      tmpbuffer: pointer to temp buffer containing uyvy packed data frame
*      width: picture width
*      height: picture height
*/
void uyvy_to_yuyv (BYTE *framebuffer, BYTE *tmpbuffer, int width, int height);

/*convert yvyu (packed) to yuyv (packed)
* args: 
*      framebuffer: pointer to frame buffer (yuyv)
*      tmpbuffer: pointer to temp buffer containing yvyu packed data frame
*      width: picture width
*      height: picture height
*/
void yvyu_to_yuyv (BYTE *framebuffer, BYTE *tmpbuffer, int width, int height);

/*convert yuv 411 packed (y41p) to yuv 422
* args: 
*      framebuffer: pointer to frame buffer (yuyv)
*      tmpbuffer: pointer to temp buffer containing y41p data frame
*      width: picture width
*      height: picture height
*/
void y41p_to_yuyv (BYTE *framebuffer, BYTE *tmpbuffer, int width, int height);

/*convert yuv mono (grey) to yuv 422
* args: 
*      framebuffer: pointer to frame buffer (yuyv)
*      tmpbuffer: pointer to temp buffer containing grey (y only) data frame
*      width: picture width
*      height: picture height
*/
void grey_to_yuyv (BYTE *framebuffer, BYTE *tmpbuffer, int width, int height);

/*convert SPCA501 (s501) to yuv 422
* s501  |Y0..width..Y0|U..width/2..U|Y1..width..Y1|V..width/2..V|
* signed values (-128;+127) must be converted to unsigned (0; 255)
* args: 
*      framebuffer: pointer to frame buffer (yuyv)
*      tmpbuffer: pointer to temp buffer containing s501 data frame
*      width: picture width
*      height: picture height
*/
void s501_to_yuyv(BYTE *framebuffer, BYTE *tmpbuffer, int width, int height);

/*convert SPCA505 (s505) to yuv 422
* s505  |Y0..width..Y0|Y1..width..Y1|U..width/2..U|V..width/2..V|
* signed values (-128;+127) must be converted to unsigned (0; 255)
* args: 
*      framebuffer: pointer to frame buffer (yuyv)
*      tmpbuffer: pointer to temp buffer containing s501 data frame
*      width: picture width
*      height: picture height
*/
void s505_to_yuyv(BYTE *framebuffer, BYTE *tmpbuffer, int width, int height);

/*convert SPCA508 (s508) to yuv 422
* s508  |Y0..width..Y0|U..width/2..U|V..width/2..V|Y1..width..Y1|
* signed values (-128;+127) must be converted to unsigned (0; 255)
* args: 
*      framebuffer: pointer to frame buffer (yuyv)
*      tmpbuffer: pointer to temp buffer containing s501 data frame
*      width: picture width
*      height: picture height
*/
void s508_to_yuyv(BYTE *framebuffer, BYTE *tmpbuffer, int width, int height);

/*convert yuyv to rgb24
* args: 
*      pyuv: pointer to buffer containing yuv data (yuyv)
*      prgb: pointer to buffer containing rgb24 data
*      width: picture width
*      height: picture height
*/
void 
yuyv2rgb (BYTE *pyuv, BYTE *prgb, int width, int height);


/*convert yuyv to bgr with lines upsidedown
* used for bitmap files (DIB24)
* args: 
*      pyuv: pointer to buffer containing yuv data (yuyv)
*      prgb: pointer to buffer containing rgb24 data
*      width: picture width
*      height: picture height
*/
void 
yuyv2bgr (BYTE *pyuv, BYTE *pbgr, int width, int height);

/* used for rgb video (fourcc="RGB ")
*  lines are in correct order
*/
void 
yuyv2bgr1 (BYTE *pyuv, BYTE *pbgr, int width, int height);

/*convert bayer raw data to rgb24
* args: 
*      pBay: pointer to buffer containing Raw bayer data data
*      pRGB24: pointer to buffer containing rgb24 data
*      width: picture width
*      height: picture height
*      pix_order: bayer pixel order (0=gb/rg   1=gr/bg  2=bg/gr  3=rg/bg)
*/
void 
bayer_to_rgb24(BYTE *pBay, BYTE *pRGB24, int width, int height, int pix_order);

/*convert rgb24 to yuyv
* args: 
*      prgb: pointer to buffer containing rgb24 data
*      pyuv: pointer to buffer containing yuv data (yuyv)
*      width: picture width
*      height: picture height
*/
void
rgb2yuyv(BYTE *prgb, BYTE *pyuv, int width, int height);

/*convert bgr24 to yuyv
* args: 
*      pbgr: pointer to buffer containing bgr24 data
*      pyuv: pointer to buffer containing yuv data (yuyv)
*      width: picture width
*      height: picture height
*/
void
bgr2yuyv(BYTE *pbgr, BYTE *pyuv, int width, int height);

/*use in utils.c for jpeg decoding  420 planar to 422
* args: 
*      out: pointer to data output of idct (macroblocks yyyy u v)
*      pic: pointer to picture buffer (yuyv)
*      width: picture width
*/
void 
yuv420pto422(int * out,unsigned char *pic,int width);

/*use in utils.c for jpeg decoding 422 planar to 422
* args: 
*      out: pointer to data output of idct (macroblocks yyyy u v)
*      pic: pointer to picture buffer (yuyv)
*      width: picture width
*/
void 
yuv422pto422(int * out,unsigned char *pic,int width);

/*use in utils.c for jpeg decoding 444 planar to 422
* args: 
*      out: pointer to data output of idct (macroblocks yyyy u v)
*      pic: pointer to picture buffer (yuyv)
*      width: picture width
*/
void 
yuv444pto422(int * out,unsigned char *pic,int width);

/*use in utils.c for jpeg decoding 400 planar to 422
* args: 
*      out: pointer to data output of idct (macroblocks yyyy )
*      pic: pointer to picture buffer (yuyv)
*      width: picture width
*/
void 
yuv400pto422(int * out,unsigned char *pic,int width);

#endif

