/*
 * Skeleton egl.h to provide compatibility for early GLES 1.0
 * applications. Several early implementations included gl.h
 * in egl.h leading applications to include only egl.h
 *
 * $Revision: 6252 $ on $Date:: 2008-08-06 16:35:08 -0700 #$
 */

#ifndef __legacy_egl_h_
#define __legacy_egl_h_

#if defined(ANDROID)
#include "../../include/public/EGL/egl.h"
#include "../../include/public/GLES/gl.h"
#else
#include <EGL/egl.h>
#include <GLES/gl.h>
#endif

#endif /* __legacy_egl_h_ */
