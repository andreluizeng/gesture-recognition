#ifndef __glextensions_h_
#ifndef __GLEXTENSIONS_H_
#define __glextensions_h_
#define __GLEXTENSIONS_H_
/*
** Copyright (C) 2008 AMD Inc.
*/

#ifndef __glext_h_
#if defined(ANDROID)
#   include "../../include/public/GLES/glext.h"
#else
#   include <GLES/glext.h>
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if 0
//--------+---------+---------+---------+---------+---------+---------+--------

#ifndef GL_ARB_vertex_buffer_object
#define GL_ARB_vertex_buffer_object

typedef int    GLintptrARB;
typedef int    GLsizeiptrARB;

#define GL_ARRAY_BUFFER_ARB                             0x8892
#define GL_ELEMENT_ARRAY_BUFFER_ARB                     0x8893

#define GL_ARRAY_BUFFER_BINDING_ARB                     0x8894
#define GL_ELEMENT_ARRAY_BUFFER_BINDING_ARB             0x8895
#define GL_VERTEX_ARRAY_BUFFER_BINDING_ARB              0x8896
#define GL_NORMAL_ARRAY_BUFFER_BINDING_ARB              0x8897
#define GL_COLOR_ARRAY_BUFFER_BINDING_ARB               0x8898
#define GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING_ARB       0x889A

#define GL_STATIC_DRAW_ARB                              0x88E4
#define GL_DYNAMIC_DRAW_ARB                             0x88E8

#define GL_WRITE_ONLY_ARB                               0x88B9

#define GL_BUFFER_SIZE_ARB                              0x8764
#define GL_BUFFER_USAGE_ARB                             0x8765
#define GL_BUFFER_ACCESS_ARB                            0x88BB

typedef void (GL_APIENTRY *PFNGLBINDBUFFERARB) (GLenum target, GLuint buffer);
typedef void (GL_APIENTRY *PFNGLDELETEBUFFERSARB)  (GLsizei n, const GLuint *buffers);
typedef void (GL_APIENTRY *PFNGLGENBUFFERSARB)  (GLsizei n, GLuint *buffers);
typedef void (GL_APIENTRY *PFNGLBUFFERDATAARB)  (GLenum target, GLsizeiptrARB size, const GLvoid *data, GLenum usage);
typedef void (GL_APIENTRY *PFNGLBUFFERSUBDATAARB)  (GLenum target, GLintptrARB offset, GLsizeiptrARB size, const GLvoid *data);

typedef void (GL_APIENTRY *PFNGLBUFFERDATAATI)  (GLenum target, GLsizei numvertices, GLsizei vertexelemsize, GLsizeiptrARB size, const GLvoid *data, GLenum usage);
typedef void (GL_APIENTRY *PFNGLMESHLISTATI)  (GLint numlists, GLenum mode[], GLsizei count[], GLenum type[], const GLvoid *indices[]);
typedef void (GL_APIENTRY *PFNGLDRAWVERTEXBUFFEROBJECTATI)  (void);

#if defined(AEE_SIMULATOR) || defined(BREW_STATIC_APP) || defined(DYNAMIC_APP)  

#if defined(__ARMCC_VERSION) && !defined(AEE_STATIC)
PFNGLBINDBUFFERARB              const glBindBufferARB;
PFNGLDELETEBUFFERSARB           const glDeleteBuffersARB;
PFNGLGENBUFFERSARB              const glGenBuffersARB;
PFNGLBUFFERDATAARB              const glBufferDataARB;
PFNGLBUFFERSUBDATAARB           const glBufferSubDataARB;

PFNGLBUFFERDATAATI              const glBufferDataATI;
PFNGLMESHLISTATI                const glMeshListATI;
PFNGLDRAWVERTEXBUFFEROBJECTATI  const glDrawVertexBufferObjectATI;

#define glBindBufferARB (*(PFNGLBINDBUFFERARB*)&(*glBindBufferARB))
#define glDeleteBuffersARB (*(PFNGLDELETEBUFFERSARB*)&(*glDeleteBuffersARB))
#define glGenBuffersARB (*(PFNGLGENBUFFERSARB*)&(*glGenBuffersARB))
#define glBufferDataARB (*(PFNGLBUFFERDATAARB*)&(*glBufferDataARB))
#define glBufferSubDataARB (*(PFNGLBUFFERSUBDATAARB*)&(*glBufferSubDataARB))

#define glBufferDataATI (*(PFNGLBUFFERDATAATI*)&(*glBufferDataATI))
#define glMeshListATI (*(PFNGLMESHLISTATI*)&(*glMeshListATI))
#define glDrawVertexBufferObjectATI (*(PFNGLDRAWVERTEXBUFFEROBJECTATI*)&(*glDrawVertexBufferObjectATI))

