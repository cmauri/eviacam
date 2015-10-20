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

#ifndef DEFS_H
#define DEFS_H
#include <inttypes.h>
#include <sys/types.h>

#define CLEAR_LINE "\x1B[K"

#ifdef WORDS_BIGENDIAN
  #define BIGENDIAN 1
#else
  #define BIGENDIAN 0
#endif

#define IO_MMAP 1
#define IO_READ 2

#define ODD(x) ((x%2)?TRUE:FALSE)

/*next index of ring buffer with size elements*/
#define NEXT_IND(ind,size) ind++;if(ind>=size) ind=0
/*previous index of ring buffer with size elements*/
//#define PREV_IND(ind,size) ind--;if(ind<0) ind=size-1

#define VIDBUFF_SIZE 45    //number of video frames in the ring buffer
#define AUDBUFF_SIZE 100   //number of audio mpeg frames in the ring buffer
#define MPG_NUM_SAMP 1152  //number of samples in a audio MPEG frame 
//#define MPG_NUM_FRAMES 2   //number of MPEG frames in a audio frame

typedef uint64_t QWORD;
typedef uint32_t DWORD;
typedef uint16_t WORD;
typedef uint8_t  BYTE;
typedef unsigned int LONG;
typedef unsigned int UINT;

typedef unsigned long long ULLONG;
typedef unsigned long      ULONG;

typedef char* pchar;

typedef int8_t     INT8;
typedef uint8_t    UINT8;
typedef int16_t    INT16;
typedef uint16_t   UINT16;
typedef int32_t    INT32;
typedef uint32_t   UINT32;
typedef int64_t    INT64;
typedef uint64_t   UINT64;

typedef float SAMPLE;

/* 0 is device default*/
static const int stdSampleRates[] = 
{ 
	0, 8000,  9600, 11025, 12000,
	16000, 22050, 24000,
	32000, 44100, 48000,
	88200, 96000,
	-1   /* Negative terminated list. */
};

/*----------- guvcview version ----------------*/
//#define VERSION ("") /*defined in config.h*/
//#define DEBUG (0)
/*---------- Thread Stack Size (bytes) --------*/
#define TSTACK (128*64*1024) /* Debian Default 128 pages of 64k = 8388608 bytes*/

/*----------- AVI max file size ---------------*/
#define AVI_MAX_SIZE (1900*1024*1024)
/* extra size beyond MAX SIZE at wich we can still write data*/ 
#define AVI_EXTRA_SIZE (20*1024*1024)

#define INCPANTILT 64 // 1°

#define WINSIZEX 480
#define WINSIZEY 700

#define AUTO_EXP 8
#define MAN_EXP	1

#define DHT_SIZE 432

#define DEFAULT_WIDTH 640
#define DEFAULT_HEIGHT 480

#define DEFAULT_IMAGE_FNAME	"Image.jpg"
#define DEFAULT_AVI_FNAME	"capture.avi"
#define DEFAULT_FPS	25
#define DEFAULT_FPS_NUM 1
#define SDL_WAIT_TIME 30 /*SDL - Thread loop sleep time */

/*clip value between 0 and 255*/
#define CLIP(value) (BYTE)(((value)>0xFF)?0xff:(((value)<0)?0:(value)))

/*MAX macro - gets the bigger value*/
#ifndef MAX
#define MAX(a,b) (((a) < (b)) ? (b) : (a))
#endif

/*FILTER FLAGS*/
#define YUV_NOFILT (0)
#define YUV_MIRROR (1<<0)
#define YUV_UPTURN (1<<1)
#define YUV_NEGATE (1<<2)
#define YUV_MONOCR (1<<3)
#define YUV_PIECES (1<<4)
#define YUV_PARTICLES (1<<5)

/*Audio Effects*/
#define SND_NOEF   (0)
#define SND_ECHO   (1<<0)
#define SND_FUZZ   (1<<1)
#define SND_REVERB (1<<2)
#define SND_WAHWAH (1<<3)
#define SND_DUCKY  (1<<4)

#endif

