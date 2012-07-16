/************************-***************************************************
 *   Copyright (C) 2011 by Andre L. V. da Silva   									*
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

//--------------------------------------------------------------------------------------
// File: main.cpp
//--------------------------------------------------------------------------------------
#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <fcntl.h>
#include <malloc.h>
#include <math.h>
#include <stdlib.h>

#include <termios.h>
#include <unistd.h>
#include <time.h>

#include <pthread.h>

#include "opencv/cv.h"
#include "opencv/cxcore.h"
#include "opencv/cvaux.h"
#include "opencv/highgui.h"

#include "glcvutils.h"
#include "glcvplane.h"

#include "netplayer_gst.h"

#include "mlp.h"
#define SMALL_W ANN_W
#define SMALL_H ANN_H

#define TEXTURE_W 320
#define TEXTURE_H 240

#define THEATER_W 1024/2
#define THEATER_H 768/2

GLCVPlane *stage = new GLCVPlane;
GLCVPlane *theater = new GLCVPlane;
GLCVPlane *status_panel = new GLCVPlane;

//IplImage *stage_img;
IplImage *theater_img;

IplImage *edge_img;
IplImage *bgr_img;


float matProj[16] = {0};
float matProjTheater[16] = {0};
float matProjOrtho[16] = {0};
float matModel[9] = {0};

pthread_t camera_thread;
pthread_t gst_loop_thread;
pthread_t status_thread;
int thread_id = 0;

char *uri_to_play;

double *mlp_input;
GLCVUtils *window = new GLCVUtils;

	
IplImage *gray;
IplImage *verysmall;
IplImage *verysmall_copy;
IplImage *smallimg;
IplImage *gray2;
IplImage *binary;
IplImage *inverted_binary;
IplImage *bgr;
IplImage *hsv;
IplImage *aux;

IplImage *font_img;

CvPoint detection_center;

CvFont axisfont;
CvFont dfont;
CvSeq *contours;

CvMemStorage *storage;

bool flag_video_playing = 0;
bool enable_control = 0;


// GL
void 	Render 				(void);


//CV and MLP
void	*UpdateTextureFromCamera 	(void *ptr);
void 	ColorSpaceConv 			(IplImage *orig, IplImage *dest, int mode);
void	ColorFilter			(IplImage *orig, IplImage *dest, int minval, int maxval, int val);
void 	IPInit				(void);
void 	IPEnd 				(void);
void 	PreProcessing			(void);
void 	FontInit 			(void);
CvSeq	*ContoursFinder 		(IplImage *edge, 	CvMemStorage *storage);
int 	PatternRecognition 		(IplImage *gray, CvSeq *contours, CvPoint *center);
int 	MotionDetect 			(int option, CvPoint center, bool reset);
int 	StateMachine 			(int option, int value1, int value2);


// GST 
void 	*GstLoop 			(void *ptr);
static void on_handoff 			(GstElement* pFakeSink, GstBuffer* pBuffer, GstPad* pPad);



GMainLoop *loop;
GstBus *bus;
GstMessage *bus_msg;


gchar*	g_pcFrameBuffer = NULL;


bool end_of_stream_flag = FALSE;
bool gst_bus_error_flag = FALSE;

int video_w = 1280;
int video_h = 720;




#define OPENED 	0
#define CLOSED 	1
#define POINTER 2
#define CHANNEL 2
#define VOLUME  3
#define STANDBY 4

#define NOTHING_IS_HAPPENING 0
#define POINTING 1
#define CLICK_PRESS 2
#define LEFT   	0
#define RIGHT  	1
#define UP     	2
#define DOWN   	3
#define STOPPED 4
#define EXIT  	5
#define INC_VOL	6
#define DEC_VOL	7
#define NX_CHAN 8
#define PR_CHAN 9
int resx;
int resy;

bool flag_on_off  = TRUE;

long counter_menu = 0;
int flag_menu_in = -1;


// channel number
int channel_n = 0;
float volume_pos = 5;

int menu_prev = -1;
int res = -1;


/*
int main (int argc, char **argv)
{
	// FOR GST
	
	g_pcFrameBuffer = (gchar*)malloc(720*480*3);
	
	netplayer_gst_init (&argc, &argv);
	loop = g_main_loop_new (NULL, FALSE);
	
	uri_to_play = g_strdup_printf ("file:///home/gpucv/Up.mp4");
  	
	netplayer_gst_play(uri_to_play, (GCallback) on_handoff);
	netplayer_gst_resume();
	
	pthread_create (&gst_loop_thread, NULL, GstLoop,(void *)&thread_id);
	
	while (! window->Kbhit ())
	{
		
		
	}
	
	return 0;

}
*/

