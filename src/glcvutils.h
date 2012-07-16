/***************************************************************************
 *   Copyright (C) 2010 by Andre L. V. da Silva   									*
 *   andreluizeng@yahoo.com.br   														*
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef __glcvutils_h__
#define __glcvutils_h__

#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <fcntl.h>
#include <malloc.h> 
#include <math.h> 
#include <stdlib.h>


#define LINUX
#define EGL_API_FB

#include <opencv/cv.h>
#include <opencv/highgui.h>

#define EGL_USE_GLES2
#define EGL_EGLEXT_PROTOTYPES 1
#define GL_GLEXT_PROTOTYPES 1

#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <GLES2/gl2ext.h>
#include <EGL/eglext.h>
//#include <EGL/eglfslext.h>

#include <termios.h>
#include <unistd.h>
#include <fcntl.h>


#define VERTEX_ARRAY	0
#define TEXCOORD_ARRAY	1

#ifndef TEXTURE_W
	#define TEXTURE_W	320
#endif

#ifndef TEXTURE_H
	#define TEXTURE_H	240
#endif

#define TRUE 1
#define FALSE 0

#define CAMERA 0
#define VIDEO_FILE 1

class GLCVUtils
{
	public:
		EGLDisplay	egldisplay;
		EGLConfig	eglconfig;
		EGLSurface	eglsurface;
		EGLContext	glcontext;

		IplImage	*frame;
		IplImage	*rgb_image;
		CvCapture 	*capture;

		bool 		GLInit						(void);
		void 		GLEnd							(void);
		bool 		CVInit						(int mode);
		void 		CVEnd							(void);
		int 		Kbhit							(void);
		void		Cv2GlCoords	(float *cv_pnt, float *gl_pnt);
		void		LoadIdentityMatrix4x4 	(float *m);
		bool 		CVLoadCamCal				(void);
		IplImage 	*CVUndistortImage 		(IplImage *image);
		IplImage 	*LoadImage					(char *path);
		void 		GenOrthoMatrix				(float left,float right,float bottom,float top,float zNear,float zFar, float *m);
		void 		GenPerspectiveMatrix 	(float FOV, float n, float f, float aspect, float *m);

	private:

		CvMat *_intrinsic;
		CvMat *_distortion;
		IplImage *_mapx;
		IplImage *_mapy;

};

#endif

