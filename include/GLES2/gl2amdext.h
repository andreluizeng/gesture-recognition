#ifndef __gl2amdext_h_
#define __gl2amdext_h_
/*
** Copyright (C) 2008 AMD Inc.
*/

#ifndef __gl2ext_h_
#if defined(ANDROID)
#   include "../../include/public/GLES2/gl2ext.h"
#else
#   include <GLES2/gl2ext.h>
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef GL_APIENTRYP
#   define GL_APIENTRYP GL_APIENTRY*
#endif


/*------------------------------------------------------------------------*
 * AMD extension tokens and functions
 *------------------------------------------------------------------------*/

/* private extension */
/* This gives ES 1.1 ability to turn on/off multisampling using glEnable/glDisable. In ES 2.0 this is always turned on */
#define GL_MULTISAMPLE                                 0x809D

/* private extension */
/* This gives ES 1.1 ability to query state with GL_FIXED type */
#ifdef GL_GLEXT_PROTOTYPES
GL_APICALL void GL_APIENTRY glGetFixedvAMD (GLenum pname, GLfixed *params);
#endif
typedef void (GL_APIENTRYP PFNGLGETFIXEDVAMDPROC) (GLenum pname, GLfixed *params);

/* AMD_alpha_test */
#ifndef GL_AMD_alpha_test
#define GL_AMD_alpha_test 1
#define GL_ALPHA_TEST_AMD                              0x0BC0
#define GL_ALPHA_TEST_FUNC_AMD                         0x0BC1
#define GL_ALPHA_TEST_REF_AMD                          0x0BC2
/*      GL_NEVER */
/*      GL_LESS */
/*      GL_EQUAL */
/*      GL_LEQUAL */
/*      GL_GREATER */
/*      GL_NOTEQUAL */
/*      GL_GEQUAL */
/*      GL_ALWAYS */
#ifdef GL_GLEXT_PROTOTYPES
GL_APICALL void GL_APIENTRY glAlphaFuncAMD (GLenum func, GLclampf ref);
#endif
typedef void (GL_APIENTRYP PFNGLALPHAFUNCAMDPROC) (GLenum func, GLclampf ref);
#endif

/* AMD_logic_op */
#ifndef GL_AMD_logic_op
#define GL_AMD_logic_op 1
#define GL_COLOR_LOGIC_OP_AMD                          0x0BF2
#define GL_LOGIC_OP_MODE_AMD                           0x0BF0
#define GL_CLEAR_AMD                                   0x1500
#define GL_AND_AMD                                     0x1501
#define GL_AND_REVERSE_AMD                             0x1502
#define GL_COPY_AMD                                    0x1503
#define GL_AND_INVERTED_AMD                            0x1504
#define GL_NOOP_AMD                                    0x1505
#define GL_XOR_AMD                                     0x1506
#define GL_OR_AMD                                      0x1507
#define GL_NOR_AMD                                     0x1508
#define GL_EQUIV_AMD                                   0x1509
#define GL_INVERT_AMD                                  0x150A
#define GL_OR_REVERSE_AMD                              0x150B
#define GL_COPY_INVERTED_AMD                           0x150C
#define GL_OR_INVERTED_AMD                             0x150D
#define GL_NAND_AMD                                    0x150E
#define GL_SET_AMD                                     0x150F
#ifdef GL_GLEXT_PROTOTYPES
GL_APICALL void GL_APIENTRY glLogicOpAMD (GLenum op);
#endif
typedef void (GL_APIENTRYP PFNGLLOGICOPAMDPROC) (GLenum op);
#endif

/* HintTarget */
#define GL_TEXTURE_COMPRESSION_HINT                     0x84EF    // USED FOR AUTOMATIC ONLINE TEXTURE COMPRESSION

/* AMD_fog_blend */
#ifndef GL_AMD_fog_blend
#define GL_AMD_fog_blend 1
#define GL_FOG_COLOR_AMD                                0x0B66
#ifdef GL_GLEXT_PROTOTYPES
GL_APICALL void GL_APIENTRY glFogfvAMD (GLenum pname, const GLfloat *params);
#endif
typedef void (GL_APIENTRYP PFNGLFOGFVAMDPROC) (GLenum pname, const GLfloat *params);
#endif

/* AMD_writeonly_rendering */
#ifndef GL_AMD_writeonly_rendering
#define GL_AMD_writeonly_rendering 1
#define GL_WRITEONLY_RENDERING_AMD                      0x8823
#endif

/* AMD_EGL_image_external_layout_specifier */
#ifndef GL_AMD_EGL_image_external_layout_specifier
#define GL_AMD_EGL_image_external_layout_specifier 1
// YUV format
#define GL_YUV_AMD                                      0x8BC7
// YUV types
#define GL_YUYV_AMD                                     0x8BC8
#define GL_UYVY_AMD                                     0x8BC9
#define GL_NV21_AMD                                     0x8BCA
#define GL_YV12_AMD                                     0x8BCB
#define GL_NV21_TILED_AMD                               0x8BCC
// YUV samplers
#define GL_SAMPLER_EXTERNAL_YUYV_AMD                          0x8BCD
#define GL_SAMPLER_EXTERNAL_UYVY_AMD                          0x8BCE
#define GL_SAMPLER_EXTERNAL_NV21_AMD                          0x8BCF
#define GL_SAMPLER_EXTERNAL_YV12_AMD                          0x8BD0
#define GL_SAMPLER_EXTERNAL_NV21_TILED_AMD                    0x8BD1
// 0x8BD1 UNUSED!
#endif