#define M_FILTER_SIZE 5

int valuesx[M_FILTER_SIZE];   // x filter
int valuesy[M_FILTER_SIZE];   // y filter
int ptrx;	// x pointer
int ptry;	// y pointer

float MovingAverageFilter_X (int value);
float MovingAverageFilter_Y (int value);



double tt;	
double value;

int min_hsv;
int max_hsv;
int val_hsv;

//--------------------------------------------------------------------------------------
// Name: main()
// Desc: main function (entry point)
//--------------------------------------------------------------------------------------
int main (int argc, char **argv)
{
	char 	key = 0;
	CvPoint endpoint = cvPoint (10, TEXTURE_H/2/2-10);
	char m[100];

	
	
	// skin color
	min_hsv = 15; 
	max_hsv = 40; 
	val_hsv = -1; //not used
	
	if (argc > 1)
		min_hsv = atoi (argv[1]);
	if (argc > 2)
		max_hsv = atoi (argv[2]);
	if (argc > 3)
		val_hsv = atoi (argv[3]);
		
	
	
	printf ("\nUsing: min: %d, max: %d, val: %d", min_hsv, max_hsv, val_hsv);
	
	printf ("\n\nYellow: ./gesture 15 40 -1");
	printf ("\nSkin: ./gesture 5 30 20");
	printf ("\n\n");
	//IplImage *img_gray;
	
	// Initializate OPENCV
	IPInit();
	printf ("\nInitializing OpenCV...");
	FontInit ();
	window->CVInit (CAMERA);
	bgr_img = cvCreateImage (cvSize (TEXTURE_W, TEXTURE_H), 8, 4); // allocates memory for the image
	printf ("OK");
	
	//printf ("\nCamera Width: %d", (int) cvGetCaptureProperty (window->capture, CV_CAP_PROP_FRAME_WIDTH));
	//printf ("\nCamera Height: %d\n\n", (int) cvGetCaptureProperty (window->capture, CV_CAP_PROP_FRAME_HEIGHT));

	// Initializate OGLES2
	printf ("\nInitializing OGLES2...");
	window->GLInit ();
	printf ("OK");
	
	// create a plane
	printf ("\nCreating the Stage...");
	stage->PlaneCreate (96, 96);  			// width, height
	stage->PlaneCreateTex (TEXTURE_W/2, TEXTURE_H/2, 3); 			// texture size (frame from usbcamera)
	//stage->PlaneCreateTex (ANN_W, ANN_H, 3); 			// texture size (frame from usbcamera)
	//stage_img = cvCreateImage (cvSize (TEXTURE_W/2, TEXTURE_H/2), 8, 3); // allocates memory for the image
	printf ("OK");

	// create a plane
	printf ("\nCreating the Theater...");
	theater->PlaneCreate (THEATER_W, THEATER_H);  			// width, height
	//theater->PlaneCreateTex (video_w, video_h, 3); 			// texture size (frame from usbcamera)
	//theater_img = cvCreateImage (cvSize (video_w, video_h), 8, 3); // allocates memory for the image
	printf ("OK");

	// create a plane
	printf ("\nCreating the Status Panel...");
	status_panel->PlaneCreate (350, 96);  			// width, height
	status_panel->PlaneCreateTex (TEXTURE_W/2, TEXTURE_H/2, 3); 			// texture size (frame from usbcamera)
	font_img = cvCreateImage (cvSize (TEXTURE_W/2, TEXTURE_H/2), 8, 3); // allocates memory for the image
	printf ("OK");

	
	window->GenPerspectiveMatrix (0.5, 1.0, -1.0, 1.0, matProj);
	window->GenPerspectiveMatrix (0.5, 1.0, -1.0, 1.0, matProjTheater);

	stage->PlaneSetProjMatrix (matProj);  // sets the projection matrix
	theater->PlaneSetProjMatrix (matProjTheater);  // sets the projection matrix
	status_panel->PlaneSetProjMatrix (matProj);
	
	window->frame = cvQueryFrame(window->capture);
		
	
	// FOR GST
	netplayer_gst_init (&argc, &argv);
	loop = g_main_loop_new (NULL, FALSE);
	
	g_pcFrameBuffer = (gchar*) malloc (video_w * video_h * 2);
	uri_to_play = g_strdup_printf ("file:///home/gesture/Up.mp4");
  	
	netplayer_gst_play(uri_to_play, (GCallback) on_handoff);
	
	//create thread for updating the texture
	pthread_create (&camera_thread, NULL, UpdateTextureFromCamera,(void *)&thread_id);
	//pthread_create (&status_thread, NULL, StatusLoop,(void *)&thread_id);
	pthread_create (&gst_loop_thread, NULL, GstLoop,(void *)&thread_id);

	
	netplayer_gst_resume();
	end_of_stream_flag = FALSE;

	while (! window->Kbhit ()) 
	{
		
		if (end_of_stream_flag) break;
	
		tt = (double)cvGetTickCount();

		Render ();
		
		tt = (double)cvGetTickCount() - tt;
		value = tt/(cvGetTickFrequency()*1000.);
		//printf( "\ntime = %gms --- %.2lf FPS", value, 1000.0 / value);
		

		//sprintf (m, "%.2lf FPS", 1000.0 / value);
		//cvSet (font_img, cvScalar (0, 0, 255), NULL);
		//cvPutText (font_img, m, endpoint, &axisfont, CV_RGB (0, 0,0));

		//key = cvWaitKey (30);
	}

	// finish opencv
	printf ("\nFinalizing OpenCV...");
	//window->CVEnd ();
	//cvReleaseImage (&stage_img);
	//cvReleaseImage (&bgr_img);
	//cvReleaseImage (&font_img);
	//IPEnd ();
	printf ("OK");
	
	printf ("\nFinalizing GST...");
	netplayer_gst_stop();
	free (g_pcFrameBuffer);
	printf ("OK");


	printf ("\nFreeing textures...");
	stage->PlaneDestroy ();
	theater->PlaneDestroy ();
	status_panel->PlaneDestroy();
	printf ("OK");
	
	// finish ogles2
	printf ("\nFinalizing OGLES2...");
	window->GLEnd ();
	printf ("OK\n\n");
   
}

