/* -----------------------------------------------------------------------*
 *
 * (C) Copyright Advanced Micro Devices, Inc. 2002 - 2008. 
 * 
 */
//! \file                                      
//! \brief  OpenVG driver
//! \author                       
//-------------------------------------------------------------------------

#ifndef _EXT_H
#define _EXT_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _LINUX
#include <VG/openvg.h>
#else
#include "openvg.h"
#endif

#define EXT_VERSION_1_0 1

#ifndef EXT_API_CALL
#define EXT_API_CALL VG_API_CALL
#endif

typedef void * PageFlip;

#define EXT_INVALID_HANDLE ((VGHandle)0)

typedef enum {
  EXT_NO_ERROR                                 = 0,
  EXT_UNSUPPORTED_IMAGE_FORMAT_ERROR           = 0xbb01,
  EXT_ILLEGAL_ARGUMENT_ERROR                   = 0xbb02,
  EXT_BAD_HANDLE_ERROR                         = 0xbb03,
} EXTErrorCode;

#define VG_IMPLICIT_PATH_CLOSURE               0x1123
#define EXT_DRAWIMAGE                          0x9A07
#define EXT_ENABLE_DITHER                       0x9A08

EXT_API_CALL void vgDrawImageAMD( VGImage src, VGImage alpha );
EXT_API_CALL VGImage vgCreateImageAMD( VGImageFormat format, void **ptr, VGint *stride, VGint width, VGint height, VGbitfield allowedQuality );
EXT_API_CALL VGint vgGetCoordinateDataSizeAMD( VGPath path, VGint startIndex, VGint numSegments );
#ifndef VG_HDK
EXT_API_CALL void* vgCreateFontAMD( VGint size, const VGbyte *font_filename );
EXT_API_CALL VGint vgSelectFontAMD( void *font );
EXT_API_CALL VGint vgDestroyFontAMD( void *font );
EXT_API_CALL void vgDrawTextAMD( VGint posx, VGint posy, VGuint color, VGbyte* text );
#endif

EXT_API_CALL PageFlip pageFlip_create(VGImage imageA, VGImage imageB, VGint width, VGint height);
EXT_API_CALL void pageFlip_destroy(PageFlip pageFlip);
EXT_API_CALL void pageFlip_render(PageFlip pageFlip, VGfloat time);
EXT_API_CALL void putToTrash_render(PageFlip pageFlip, VGfloat time);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* #ifndef _EXT_H */
