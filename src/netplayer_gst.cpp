/*
 * Copyright (C) 2010 Andre Silva.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "netplayer_gst.h"


GstElement *pipeline;
GstElement *bin;
GstElement *videosink;


//GstSeekFlags seek_flags = GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT;
int seek_flags = GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT;


void netplayer_gst_init (int *argc, char ***argv)
{
    	gst_init (argc, argv);

	return;
}


void netplayer_gst_play (const char *uri, GCallback handoffHandlerFunc)
{
	GstElement* pFfConv = NULL;
	GstElement* pSinkBin = NULL;
	GstPad*     pFfConvSinkPad = NULL;
	GstPad*     pSinkPad = NULL;



    	netplayer_gst_stop ();

    	pipeline = gst_pipeline_new ("gst-player");

    	bin = gst_element_factory_make ("playbin2", "bin");
    	videosink = gst_element_factory_make ("fakesink", "videosink");
	//videosink = gst_element_factory_make ("mfw_v4lsink", "videosink");
	g_object_set (G_OBJECT (videosink), "sync", TRUE, "signal-handoffs", TRUE, NULL);
	g_signal_connect (videosink, "handoff", handoffHandlerFunc, NULL);

    	g_object_set (G_OBJECT (bin), "video-sink", videosink, NULL);
	g_object_set (G_OBJECT (bin), "volume", 0.5, NULL);

        bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
        gst_bus_add_watch (bus, bus_call, loop);
	gst_object_unref (bus);
       	g_object_set (G_OBJECT (bin), "uri", uri, NULL);
	
	
	// colorspace conversion
	// it is add in a new bin, and then this bin is added to the first one (above)
	pFfConv = gst_element_factory_make ("ffmpegcolorspace", "ffconv");
	if (!pFfConv)
	{
		printf("Couldn't create pFfConv\n");
	}
	
	
	//Put the fake sink and caps fiter into a single bin
	pSinkBin = gst_bin_new("SinkBin");
	if (!pSinkBin)
	{
		printf("Couldn't create pSinkBin\n");
	}
	gst_bin_add_many (GST_BIN (pSinkBin), pFfConv, videosink, NULL);
 	gst_element_link_filtered (pFfConv, videosink, gst_caps_new_simple ("video/x-raw-rgb","bpp",G_TYPE_INT,16, NULL));

	//In order to link the sink bin to the play been we have to create
	//a ghost pad that points to the capsfilter sink pad
	pFfConvSinkPad = gst_element_get_static_pad(pFfConv, "sink");
	if (!pFfConvSinkPad)
	{
		printf("Couldn't create pFfCovSinkPad\n");
	}

	pSinkPad = gst_ghost_pad_new( "sink", pFfConvSinkPad );
	if (!pSinkPad)
	{
		printf("Couldn't create pSinkPad\n");
	}
	gst_element_add_pad(pSinkBin, pSinkPad);
        gst_object_unref( pFfConvSinkPad);
 
	// force the SinkBin to be used as the video sink
	g_object_set (G_OBJECT (bin), "video-sink", pSinkBin, NULL);

	gst_bin_add (GST_BIN (pipeline), bin);
	
    	gst_element_set_state (pipeline, GST_STATE_PAUSED);
	
	return;
}

void netplayer_gst_stop (void)
{
    	if (pipeline)
    	{
        	gst_element_set_state (pipeline, GST_STATE_NULL);
        	gst_object_unref (GST_OBJECT (pipeline));
        	pipeline = NULL;
    	}

	return;
}

void netplayer_gst_pause (void)
{
    	gst_element_set_state (pipeline, GST_STATE_PAUSED);

	return;
}

void netplayer_gst_resume (void)
{
    	gst_element_set_state (pipeline, GST_STATE_PLAYING);

	return;
}

guint64 netplayer_gst_query_duration (void)
{
    	GstFormat format = GST_FORMAT_TIME;
    	gint64 cur = 0;

    	gst_element_query_duration (pipeline, &format, &cur);

	if (format != GST_FORMAT_TIME)
        	return GST_CLOCK_TIME_NONE;

    	return cur;
}

guint64 netplayer_gst_query_position (void)
{
            GstFormat format = GST_FORMAT_TIME;
            gint64 cur = 0;
        
            gst_element_query_position (pipeline, &format, &cur);

        if (format != GST_FORMAT_TIME)
                return GST_CLOCK_TIME_NONE;

            return cur;
}

void netplayer_gst_seek_absolute (guint64 value)
{
	
    gst_element_seek (pipeline, 
		      1.0,
                      GST_FORMAT_TIME,
                      (GstSeekFlags) seek_flags,
                      GST_SEEK_TYPE_SET, 
		      value,
                      GST_SEEK_TYPE_NONE, 
		      GST_CLOCK_TIME_NONE);
}

void netplayer_gst_fast_forward (int value)
{
	gint64 x;

	x = value * 1000000000;

	netplayer_gst_pause ();

    	gst_element_seek (	bin,
				1.0,
                	      	GST_FORMAT_TIME,
                      		GST_SEEK_FLAG_FLUSH,
                      		GST_SEEK_TYPE_CUR, x,
                      		GST_SEEK_TYPE_NONE,
				GST_SEEK_TYPE_NONE
			);

	netplayer_gst_resume ();
}


void netplayer_gst_fast_rewind (int value)
{
	gint64 x;

	x = value * 1000000000;
	x = -x;

	netplayer_gst_pause ();

    	gst_element_seek (	bin,
				1.0,
                	      	GST_FORMAT_TIME,
                      		GST_SEEK_FLAG_FLUSH,
                      		GST_SEEK_TYPE_CUR, x,
                      		GST_SEEK_TYPE_NONE,
				GST_SEEK_TYPE_NONE
			);

	netplayer_gst_resume ();
}

//gets the pad width
gint get_pad_width (GstPad* pVideoPad)
{
	gint iWidth = 0;
	GstCaps* pCaps = NULL;
	GstStructure* pStructure = NULL;

	pCaps = gst_pad_get_negotiated_caps (pVideoPad);
	if (pCaps)
	{
		pStructure = gst_caps_get_structure (pCaps, 0);
		gst_structure_get_int (pStructure, "width", &iWidth);
	}
	else
		g_print ("gst_pad_width() - Could not get caps for the pad!\n");

	return iWidth;
}
//gets the pad height
gint get_pad_height (GstPad* pVideoPad)
{
	gint iHeight = 0;
	GstCaps* pCaps = NULL;
	GstStructure* pStructure = NULL;

	pCaps = gst_pad_get_negotiated_caps (pVideoPad);
	if (pCaps)
	{
		pStructure = gst_caps_get_structure (pCaps, 0);
		gst_structure_get_int (pStructure, "height", &iHeight);
	}
	else
		g_print ("gst_pad_height() - Could not get caps for the pad!\n");

	return iHeight;
}