void Render (void)
{
	static float x_angle = 0;
	static float y_angle = 0;
	static float z_angle = 0;

	static bool flag = 0;
	
	// Clear the colorbuffer and depth-buffer
	glClearColor (0.0f, 0.0f, 0.0f, 0.0f);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	PreProcessing();
	
	stage->PlaneSetTex(verysmall);
	stage->PlaneMove(PLANE_Z_AXIS, -1000);
	//stage->PlaneMove(PLANE_X_AXIS, -195);
	stage->PlaneMove(PLANE_Y_AXIS, 200);
	stage->PlaneDraw();

	//status_panel->PlaneSetTex(font_img);
	//status_panel->PlaneMove(PLANE_Z_AXIS, -1000);
	//status_panel->PlaneMove(PLANE_X_AXIS, 50);
	//status_panel->PlaneMove(PLANE_Y_AXIS, 200);
	//status_panel->PlaneRotate(PLANE_Y_AXIS, y_angle);
	//status_panel->PlaneSetColor(cvScalar (0, 0, 0));
	//status_panel->PlaneDraw();

	//stage->PlaneLoadGLTextures(window->egldisplay, bgr_img);

	//cube1->CubeSetFaceTex(FRONT_FACE, bgr_img);
	//cube1->CubeSetFaceTex(BOTTOM_FACE, bgr_img);
	//cube1->CubeSetFaceTex(TOP_FACE, bgr_img);
	//cube1->CubeSetFaceTex(LEFT_FACE, bgr_img);
	//cube1->CubeSetFaceTex(RIGHT_FACE, bgr_img);
	//cube1->CubeSetFaceTex(BACK_FACE, bgr_img);

	//theater->PlaneSetColor(cvScalar(255,0,0));
	//memmove (theater_img->imageData, g_pcFrameBuffer, theater_img->imageSize);
	theater->PlaneSetTexBuf(g_pcFrameBuffer, 720, 480);
	theater->PlaneMove(PLANE_Z_AXIS, -1050);
	theater->PlaneMove(PLANE_Y_AXIS, -50);
	
	
//	x_angle += 0.5;
	//y_angle += 0.5;
//	z_angle += 1.5;
	
	if (enable_control)
	{
		detection_center.x = MovingAverageFilter_X(detection_center.x);
		detection_center.y = MovingAverageFilter_Y(detection_center.y);		
	
		if (flag)
		{
			printf ("\nControl Enabled - (%d,%d)", detection_center.x, detection_center.y); 
			flag = 0;
		}
		y_angle = detection_center.x * (-1.34) + 78.7;
		x_angle = detection_center.y * (+1.34) - 78.7;
	}
	else
	{
		if (! flag)
		{
			printf ("\nControl Disabled");
			flag = 1;
		}
	}
	//theater->PlaneRotate(PLANE_Z_AXIS, z_angle);
	theater->PlaneRotate(PLANE_Y_AXIS, y_angle);
	theater->PlaneRotate(PLANE_X_AXIS, x_angle);
	theater->PlaneDraw();	

	// Swap Buffers.
	// Brings to the native display the current render surface.
	eglSwapBuffers (window->egldisplay, window->eglsurface);
	assert (eglGetError () == EGL_SUCCESS);
	

	return;
}