#else   //defined(__ARMCC_VERSION) && !defined(AEE_STATIC)
PFNGLBINDBUFFERARBPROC              glBindBufferARB;
PFNGLDELETEBUFFERSARBPROC           glDeleteBuffersARB;
PFNGLGENBUFFERSARBPROC              glGenBuffersARB;
PFNGLBUFFERDATAARBPROC              glBufferDataARB;
PFNGLBUFFERSUBDATAARBPROC           glBufferSubDataARB;

PFNGLBUFFERDATAATIPROC              glBufferDataATI;
PFNGLMESHLISTATIPROC                glMeshListATI;
PFNGLDRAWVERTEXBUFFEROBJECTATIPROC  glDrawVertexBufferObjectATI;

#endif

#else

GL_API void GL_APIENTRY glBindBufferARB(GLenum target, GLuint buffer);
GL_API void GL_APIENTRY glDeleteBuffersARB(GLsizei n, const GLuint *buffers);
GL_API void GL_APIENTRY glGenBuffersARB(GLsizei n, GLuint *buffers);
GL_API void GL_APIENTRY glBufferDataARB(GLenum target, GLsizeiptrARB size, const GLvoid *data, GLenum usage);
GL_API void GL_APIENTRY glBufferSubDataARB(GLenum target, GLintptrARB offset, GLsizeiptrARB size, const GLvoid *data);

GL_API void GL_APIENTRY glBufferDataATI(GLenum target, GLsizei numvertices, GLsizei vertexelemsize, GLsizeiptrARB size, const GLvoid *data, GLenum usage);
GL_API void GL_APIENTRY glMeshListATI(GLint numlists, GLenum mode[], GLsizei count[], GLenum type[], const GLvoid *indices[]);
GL_API void GL_APIENTRY glDrawVertexBufferObjectATI(void);

#endif

#endif  /* GL_ARB_vertex_buffer_object */

//--------+---------+---------+---------+---------+---------+---------+--------

#ifndef GL_ATI_compressed_texture_atitc
#define GL_ATI_compressed_texture_atitc

#define GL_COMPRESSED_RGB_ATI_TC                        0x8C92
#define GL_COMPRESSED_RGBA_ATI_TC                       0x8C93

#endif  /* GL_ATI_compressed_texture_atitc */

//--------+---------+---------+---------+---------+---------+---------+--------

#ifndef GL_ATI_extended_texture_coordinate_data_formats
#define GL_ATI_extended_texture_coordinate_data_formats

#define GL_BYTE_4_4_ATI                                 0x8C96
#define GL_SHORT_4_12_ATI                               0x8C97
#define GL_SHORT_8_8_ATI                                0x8C98

#endif  /* GL_ATI_extended_texture_coordinate_data_formats */

/* Changes to the Imaging Subset */
#define GL_CONSTANT_COLOR                               0x8001
#define GL_ONE_MINUS_CONSTANT_COLOR                     0x8002
#define GL_CONSTANT_ALPHA                               0x8003
#define GL_ONE_MINUS_CONSTANT_ALPHA                     0x8004
#define GL_BLEND_COLOR                                  0x8005

//--------+---------+---------+---------+---------+---------+---------+--------

#ifndef GL_EXT_blend_minmax
#define GL_EXT_blend_minmax

#define GL_MIN_EXT                                      0x8007
#define GL_MAX_EXT                                      0x8008

#endif  /* GL_EXT_blend_minmax */

//--------+---------+---------+---------+---------+---------+---------+--------

#ifndef GL_ATI_vertex_assembly_program
#define GL_ATI_vertex_assembly_program

#define GL_VERTEX_PROGRAM_ATI                           0x8620
#define GL_PROGRAM_FORMAT_ASCII_ATI                     0x8875
#define GL_VERTEX_PROGRAM_ERROR_STRING_ATI              0x8874

GL_API void GL_APIENTRY glProgramStringATI(GLenum target, GLenum format, GLsizei len, const GLvoid *string);
GL_API void GL_APIENTRY glBindProgramATI(GLenum target, GLuint program);
GL_API void GL_APIENTRY glDeleteProgramsATI(GLsizei n, const GLuint *programs);
GL_API void GL_APIENTRY glGenProgramsATI(GLsizei n, GLuint *programs);
GL_API void GL_APIENTRY glProgramConstantfvATI(GLenum target, GLuint startregister, GLuint vector4fcount, const GLfloat *params);
GL_API void GL_APIENTRY glProgramConstantxvATI(GLenum target, GLuint startregister, GLuint vector4fcount, const GLfixed *params);
GL_API void GL_APIENTRY glGetProgramConstantfvATI(GLenum target, GLuint startregister, GLuint vector4fcount, GLfloat *params);
GL_API void GL_APIENTRY glGetProgramConstantxvATI(GLenum target, GLuint startregister, GLuint vector4fcount, GLfixed *params);
GL_API GLboolean GL_APIENTRY glIsProgramATI(GLuint program);

#endif /* GL_ATI_vertex_assembly_program */

//--------+---------+---------+---------+---------+---------+---------+--------

#endif

#ifdef __cplusplus
}
#endif


#endif  // __glextensions_h_
#endif  // __GLEXTENSIONS_H_
