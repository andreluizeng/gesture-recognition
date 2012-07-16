APPNAME			= gesture
DESTDIR			= ../bin

BSP_DIR			= /home/andre/bsps/mx6x/november/ltib/rootfs/home/gesture/

#TOOLCHAIN		= /opt/freescale/usr/local/gcc-4.1.2-glibc-2.5-nptl-3/arm-none-linux-gnueabi
#CROSS_COMPILE	= $(TOOLCHAIN)/bin/arm-none-linux-gnueabi-
#CC					= $(CROSS_COMPILE)gcc
#LD					= $(CROSS_COMPILE)g++
CC = gcc
LD = g++

DEL_FILE		= rm -rf
CP_FILE			= cp -rf      


CFLAGS			= -Wall -O2 -fsigned-char -march=armv7-a -mfpu=neon -mfloat-abi=softfp \
                                -I. -I../home/andre/bsps/mx6x/november/ltib/rootfs/usr/include \
				-I/home/andre/bsps/mx6x/november/ltib/rootfs/usr/include/gstreamer-0.10 -I/home/andre/bsps/mx6x/november/ltib/rootfs/usr/include/glib-2.0 -I/home/andre/bsps/mx6x/november/ltib/rootfs/usr/lib/glib-2.0/include -I/home/andre/bsps/mx6x/november/ltib/rootfs/usr/include/libxml2 \

LFLAGS			= -L../lib -lEGL -lGLESv2 -lpthread \
			-lcxcore -lcv -lcvaux -lhighgui -lml \
			-L/home/andre/bsps/mx6x/november/ltib/rootfs/usr/lib -lgstreamer-0.10 -lgobject-2.0 -lgmodule-2.0 -lgthread-2.0 -lrt -lxml2 -lglib-2.0  \


OBJECTS			= main.o glcvutils.o glcvplane.o netplayer_gst.o mlp.o


first: all

all: $(APPNAME)

$(APPNAME): $(OBJECTS)
	$(LD) $(LFLAGS) -o $(DESTDIR)/$(APPNAME) $(OBJECTS)

main.o: main.cpp
	$(LD) $(CFLAGS) -c -o main.o main.cpp

glcvutils.o: glcvutils.cpp
	$(LD) $(CFLAGS) -c -o glcvutils.o glcvutils.cpp

glcvplane.o: glcvplane.cpp
	$(LD) $(CFLAGS) -c -o glcvplane.o glcvplane.cpp

netplayer_gst.o: netplayer_gst.cpp
	$(LD) $(CFLAGS) -c -o netplayer_gst.o netplayer_gst.cpp

mlp.o: mlp.cpp
	$(LD) $(CFLAGS) -c -o mlp.o mlp.cpp



clean:
	$(DEL_FILE) $(OBJECTS)
	$(DEL_FILE) *~ core *.core

distclean: clean
	$(DEL_FILE) $(DESTDIR)/$(APPNAME)


install: all
	$(CP_FILE) $(DESTDIR)/$(APPNAME) $(BSP_DIR)