void *UpdateTextureFromCamera (void *ptr)
{
	while(1)
	{
		window->frame = cvQueryFrame(window->capture);
		//cvFlip (window->frame, window->frame, 1);  // mirrored image
		cvCvtColor(window->frame, bgr_img, CV_BGR2BGRA);
		cvResize (window->frame, verysmall, CV_INTER_LINEAR);

	}

}

void *GstLoop (void *ptr)
{
	while (1)
	{
		while ((bus_msg = gst_bus_pop (bus))) 
		{
			// Call your bus message handler
			bus_call (bus, bus_msg, NULL);
			gst_message_unref (bus_msg);
		}

	}
}


//----------------------------------------------------------------------
// GSTREAMER CODE

gboolean bus_call (GstBus *bus, GstMessage *msg, gpointer    data)
{
	//GMainLoop *loop = (GMainLoop *) data;

  	switch (GST_MESSAGE_TYPE (msg))
	{
    		case GST_MESSAGE_EOS:	g_print ("End of stream\n");
						
					//if (flag_video_playing)
					//{
					//	flag_video_playing = 0;
					//	uri_to_play = g_strdup_printf ("file:///home/gpucv/Up.mp4");
					//}
					//else
					//{
					//	flag_video_playing = 1;
//						uri_to_play = g_strdup_printf ("file:///home/gpucv/finalfantasy.avi");
//					}
					netplayer_gst_play(uri_to_play, (GCallback) on_handoff);
					netplayer_gst_resume();
					end_of_stream_flag = FALSE;

					break;

    		case GST_MESSAGE_ERROR:{
						gchar  *debug;
      						GError *error;
						gst_message_parse_error (msg, &error, &debug);
      						g_free (debug);
	
      						g_printerr ("Error: %s\n", error->message);
      						g_error_free (error);
	
      					//	g_main_loop_quit (loop);
						gst_bus_error_flag = TRUE;
						break;
					}
		      		
    		default:		end_of_stream_flag = FALSE;
					gst_bus_error_flag = FALSE;
					break;
  	}

  return TRUE;
}

//handoff function, called every frame
void on_handoff (GstElement* pFakeSink, GstBuffer* pBuffer, GstPad* pPad)
{
	
	video_w = get_pad_width (pPad);
	video_h = get_pad_height (pPad);

	gst_buffer_ref (pBuffer);
	memmove (g_pcFrameBuffer, GST_BUFFER_DATA (pBuffer), video_w * video_h * 2);
	gst_buffer_unref (pBuffer);
}

// CV PART