/* EXT_blend_minmax */
#ifndef GL_EXT_blend_minmax
#define GL_EXT_blend_minmax 1
#define GL_MIN_EXT                                      0x8007
#define GL_MAX_EXT                                      0x8008
#endif

/* AMD_tiled_rendering */
#ifndef GL_AMD_tiled_rendering
#define GL_AMD_tiled_rendering 1
#ifdef GL_GLEXT_PROTOTYPES
GL_APICALL void GL_APIENTRY glActivateTileAMD(GLuint x, GLuint y, GLuint width, GLuint height,
                                              GLbitfield preserveMask);
GL_APICALL void GL_APIENTRY glEndTilingAMD(GLbitfield preserveMask);
#endif
typedef void (GL_APIENTRYP PFNGLACTIVATETILEAMD) (GLuint x, GLuint y, GLuint width, GLuint height,
                                                  GLbitfield preserveMask);
typedef void (GL_APIENTRYP PFNGLENDTILINGAMD) (GLbitfield preserveMask);
#endif

/* AMD_panel_settings */
#ifndef GL_AMD_panel_settings
#define GL_AMD_panel_settings 1
#define GL_PANEL_MODE_IFD_AMD                   0x8880 //TEMP TOKEN
#define GL_PANEL_MODE_IFH_AMD                   0x8881 //TEMP TOKEN
#define GL_PANEL_POWERFLAGS_OVERRIDE_AMD        0x8882 //TEMP TOKEN
#define GL_PANEL_AUTO_TEX_COMPRESSION_AMD       0x8883 //TEMP TOKEN
#endif

/* GL_OES_egl_image_external (DRAFT) */
#ifndef GL_OES_egl_image_external
#define GL_OES_egl_image_external 1
#define GL_TEXTURE_EXTERNAL_OES                         0x8D65
#define GL_SAMPLER_EXTERNAL_OES                         0x8D66
#define GL_TEXTURE_BINDING_EXTERNAL_OES                 0x8D67
#define GL_REQUIRED_TEXTURE_IMAGE_UNITS_OES             0x8D68
#endif

/* Private entry points for Profiler support */
#define GL_TEXTURE_WIDTH_AMD                            0x8BD2
#define GL_TEXTURE_HEIGHT_AMD                           0x8BD3
#define GL_TEXTURE_DEPTH_AMD                            0x8BD4
#define GL_TEXTURE_INTERNAL_FORMAT_AMD                  0x8BD5
#define GL_TEXTURE_FORMAT_AMD                           0x8BD6
#define GL_TEXTURE_TYPE_AMD                             0x8BD7
#define GL_TEXTURE_IMAGE_VALID_AMD                      0x8BD8
#define GL_TEXTURE_NUM_LEVELS_AMD                       0x8BD9
#define GL_TEXTURE_TARGET_AMD                           0x8BDA
#define GL_TEXTURE_OBJECT_VALID_AMD                     0x8BDB
#define GL_STATE_RESTORE                                0x8BDC

GL_APICALL void GL_APIENTRY glGetTexSubImageATI(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, GLvoid *texels);

GL_APICALL void GL_APIENTRY glGetTexturesAMD(GLuint *textures, GLint maxTextures, GLint *numTextures );
GL_APICALL void GL_APIENTRY glGetBuffersAMD(GLuint *buffers, GLint maxBuffers , GLint *numBuffers);
GL_APICALL void GL_APIENTRY glGetRenderbuffersAMD(GLuint *renderbuffers, GLint maxRenderbuffers, GLint *numRenderbuffers );
GL_APICALL void GL_APIENTRY glGetFramebuffersAMD(GLuint *framebuffers, GLint maxFramebuffers, GLint *numFramebuffers );
GL_APICALL void GL_APIENTRY glGetShadersAMD(GLuint *shaders, GLint maxShaders, GLint *numShaders );
GL_APICALL void GL_APIENTRY glGetProgramsAMD(GLuint *programs, GLint maxPrograms, GLint *numPrograms );
GL_APICALL GLboolean GL_APIENTRY glIsProgramBinaryAMD(GLuint program);
GL_APICALL void GL_APIENTRY glGetTexLevelParameterivAMD( GLuint texture, GLenum face, GLint level, GLenum pname, GLint *params );
GL_APICALL void GL_APIENTRY glTexObjectStateOverrideiAMD( GLenum target, GLenum pname, GLint param );


#ifdef __cplusplus
}
#endif

#endif /* __gl2amdext_h_ */
