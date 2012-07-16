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

#include "glcvutils.h"

//--------------------------------------------------------------------------------------
// Name: GLInit ()
// Desc: Initializate the EGL functions
//--------------------------------------------------------------------------------------
bool GLCVUtils::GLInit (void)
{
	static const EGLint s_configAttribs[] =
	{
		EGL_RED_SIZE,		5,
		EGL_GREEN_SIZE, 	6,
		EGL_BLUE_SIZE,		5,
		EGL_ALPHA_SIZE, 	EGL_DONT_CARE,
		EGL_SAMPLES,		0,
		EGL_DEPTH_SIZE, 	24,
		EGL_NONE
	};

	EGLint numconfigs;

	EGLint ContextAttribList[] =	{EGL_CONTEXT_CLIENT_VERSION, 2,	EGL_NONE};

	// Initialize Display - Get the default display.
	egldisplay = eglGetDisplay (EGL_DEFAULT_DISPLAY);
	eglInitialize (egldisplay, NULL, NULL);
	assert (eglGetError () == EGL_SUCCESS);
	
	//Initialize EGL.
	eglInitialize(egldisplay, NULL, NULL);
	assert (eglGetError () == EGL_SUCCESS);

	// EGL Surface Config
	eglChooseConfig (egldisplay, s_configAttribs, &eglconfig, 1, &numconfigs);
	assert (eglGetError () == EGL_SUCCESS);
	assert (numconfigs == 1);

	//You must pass in the file system handle to the linux framebuffer when creating a window
	EGLNativeDisplayType native_display = fbGetDisplay();
	EGLNativeWindowType  native_window  = fbCreateWindow(native_display, 0, 1, 1024, 768);

	eglsurface = eglCreateWindowSurface (egldisplay, eglconfig, native_window, NULL);
	assert (eglGetError () == EGL_SUCCESS);

	eglBindAPI (EGL_OPENGL_ES_API);

	// Create the EGL graphic Context
	EGLContext eglcontext = eglCreateContext (egldisplay, eglconfig, EGL_NO_CONTEXT, ContextAttribList);
	assert (eglGetError() == EGL_SUCCESS);

	eglMakeCurrent (egldisplay, eglsurface, eglsurface, eglcontext);
	assert (eglGetError() == EGL_SUCCESS);

	// clear buffer to the black color
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	assert (eglGetError() == EGL_SUCCESS);

	// Swap Buffers.
	// Brings to the native display the current render surface.
	eglSwapBuffers (egldisplay, eglsurface);
	assert (eglGetError () == EGL_SUCCESS);

	return true;
}

void GLCVUtils::GLEnd (void)
{
	eglMakeCurrent(egldisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	eglTerminate(egldisplay);
	eglReleaseThread ();
	
	return;
}

//--------------------------------------------------------------------------------------
// Name: CVInit()
// Desc: Initializate OpenCV
//--------------------------------------------------------------------------------------
bool GLCVUtils::CVInit (int mode)
{
	if (mode == VIDEO_FILE)
	{
		capture = cvCreateFileCapture ("Ratatouille.avi");
		if (! capture) printf ("\n\n\nerror creating file capture\n\n\n");
			
	}
	
	else if (mode == CAMERA)
	{
		// create capture
		capture = cvCreateCameraCapture (0);
		if (! capture)
		{
			printf ("\nError creating capture");
			return false;
		}

		// sets camera (hardware) resolution
		cvSetCaptureProperty (capture, CV_CAP_PROP_FRAME_WIDTH,  TEXTURE_W);
		cvSetCaptureProperty (capture, CV_CAP_PROP_FRAME_HEIGHT, TEXTURE_H);
		
	}
	else
	{
		printf ("\nWrong Mode");
	}

	return true;
}
//--------------------------------------------------------------------------------------
// Name: CVEnd()
// Desc: Free OpenCV´s variable
//--------------------------------------------------------------------------------------
void GLCVUtils::CVEnd (void)
{

	cvReleaseCapture (&capture);

	return;
}


//--------------------------------------------------------------------------------------
// Name: CVEnd()
// Desc: Free OpenCV´s variable
//--------------------------------------------------------------------------------------
int GLCVUtils::Kbhit (void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

  ch = getchar();
    
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);

  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }

  return 0;
}