// Color space conversion
// CV_BGR2HSV
// CV_HSV2BGR
void ColorSpaceConv (IplImage *orig, IplImage *dest, int mode)
{
	switch (mode)
	{
		case CV_BGR2HSV: 	cvCvtColor (orig, dest, CV_BGR2HSV);
					break;
					
		case CV_HSV2BGR:	cvCvtColor (orig, dest, CV_HSV2BGR);
					break;

		case CV_BGR2GRAY:	cvCvtColor (orig, dest, CV_BGR2GRAY);
					break;

		default: 		printf ("\nERROR: Unknown color space\n");
					cvCopy (orig, dest);
					
	}
	
	return;
}


// skin color filter
void ColorFilter (IplImage *orig, IplImage *dest, int minval, int maxval, int val)
{
	uchar *ptr_hsv;
	int i, j;
	
	for (i = 0; i < orig->height; i++)
	{
		for (j = 0; j < orig->width; j++)
		{
			ptr_hsv = cvPtr2D (orig, i, j, 0);

			if (val >= 0)
			{
				if  (((ptr_hsv[0] < minval) || (ptr_hsv[0] > maxval)) && (ptr_hsv[1] < val)) //  R
				{
					ptr_hsv[0] = 0;  // H
					ptr_hsv[1] = 0;  // S
					ptr_hsv[2] = 0;  // V
				}

			}
			else
			{
				if  (((ptr_hsv[0] < minval) || (ptr_hsv[0] > maxval)))// || (ptr_hsv[1] < 30)) //  R
				{
					ptr_hsv[0] = 0;  // H
					ptr_hsv[1] = 0;  // S
					ptr_hsv[2] = 0;  // V
				}
			}

		}
	}

	return;
}


void IPInit (void)
{
	
	storage = cvCreateMemStorage(0);
	
		CvSeq *contours;
	
	// initializates the MLP module
	MLPInit ();
	printf ("\nLoading netweights...");	
	LoadNetWeights ();
	printf ("OK\n");

	
	mlp_input = (double *) malloc (sizeof (double) * SMALL_W * SMALL_H + (sizeof (double)));
	
	if (! mlp_input) 
	{
		printf ("\nError allocating memory, aborting...");
		exit (0);
	}


	aux 		= cvCreateImage (cvSize (TEXTURE_W, TEXTURE_H), 8, 3);	
	verysmall 	= cvCreateImage (cvSize (TEXTURE_W/2, TEXTURE_H/2), 8, 3);
	verysmall_copy 	= cvCreateImage (cvGetSize(verysmall), 8, 3);
	smallimg 	= cvCreateImage (cvSize (ANN_W, ANN_H), 8, 1);	
	gray 		= cvCreateImage (cvGetSize(verysmall), 8, 1);
	binary 		= cvCreateImage (cvGetSize(verysmall), 8, 1);
	inverted_binary = cvCreateImage (cvGetSize(verysmall), 8, 1);
	bgr 		= cvCreateImage (cvGetSize(verysmall), 8, 3);
	hsv 		= cvCreateImage (cvGetSize(verysmall), 8, 3);
}

void IPEnd (void)
{
	cvReleaseImage(&aux);
	cvReleaseImage(&verysmall);
	cvReleaseImage(&verysmall_copy);
	cvReleaseImage(&gray);
	cvReleaseImage(&binary);
	cvReleaseImage(&inverted_binary);
	cvReleaseImage(&bgr);
	cvReleaseImage(&hsv);
	cvReleaseImage (&smallimg);

	free (mlp_input);

	cvReleaseMemStorage 	(&storage);	
	
	return;
}

void FontInit (void)
{
	float hscale      = 0.5f;
	float vscale      = 0.5f;
	float italicscale = 0.0f;
	int  thickness    = 1;

	cvInitFont (&dfont, CV_FONT_HERSHEY_SIMPLEX , hscale, vscale, italicscale, thickness, CV_AA);

	float axhscale      = 0.8f;
	float axvscale      = 1.5f;

	cvInitFont (&axisfont, CV_FONT_HERSHEY_SIMPLEX , axhscale, axvscale, italicscale, thickness, CV_AA);


	return;
}


