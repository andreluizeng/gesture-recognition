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

#ifndef __NETPLAYER_GST_H__
#define __NETPLAYER_GST_H__

#include <gst/gst.h>
#include <gst/interfaces/xoverlay.h>

extern GstElement *pipeline;
extern GstElement *bin;
extern GMainLoop *loop;
extern GstBus *bus;

void 		netplayer_gst_init 		(int *argc, char ***argv);
void 		netplayer_gst_play 		(const char *uri, GCallback handoffHandlerFunc);
void 		netplayer_gst_stop 		(void);
void 		netplayer_gst_pause 		(void);
void		netplayer_gst_resume 		(void);
void		netplayer_gst_play_init 	(void);
void   		netplayer_gst_seek_absolute  	(guint64 value);
extern void 	netplayer_gst_fast_forward 	(int value);
extern void 	netplayer_gst_fast_rewind 	(int value);

extern gboolean bus_call 		(GstBus *bus, GstMessage *msg, gpointer data);
	
guint64 netplayer_gst_query_duration 	(void);
guint64 netplayer_gst_query_position 	(void);


gint get_pad_width (GstPad* pVideoPad);
gint get_pad_height (GstPad* pVideoPad);





#endif