// converts 2d cv coordinates to 2d gl coordinates
void GLCVUtils::Cv2GlCoords (float *cv_pnt, float *gl_pnt)
{
	float corr_x = 0;
	float corr_y = 0;
	
	float corr_x_near = 0;
	float corr_x_far = 0;
	float corr_y_near = 0;
	float corr_y_far = 0;
	float a = 0;
	float b = 0;
	
			
	corr_x = 0;
	corr_y = 0;
	corr_x_near = 0;
	corr_x_far = 0;
	corr_y_near = 0;
	corr_y_far = 0;
	a = 0;
	b = 0;

	gl_pnt[0] = 0;
	gl_pnt[1] = 0;
	gl_pnt[2] = 0;

	for (int i = 0; i < 3; i++)
	{
		gl_pnt[i] = cv_pnt[i];
	}


	// CORRECTIONS TO APPLY VALUES TO GL WORLD

	corr_x_near = 0.46 * gl_pnt[0] - 73.16;
	corr_x_far  = 1.56 * gl_pnt[0] - 249.98;
	corr_y_near = -0.68 * gl_pnt[1] + 92.02;
	corr_y_far  = -2.08 * gl_pnt[1] + 257.68;

	a = (corr_x_far - corr_x_near) / (-1000.00);
	b = corr_x_near + (a * 300);
	corr_x = a * gl_pnt[2] + b;
	//corr_x = 0;
	gl_pnt[0] = corr_x;  // X AXIS
	
	a = (corr_y_far - corr_y_near) / (-1000.00);
	b = corr_y_near + (a * 300);
	corr_y = a * gl_pnt[2] + b;
	//corr_y = 0;
	gl_pnt[1] = corr_y;
	
	return;
}

//--------------------------------------------------------------------------------------
// Name: LoadIdentityMatrix4x4
// Desc: Load a 4x4 indentity matrix
//--------------------------------------------------------------------------------------
void GLCVUtils::LoadIdentityMatrix4x4 (float *m)
{
	m[0] = 1;
	m[1] = 0;
	m[2] = 0;
	m[3] = 0;
	
	m[4] = 0;
	m[5] = 1;
	m[6] = 0;
	m[7] = 0;
	
	m[8] = 0;
	m[9] = 0;
	m[10] = 1;
	m[11] = 0;
	
	m[12] = 0;
	m[13] = 0;
	m[14] = 0;
	m[15] = 1;

	return;
}

//--------------------------------------------------------------------------------------
// Name: CVLoadCamCal	
// Desc: Loads camera calibration files
//--------------------------------------------------------------------------------------
bool GLCVUtils::CVLoadCamCal (void)
{
	_intrinsic = (CvMat*) cvLoad("Intrinsics.xml");
	_distortion = (CvMat*) cvLoad("Distortion.xml");

	if (! _intrinsic)
	{
		printf ("\nError loading Intrinsics.xml");
		return FALSE;
	}
	if (! _distortion)
	{
		printf ("\nError loading Distortion.xml");
		return FALSE;
	}

	// Build the undistort map that we will use for all
	// subsequent frames.
	_mapx= cvCreateImage (cvSize(TEXTURE_W, TEXTURE_H), IPL_DEPTH_32F, 1);
	_mapy = cvCreateImage (cvSize(TEXTURE_W, TEXTURE_H), IPL_DEPTH_32F, 1);
	cvInitUndistortMap (_intrinsic, _distortion, _mapx, _mapy);

	return TRUE;
}


//--------------------------------------------------------------------------------------
// Name: CVUndistortImage
// Desc: undistorts the given image using calibration parameters
//--------------------------------------------------------------------------------------
IplImage *GLCVUtils::CVUndistortImage (IplImage *image)
{
	IplImage *t = cvCloneImage(image);
	cvRemap (t, image, _mapx, _mapy); // Undistort image
	cvReleaseImage(&t);

	return image;
}
//--------------------------------------------------------------------------------------
// Name: GenOrthoMatrix
// Desc: Generate a orthogonal matrix
//--------------------------------------------------------------------------------------
IplImage *GLCVUtils::LoadImage (char *path)
{
	IplImage *temp;
	IplImage *img;

	temp = cvLoadImage (path);

	if ((temp->width == TEXTURE_W) && (temp->height == TEXTURE_H))
	{
		return temp;
	}

	img = cvCreateImage (cvSize (TEXTURE_W, TEXTURE_H), 8, 3);
	cvResize (temp, img, CV_INTER_LINEAR);

	cvReleaseImage (&temp);

	return img;
}

//--------------------------------------------------------------------------------------
// Name: GenOrthoMatrix
// Desc: Generate a orthogonal matrix
//--------------------------------------------------------------------------------------
void GLCVUtils::GenOrthoMatrix(float left,float right,float bottom,float top,float zNear,float zFar, float *m)
{	
	float tx=-(right+left)/(right-left);
	float ty=-(top+bottom)/(top-bottom);
	float tz=-(zFar+zNear)/(zFar-zNear);
	LoadIdentityMatrix4x4 (m);
	m[0]=2/(right-left);
	m[5]=2/(top-bottom);
	m[10]=-2/(zFar-zNear);	
	m[12]=tx;
	m[13]=ty;
	m[14]=tz;

}

//--------------------------------------------------------------------------------------
// Name: GenPerspectiveMatrix
// Desc: Generate a perspective matrix
//--------------------------------------------------------------------------------------
void GLCVUtils::GenPerspectiveMatrix (float FOV, float n, float f, float aspect, float *m)
{

// n = near plane
// f = far plane

	float e = 1 / tan (FOV / 2);			
	float a = aspect;		// viewport

	// Build a perspective projection matrix
	m[ 0] = e;
	m[ 5] = e / a;
	m[10] = - (n + f) / (f - n);
	m[11] = - (2 * f * n) / (f - n);
	m[14] = -1.0;

	return;
}