void PreProcessing (void)
{
	
	// filters most of the background and different colors
	ColorSpaceConv(verysmall, hsv, CV_BGR2HSV);
	
	//ColorFilter(hsv, hsv, 15, 40);  //yellow
	ColorFilter(hsv, hsv, min_hsv, max_hsv, val_hsv);  //palm color
	ColorSpaceConv(hsv, verysmall_copy, CV_HSV2BGR);
	ColorSpaceConv (verysmall_copy, gray, CV_BGR2GRAY);
	
	/*uchar *ptr;
	for (int i = 0; i < verysmall_copy->height; i++)
	{
		for (int j = 0; j < verysmall_copy->width; j++)
		{	
			ptr = cvPtr2D (verysmall_copy, i, j, 0);
			
			if ( (i == 0) || (j == 0) || ((i+1) == verysmall_copy->height) || ((j+1) == verysmall_copy->width))
			{
				ptr[0] = 255;  // B
				ptr[1] = 255;  // G
				ptr[2] = 255;  // R
			}
		}
	}
	*/
	//cvEqualizeHist (gray, gray);
	cvErode (gray, gray, NULL, 2);
	cvDilate (gray, gray, NULL, 1);
	cvSmooth (gray, gray, CV_MEDIAN, 3, 1);

	cvCopy (gray, binary);
	//cvThreshold (gray, binary, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	contours = ContoursFinder (binary, storage);


	//key = 'a';
	menu_prev = res;
	static int prev_resp = -1;
	static int resp_counter = 0;  // at least 15 frames to conclude that the command is not a noise
	
	static int menu_option;

	prev_resp = res;
	res = PatternRecognition (gray, contours, &detection_center);
	//ConvexHull(verysmall_copy, contours);
	
	if (prev_resp == res) resp_counter++;
	else resp_counter = 0;
	
	if (resp_counter >= 5)
	{
		if (res == 0) enable_control = true;
		if (res == 4) enable_control = false;
		
	}
	cvClearMemStorage(storage);
			
	return;
}

// finds hull points
CvSeq *ContoursFinder (IplImage *edge, 	CvMemStorage *storage)
{
	CvSeq* contoursss = NULL;
		
	cvFindContours (edge, storage, &contoursss, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));
	
	return contoursss;
}

