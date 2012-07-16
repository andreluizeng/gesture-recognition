#ifndef _FTVG_UTILS_H
#define _FTVG_UTILS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <openvg.h>

void* feCreateFont(VGint size, const VGbyte *font_filename);
VGint feSelectFont(void *font);
VGint feDestroyFont(void *font);
void feDrawText(VGint posx, VGint posy, VGuint color, VGbyte* text);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