int PatternRecognition (IplImage *gray, CvSeq *contours, CvPoint *center)
{
	double result1 = 0;
	double result2 = 0;
	CvSeq *contours2 = NULL;
	
	
	CvRect rect;
	
	//IplImage *smallimg = cvCreateImage (cvSize (ANN_W, ANN_H), 8, 1);
	IplImage *binary = cvCreateImage (cvGetSize (smallimg), 8, 1);
	
	
	if (! contours) return -1;

	while (contours) 
	{
				

		result1 = fabs(cvContourArea (contours, CV_WHOLE_SEQ));

		if (result1 >= result2)
		{
			result2 = result1;
			contours2 = contours;

		}
			
		contours = contours->h_next;
	} 

	//printf ("%f area\n", fabs(cvContourArea(contours2,CV_WHOLE_SEQ)));
	if (fabs(cvContourArea(contours2,CV_WHOLE_SEQ)) <= 500) contours2 = NULL;
	
	if(contours2 == NULL)
		return -1;
	
	cvDrawContours (verysmall, contours2, CV_RGB(255,0,0), CV_RGB(0,255,0), -1, 1, CV_AA, cvPoint(0,0) );
	//cvNamedWindow ("2", CV_WINDOW_AUTOSIZE); 
	//cvMoveWindow ("2", 330, 10 );
	//cvNamedWindow ("3", CV_WINDOW_AUTOSIZE); 
	//cvMoveWindow ("3", 10, 250 );
	//cvNamedWindow ("4", CV_WINDOW_AUTOSIZE); 
	//cvMoveWindow ("4", 330, 250 );

	cvZero (gray);
	rect = cvBoundingRect(contours2, 0);
	center->x = rect.x + (rect.width / 2);
	center->y = rect.y + (rect.height / 2);
	
	//cvRectangle (img, cvPoint(rect.x, rect.y +rect.height), cvPoint(rect.x + rect.width, rect.y), CV_RGB(0,0,255), 2, 8, 0);
	cvDrawContours (gray, contours2, cvScalarAll (255), cvScalarAll (255), -1, -1, CV_AA, cvPoint(0,0) );
	//cvShowImage ("4", black);
	
	cvSetImageROI (gray, rect);
	cvResize (gray, smallimg, CV_INTER_LINEAR);
	cvResetImageROI(gray);
	//cvThreshold (smallimg, binary, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

	//cvShowImage ("2", smallimg);		
			
	uchar *ptr_small;
	int i, j;
	int k = 0;
	mlp_input[k] = -1;
	k++;
	
	for (i = 0; i < smallimg->height; i++)
	{
		for (j = 0; j < smallimg->width; j++)
		{
			ptr_small = cvPtr2D (smallimg, i, j, 0);
			if (! ptr_small[0]) mlp_input[k] = -1;
			else mlp_input[k] = 1;
			//mlp_input[k] = (double)(ptr_small[0] / 255.0);
			//printf ("\n%lf", mlp_input[k]);
			k++;
		}
	}
//	cvShowImage ("3", binary);		

	// save the information in a txt file
	//CreatePatterns(smallimg);
			
			
	//IplImage *resp = cvCreateImage (cvSize (300, 200), 8, 3);
	//cvZero (resp);
	//CvPoint p;
	//p.x = 100;
	//p.y = 100;

	double *output = MLPGestureRecog(mlp_input);
				
	int pos;
	double max = 0;
	//printf ("\n\nOutput: ");
	for (int i = 0; i < N_NEURONS_C2; i++)
	{
	//	printf ("%.2lf ", output[i]);
		if (output[i] > max)
		{
			pos = i;
			
			// noise
			if ((max >= 0) && (output[i] == max))
			{
				max = 0;
				break;
			}
			else
			{
			
				max = output[i];
			}
		}
	}
	//printf ("\n\n");
	

	char a[100]; 
	int ret = -1;
	if (max >= 0.8) 
	{
		switch (pos)
		{
			case 0: 	//sprintf (a, "OPENED");
					//cvPutText (resp, a, p , &axisfont, CV_RGB (255, 255, 0));
					ret = OPENED;
					//printf ("\nOpened");
					break;
							
			case 1: 	//sprintf (a, "CLOSED");
					ret = STANDBY;
					//cvPutText (resp, a, p, &axisfont, CV_RGB (0, 255, 255));
					//printf ("\nClosed");
					break;
	

		//	case 2: 	//sprintf (a, "CHANNEL");
//					ret = CHANNEL;
					//cvPutText (resp, a, p, &axisfont, CV_RGB (0, 255, 0));
		//			printf ("\nHorizontal Rotation");
		//			break;

		//	case 3: 	//sprintf (a, "VOLUME");
//					ret = VOLUME;
		//			//cvPutText (resp, a, p, &axisfont, CV_RGB (0, 255, 0));
					//printf ("\nVertical Rotation");
					
			//		break;



			default: break;
		}
	}

	free (output);

				
	//cvShowImage ("3", resp);
	//cvReleaseImage (&resp);
			

	//cvReleaseImage (&gray);
	
	cvReleaseImage (&binary);

	return ret;
}


int MotionDetect (int option, CvPoint center, bool reset)
{
	int var;
	static int tempx = 0;
	static int tempy = 0;
	static int frame_count = 0;
	
	int horizontal;
	int vertical;
	
	int state = -1;
	
	
	if (reset)
	{
		tempx = 0;
		tempy = 0;
	}
	
	// just to avoid hysteresis 
	if (frame_count > 3)
	{
		
		if (option == CHANNEL)
		{
			var = center.x - tempx;
			
			if (var >= 20)
			{
				//printf ("\nMOVING TO THE RIGHT");
				horizontal = RIGHT;
			}
			else if (var <= -20)
			{
				//printf ("\nMOVING TO THE LEFT");
				horizontal = LEFT;
			}
			
			else
			{
				//printf ("\nSTOPPED");
				horizontal = STOPPED;
			}
			
			tempx = center.x;

			state = StateMachine (CHANNEL, horizontal, vertical);
		}

		if (option == VOLUME)
		{
			var = center.y - tempy;
			
			if (var >= 20)
			{
			//	printf ("\nMOVING DOWN");
				vertical = DOWN;
			}
			else if (var <= -20)
			{
			//	printf ("\nMOVING UP");
				vertical = UP;
			}
			else
			{
			//	printf ("\nSTOPPED");
				vertical = STOPPED;
			}
			
			tempy = center.y;
			
			state = StateMachine (VOLUME, horizontal, vertical);

		}

	
		if (option == STANDBY)
		{
			
			var = center.x - tempx;
			
			if (var >= 10)
			{
				//printf ("\nMOVING TO THE RIGHT");
				horizontal = RIGHT;
			}
			else if (var <= -10)
			{
				//printf ("\nMOVING TO THE LEFT");
				horizontal = LEFT;
			}
			
			else
			{
				//printf ("\nSTOPPED");
				horizontal = STOPPED;
			}
			
			tempx = center.x;
			

			var = center.y - tempy;
			
			if (var >= 10)
			{
			//	printf ("\nMOVING DOWN");
				vertical = DOWN;
			}
			else if (var <= -10)
			{
			//	printf ("\nMOVING UP");
				vertical = UP;
			}
			else
			{
			//	printf ("\nSTOPPED");
				vertical = STOPPED;
			}
			
			tempy = center.y;

			
			state = StateMachine (STANDBY, horizontal, vertical);
		}

	}
	
	else
	{
		frame_count++;
	}
	
	
	return state;
}

// state machine for hand gestures sequence
int StateMachine (int option, int value1, int value2)
{
	static int state = -1;
	static int value1_prev = 0;
	static int value2_prev = 0;
	int new_state;
	static int exit_cmd = 0;
	
	if (option == CHANNEL)
	{
		
		//checking right
		if (value1 == STOPPED)
		{
			state = STOPPED;
		}
		
		else if (((value1_prev == STOPPED) ) && (value1 == RIGHT))
		{
			state = NX_CHAN;
		}

		//checking left
		else if (((value1_prev == STOPPED) ) && (value1 == LEFT))
		{
			state = PR_CHAN;
		}

		else if (value1_prev == value1)
		{
			state = STOPPED;
		}


		value1_prev = value1;
	}


	if (option == VOLUME)
	{
		
		//checking right
		if (value2 == STOPPED)
		{
			state = STOPPED;
		}
		
		else if (((value2_prev == STOPPED) ) && (value2 == DOWN))
		{
			state = DEC_VOL;
		}

		//checking left
		else if (((value2_prev == STOPPED) ) && (value2 == UP))
		{
			state = INC_VOL;
		}

		else if (value2_prev == value2)
		{
			state = STOPPED;
		}


		value2_prev = value2;
	}
	
	if (option == STANDBY)
	{
	//	printf ("\nstate: %d", exit_cmd);		
		if ((exit_cmd == 0) && (value2 == UP))
		{
			exit_cmd = 1;

			return FALSE;
		}
		
		else if ( (exit_cmd == 1) && (value1 == RIGHT))
		{
			exit_cmd = 2;

			return FALSE;
		}
		
		
		else if ((exit_cmd == 2) && (value2 == DOWN))
		{
			exit_cmd = 3;
			return FALSE;

		}
		
		else if ((exit_cmd == 3) && (value1 == LEFT))
		{
			exit_cmd = 4;
			return FALSE;
		}

		else if ((exit_cmd == 4) && (value2 == UP))
		{
			exit_cmd = 0;
			return EXIT;
		}

		else
		{
			exit_cmd = 0;
			return FALSE;
		}
	}
	
	return state;
}

float MovingAverageFilter_X (int value)
{
	float average;
	int sum = 0;
	int i;

	if (ptrx >= M_FILTER_SIZE)
	{
		ptrx = 0;
	}			

	valuesx[ptrx] = value;

	for (i = 0; i < M_FILTER_SIZE; i++)
	{
		sum = sum + valuesx[i];
	}
	
	average = sum / i;

	ptrx++;

	return average;	
}

float MovingAverageFilter_Y (int value)
{
	float average;
	int sum = 0;
	int i;

	if (ptrx >= M_FILTER_SIZE)
	{
		ptry = 0;
	}			

	valuesy[ptry] = value;

	for (i = 0; i < M_FILTER_SIZE; i++)
	{
		sum = sum + valuesy[i];
	}
	
	average = sum / i;

	ptry++;

	return average;
}
