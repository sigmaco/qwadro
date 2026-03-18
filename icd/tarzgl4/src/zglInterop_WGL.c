/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#define EXPORT_GL_SYMBOLS

#include "zglDefs.h"
#include "zglCommands.h"
#include "zglObjects.h"

//#pragma comment(lib, "opengl32")

_ZGL afxString const zgl2Signature = AFX_STRING(
    "           :::     :::::::::   ::::::::  :::              :::  ::::::::        \n"
    "          :+:      :+:    :+: :+:    :+: :+:             :+:  :+:    :+:       \n"
    "         +:+ +:+   +:+    +:+ +:+        +:+            +:+         +:+        \n"
    "        +#+  +:+   +#+    +:+ :#:        +#+           +#+        +#+          \n"
    "       +#+#+#+#+#+ +#+    +#+ +#+   +#+# +#+          +#+       +#+            \n"
    "             #+#   #+#    #+# #+#    #+# #+#         #+#       #+#             \n"
    "             ###   #########   ########  ########## ###       ##########       \n"
    "                                                                               \n"
    "              Q W A D R O   E X E C U T I O N   E C O S Y S T E M              \n"
    "                                                                               \n"
    "                               Public Test Build                               \n"
    "                           (c) 2017 SIGMA FEDERATION                           \n"
    "                                www.sigmaco.org                                \n"
    "                                                                               \n"
);


_ZGL afxBool vgiWndClssRegistered = FALSE;
_ZGL WNDCLASSEXA vgiWndClss = { 0 };

_ZGL const char *glVmtNames[] =
{
    // 100
    "glCullFace", // 0
    "glFrontFace",
    "glHint",
    "glLineWidth",
    "glPointSize",
    "glPolygonMode",
    "glScissor",
    "glTexParameterf",
    "glTexParameterfv",
    "glTexParameteri",
    "glTexParameteriv",
    "glTexImage1D",
    "glTexImage2D",
    "glDrawBuffer",
    "glClear",
    "glClearColor",
    "glClearStencil",
    "glClearDepth",
    "glStencilMask",
    "glColorMask",
    "glDepthMask",
    "glDisable",
    "glEnable",
    "glFinish",
    "glFlush",
    "glBlendFunc",
    "glLogicOp",
    "glStencilFunc",
    "glStencilOp",
    "glDepthFunc",
    "glPixelStoref",
    "glPixelStorei",
    "glReadBuffer",
    "glReadPixels",
    "glGetBooleanv",
    "glGetDoublev",
    "glGetError",
    "glGetFloatv",
    "glGetIntegerv",
    "glGetString",
    "glGetTexImage",
    "glGetTexParameterfv",
    "glGetTexParameteriv",
    "glGetTexLevelParameterfv",
    "glGetTexLevelParameteriv",
    "glIsEnabled",
    "glDepthRange",
    "glViewport", // 47
    // 110
    "glDrawArrays", // 0
    "glDrawElements",
    "glGetPointerv",
    "glPolygonOffset",
    "glCopyTexImage1D",
    "glCopyTexImage2D",
    "glCopyTexSubImage1D",
    "glCopyTexSubImage2D",
    "glTexSubImage1D",
    "glTexSubImage2D",
    "glBindTexture",
    "glDeleteTextures",
    "glGenTextures",
    "glIsTexture", // 13
    // 120
    "glDrawRangeElements", // 0
    "glTexImage3D",
    "glTexSubImage3D",
    "glCopyTexSubImage3D", // 3
    // 130
    "glActiveTexture", // 0
    "glSampleCoverage",
    "glCompressedTexImage3D",
    "glCompressedTexImage2D",
    "glCompressedTexImage1D",
    "glCompressedTexSubImage3D",
    "glCompressedTexSubImage2D",
    "glCompressedTexSubImage1D",
    "glGetCompressedTexImage", // 8
    // 140
    "glBlendFuncSeparate", // 0
    "glMultiDrawArrays",
    "glMultiDrawElements",
    "glPointParameterf",
    "glPointParameterfv",
    "glPointParameteri",
    "glPointParameteriv",
    "glBlendColor",
    "glBlendEquation", // 8
    // 150
    "glGenQueries", // 0
    "glDeleteQueries",
    "glIsQuery",
    "glBeginQuery",
    "glEndQuery",
    "glGetQueryiv",
    "glGetQueryObjectiv",
    "glGetQueryObjectuiv",
    "glBindBuffer",
    "glDeleteBuffers",
    "glGenBuffers",
    "glIsBuffer",
    "glBufferData",
    "glBufferSubData",
    "glGetBufferSubData",
    "glMapBuffer",
    "glUnmapBuffer",
    "glGetBufferParameteriv",
    "glGetBufferPointerv", // 18
    // 200
    "glBlendEquationSeparate", // 0
    "glDrawBuffers",
    "glStencilOpSeparate",
    "glStencilFuncSeparate",
    "glStencilMaskSeparate",
    "glAttachShader",
    "glBindAttribLocation",
    "glCompileShader",
    "glCreateProgram",
    "glCreateShader",
    "glDeleteProgram",
    "glDeleteShader",
    "glDetachShader",
    "glDisableVertexAttribArray",
    "glEnableVertexAttribArray",
    "glGetActiveAttrib",
    "glGetActiveUniform",
    "glGetAttachedShaders",
    "glGetAttribLocation",
    "glGetProgramiv",
    "glGetProgramInfoLog",
    "glGetShaderiv",
    "glGetShaderInfoLog",
    "glGetShaderSource",
    "glGetUniformLocation",
    "glGetUniformfv",
    "glGetUniformiv",
    "glGetVertexAttribdv",
    "glGetVertexAttribfv",
    "glGetVertexAttribiv",
    "glGetVertexAttribPointerv",
    "glIsProgram",
    "glIsShader",
    "glLinkProgram",
    "glShaderSource",
    "glUseProgram",
    "glUniform1f",
    "glUniform2f",
    "glUniform3f",
    "glUniform4f",
    "glUniform1i",
    "glUniform2i",
    "glUniform3i",
    "glUniform4i",
    "glUniform1fv",
    "glUniform2fv",
    "glUniform3fv",
    "glUniform4fv",
    "glUniform1iv",
    "glUniform2iv",
    "glUniform3iv",
    "glUniform4iv",
    "glUniformMatrix2fv",
    "glUniformMatrix3fv",
    "glUniformMatrix4fv",
    "glValidateProgram",
    "glVertexAttrib1d",
    "glVertexAttrib1dv",
    "glVertexAttrib1f",
    "glVertexAttrib1fv",
    "glVertexAttrib1s",
    "glVertexAttrib1sv",
    "glVertexAttrib2d",
    "glVertexAttrib2dv",
    "glVertexAttrib2f",
    "glVertexAttrib2fv",
    "glVertexAttrib2s",
    "glVertexAttrib2sv",
    "glVertexAttrib3d",
    "glVertexAttrib3dv",
    "glVertexAttrib3f",
    "glVertexAttrib3fv",
    "glVertexAttrib3s",
    "glVertexAttrib3sv",
    "glVertexAttrib4Nbv",
    "glVertexAttrib4Niv",
    "glVertexAttrib4Nsv",
    "glVertexAttrib4Nub",
    "glVertexAttrib4Nubv",
    "glVertexAttrib4Nuiv",
    "glVertexAttrib4Nusv",
    "glVertexAttrib4bv",
    "glVertexAttrib4d",
    "glVertexAttrib4dv",
    "glVertexAttrib4f",
    "glVertexAttrib4fv",
    "glVertexAttrib4iv",
    "glVertexAttrib4s",
    "glVertexAttrib4sv",
    "glVertexAttrib4ubv",
    "glVertexAttrib4uiv",
    "glVertexAttrib4usv",
    "glVertexAttribPointer", // 92
    // 210
    "glUniformMatrix2x3fv", // 0
    "glUniformMatrix3x2fv",
    "glUniformMatrix2x4fv",
    "glUniformMatrix4x2fv",
    "glUniformMatrix3x4fv",
    "glUniformMatrix4x3fv", // 5
    // 300
    "glColorMaski", // 0
    "glGetBooleani_v",
    "glGetIntegeri_v",
    "glEnablei",
    "glDisablei",
    "glIsEnabledi",
    "glBeginTransformFeedback",
    "glEndTransformFeedback",
    "glBindBufferRange",
    "glBindBufferBase",
    "glTransformFeedbackVaryings",
    "glGetTransformFeedbackVarying",
    "glClampColor",
    "glBeginConditionalRender",
    "glEndConditionalRender",
    "glVertexAttribIPointer",
    "glGetVertexAttribIiv",
    "glGetVertexAttribIuiv",
    "glVertexAttribI1i",
    "glVertexAttribI2i",
    "glVertexAttribI3i",
    "glVertexAttribI4i",
    "glVertexAttribI1ui",
    "glVertexAttribI2ui",
    "glVertexAttribI3ui",
    "glVertexAttribI4ui",
    "glVertexAttribI1iv",
    "glVertexAttribI2iv",
    "glVertexAttribI3iv",
    "glVertexAttribI4iv",
    "glVertexAttribI1uiv",
    "glVertexAttribI2uiv",
    "glVertexAttribI3uiv",
    "glVertexAttribI4uiv",
    "glVertexAttribI4bv",
    "glVertexAttribI4sv",
    "glVertexAttribI4ubv",
    "glVertexAttribI4usv",
    "glGetUniformuiv",
    "glBindFragDataLocation",
    "glGetFragDataLocation",
    "glUniform1ui",
    "glUniform2ui",
    "glUniform3ui",
    "glUniform4ui",
    "glUniform1uiv",
    "glUniform2uiv",
    "glUniform3uiv",
    "glUniform4uiv",
    "glTexParameterIiv",
    "glTexParameterIuiv",
    "glGetTexParameterIiv",
    "glGetTexParameterIuiv",
    "glClearBufferiv",
    "glClearBufferuiv",
    "glClearBufferfv",
    "glClearBufferfi",
    "glGetStringi",
    "glIsRenderbuffer",
    "glBindRenderbuffer",
    "glDeleteRenderbuffers",
    "glGenRenderbuffers",
    "glRenderbufferStorage",
    "glGetRenderbufferParameteriv",
    "glIsFramebuffer",
    "glBindFramebuffer",
    "glDeleteFramebuffers",
    "glGenFramebuffers",
    "glCheckFramebufferStatus",
    "glFramebufferTexture1D",
    "glFramebufferTexture2D",
    "glFramebufferTexture3D",
    "glFramebufferRenderbuffer",
    "glGetFramebufferAttachmentParameteriv",
    "glGenerateMipmap",
    "glBlitFramebuffer",
    "glRenderbufferStorageMultisample",
    "glFramebufferTextureLayer",
    "glMapBufferRange",
    "glFlushMappedBufferRange",
    "glBindVertexArray",
    "glDeleteVertexArrays",
    "glGenVertexArrays",
    "glIsVertexArray", // 83
    // 310
    "glDrawArraysInstanced", // 0
    "glDrawElementsInstanced",
    "glTexBuffer",
    "glPrimitiveRestartIndex",
    "glCopyBufferSubData",
    "glGetUniformIndices",
    "glGetActiveUniformsiv",
    "glGetActiveUniformName",
    "glGetUniformBlockIndex",
    "glGetActiveUniformBlockiv",
    "glGetActiveUniformBlockName",
    "glUniformBlockBinding", // 11
    // 320
    "glDrawElementsBaseVertex", // 0
    "glDrawRangeElementsBaseVertex",
    "glDrawElementsInstancedBaseVertex",
    "glMultiDrawElementsBaseVertex",
    "glProvokingVertex",
    "glFenceSync",
    "glIsSync",
    "glDeleteSync",
    "glClientWaitSync",
    "glWaitSync",
    "glGetInteger64v",
    "glGetSynciv",
    "glGetInteger64i_v",
    "glGetBufferParameteri64v",
    "glFramebufferTexture",
    "glTexImage2DMultisample",
    "glTexImage3DMultisample",
    "glGetMultisamplefv",
    "glSampleMaski", // 18
    // 330
    "glBindFragDataLocationIndexed", // 0
    "glGetFragDataIndex",
    "glGenSamplers",
    "glDeleteSamplers",
    "glIsSampler",
    "glBindSampler",
    "glSamplerParameteri",
    "glSamplerParameteriv",
    "glSamplerParameterf",
    "glSamplerParameterfv",
    "glSamplerParameterIiv",
    "glSamplerParameterIuiv",
    "glGetSamplerParameteriv",
    "glGetSamplerParameterIiv",
    "glGetSamplerParameterfv",
    "glGetSamplerParameterIuiv",
    "glQueryCounter",
    "glGetQueryObjecti64v",
    "glGetQueryObjectui64v",
    "glVertexAttribDivisor",
    "glVertexAttribP1ui",
    "glVertexAttribP1uiv",
    "glVertexAttribP2ui",
    "glVertexAttribP2uiv",
    "glVertexAttribP3ui",
    "glVertexAttribP3uiv",
    "glVertexAttribP4ui",
    "glVertexAttribP4uiv", // 27
    // 400
    "glMinSampleShading", // 0
    "glBlendEquationi",
    "glBlendEquationSeparatei",
    "glBlendFunci",
    "glBlendFuncSeparatei",
    "glDrawArraysIndirect",
    "glDrawElementsIndirect",
    "glUniform1d",
    "glUniform2d",
    "glUniform3d",
    "glUniform4d",
    "glUniform1dv",
    "glUniform2dv",
    "glUniform3dv",
    "glUniform4dv",
    "glUniformMatrix2dv",
    "glUniformMatrix3dv",
    "glUniformMatrix4dv",
    "glUniformMatrix2x3dv",
    "glUniformMatrix2x4dv",
    "glUniformMatrix3x2dv",
    "glUniformMatrix3x4dv",
    "glUniformMatrix4x2dv",
    "glUniformMatrix4x3dv",
    "glGetUniformdv",
    "glGetSubroutineUniformLocation",
    "glGetSubroutineIndex",
    "glGetActiveSubroutineUniformiv",
    "glGetActiveSubroutineUniformName",
    "glGetActiveSubroutineName",
    "glUniformSubroutinesuiv",
    "glGetUniformSubroutineuiv",
    "glGetProgramStageiv",
    "glPatchParameteri",
    "glPatchParameterfv",
    "glBindTransformFeedback",
    "glDeleteTransformFeedbacks",
    "glGenTransformFeedbacks",
    "glIsTransformFeedback",
    "glPauseTransformFeedback",
    "glResumeTransformFeedback",
    "glDrawTransformFeedback",
    "glDrawTransformFeedbackStream",
    "glBeginQueryIndexed",
    "glEndQueryIndexed",
    "glGetQueryIndexediv", // 45
    // 410
    "glReleaseShaderCompiler", // 0
    "glShaderBinary",
    "glGetShaderPrecisionFormat",
    "glDepthRangef",
    "glClearDepthf",
    "glGetProgramBinary",
    "glProgramBinary",
    "glProgramParameteri",
    "glUseProgramStages",
    "glActiveShaderProgram",
    "glCreateShaderProgramv",
    "glBindProgramPipeline",
    "glDeleteProgramPipelines",
    "glGenProgramPipelines",
    "glIsProgramPipeline",
    "glGetProgramPipelineiv",
    "glProgramUniform1i",
    "glProgramUniform1iv",
    "glProgramUniform1f",
    "glProgramUniform1fv",
    "glProgramUniform1d",
    "glProgramUniform1dv",
    "glProgramUniform1ui",
    "glProgramUniform1uiv",
    "glProgramUniform2i",
    "glProgramUniform2iv",
    "glProgramUniform2f",
    "glProgramUniform2fv",
    "glProgramUniform2d",
    "glProgramUniform2dv",
    "glProgramUniform2ui",
    "glProgramUniform2uiv",
    "glProgramUniform3i",
    "glProgramUniform3iv",
    "glProgramUniform3f",
    "glProgramUniform3fv",
    "glProgramUniform3d",
    "glProgramUniform3dv",
    "glProgramUniform3ui",
    "glProgramUniform3uiv",
    "glProgramUniform4i",
    "glProgramUniform4iv",
    "glProgramUniform4f",
    "glProgramUniform4fv",
    "glProgramUniform4d",
    "glProgramUniform4dv",
    "glProgramUniform4ui",
    "glProgramUniform4uiv",
    "glProgramUniformMatrix2fv",
    "glProgramUniformMatrix3fv",
    "glProgramUniformMatrix4fv",
    "glProgramUniformMatrix2dv",
    "glProgramUniformMatrix3dv",
    "glProgramUniformMatrix4dv",
    "glProgramUniformMatrix2x3fv",
    "glProgramUniformMatrix3x2fv",
    "glProgramUniformMatrix2x4fv",
    "glProgramUniformMatrix4x2fv",
    "glProgramUniformMatrix3x4fv",
    "glProgramUniformMatrix4x3fv",
    "glProgramUniformMatrix2x3dv",
    "glProgramUniformMatrix3x2dv",
    "glProgramUniformMatrix2x4dv",
    "glProgramUniformMatrix4x2dv",
    "glProgramUniformMatrix3x4dv",
    "glProgramUniformMatrix4x3dv",
    "glValidateProgramPipeline",
    "glGetProgramPipelineInfoLog",
    "glVertexAttribL1d",
    "glVertexAttribL2d",
    "glVertexAttribL3d",
    "glVertexAttribL4d",
    "glVertexAttribL1dv",
    "glVertexAttribL2dv",
    "glVertexAttribL3dv",
    "glVertexAttribL4dv",
    "glVertexAttribLPointer",
    "glGetVertexAttribLdv",
    "glViewportArrayv",
    "glViewportIndexedf",
    "glViewportIndexedfv",
    "glScissorArrayv",
    "glScissorIndexed",
    "glScissorIndexedv",
    "glDepthRangeArrayv",
    "glDepthRangeIndexed",
    "glGetFloati_v",
    "glGetDoublei_v", // 87
    // 420
    "glDrawArraysInstancedBaseInstance", // 0
    "glDrawElementsInstancedBaseInstance",
    "glDrawElementsInstancedBaseVertexBaseInstance",
    "glGetInternalformativ",
    "glGetActiveAtomicCounterBufferiv",
    "glBindImageTexture",
    "glMemoryBarrier",
    "glTexStorage1D",
    "glTexStorage2D",
    "glTexStorage3D",
    "glDrawTransformFeedbackInstanced",
    "glDrawTransformFeedbackStreamInstanced", // 11
    // 430
    "glClearBufferData", // 0
    "glClearBufferSubData",
    "glDispatchCompute",
    "glDispatchComputeIndirect",
    "glCopyImageSubData",
    "glFramebufferParameteri",
    "glGetFramebufferParameteriv",
    "glGetInternalformati64v",
    "glInvalidateTexSubImage",
    "glInvalidateTexImage",
    "glInvalidateBufferSubData",
    "glInvalidateBufferData",
    "glInvalidateFramebuffer",
    "glInvalidateSubFramebuffer",
    "glMultiDrawArraysIndirect",
    "glMultiDrawElementsIndirect",
    "glGetProgramInterfaceiv",
    "glGetProgramResourceIndex",
    "glGetProgramResourceName",
    "glGetProgramResourceiv",
    "glGetProgramResourceLocation",
    "glGetProgramResourceLocationIndex",
    "glShaderStorageBlockBinding",
    "glTexBufferRange",
    "glTexStorage2DMultisample",
    "glTexStorage3DMultisample",
    "glTextureView",
    "glBindVertexBuffer",
    "glVertexAttribFormat",
    "glVertexAttribIFormat",
    "glVertexAttribLFormat",
    "glVertexAttribBinding",
    "glVertexBindingDivisor",
    "glDebugMessageControl",
    "glDebugMessageInsert",
    "glDebugMessageCallback",
    "glGetDebugMessageLog",
    "glPushDebugGroup",
    "glPopDebugGroup",
    "glObjectLabel",
    "glGetObjectLabel",
    "glObjectPtrLabel",
    "glGetObjectPtrLabel", // 42
    // 440
    "glBufferStorage", // 0
    "glClearTexImage",
    "glClearTexSubImage",
    "glBindBuffersBase",
    "glBindBuffersRange",
    "glBindTextures",
    "glBindSamplers",
    "glBindImageTextures",
    "glBindVertexBuffers", // 8
    // 450
    "glClipControl", // 0
    "glCreateTransformFeedbacks",
    "glTransformFeedbackBufferBase",
    "glTransformFeedbackBufferRange",
    "glGetTransformFeedbackiv",
    "glGetTransformFeedbacki_v",
    "glGetTransformFeedbacki64_v",
    "glCreateBuffers",
    "glNamedBufferStorage",
    "glNamedBufferData",
    "glNamedBufferSubData",
    "glCopyNamedBufferSubData",
    "glClearNamedBufferData",
    "glClearNamedBufferSubData",
    "glMapNamedBuffer",
    "glMapNamedBufferRange",
    "glUnmapNamedBuffer",
    "glFlushMappedNamedBufferRange",
    "glGetNamedBufferParameteriv",
    "glGetNamedBufferParameteri64v",
    "glGetNamedBufferPointerv",
    "glGetNamedBufferSubData",
    "glCreateFramebuffers",
    "glNamedFramebufferRenderbuffer",
    "glNamedFramebufferParameteri",
    "glNamedFramebufferTexture",
    "glNamedFramebufferTextureLayer",
    "glNamedFramebufferCanvas",
    "glNamedFramebufferCanvass",
    "glNamedFramebufferReadBuffer",
    "glInvalidateNamedFramebufferData",
    "glInvalidateNamedFramebufferSubData",
    "glClearNamedFramebufferiv",
    "glClearNamedFramebufferuiv",
    "glClearNamedFramebufferfv",
    "glClearNamedFramebufferfi",
    "glBlitNamedFramebuffer",
    "glCheckNamedFramebufferStatus",
    "glGetNamedFramebufferParameteriv",
    "glGetNamedFramebufferAttachmentParameteriv",
    "glCreateRenderbuffers",
    "glNamedRenderbufferStorage",
    "glNamedRenderbufferStorageMultisample",
    "glGetNamedRenderbufferParameteriv",
    "glCreateTextures",
    "glTextureBuffer",
    "glTextureBufferRange",
    "glTextureStorage1D",
    "glTextureStorage2D",
    "glTextureStorage3D",
    "glTextureStorage2DMultisample",
    "glTextureStorage3DMultisample",
    "glTextureSubImage1D",
    "glTextureSubImage2D",
    "glTextureSubImage3D",
    "glCompressedTextureSubImage1D",
    "glCompressedTextureSubImage2D",
    "glCompressedTextureSubImage3D",
    "glCopyTextureSubImage1D",
    "glCopyTextureSubImage2D",
    "glCopyTextureSubImage3D",
    "glTextureParameterf",
    "glTextureParameterfv",
    "glTextureParameteri",
    "glTextureParameterIiv",
    "glTextureParameterIuiv",
    "glTextureParameteriv",
    "glGenerateTextureMipmap",
    "glBindTextureUnit",
    "glGetTextureImage",
    "glGetCompressedTextureImage",
    "glGetTextureLevelParameterfv",
    "glGetTextureLevelParameteriv",
    "glGetTextureParameterfv",
    "glGetTextureParameterIiv",
    "glGetTextureParameterIuiv",
    "glGetTextureParameteriv",
    "glCreateVertexArrays",
    "glDisableVertexArrayAttrib",
    "glEnableVertexArrayAttrib",
    "glVertexArrayElementBuffer",
    "glVertexArrayVertexBuffer",
    "glVertexArrayVertexBuffers",
    "glVertexArrayAttribBinding",
    "glVertexArrayAttribFormat",
    "glVertexArrayAttribIFormat",
    "glVertexArrayAttribLFormat",
    "glVertexArrayBindingDivisor",
    "glGetVertexArrayiv",
    "glGetVertexArrayIndexediv",
    "glGetVertexArrayIndexed64iv",
    "glCreateSamplers",
    "glCreateProgramPipelines",
    "glCreateQueries",
    "glGetQueryBufferObjecti64v",
    "glGetQueryBufferObjectiv",
    "glGetQueryBufferObjectui64v",
    "glGetQueryBufferObjectuiv",
    "glMemoryBarrierByRegion",
    "glGetTextureSubImage",
    "glGetCompressedTextureSubImage",
    "glGetGraphicsResetStatus",
    "glGetnCompressedTexImage",
    "glGetnTexImage",
    "glGetnUniformdv",
    "glGetnUniformfv",
    "glGetnUniformiv",
    "glGetnUniformuiv",
    "glReadnPixels",
    "glTextureBarrier", // 109
    // 460
    "glSpecializeShader", // 0
    "glMultiDrawArraysIndirectCount",
    "glMultiDrawElementsIndirectCount",
    "glPolygonOffsetClamp" // 3
};

_ZGL afxString const glSymbolStrings[] =
{
    // 100
    AFX_STRING("glCullFace"), // 0
    AFX_STRING("glFrontFace"),
    AFX_STRING("glHint"),
    AFX_STRING("glLineWidth"),
    AFX_STRING("glPointSize"),
    AFX_STRING("glPolygonMode"),
    AFX_STRING("glScissor"),
    AFX_STRING("glTexParameterf"),
    AFX_STRING("glTexParameterfv"),
    AFX_STRING("glTexParameteri"),
    AFX_STRING("glTexParameteriv"),
    AFX_STRING("glTexImage1D"),
    AFX_STRING("glTexImage2D"),
    AFX_STRING("glDrawBuffer"),
    AFX_STRING("glClear"),
    AFX_STRING("glClearColor"),
    AFX_STRING("glClearStencil"),
    AFX_STRING("glClearDepth"),
    AFX_STRING("glStencilMask"),
    AFX_STRING("glColorMask"),
    AFX_STRING("glDepthMask"),
    AFX_STRING("glDisable"),
    AFX_STRING("glEnable"),
    AFX_STRING("glFinish"),
    AFX_STRING("glFlush"),
    AFX_STRING("glBlendFunc"),
    AFX_STRING("glLogicOp"),
    AFX_STRING("glStencilFunc"),
    AFX_STRING("glStencilOp"),
    AFX_STRING("glDepthFunc"),
    AFX_STRING("glPixelStoref"),
    AFX_STRING("glPixelStorei"),
    AFX_STRING("glReadBuffer"),
    AFX_STRING("glReadPixels"),
    AFX_STRING("glGetBooleanv"),
    AFX_STRING("glGetDoublev"),
    AFX_STRING("glGetError"),
    AFX_STRING("glGetFloatv"),
    AFX_STRING("glGetIntegerv"),
    AFX_STRING("glGetString"),
    AFX_STRING("glGetTexImage"),
    AFX_STRING("glGetTexParameterfv"),
    AFX_STRING("glGetTexParameteriv"),
    AFX_STRING("glGetTexLevelParameterfv"),
    AFX_STRING("glGetTexLevelParameteriv"),
    AFX_STRING("glIsEnabled"),
    AFX_STRING("glDepthRange"),
    AFX_STRING("glViewport"), // 47
        // 110
    AFX_STRING("glDrawArrays"), // 0
    AFX_STRING("glDrawElements"),
    AFX_STRING("glGetPointerv"),
    AFX_STRING("glPolygonOffset"),
    AFX_STRING("glCopyTexImage1D"),
    AFX_STRING("glCopyTexImage2D"),
    AFX_STRING("glCopyTexSubImage1D"),
    AFX_STRING("glCopyTexSubImage2D"),
    AFX_STRING("glTexSubImage1D"),
    AFX_STRING("glTexSubImage2D"),
    AFX_STRING("glBindTexture"),
    AFX_STRING("glDeleteTextures"),
    AFX_STRING("glGenTextures"),
    AFX_STRING("glIsTexture"), // 13
        // 120
    AFX_STRING("glDrawRangeElements"), // 0
    AFX_STRING("glTexImage3D"),
    AFX_STRING("glTexSubImage3D"),
    AFX_STRING("glCopyTexSubImage3D"), // 3
        // 130
    AFX_STRING("glActiveTexture"), // 0
    AFX_STRING("glSampleCoverage"),
    AFX_STRING("glCompressedTexImage3D"),
    AFX_STRING("glCompressedTexImage2D"),
    AFX_STRING("glCompressedTexImage1D"),
    AFX_STRING("glCompressedTexSubImage3D"),
    AFX_STRING("glCompressedTexSubImage2D"),
    AFX_STRING("glCompressedTexSubImage1D"),
    AFX_STRING("glGetCompressedTexImage"), // 8
        // 140
    AFX_STRING("glBlendFuncSeparate"), // 0
    AFX_STRING("glMultiDrawArrays"),
    AFX_STRING("glMultiDrawElements"),
    AFX_STRING("glPointParameterf"),
    AFX_STRING("glPointParameterfv"),
    AFX_STRING("glPointParameteri"),
    AFX_STRING("glPointParameteriv"),
    AFX_STRING("glBlendColor"),
    AFX_STRING("glBlendEquation"), // 8
        // 150
    AFX_STRING("glGenQueries"), // 0
    AFX_STRING("glDeleteQueries"),
    AFX_STRING("glIsQuery"),
    AFX_STRING("glBeginQuery"),
    AFX_STRING("glEndQuery"),
    AFX_STRING("glGetQueryiv"),
    AFX_STRING("glGetQueryObjectiv"),
    AFX_STRING("glGetQueryObjectuiv"),
    AFX_STRING("glBindBuffer"),
    AFX_STRING("glDeleteBuffers"),
    AFX_STRING("glGenBuffers"),
    AFX_STRING("glIsBuffer"),
    AFX_STRING("glBufferData"),
    AFX_STRING("glBufferSubData"),
    AFX_STRING("glGetBufferSubData"),
    AFX_STRING("glMapBuffer"),
    AFX_STRING("glUnmapBuffer"),
    AFX_STRING("glGetBufferParameteriv"),
    AFX_STRING("glGetBufferPointerv"), // 18
        // 200
    AFX_STRING("glBlendEquationSeparate"), // 0
    AFX_STRING("glDrawBuffers"),
    AFX_STRING("glStencilOpSeparate"),
    AFX_STRING("glStencilFuncSeparate"),
    AFX_STRING("glStencilMaskSeparate"),
    AFX_STRING("glAttachShader"),
    AFX_STRING("glBindAttribLocation"),
    AFX_STRING("glCompileShader"),
    AFX_STRING("glCreateProgram"),
    AFX_STRING("glCreateShader"),
    AFX_STRING("glDeleteProgram"),
    AFX_STRING("glDeleteShader"),
    AFX_STRING("glDetachShader"),
    AFX_STRING("glDisableVertexAttribArray"),
    AFX_STRING("glEnableVertexAttribArray"),
    AFX_STRING("glGetActiveAttrib"),
    AFX_STRING("glGetActiveUniform"),
    AFX_STRING("glGetAttachedShaders"),
    AFX_STRING("glGetAttribLocation"),
    AFX_STRING("glGetProgramiv"),
    AFX_STRING("glGetProgramInfoLog"),
    AFX_STRING("glGetShaderiv"),
    AFX_STRING("glGetShaderInfoLog"),
    AFX_STRING("glGetShaderSource"),
    AFX_STRING("glGetUniformLocation"),
    AFX_STRING("glGetUniformfv"),
    AFX_STRING("glGetUniformiv"),
    AFX_STRING("glGetVertexAttribdv"),
    AFX_STRING("glGetVertexAttribfv"),
    AFX_STRING("glGetVertexAttribiv"),
    AFX_STRING("glGetVertexAttribPointerv"),
    AFX_STRING("glIsProgram"),
    AFX_STRING("glIsShader"),
    AFX_STRING("glLinkProgram"),
    AFX_STRING("glShaderSource"),
    AFX_STRING("glUseProgram"),
    AFX_STRING("glUniform1f"),
    AFX_STRING("glUniform2f"),
    AFX_STRING("glUniform3f"),
    AFX_STRING("glUniform4f"),
    AFX_STRING("glUniform1i"),
    AFX_STRING("glUniform2i"),
    AFX_STRING("glUniform3i"),
    AFX_STRING("glUniform4i"),
    AFX_STRING("glUniform1fv"),
    AFX_STRING("glUniform2fv"),
    AFX_STRING("glUniform3fv"),
    AFX_STRING("glUniform4fv"),
    AFX_STRING("glUniform1iv"),
    AFX_STRING("glUniform2iv"),
    AFX_STRING("glUniform3iv"),
    AFX_STRING("glUniform4iv"),
    AFX_STRING("glUniformMatrix2fv"),
    AFX_STRING("glUniformMatrix3fv"),
    AFX_STRING("glUniformMatrix4fv"),
    AFX_STRING("glValidateProgram"),
    AFX_STRING("glVertexAttrib1d"),
    AFX_STRING("glVertexAttrib1dv"),
    AFX_STRING("glVertexAttrib1f"),
    AFX_STRING("glVertexAttrib1fv"),
    AFX_STRING("glVertexAttrib1s"),
    AFX_STRING("glVertexAttrib1sv"),
    AFX_STRING("glVertexAttrib2d"),
    AFX_STRING("glVertexAttrib2dv"),
    AFX_STRING("glVertexAttrib2f"),
    AFX_STRING("glVertexAttrib2fv"),
    AFX_STRING("glVertexAttrib2s"),
    AFX_STRING("glVertexAttrib2sv"),
    AFX_STRING("glVertexAttrib3d"),
    AFX_STRING("glVertexAttrib3dv"),
    AFX_STRING("glVertexAttrib3f"),
    AFX_STRING("glVertexAttrib3fv"),
    AFX_STRING("glVertexAttrib3s"),
    AFX_STRING("glVertexAttrib3sv"),
    AFX_STRING("glVertexAttrib4Nbv"),
    AFX_STRING("glVertexAttrib4Niv"),
    AFX_STRING("glVertexAttrib4Nsv"),
    AFX_STRING("glVertexAttrib4Nub"),
    AFX_STRING("glVertexAttrib4Nubv"),
    AFX_STRING("glVertexAttrib4Nuiv"),
    AFX_STRING("glVertexAttrib4Nusv"),
    AFX_STRING("glVertexAttrib4bv"),
    AFX_STRING("glVertexAttrib4d"),
    AFX_STRING("glVertexAttrib4dv"),
    AFX_STRING("glVertexAttrib4f"),
    AFX_STRING("glVertexAttrib4fv"),
    AFX_STRING("glVertexAttrib4iv"),
    AFX_STRING("glVertexAttrib4s"),
    AFX_STRING("glVertexAttrib4sv"),
    AFX_STRING("glVertexAttrib4ubv"),
    AFX_STRING("glVertexAttrib4uiv"),
    AFX_STRING("glVertexAttrib4usv"),
    AFX_STRING("glVertexAttribPointer"), // 92
        // 210
    AFX_STRING("glUniformMatrix2x3fv"), // 0
    AFX_STRING("glUniformMatrix3x2fv"),
    AFX_STRING("glUniformMatrix2x4fv"),
    AFX_STRING("glUniformMatrix4x2fv"),
    AFX_STRING("glUniformMatrix3x4fv"),
    AFX_STRING("glUniformMatrix4x3fv"), // 5
        // 300
    AFX_STRING("glColorMaski"), // 0
    AFX_STRING("glGetBooleani_v"),
    AFX_STRING("glGetIntegeri_v"),
    AFX_STRING("glEnablei"),
    AFX_STRING("glDisablei"),
    AFX_STRING("glIsEnabledi"),
    AFX_STRING("glBeginTransformFeedback"),
    AFX_STRING("glEndTransformFeedback"),
    AFX_STRING("glBindBufferRange"),
    AFX_STRING("glBindBufferBase"),
    AFX_STRING("glTransformFeedbackVaryings"),
    AFX_STRING("glGetTransformFeedbackVarying"),
    AFX_STRING("glClampColor"),
    AFX_STRING("glBeginConditionalRender"),
    AFX_STRING("glEndConditionalRender"),
    AFX_STRING("glVertexAttribIPointer"),
    AFX_STRING("glGetVertexAttribIiv"),
    AFX_STRING("glGetVertexAttribIuiv"),
    AFX_STRING("glVertexAttribI1i"),
    AFX_STRING("glVertexAttribI2i"),
    AFX_STRING("glVertexAttribI3i"),
    AFX_STRING("glVertexAttribI4i"),
    AFX_STRING("glVertexAttribI1ui"),
    AFX_STRING("glVertexAttribI2ui"),
    AFX_STRING("glVertexAttribI3ui"),
    AFX_STRING("glVertexAttribI4ui"),
    AFX_STRING("glVertexAttribI1iv"),
    AFX_STRING("glVertexAttribI2iv"),
    AFX_STRING("glVertexAttribI3iv"),
    AFX_STRING("glVertexAttribI4iv"),
    AFX_STRING("glVertexAttribI1uiv"),
    AFX_STRING("glVertexAttribI2uiv"),
    AFX_STRING("glVertexAttribI3uiv"),
    AFX_STRING("glVertexAttribI4uiv"),
    AFX_STRING("glVertexAttribI4bv"),
    AFX_STRING("glVertexAttribI4sv"),
    AFX_STRING("glVertexAttribI4ubv"),
    AFX_STRING("glVertexAttribI4usv"),
    AFX_STRING("glGetUniformuiv"),
    AFX_STRING("glBindFragDataLocation"),
    AFX_STRING("glGetFragDataLocation"),
    AFX_STRING("glUniform1ui"),
    AFX_STRING("glUniform2ui"),
    AFX_STRING("glUniform3ui"),
    AFX_STRING("glUniform4ui"),
    AFX_STRING("glUniform1uiv"),
    AFX_STRING("glUniform2uiv"),
    AFX_STRING("glUniform3uiv"),
    AFX_STRING("glUniform4uiv"),
    AFX_STRING("glTexParameterIiv"),
    AFX_STRING("glTexParameterIuiv"),
    AFX_STRING("glGetTexParameterIiv"),
    AFX_STRING("glGetTexParameterIuiv"),
    AFX_STRING("glClearBufferiv"),
    AFX_STRING("glClearBufferuiv"),
    AFX_STRING("glClearBufferfv"),
    AFX_STRING("glClearBufferfi"),
    AFX_STRING("glGetStringi"),
    AFX_STRING("glIsRenderbuffer"),
    AFX_STRING("glBindRenderbuffer"),
    AFX_STRING("glDeleteRenderbuffers"),
    AFX_STRING("glGenRenderbuffers"),
    AFX_STRING("glRenderbufferStorage"),
    AFX_STRING("glGetRenderbufferParameteriv"),
    AFX_STRING("glIsFramebuffer"),
    AFX_STRING("glBindFramebuffer"),
    AFX_STRING("glDeleteFramebuffers"),
    AFX_STRING("glGenFramebuffers"),
    AFX_STRING("glCheckFramebufferStatus"),
    AFX_STRING("glFramebufferTexture1D"),
    AFX_STRING("glFramebufferTexture2D"),
    AFX_STRING("glFramebufferTexture3D"),
    AFX_STRING("glFramebufferRenderbuffer"),
    AFX_STRING("glGetFramebufferAttachmentParameteriv"),
    AFX_STRING("glGenerateMipmap"),
    AFX_STRING("glBlitFramebuffer"),
    AFX_STRING("glRenderbufferStorageMultisample"),
    AFX_STRING("glFramebufferTextureLayer"),
    AFX_STRING("glMapBufferRange"),
    AFX_STRING("glFlushMappedBufferRange"),
    AFX_STRING("glBindVertexArray"),
    AFX_STRING("glDeleteVertexArrays"),
    AFX_STRING("glGenVertexArrays"),
    AFX_STRING("glIsVertexArray"), // 83
        // 310
    AFX_STRING("glDrawArraysInstanced"), // 0
    AFX_STRING("glDrawElementsInstanced"),
    AFX_STRING("glTexBuffer"),
    AFX_STRING("glPrimitiveRestartIndex"),
    AFX_STRING("glCopyBufferSubData"),
    AFX_STRING("glGetUniformIndices"),
    AFX_STRING("glGetActiveUniformsiv"),
    AFX_STRING("glGetActiveUniformName"),
    AFX_STRING("glGetUniformBlockIndex"),
    AFX_STRING("glGetActiveUniformBlockiv"),
    AFX_STRING("glGetActiveUniformBlockName"),
    AFX_STRING("glUniformBlockBinding"), // 11
        // 320
    AFX_STRING("glDrawElementsBaseVertex"), // 0
    AFX_STRING("glDrawRangeElementsBaseVertex"),
    AFX_STRING("glDrawElementsInstancedBaseVertex"),
    AFX_STRING("glMultiDrawElementsBaseVertex"),
    AFX_STRING("glProvokingVertex"),
    AFX_STRING("glFenceSync"),
    AFX_STRING("glIsSync"),
    AFX_STRING("glDeleteSync"),
    AFX_STRING("glClientWaitSync"),
    AFX_STRING("glWaitSync"),
    AFX_STRING("glGetInteger64v"),
    AFX_STRING("glGetSynciv"),
    AFX_STRING("glGetInteger64i_v"),
    AFX_STRING("glGetBufferParameteri64v"),
    AFX_STRING("glFramebufferTexture"),
    AFX_STRING("glTexImage2DMultisample"),
    AFX_STRING("glTexImage3DMultisample"),
    AFX_STRING("glGetMultisamplefv"),
    AFX_STRING("glSampleMaski"), // 18
        // 330
    AFX_STRING("glBindFragDataLocationIndexed"), // 0
    AFX_STRING("glGetFragDataIndex"),
    AFX_STRING("glGenSamplers"),
    AFX_STRING("glDeleteSamplers"),
    AFX_STRING("glIsSampler"),
    AFX_STRING("glBindSampler"),
    AFX_STRING("glSamplerParameteri"),
    AFX_STRING("glSamplerParameteriv"),
    AFX_STRING("glSamplerParameterf"),
    AFX_STRING("glSamplerParameterfv"),
    AFX_STRING("glSamplerParameterIiv"),
    AFX_STRING("glSamplerParameterIuiv"),
    AFX_STRING("glGetSamplerParameteriv"),
    AFX_STRING("glGetSamplerParameterIiv"),
    AFX_STRING("glGetSamplerParameterfv"),
    AFX_STRING("glGetSamplerParameterIuiv"),
    AFX_STRING("glQueryCounter"),
    AFX_STRING("glGetQueryObjecti64v"),
    AFX_STRING("glGetQueryObjectui64v"),
    AFX_STRING("glVertexAttribDivisor"),
    AFX_STRING("glVertexAttribP1ui"),
    AFX_STRING("glVertexAttribP1uiv"),
    AFX_STRING("glVertexAttribP2ui"),
    AFX_STRING("glVertexAttribP2uiv"),
    AFX_STRING("glVertexAttribP3ui"),
    AFX_STRING("glVertexAttribP3uiv"),
    AFX_STRING("glVertexAttribP4ui"),
    AFX_STRING("glVertexAttribP4uiv"), // 27
        // 400
    AFX_STRING("glMinSampleShading"), // 0
    AFX_STRING("glBlendEquationi"),
    AFX_STRING("glBlendEquationSeparatei"),
    AFX_STRING("glBlendFunci"),
    AFX_STRING("glBlendFuncSeparatei"),
    AFX_STRING("glDrawArraysIndirect"),
    AFX_STRING("glDrawElementsIndirect"),
    AFX_STRING("glUniform1d"),
    AFX_STRING("glUniform2d"),
    AFX_STRING("glUniform3d"),
    AFX_STRING("glUniform4d"),
    AFX_STRING("glUniform1dv"),
    AFX_STRING("glUniform2dv"),
    AFX_STRING("glUniform3dv"),
    AFX_STRING("glUniform4dv"),
    AFX_STRING("glUniformMatrix2dv"),
    AFX_STRING("glUniformMatrix3dv"),
    AFX_STRING("glUniformMatrix4dv"),
    AFX_STRING("glUniformMatrix2x3dv"),
    AFX_STRING("glUniformMatrix2x4dv"),
    AFX_STRING("glUniformMatrix3x2dv"),
    AFX_STRING("glUniformMatrix3x4dv"),
    AFX_STRING("glUniformMatrix4x2dv"),
    AFX_STRING("glUniformMatrix4x3dv"),
    AFX_STRING("glGetUniformdv"),
    AFX_STRING("glGetSubroutineUniformLocation"),
    AFX_STRING("glGetSubroutineIndex"),
    AFX_STRING("glGetActiveSubroutineUniformiv"),
    AFX_STRING("glGetActiveSubroutineUniformName"),
    AFX_STRING("glGetActiveSubroutineName"),
    AFX_STRING("glUniformSubroutinesuiv"),
    AFX_STRING("glGetUniformSubroutineuiv"),
    AFX_STRING("glGetProgramStageiv"),
    AFX_STRING("glPatchParameteri"),
    AFX_STRING("glPatchParameterfv"),
    AFX_STRING("glBindTransformFeedback"),
    AFX_STRING("glDeleteTransformFeedbacks"),
    AFX_STRING("glGenTransformFeedbacks"),
    AFX_STRING("glIsTransformFeedback"),
    AFX_STRING("glPauseTransformFeedback"),
    AFX_STRING("glResumeTransformFeedback"),
    AFX_STRING("glDrawTransformFeedback"),
    AFX_STRING("glDrawTransformFeedbackStream"),
    AFX_STRING("glBeginQueryIndexed"),
    AFX_STRING("glEndQueryIndexed"),
    AFX_STRING("glGetQueryIndexediv"), // 45
        // 410
    AFX_STRING("glReleaseShaderCompiler"), // 0
    AFX_STRING("glShaderBinary"),
    AFX_STRING("glGetShaderPrecisionFormat"),
    AFX_STRING("glDepthRangef"),
    AFX_STRING("glClearDepthf"),
    AFX_STRING("glGetProgramBinary"),
    AFX_STRING("glProgramBinary"),
    AFX_STRING("glProgramParameteri"),
    AFX_STRING("glUseProgramStages"),
    AFX_STRING("glActiveShaderProgram"),
    AFX_STRING("glCreateShaderProgramv"),
    AFX_STRING("glBindProgramPipeline"),
    AFX_STRING("glDeleteProgramPipelines"),
    AFX_STRING("glGenProgramPipelines"),
    AFX_STRING("glIsProgramPipeline"),
    AFX_STRING("glGetProgramPipelineiv"),
    AFX_STRING("glProgramUniform1i"),
    AFX_STRING("glProgramUniform1iv"),
    AFX_STRING("glProgramUniform1f"),
    AFX_STRING("glProgramUniform1fv"),
    AFX_STRING("glProgramUniform1d"),
    AFX_STRING("glProgramUniform1dv"),
    AFX_STRING("glProgramUniform1ui"),
    AFX_STRING("glProgramUniform1uiv"),
    AFX_STRING("glProgramUniform2i"),
    AFX_STRING("glProgramUniform2iv"),
    AFX_STRING("glProgramUniform2f"),
    AFX_STRING("glProgramUniform2fv"),
    AFX_STRING("glProgramUniform2d"),
    AFX_STRING("glProgramUniform2dv"),
    AFX_STRING("glProgramUniform2ui"),
    AFX_STRING("glProgramUniform2uiv"),
    AFX_STRING("glProgramUniform3i"),
    AFX_STRING("glProgramUniform3iv"),
    AFX_STRING("glProgramUniform3f"),
    AFX_STRING("glProgramUniform3fv"),
    AFX_STRING("glProgramUniform3d"),
    AFX_STRING("glProgramUniform3dv"),
    AFX_STRING("glProgramUniform3ui"),
    AFX_STRING("glProgramUniform3uiv"),
    AFX_STRING("glProgramUniform4i"),
    AFX_STRING("glProgramUniform4iv"),
    AFX_STRING("glProgramUniform4f"),
    AFX_STRING("glProgramUniform4fv"),
    AFX_STRING("glProgramUniform4d"),
    AFX_STRING("glProgramUniform4dv"),
    AFX_STRING("glProgramUniform4ui"),
    AFX_STRING("glProgramUniform4uiv"),
    AFX_STRING("glProgramUniformMatrix2fv"),
    AFX_STRING("glProgramUniformMatrix3fv"),
    AFX_STRING("glProgramUniformMatrix4fv"),
    AFX_STRING("glProgramUniformMatrix2dv"),
    AFX_STRING("glProgramUniformMatrix3dv"),
    AFX_STRING("glProgramUniformMatrix4dv"),
    AFX_STRING("glProgramUniformMatrix2x3fv"),
    AFX_STRING("glProgramUniformMatrix3x2fv"),
    AFX_STRING("glProgramUniformMatrix2x4fv"),
    AFX_STRING("glProgramUniformMatrix4x2fv"),
    AFX_STRING("glProgramUniformMatrix3x4fv"),
    AFX_STRING("glProgramUniformMatrix4x3fv"),
    AFX_STRING("glProgramUniformMatrix2x3dv"),
    AFX_STRING("glProgramUniformMatrix3x2dv"),
    AFX_STRING("glProgramUniformMatrix2x4dv"),
    AFX_STRING("glProgramUniformMatrix4x2dv"),
    AFX_STRING("glProgramUniformMatrix3x4dv"),
    AFX_STRING("glProgramUniformMatrix4x3dv"),
    AFX_STRING("glValidateProgramPipeline"),
    AFX_STRING("glGetProgramPipelineInfoLog"),
    AFX_STRING("glVertexAttribL1d"),
    AFX_STRING("glVertexAttribL2d"),
    AFX_STRING("glVertexAttribL3d"),
    AFX_STRING("glVertexAttribL4d"),
    AFX_STRING("glVertexAttribL1dv"),
    AFX_STRING("glVertexAttribL2dv"),
    AFX_STRING("glVertexAttribL3dv"),
    AFX_STRING("glVertexAttribL4dv"),
    AFX_STRING("glVertexAttribLPointer"),
    AFX_STRING("glGetVertexAttribLdv"),
    AFX_STRING("glViewportArrayv"),
    AFX_STRING("glViewportIndexedf"),
    AFX_STRING("glViewportIndexedfv"),
    AFX_STRING("glScissorArrayv"),
    AFX_STRING("glScissorIndexed"),
    AFX_STRING("glScissorIndexedv"),
    AFX_STRING("glDepthRangeArrayv"),
    AFX_STRING("glDepthRangeIndexed"),
    AFX_STRING("glGetFloati_v"),
    AFX_STRING("glGetDoublei_v"), // 87
        // 420
    AFX_STRING("glDrawArraysInstancedBaseInstance"), // 0
    AFX_STRING("glDrawElementsInstancedBaseInstance"),
    AFX_STRING("glDrawElementsInstancedBaseVertexBaseInstance"),
    AFX_STRING("glGetInternalformativ"),
    AFX_STRING("glGetActiveAtomicCounterBufferiv"),
    AFX_STRING("glBindImageTexture"),
    AFX_STRING("glMemoryBarrier"),
    AFX_STRING("glTexStorage1D"),
    AFX_STRING("glTexStorage2D"),
    AFX_STRING("glTexStorage3D"),
    AFX_STRING("glDrawTransformFeedbackInstanced"),
    AFX_STRING("glDrawTransformFeedbackStreamInstanced"), // 11
        // 430
    AFX_STRING("glClearBufferData"), // 0
    AFX_STRING("glClearBufferSubData"),
    AFX_STRING("glDispatchCompute"),
    AFX_STRING("glDispatchComputeIndirect"),
    AFX_STRING("glCopyImageSubData"),
    AFX_STRING("glFramebufferParameteri"),
    AFX_STRING("glGetFramebufferParameteriv"),
    AFX_STRING("glGetInternalformati64v"),
    AFX_STRING("glInvalidateTexSubImage"),
    AFX_STRING("glInvalidateTexImage"),
    AFX_STRING("glInvalidateBufferSubData"),
    AFX_STRING("glInvalidateBufferData"),
    AFX_STRING("glInvalidateFramebuffer"),
    AFX_STRING("glInvalidateSubFramebuffer"),
    AFX_STRING("glMultiDrawArraysIndirect"),
    AFX_STRING("glMultiDrawElementsIndirect"),
    AFX_STRING("glGetProgramInterfaceiv"),
    AFX_STRING("glGetProgramResourceIndex"),
    AFX_STRING("glGetProgramResourceName"),
    AFX_STRING("glGetProgramResourceiv"),
    AFX_STRING("glGetProgramResourceLocation"),
    AFX_STRING("glGetProgramResourceLocationIndex"),
    AFX_STRING("glShaderStorageBlockBinding"),
    AFX_STRING("glTexBufferRange"),
    AFX_STRING("glTexStorage2DMultisample"),
    AFX_STRING("glTexStorage3DMultisample"),
    AFX_STRING("glTextureView"),
    AFX_STRING("glBindVertexBuffer"),
    AFX_STRING("glVertexAttribFormat"),
    AFX_STRING("glVertexAttribIFormat"),
    AFX_STRING("glVertexAttribLFormat"),
    AFX_STRING("glVertexAttribBinding"),
    AFX_STRING("glVertexBindingDivisor"),
    AFX_STRING("glDebugMessageControl"),
    AFX_STRING("glDebugMessageInsert"),
    AFX_STRING("glDebugMessageCallback"),
    AFX_STRING("glGetDebugMessageLog"),
    AFX_STRING("glPushDebugGroup"),
    AFX_STRING("glPopDebugGroup"),
    AFX_STRING("glObjectLabel"),
    AFX_STRING("glGetObjectLabel"),
    AFX_STRING("glObjectPtrLabel"),
    AFX_STRING("glGetObjectPtrLabel"), // 42
        // 440
    AFX_STRING("glBufferStorage"), // 0
    AFX_STRING("glClearTexImage"),
    AFX_STRING("glClearTexSubImage"),
    AFX_STRING("glBindBuffersBase"),
    AFX_STRING("glBindBuffersRange"),
    AFX_STRING("glBindTextures"),
    AFX_STRING("glBindSamplers"),
    AFX_STRING("glBindImageTextures"),
    AFX_STRING("glBindVertexBuffers"), // 8
        // 450
    AFX_STRING("glClipControl"), // 0
    AFX_STRING("glCreateTransformFeedbacks"),
    AFX_STRING("glTransformFeedbackBufferBase"),
    AFX_STRING("glTransformFeedbackBufferRange"),
    AFX_STRING("glGetTransformFeedbackiv"),
    AFX_STRING("glGetTransformFeedbacki_v"),
    AFX_STRING("glGetTransformFeedbacki64_v"),
    AFX_STRING("glCreateBuffers"),
    AFX_STRING("glNamedBufferStorage"),
    AFX_STRING("glNamedBufferData"),
    AFX_STRING("glNamedBufferSubData"),
    AFX_STRING("glCopyNamedBufferSubData"),
    AFX_STRING("glClearNamedBufferData"),
    AFX_STRING("glClearNamedBufferSubData"),
    AFX_STRING("glMapNamedBuffer"),
    AFX_STRING("glMapNamedBufferRange"),
    AFX_STRING("glUnmapNamedBuffer"),
    AFX_STRING("glFlushMappedNamedBufferRange"),
    AFX_STRING("glGetNamedBufferParameteriv"),
    AFX_STRING("glGetNamedBufferParameteri64v"),
    AFX_STRING("glGetNamedBufferPointerv"),
    AFX_STRING("glGetNamedBufferSubData"),
    AFX_STRING("glCreateFramebuffers"),
    AFX_STRING("glNamedFramebufferRenderbuffer"),
    AFX_STRING("glNamedFramebufferParameteri"),
    AFX_STRING("glNamedFramebufferTexture"),
    AFX_STRING("glNamedFramebufferTextureLayer"),
    AFX_STRING("glNamedFramebufferCanvas"),
    AFX_STRING("glNamedFramebufferCanvass"),
    AFX_STRING("glNamedFramebufferReadBuffer"),
    AFX_STRING("glInvalidateNamedFramebufferData"),
    AFX_STRING("glInvalidateNamedFramebufferSubData"),
    AFX_STRING("glClearNamedFramebufferiv"),
    AFX_STRING("glClearNamedFramebufferuiv"),
    AFX_STRING("glClearNamedFramebufferfv"),
    AFX_STRING("glClearNamedFramebufferfi"),
    AFX_STRING("glBlitNamedFramebuffer"),
    AFX_STRING("glCheckNamedFramebufferStatus"),
    AFX_STRING("glGetNamedFramebufferParameteriv"),
    AFX_STRING("glGetNamedFramebufferAttachmentParameteriv"),
    AFX_STRING("glCreateRenderbuffers"),
    AFX_STRING("glNamedRenderbufferStorage"),
    AFX_STRING("glNamedRenderbufferStorageMultisample"),
    AFX_STRING("glGetNamedRenderbufferParameteriv"),
    AFX_STRING("glCreateTextures"),
    AFX_STRING("glTextureBuffer"),
    AFX_STRING("glTextureBufferRange"),
    AFX_STRING("glTextureStorage1D"),
    AFX_STRING("glTextureStorage2D"),
    AFX_STRING("glTextureStorage3D"),
    AFX_STRING("glTextureStorage2DMultisample"),
    AFX_STRING("glTextureStorage3DMultisample"),
    AFX_STRING("glTextureSubImage1D"),
    AFX_STRING("glTextureSubImage2D"),
    AFX_STRING("glTextureSubImage3D"),
    AFX_STRING("glCompressedTextureSubImage1D"),
    AFX_STRING("glCompressedTextureSubImage2D"),
    AFX_STRING("glCompressedTextureSubImage3D"),
    AFX_STRING("glCopyTextureSubImage1D"),
    AFX_STRING("glCopyTextureSubImage2D"),
    AFX_STRING("glCopyTextureSubImage3D"),
    AFX_STRING("glTextureParameterf"),
    AFX_STRING("glTextureParameterfv"),
    AFX_STRING("glTextureParameteri"),
    AFX_STRING("glTextureParameterIiv"),
    AFX_STRING("glTextureParameterIuiv"),
    AFX_STRING("glTextureParameteriv"),
    AFX_STRING("glGenerateTextureMipmap"),
    AFX_STRING("glBindTextureUnit"),
    AFX_STRING("glGetTextureImage"),
    AFX_STRING("glGetCompressedTextureImage"),
    AFX_STRING("glGetTextureLevelParameterfv"),
    AFX_STRING("glGetTextureLevelParameteriv"),
    AFX_STRING("glGetTextureParameterfv"),
    AFX_STRING("glGetTextureParameterIiv"),
    AFX_STRING("glGetTextureParameterIuiv"),
    AFX_STRING("glGetTextureParameteriv"),
    AFX_STRING("glCreateVertexArrays"),
    AFX_STRING("glDisableVertexArrayAttrib"),
    AFX_STRING("glEnableVertexArrayAttrib"),
    AFX_STRING("glVertexArrayElementBuffer"),
    AFX_STRING("glVertexArrayVertexBuffer"),
    AFX_STRING("glVertexArrayVertexBuffers"),
    AFX_STRING("glVertexArrayAttribBinding"),
    AFX_STRING("glVertexArrayAttribFormat"),
    AFX_STRING("glVertexArrayAttribIFormat"),
    AFX_STRING("glVertexArrayAttribLFormat"),
    AFX_STRING("glVertexArrayBindingDivisor"),
    AFX_STRING("glGetVertexArrayiv"),
    AFX_STRING("glGetVertexArrayIndexediv"),
    AFX_STRING("glGetVertexArrayIndexed64iv"),
    AFX_STRING("glCreateSamplers"),
    AFX_STRING("glCreateProgramPipelines"),
    AFX_STRING("glCreateQueries"),
    AFX_STRING("glGetQueryBufferObjecti64v"),
    AFX_STRING("glGetQueryBufferObjectiv"),
    AFX_STRING("glGetQueryBufferObjectui64v"),
    AFX_STRING("glGetQueryBufferObjectuiv"),
    AFX_STRING("glMemoryBarrierByRegion"),
    AFX_STRING("glGetTextureSubImage"),
    AFX_STRING("glGetCompressedTextureSubImage"),
    AFX_STRING("glGetGraphicsResetStatus"),
    AFX_STRING("glGetnCompressedTexImage"),
    AFX_STRING("glGetnTexImage"),
    AFX_STRING("glGetnUniformdv"),
    AFX_STRING("glGetnUniformfv"),
    AFX_STRING("glGetnUniformiv"),
    AFX_STRING("glGetnUniformuiv"),
    AFX_STRING("glReadnPixels"),
    AFX_STRING("glTextureBarrier"), // 109
        // 460
    AFX_STRING("glSpecializeShader"), // 0
    AFX_STRING("glMultiDrawArraysIndirectCount"),
    AFX_STRING("glMultiDrawElementsIndirectCount"),
    AFX_STRING("glPolygonOffsetClamp") // 3
};


#if 0
int(WINAPI*wglChoosePixelFormat)(HDC hdc, CONST PIXELFORMATDESCRIPTOR *ppfd);  // "wglChoosePixelFormat" funciona com Intel mas não com AMD.
int(WINAPI*wglDescribePixelFormat)(HDC hdc, int iPixelFormat, UINT nBytes, LPPIXELFORMATDESCRIPTOR ppfd);
BOOL(WINAPI*wglSetPixelFormat)(HDC hdc, int format, CONST PIXELFORMATDESCRIPTOR * ppfd); // "wglSetPixelFormat" funciona com Intel mas não com AMD.
int(WINAPI*wglGetPixelFormat)(HDC hdc);

BOOL(WINAPI*wglCopyContextWIN)(HGLRC, HGLRC, UINT) = wglCopyContext;
HGLRC(WINAPI*wglCreateContextWIN)(HDC) = wglCreateContext;
HGLRC(WINAPI*wglCreateLayerContextWIN)(HDC, int) = wglCreateLayerContext;
BOOL(WINAPI*wglDeleteContextWIN)(HGLRC) = wglDeleteContext;
HGLRC(WINAPI*wglGetCurrentContextWIN)(VOID) = wglGetCurrentContext;
HDC(WINAPI*wglGetCurrentDCWIN)(VOID) = wglGetCurrentDC;
PROC(WINAPI*wglGetProcAddressWIN)(LPCSTR) = wglGetProcAddress;
BOOL(WINAPI*wglMakeCurrentWIN)(HDC, HGLRC) = wglMakeCurrent;
BOOL(WINAPI*wglSwapBuffersWIN)(HDC) = NIL;// wglSwapBuffers;
BOOL(WINAPI*wglSwapLayerBuffersWIN)(HDC, UINT) = wglSwapLayerBuffers;
#endif
// ARB/EXT
#if 0
PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB = NIL;
PFNWGLGETEXTENSIONSSTRINGEXTPROC wglGetExtensionsStringEXT = NIL;
PFNWGLGETPIXELFORMATATTRIBIVARBPROC wglGetPixelFormatAttribivARB = NIL;
PFNWGLGETPIXELFORMATATTRIBFVARBPROC wglGetPixelFormatAttribfvARB = NIL;
PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = NIL;
PFNWGLGETCURRENTREADDCARBPROC wglGetCurrentReadDCARB = NIL;
PFNWGLMAKECONTEXTCURRENTARBPROC wglMakeContextCurrentARB = NIL;
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NIL;
PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = NIL;
PFNWGLDXOPENDEVICENVPROC wglDXOpenDeviceNV = NIL;
PFNWGLDXCLOSEDEVICENVPROC wglDXCloseDeviceNV = NIL;
PFNWGLDXREGISTEROBJECTNVPROC wglDXRegisterObjectNV = NIL;
PFNWGLDXUNREGISTEROBJECTNVPROC wglDXUnregisterObjectNV = NIL;
PFNWGLDXLOCKOBJECTSNVPROC wglDXLockObjectsNV = NIL;
PFNWGLDXUNLOCKOBJECTSNVPROC wglDXUnlockObjectsNV = NIL;
#endif

signed int(*__stdcall zglDrvCopyContext)(void *a1, void *a2, int a3) = NIL;
int(*__stdcall zglDrvCreateContext)(HDC hDC) = NIL;
int(*__stdcall zglDrvCreateLayerContext)(HDC hDC, int a2) = NIL; // ignore layer
signed int(*__stdcall zglDrvDeleteContext)(void *a1) = NIL;
LONG(*__stdcall zglDrvDescribePixelFormat)(/*DHPDEV*/HDC dhpdev, LONG iPixelFormat, ULONG cjpfd, PIXELFORMATDESCRIPTOR *ppfd) = NIL;
int(*__stdcall zglDrvGetProcAddress)(int Key) = NIL; // key is a string
int(*__stdcall zglDrvPresentBuffers)(HDC hDC, int a2) = NIL;
signed int(*__stdcall zglDrvReleaseContext)(void *a1) = NIL;
int(*__stdcall zglDrvSetContext)(HGDIOBJ h, int a2, int a3) = NIL; // h = HDC, a2 = new ctx
BOOL(*__stdcall zglDrvSetPixelFormat)(/*SURFOBJ**/ void*pso, LONG iPixelFormat, HWND hwnd) = NIL;
BOOL(*__stdcall zglDrvSwapBuffers)(/*SURFOBJ**/ void*pso, /*WNDOBJ**/ void*pwo) = NIL; // pso = HDC, pwo = HWND


_ZGL afxError TestCoreSymbols(HMODULE opengl32, glVmt const* vmt)
{
    afxError err = { 0 };
    afxUnit symCnt = (sizeof(*vmt) / sizeof(vmt->ptr));
    void*const*pa = &vmt->ptr;

    for (afxUnit i = 0; i < symCnt; i++)
    {
        if (pa[i])
        {
            void* ptr = wglGetProcAddressSIG(opengl32, glSymbolStrings[i].start);

            if (!ptr)
            {
                afxString128 tmp;
                AfxMakeString128(&tmp, 0);

                AfxFormatString(&tmp.s, "%.*s%.*s", AfxPushString(&glSymbolStrings[i]), AfxPushString(&AFX_STRING("ARB")));
                ptr = wglGetProcAddressSIG(opengl32, tmp.s.start);

                if (!ptr)
                {
                    AfxFormatString(&tmp.s, "%.*s%.*s", AfxPushString(&glSymbolStrings[i]), AfxPushString(&AFX_STRING("EXT")));
                    ptr = wglGetProcAddressSIG(opengl32, tmp.s.start);
                }
            }

            if (ptr)
            {
                AFX_ASSERT(ptr == pa[i]);
            }
        }
    }
    return err;
}

_ZGL afxError wglLoadCoreSymbols(HMODULE opengl32, glVmt* gl)
{
    afxError err = { 0 };
    afxUnit symCnt = (sizeof(*gl) / sizeof(gl->ptr));
    wglLoadSymbolsSIG(opengl32, 0, symCnt, &gl->ptr, 1);
    TestCoreSymbols(opengl32, gl);
}

_ZGL afxError __stdcall wglLoadSymbolsSIG(HMODULE opengl32, afxUnit base, afxUnit cnt, void* vmt[], afxBool echo)
{
    afxError err = { 0 };
    afxString128 tmp;
    AfxMakeString128(&tmp, 0);
    afxString name;
    afxString arbAndExt[2];
    AfxMakeString(&arbAndExt[0], 0, "ARB", 3);
    AfxMakeString(&arbAndExt[1], 0, "EXT", 3);
    
    char const **names = glVmtNames;

    AFX_ASSERT(names);
    AFX_ASSERT(vmt);
    AFX_ASSERT(opengl32);
    PROC(WINAPI *f)(LPCSTR) = (void*)GetProcAddress(opengl32, "wglGetProcAddress");
    PROC(WINAPI *f2)(LPCSTR) = NIL;

    if (!f)
        f = (void*)GetProcAddress(opengl32, "DrvGetProcAddress");
    else
        if ((f2 = (void*)f("wglGetProcAddress")))
            f = f2;

    afxUnit baseAddonIdx = offsetof(glVmt, ReleaseShaderCompiler) / sizeof(void*);
    
    if (baseAddonIdx >= cnt)
        baseAddonIdx = 0;

    for (afxUnit i = 0; i < cnt; i++)
    {
        AFX_ASSERT(names[base + i]);
        AfxMakeString(&name, 0, names[base + i], 0);
        void *sym = NIL;

        if (f)
        {
            if ((sym = f((void*)name.start)))
            {
                if (i >= baseAddonIdx && echo)
                {
                    AfxReportMessage("  %3u %.*s", i, AfxPushString(&name));
                }
            }
            else
            {
                for (afxUnit j = 0; j < 2; j++)
                {
                    AfxFormatString(&tmp.s, "%.*s%.*s", AfxPushString(&name), AfxPushString(&arbAndExt[j]));

                    if ((sym = f((void*)tmp.buf)))
                    {
                        if (i >= baseAddonIdx && echo)
                        {
                            AfxReportY("  %3u %.*s --- EXT", i, AfxPushString(&tmp.s));
                        }
                        break;
                    }
                    else if ((sym = GetProcAddress(opengl32, (void*)tmp.buf)))
                    {
                        if (i >= baseAddonIdx && echo)
                        {
                            AfxReportY("  %3u %.*s fallback found.", i, AfxPushString(&tmp.s));
                        }
                        break;
                    }
                }
            }
        }

        if (!sym && (sym = GetProcAddress(opengl32, (void*)name.start)))
        {
            if (i >= baseAddonIdx && echo)
            {
                AfxReportMessage("  %3u %.*s fallback found.", i, AfxPushString(&name));
            }
        }

        vmt[i] = sym;
    }
    return err;
}

//wglVmt wgl;

_ZGL PROC wglGetProcAddressSIG(HMODULE opengl32, LPCSTR lpProcName)
{
    afxError err = { 0 };
    PROC f;

    if (/*(!wgl.GetProcAddress) || */!wglGetProcAddressWIN || !(f = wglGetProcAddressWIN(lpProcName)))
        f = GetProcAddress(opengl32, lpProcName);

    return f;
}

// WGL extensions only
_ZGL afxBool wglHasWsiExtensionSIG(HDC hDc, afxChar const* ext)
{
    afxError err = { 0 };

    /*
        Prefix.
        WGL extensions do not require a specific prefix (like WGL_ or GL_) when querying or checking for their presence. 
        When you call wglGetExtensionsStringEXT or wglGetExtensionsStringARB, it returns a list of extensions, without 
        the WGL_ prefix. For example, a typical WGL extension string might include something like: "WGL_ARB_pixel_format WGL_ARB_create_context".

        But when you query these extensions, you should use the name without the WGL_ prefix (e.g., ARB_pixel_format, 
        ARB_create_context).
    */

    if (ext && (*ext != 'W'))
    {
        AfxReportError("wglHasWsiExtensionSIG() works only for WGL extensions");
        AfxThrowError();
        return FALSE;
    }

    // Ensure the extension functions are available
    if (wglGetExtensionsStringEXT == NULL && wglGetExtensionsStringARB == NULL)
    {
        AfxThrowError();
        return FALSE;
    }
    
    // Get the extensions string
    const char* extensions = NULL;
    if (wglGetExtensionsStringARB != NULL && hDc != INVALID_HANDLE_VALUE)
        extensions = wglGetExtensionsStringARB(hDc);
    else if (wglGetExtensionsStringEXT != NULL)
        extensions = wglGetExtensionsStringEXT();

    // Check for a valid extensions string
    if (extensions == NULL || ext == NULL)
        return FALSE;

    // Loop through the extensions string and search for the target extension
    const char* start = extensions;
    while (start != NULL) {
        // Find the next space or the end of the string
        const char* space = strchr(start, ' ');

        // If no space is found, we are at the last extension
        const char* end = (space != NULL) ? space : start + strlen(start);

        // Check if the current extension matches the requested extension
        size_t len = end - start;
        if (strncmp(start, ext, len) == 0 && (strlen(ext) == len) && (space == NULL || space == end || *(space - 1) == ' ')) {
            return TRUE;
        }

        // Move to the next extension in the list
        start = (space != NULL) ? space + 1 : NULL;
    }

    // Return false if the extension is not found
    return FALSE;
}

_ZGL afxBool glHasExtensionSIG(glVmt const* gl, afxChar const* ext)
{
    afxError err = { 0 };

    /*
        Prefix.
        OpenGL extensions typically do not require the GL_ prefix when you're checking for their presence using a 
        function like glGetString(GL_EXTENSIONS). However, many extensions are named with the GL_ prefix (e.g., 
        GL_ARB_vertex_buffer_object), so you should include the prefix when checking for OpenGL extensions.

        OpenGL extension strings returned by glGetString(GL_EXTENSIONS) usually look like this:
        "GL_ARB_vertex_buffer_object GL_ARB_multitexture"
        So you need to include the GL_ prefix when querying for OpenGL extensions, e.g., "GL_ARB_vertex_buffer_object".
    */

    if (ext && (*ext == 'W'))
    {
        AfxReportError("glHasExtensionSIG() does not work for WGL extensions");
        AfxThrowError();
        return FALSE;
    }

    // Ensure glGetString or glGetStringi is available
    if (gl->GetString == NULL && gl->GetStringi == NULL)
        return FALSE;

    // Check if we're dealing with OpenGL 3.0+ or older
    const char* extensions = NULL;

    // Try to use glGetString(GL_EXTENSIONS) if available (for older OpenGL versions or compatibility profile)
    if (gl->GetString != NULL) {
        extensions = (const char*)gl->GetString(GL_EXTENSIONS);

        // If extensions string is NULL, fall back to glGetStringi
        if (extensions == NULL && gl->GetStringi != NULL) {
            // Start checking extensions using glGetStringi (for OpenGL 3.0+ and above)
            GLint numExtensions = 0;
            gl->GetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

            // Loop through all extensions
            for (GLint i = 0; i < numExtensions; i++) {
                const char* extName = (const char*)gl->GetStringi(GL_EXTENSIONS, i);
                if (strcmp(extName, ext) == 0) {
                    return TRUE;
                }
            }
            return FALSE;  // Extension not found
        }
    }

    // If extensions string is still NULL, something went wrong
    if (extensions == NULL || ext == NULL)
        return FALSE;

    // Ensure the extension name starts with the "GL_" prefix
    //char fullExt[strlen("GL_") + strlen(ext) + 1];
    //strcpy(fullExt, "GL_");
    //strcat(fullExt, ext);

    // Loop through the extensions string and search for the target extension
    const char* start = extensions;
    while (start != NULL) {
        // Find the next space or the end of the string
        const char* space = strchr(start, ' ');

        // If no space is found, we are at the last extension
        const char* end = (space != NULL) ? space : start + strlen(start);

        // Check if the current extension matches the requested extension
        size_t len = end - start;
        if (strncmp(start, /*fullExt*/ext, len) == 0 && (strlen(/*fullExt*/ext) == len) && (space == NULL || space == end || *(space - 1) == ' ')) {
            return TRUE;
        }

        // Move to the next extension in the list
        start = (space != NULL) ? space + 1 : NULL;
    }

    // Return false if the extension is not found
    return FALSE;
}

HMODULE GetModuleHandleFromFunctionAddress(void* funcAddress)
{
    MEMORY_BASIC_INFORMATION memInfo;
    if (VirtualQuery(funcAddress, &memInfo, sizeof(memInfo)) == 0)
    {
        return NIL;
    }
    return (HMODULE)memInfo.BaseAddress;
}

#include <DbgHelp.h>
#pragma comment(lib, "dbghelp.lib")

typedef struct
{
    const char *dli_fname;  // Pathname of shared object that contains address
    void       *dli_fbase;  // Base address at which shared object is loaded
    const char *dli_sname;  // Name of symbol whose definition overlaps addr
    void       *dli_saddr;  // Exact address of symbol named in dli_sname
} Dl_info;

HMODULE GetSymbolInfoFromAddress(void* addr, Dl_info* info)
{
    HANDLE process = GetCurrentProcess();
    DWORD64 address = (DWORD64)(addr);

    // Initialize the symbol handler for the process
    SymSetOptions(SYMOPT_DEFERRED_LOADS | SYMOPT_UNDNAME);
    if (!SymInitialize(process, NULL, TRUE))
    {
        return 0;
    }

    // Get symbol information
    SYMBOL_INFO* symbol = (SYMBOL_INFO*)malloc(sizeof(SYMBOL_INFO) + 256);
    symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
    symbol->MaxNameLen = 255;

    if (SymFromAddr(process, address, NULL, symbol))
    {
        info->dli_fbase = (void*)symbol->ModBase;
        info->dli_saddr = (void*)symbol->Address;
        info->dli_sname = (void*)symbol->Name;
        return (void*)symbol->ModBase;
    }
    else
    {
        return 0;
        // err
    }

    free(symbol);
    SymCleanup(process);
    return 0;
}

_ZGL LRESULT WINAPI _ZglTmpWndPrcCbW32(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    afxError err = { 0 };
    //afxObject obj = (afxObject)(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    if (message == WM_SHOWWINDOW)
    {
        // Sent to a window when the window is about to be hidden or shown.
        return 0; // If an application processes this message, it should return zero.
    }
    return DefWindowProcA(hWnd, message, wParam, lParam);
}

#if 0
_ZGL void __stdcall wglLoadWsiSymbolsSIG(HMODULE opengl32, afxUnit* verMajor, afxUnit* verMinor, afxUnit* verPatch)
{
    afxError err = { 0 };

    static afxBool loaded = FALSE;

    if (loaded)
        return;
    
#if 0
    wgl.GetProcAddress = (void*)GetProcAddress(opengl32, "wglGetProcAddress");

    wgl.CreateContext = (void*)GetProcAddress(opengl32, "wglCreateContext");
    wgl.DeleteContext = (void*)GetProcAddress(opengl32, "wglDeleteContext");
    wgl.GetCurrentContext = (void*)GetProcAddress(opengl32, "wglGetCurrentContext");
    wgl.GetCurrentDC = (void*)GetProcAddress(opengl32, "wglGetCurrentDC");
    wgl.MakeCurrent = (void*)GetProcAddress(opengl32, "wglMakeCurrent");
    wgl.ChoosePixelFormat = (void*)GetProcAddress(opengl32, "wglChoosePixelFormat"); // "wglChoosePixelFormat" funciona com Intel mas não com AMD.
    wgl.SetPixelFormat = (void*)GetProcAddress(opengl32, "wglSetPixelFormat"); // "wglSetPixelFormat" funciona com Intel mas não com AMD.
    wgl.DescribePixelFormat = (void*)GetProcAddress(opengl32, "wglDescribePixelFormat");    
    wgl.GetPixelFormat = (void*)GetProcAddress(opengl32, "wglGetPixelFormat");
    wgl.SwapBuffers = (void*)GetProcAddress(opengl32, "wglSwapBuffers");
    wgl.SwapLayerBuffers = (void*)GetProcAddress(opengl32, "wglSwapLayerBuffers");
#endif

    HDC bkpHdc = wglGetCurrentDC();
    HGLRC bkpHglrc = wglGetCurrentContext();
    
    HDC tmpHdc = NIL;
    HWND tmpHwnd = NIL;
    HGLRC tmpHrc = NIL;

    if (!loaded)
    {
        loaded = TRUE;
        wglChoosePixelFormatARB = NIL;
        wglCreateContextAttribsARB = NIL;
        wglMakeContextCurrentARB = NIL;

        DWORD pfdFlags = PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DIRECT3D_ACCELERATED | PFD_SWAP_EXCHANGE;
        DWORD dwExStyle = WS_EX_APPWINDOW;
        DWORD wndStyles = WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

        if (!(tmpHwnd = CreateWindowExA(dwExStyle, vgiWndClss.lpszClassName, "", wndStyles, 0, 0, 1, 1, NIL, NIL, vgiWndClss.hInstance, NIL))) AfxThrowError();
        else
        {
            if (!(tmpHdc = GetDC(tmpHwnd))) AfxThrowError();
            else
            {
                int pxlfmt;
                PIXELFORMATDESCRIPTOR pfd = { 0 };
                pfd.nSize = sizeof(pfd);
                pfd.nVersion = 1;
                pfd.dwFlags = pfdFlags;
                pfd.iPixelType = PFD_TYPE_RGBA;
                pfd.cColorBits = AFX_MIN(24, GetDeviceCaps(tmpHdc, BITSPIXEL));
                pfd.cAlphaBits = 8;
                //pfd.cDepthBits = 24;
                //pfd.cStencilBits = 8;

                HDC hScreenDc = GetDC(NULL);
                pxlfmt = GetPixelFormat(hScreenDc);

                if (pxlfmt)
                    DescribePixelFormat(hScreenDc, pxlfmt, sizeof(pfd), &pfd);

                ReleaseDC(NULL, hScreenDc);

                if (!(pxlfmt = ChoosePixelFormat(tmpHdc, &(pfd))))
                    AfxThrowError();

                if (!(DescribePixelFormat(tmpHdc, pxlfmt, sizeof(pfd), &pfd)))
                    AfxThrowError();

                if (!SetPixelFormat(tmpHdc, pxlfmt, &(pfd)))
                    AfxThrowError();

                if (!(tmpHrc = wglCreateContext(tmpHdc))) AfxThrowError();
                else
                {
                    if (!(wglMakeCurrent(tmpHdc, tmpHrc))) AfxThrowError();
                    else
                    {
                        afxString ver;
                        afxUnit vMajor, vMinor, vPatch;
                        PFNGLGETSTRINGPROC _glGetString = (void*)wglGetProcAddress("glGetString");
                        AfxMakeString(&ver, 0, (afxChar const*)_glGetString(GL_VERSION), 0);
                        AfxScanString(&ver, "%u.%u.%u", &vMajor, &vMinor, &vPatch);

                        if (verMajor)
                            *verMajor = vMajor;

                        if (verMinor)
                            *verMinor = vMinor;

                        if (verPatch)
                            *verPatch = vPatch;
                    }

                    if (err)
                        wglDeleteContext(tmpHrc), tmpHrc = NIL;
                }

                if (err)
                    ReleaseDC(tmpHwnd, tmpHdc), tmpHdc = NIL;
            }

            if (err)
                DestroyWindow(tmpHwnd), tmpHwnd = NIL;
        }
    }

    // ARB/EXT
    wglVmt;
    wglGetPixelFormatAttribivARB = (void*)wglGetProcAddress("wglGetPixelFormatAttribivARB");
    wglGetPixelFormatAttribfvARB = (void*)wglGetProcAddress("wglGetPixelFormatAttribfvARB");
    wglChoosePixelFormatARB = (void*)wglGetProcAddress("wglChoosePixelFormatARB");
    
    wglGetCurrentReadDCARB = (void*)wglGetProcAddress("wglGetCurrentReadDCARB");
    wglMakeContextCurrentARB = (void*)wglGetProcAddress("wglMakeContextCurrentARB");
    wglCreateContextAttribsARB = (void*)wglGetProcAddress("wglCreateContextAttribsARB");
    
    wglGetExtensionsStringARB = (void*)wglGetProcAddress("wglGetExtensionsStringARB");
    wglGetExtensionsStringEXT = (void*)wglGetProcAddress("wglGetExtensionsStringEXT");

    wglSwapIntervalEXT = (void*)wglGetProcAddress("wglSwapIntervalEXT");

    wglDXOpenDeviceNV = (void*)wglGetProcAddress("wglDXOpenDeviceNV");
    wglDXCloseDeviceNV = (void*)wglGetProcAddress("wglDXCloseDeviceNV");
    wglDXRegisterObjectNV = (void*)wglGetProcAddress("wglDXRegisterObjectNV");
    wglDXUnregisterObjectNV = (void*)wglGetProcAddress("wglDXUnregisterObjectNV");
    wglDXLockObjectsNV = (void*)wglGetProcAddress("wglDXLockObjectsNV");
    wglDXUnlockObjectsNV = (void*)wglGetProcAddress("wglDXUnlockObjectsNV");

    // DRV internals
#if 0
    Dl_info nfo = { 0 };
    HMODULE mod = GetModuleHandleFromFunctionAddress(&wgl.CreateContextAttribsARB);
    afxChar buf[256];
    GetModuleFileNameA(mod, buf, sizeof(buf));
    mod = LoadLibraryA("ig7icd32");
#else
    HMODULE mod = opengl32;
#endif

    zglDrvCopyContext = (void*)wglGetProcAddressSIG(mod, "DrvCopyContext");
    zglDrvCreateContext = (void*)wglGetProcAddressSIG(mod, "DrvCreateContext");
    zglDrvCreateLayerContext = (void*)wglGetProcAddressSIG(mod, "DrvCreateLayerContext");
    zglDrvDeleteContext = (void*)wglGetProcAddressSIG(mod, "DrvDeleteContext");
    zglDrvDescribePixelFormat = (void*)wglGetProcAddressSIG(mod, "DrvDescribePixelFormat");
    zglDrvGetProcAddress = (void*)wglGetProcAddressSIG(mod, "DrvGetProcAddress");
    zglDrvReleaseContext = (void*)wglGetProcAddressSIG(mod, "DrvReleaseContext");
    zglDrvSetContext = (void*)wglGetProcAddressSIG(mod, "DrvSetContext");
    zglDrvSetPixelFormat = (void*)GetProcAddress(mod, "DrvSetPixelFormat");
    zglDrvSwapBuffers = (void*)GetProcAddress(mod, "DrvSwapBuffers");
    zglDrvPresentBuffers = (void*)wglGetProcAddressSIG(mod, "DrvPresentBuffers");

    int a = 0;

    if (((tmpHrc || tmpHdc || tmpHwnd)))
    {
        wglMakeCurrent(NIL, NIL);
        wglDeleteContext(tmpHrc), tmpHrc = NIL;
        ReleaseDC(tmpHwnd, tmpHdc), tmpHdc = NIL;
        DestroyWindow(tmpHwnd), tmpHwnd = NIL;
    }
    wglMakeCurrent(bkpHdc, bkpHglrc);
}
#endif

_ZGL BOOL __stdcall wglChooseBestPixelFormatSIG(HDC hDC, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats, afxBool* byWgl)
{


    if (wglChoosePixelFormatARB(hDC, piAttribIList, pfAttribFList, nMaxFormats, piFormats, nNumFormats))
    {
        if (byWgl)
            *byWgl = TRUE;

        return TRUE;
    }
    else
    {
        DWORD dwPfFlags = 0;
        PIXELFORMATDESCRIPTOR pfd = { 0 };
        pfd.nSize = sizeof(pfd);
        pfd.nVersion = 1;

        afxUnit attCnt = 0;
        while (piAttribIList[attCnt * 2] != 0)
        {
            afxUnit key = piAttribIList[attCnt * 2];
            afxUnit val = piAttribIList[attCnt * 2 + 1];
            
            switch (key)
            {
            case WGL_ACCELERATION_ARB:
            {
                if (val == WGL_FULL_ACCELERATION_ARB)
                {
                    dwPfFlags |= PFD_DIRECT3D_ACCELERATED;
                }
                else if (val == WGL_GENERIC_ACCELERATION_ARB)
                {
                    dwPfFlags |= PFD_GENERIC_ACCELERATED;
                }
                else if (val == WGL_NO_ACCELERATION_ARB)
                {
                    // nothing
                }
                break;
            }
            case WGL_SUPPORT_OPENGL_ARB:
            {
                dwPfFlags |= (val ? PFD_SUPPORT_OPENGL : NIL);
                break;
            }
            case WGL_SUPPORT_GDI_ARB:
            {
                dwPfFlags |= (val ? PFD_SUPPORT_GDI : NIL);
                break;
            }
            case WGL_DRAW_TO_WINDOW_ARB:
            {
                dwPfFlags |= (val ? PFD_DRAW_TO_WINDOW : NIL);
                break;
            }
            case WGL_DRAW_TO_BITMAP_ARB:
            case WGL_DRAW_TO_PBUFFER_ARB:
            {
                dwPfFlags |= (val ? PFD_DRAW_TO_BITMAP : NIL);
                break;
            }
            case WGL_DOUBLE_BUFFER_ARB:
            {
                dwPfFlags |= (val ? PFD_DOUBLEBUFFER : PFD_DOUBLEBUFFER_DONTCARE);
                break;
            }
            case WGL_PIXEL_TYPE_ARB:
            {
                if (val == WGL_TYPE_RGBA_ARB)
                    dwPfFlags |= PFD_TYPE_RGBA;
                else if (val == WGL_TYPE_RGBA_FLOAT_ARB)
                    dwPfFlags |= PFD_TYPE_RGBA;
                else if (val == WGL_TYPE_COLORINDEX_ARB)
                    dwPfFlags |= PFD_TYPE_COLORINDEX;

                break;
            }
            case WGL_STEREO_ARB:
            {
                dwPfFlags |= (val ? PFD_STEREO : PFD_STEREO_DONTCARE);
                break;
            }
            case WGL_SWAP_METHOD_ARB:
            {
                if (val == WGL_SWAP_EXCHANGE_ARB)
                    dwPfFlags |= PFD_SWAP_EXCHANGE;
                else if (val == WGL_SWAP_COPY_ARB)
                    dwPfFlags |= PFD_SWAP_COPY;
                else if (val == WGL_SWAP_UNDEFINED_ARB)
                    dwPfFlags |= NIL;

                break;
            }
            case WGL_SWAP_LAYER_BUFFERS_ARB:
            {
                dwPfFlags |= PFD_SWAP_LAYER_BUFFERS;
                break;
            }
            case WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB:
            {
                break;
            }
            case WGL_COLORSPACE_SRGB_EXT:
            {
                break;
            }
            case WGL_TRANSPARENT_ARB:
            {
                dwPfFlags |= (val ? PFD_SUPPORT_COMPOSITION : NIL);
                break;
            }

            case WGL_DEPTH_BITS_ARB:
            {
                pfd.cDepthBits = val;
                break;
            }
            case WGL_STENCIL_BITS_ARB:
            {
                pfd.cStencilBits = val;
                break;
            }
            case WGL_COLOR_BITS_ARB:
            {
                pfd.cColorBits = val;
                break;
            }
            case WGL_RED_BITS_ARB:
            {
                pfd.cRedBits = val;
                break;
            }
            case WGL_GREEN_BITS_ARB:
            {
                pfd.cGreenBits = val;
                break;
            }
            case WGL_BLUE_BITS_ARB:
            {
                pfd.cBlueBits = val;
                break;
            }
            case WGL_ALPHA_BITS_ARB:
            {
                pfd.cAlphaBits = val;
                break;
            }
            case WGL_RED_SHIFT_ARB:
            {
                pfd.cRedShift = val;
                break;
            }
            case WGL_GREEN_SHIFT_ARB:
            {
                pfd.cGreenShift = val;
                break;
            }
            case WGL_BLUE_SHIFT_ARB:
            {
                pfd.cBlueShift = val;
                break;
            }
            case WGL_ALPHA_SHIFT_ARB:
            {
                pfd.cAlphaShift = val;
                break;
            }
            case WGL_AUX_BUFFERS_ARB:
            {
                pfd.cAuxBuffers = val;
                break;
            }
            case WGL_ACCUM_BITS_ARB:
            {
                pfd.cAccumBits = val;
                break;
            }
            case WGL_ACCUM_RED_BITS_ARB:
            {
                pfd.cAccumRedBits = val;
                break;
            }
            case WGL_ACCUM_GREEN_BITS_ARB:
            {
                pfd.cAccumGreenBits = val;
                break;
            }
            case WGL_ACCUM_BLUE_BITS_ARB:
            {
                pfd.cAccumBlueBits = val;
                break;
            }
            case WGL_ACCUM_ALPHA_BITS_ARB:
            {
                pfd.cAccumAlphaBits = val;
                break;
            }
            default: break;
            }

            attCnt++;
        }

        int pxlFmt = ChoosePixelFormat(hDC, &pfd);
        piFormats[0] = pxlFmt;
        *nNumFormats = 1;

        if (byWgl)
            *byWgl = FALSE;

        return !!pxlFmt;
    }
    

    int numFormats;
    int bestFormat = 0;
    int bestMatchScore = -1;

    // Retrieve the number of pixel formats
    int maxPixelFormats;

    if (wglGetPixelFormatAttribivARB(hDC, 0, 0, 1, (int[]) { WGL_NUMBER_PIXEL_FORMATS_ARB }, &maxPixelFormats) != TRUE)
    {
        return 0;
    }

    //WGL_NUMBER_PIXEL_FORMATS_ARB, // The number of pixel formats for the device context.The <iLayerPlane> and <iPixelFormat> parameters are ignored if this attribute is specified.

    int intAttrCnt = 0;
    int fltAttrCnt = 0;

    for (int j = 0; piAttribIList && piAttribIList[2 * j]; j++)
        intAttrCnt++;

    for (int j = 0; pfAttribFList && pfAttribFList[2 * j]; j++)
        fltAttrCnt++;

#if 0
    int attrFmtFetchAttrs[] =
    {
        WGL_DRAW_TO_WINDOW_ARB, // True if the pixel format can be used with a window.The <iLayerPlane> parameter is ignored if this attribute is specified.
        WGL_DRAW_TO_BITMAP_ARB, // True if the pixel format can be used with a memory bitmap.The <iLayerPlane> parameter is ignored if this attribute is specified.
        WGL_ACCELERATION_ARB, // Indicates whether the pixel format is supported by the driver. If this is set to WGL_NO_ACCELERATION_ARB then only the software renderer supports this pixel format; if this is set to WGL_GENERIC_ACCELERATION_ARB then the pixel format is supported by an MCD driver; if this is set to WGL_FULL_ACCELERATION_ARB then the pixel format is supported by an ICD driver.
        WGL_NEED_PALETTE_ARB, // A logical palette is required to achieve the best results for this pixel format.The <iLayerPlane> parameter is ignored if this attribute is specified.
        WGL_NEED_SYSTEM_PALETTE_ARB, // The hardware supports one hardware palette in 256-color mode only.The <iLayerPlane> parameter is ignored if this attribute is specified.
        WGL_SWAP_LAYER_BUFFERS_ARB, // True if the pixel format supports swapping layer planes independently of the main planes.If the pixel format does not support a back buffer then this is set to FALSE.The <iLayerPlane> parameter is ignored if this attribute is specified.
        WGL_SWAP_METHOD_ARB, // If the pixel format supports a back buffer, then this indicates how they are swapped.If this attribute is set to WGL_SWAP_EXCHANGE_ARB then swapping exchanges the front and back buffer contents; if it is set to WGL_SWAP_COPY_ARB then swapping copies the back buffer contents to the front buffer; if it is set to WGL_SWAP_UNDEFINED_ARB then the back buffer contents are copied to the front buffer but the back buffer contents are undefined after the operation.If the pixel format does not support a back buffer then this parameter is set to WGL_SWAP_UNDEFINED_ARB.The <iLayerPlane> parameter is ignored if this attribute is specified.
        WGL_NUMBER_OVERLAYS_ARB, // The number of overlay planes.The <iLayerPlane> parameter is ignored if this attribute is specified.
        WGL_NUMBER_UNDERLAYS_ARB, // The number of underlay planes.The <iLayerPlane> parameter is ignored if this attribute is specified.
        WGL_TRANSPARENT_ARB, // True if transparency is supported.
        WGL_TRANSPARENT_RED_VALUE_ARB, // Specifies the transparent red color value.Typically this value is the same for all layer planes.This value is undefined if transparency is not supported.
        WGL_TRANSPARENT_GREEN_VALUE_ARB, // Specifies the transparent green value.Typically this value is the same for all layer planes.This value is undefined if transparency is not supported.
        WGL_TRANSPARENT_BLUE_VALUE_ARB, // Specifies the transparent blue color value.Typically this value is the same for all layer planes.This value is undefined if transparency is not supported.
        WGL_TRANSPARENT_ALPHA_VALUE_ARB, // Specifies the transparent alpha value.This is reserved for future use.
        WGL_TRANSPARENT_INDEX_VALUE_ARB, // Specifies the transparent color index value.Typically this value is the same for all layer planes.This value is undefined if transparency is not supported.
        WGL_SHARE_DEPTH_ARB, // True if the layer plane shares the depth buffer with the main planes.If <iLayerPlane> is zero, this is always true.
        WGL_SHARE_STENCIL_ARB, // True if the layer plane shares the stencil buffer with the main planes.If <iLayerPlane> is zero, this is always true.
        WGL_SHARE_ACCUM_ARB, // True if the layer plane shares the accumulation buffer with the main planes.If <iLayerPlane> is zero, this is always true.
        WGL_SUPPORT_GDI_ARB, // True if GDI rendering is supported.
        WGL_SUPPORT_OPENGL_ARB, // True if OpenGL is supported.
        WGL_DOUBLE_BUFFER_ARB, // True if the color buffer has back / front pairs.
        WGL_STEREO_ARB, // True if the color buffer has left / right pairs.
        WGL_PIXEL_TYPE_ARB, // The type of pixel data.This can be set to WGL_TYPE_RGBA_ARB or WGL_TYPE_COLORINDEX_ARB
        WGL_COLOR_BITS_ARB, // The number of color bitplanes in each color buffer.For RGBA pixel types, it is the size of the color buffer, excluding the alpha bitplanes.For color - index pixels, it is the size of the color index buffer.
        WGL_RED_BITS_ARB, // The number of red bitplanes in each RGBA color buffer.
        WGL_RED_SHIFT_ARB, // The shift count for red bitplanes in each RGBA color buffer.
        WGL_GREEN_BITS_ARB, // The number of green bitplanes in each RGBA color buffer.
        WGL_GREEN_SHIFT_ARB, // The shift count for green bitplanes in each RGBA color buffer.
        WGL_BLUE_BITS_ARB, // The number of blue bitplanes in each RGBA color buffer.
        WGL_BLUE_SHIFT_ARB, // The shift count for blue bitplanes in each RGBA color buffer.
        WGL_ALPHA_BITS_ARB, // The number of alpha bitplanes in each RGBA color buffer.
        WGL_ALPHA_SHIFT_ARB, // The shift count for alpha bitplanes in each RGBA color buffer.
        WGL_ACCUM_BITS_ARB, // The total number of bitplanes in the accumulation buffer.
        WGL_ACCUM_RED_BITS_ARB, // The number of red bitplanes in the accumulation buffer.
        WGL_ACCUM_GREEN_BITS_ARB, // The number of green bitplanes in the accumulation buffer.
        WGL_ACCUM_BLUE_BITS_ARB, // The number of blue bitplanes in the accumulation buffer.
        WGL_ACCUM_ALPHA_BITS_ARB, // The number of alpha bitplanes in the accumulation buffer.
        WGL_DEPTH_BITS_ARB, // The depth of the depth(z - axis) buffer.
        WGL_STENCIL_BITS_ARB, // The depth of the stencil buffer.
        WGL_AUX_BUFFERS_ARB, // The number of auxiliary buffers.
    };

    for (int i = 1; i < maxPixelFormats; i++)
    {
        int score = 0;

        for (int k = 0; k < ARRAY_SIZE(attrFmtFetchAttrs); k++)
        {
            afxBool found = FALSE;

            int valuei = 0;
            wgl->GetPixelFormatAttribivARB(hDC, i, 0, 1, &attrFmtFetchAttrs[k], &valuei);

            for (int j = 0; j < intAttrCnt; j++)
            {
                if (piAttribIList[2 * j] == attrFmtFetchAttrs[i])
                {
                    found = TRUE;

                    int valueb = piAttribIList[2 * j + 1];

                    if (valueb == valuei)
                        score += 10;
                }
            }

            if (!found)
            {
                if (valuei == 0)
                    score += 1;
            }
        }

        if (score > bestMatchScore)
        {
            bestMatchScore = score;
            bestFormat = i;
        }
    }
#else
    for (int i = 1; i < maxPixelFormats; i++)
    {
        int score = 0;

        for (int j = 0; j < intAttrCnt; j++)
        {
            int valuei = 0;
            wglGetPixelFormatAttribivARB(hDC, i, 0, 1, &piAttribIList[2 * j], &valuei);

            int valueb = piAttribIList[2 * j + 1];

            if (valueb == valuei)
                score += 2;
            //else if ((valueb && valueb < valuei) || (!valueb && valuei > valueb))
              //  score += 1;
        }

        for (int j = 0; j < fltAttrCnt; j++)
        {
            FLOAT valuef = 0;
            wglGetPixelFormatAttribfvARB(hDC, i, 0, 1, (int*)&pfAttribFList[2 * j], &valuef);

            float valuec = pfAttribFList[2 * j + 1];

            if (valuec == valuef)
                score += 2;
            //else if (valuec && valuec < valuef)
                //score += 1;
        }

        if (score > bestMatchScore)
        {
            bestMatchScore = score;
            bestFormat = i;
        }
    }
#endif
    *piFormats = bestFormat;
    *nNumFormats = !!bestFormat;

    return !!bestFormat;
}

_ZGL void APIENTRY _glDbgMsgCb(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
    afxChar*sourceStr, *typeStr, *severityStr;

    switch (source)
    {
    case GL_DEBUG_SOURCE_API: sourceStr = "API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM: sourceStr = "WINDOW SYSTEM"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: sourceStr = "SHADER COMPILER"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY: sourceStr = "THIRD PARTY"; break;
    case GL_DEBUG_SOURCE_APPLICATION: sourceStr = "APPLICATION"; break;
    case GL_DEBUG_SOURCE_OTHER: sourceStr = "OTHER"; break;
    }

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR: typeStr = "ERROR"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: typeStr = "DEPRECATED_BEHAVIOR"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: typeStr = "UNDEFINED_BEHAVIOR"; break;
    case GL_DEBUG_TYPE_PORTABILITY: typeStr = "PORTABILITY"; break;
    case GL_DEBUG_TYPE_PERFORMANCE: typeStr = "PERFORMANCE"; break;
    case GL_DEBUG_TYPE_MARKER: typeStr = "MARKER"; break;
    case GL_DEBUG_TYPE_OTHER: typeStr = "OTHER"; break;
    }

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_NOTIFICATION: severityStr = "NOTIFICATION"; break;
    case GL_DEBUG_SEVERITY_LOW: severityStr = "LOW"; break;
    case GL_DEBUG_SEVERITY_MEDIUM: severityStr = "MEDIUM"; break;
    case GL_DEBUG_SEVERITY_HIGH: severityStr = "HIGH"; break;
    }
    AfxReportError("%s %s %s %u %s", sourceStr, typeStr, severityStr, id, message);
}

_ZGL afxError wglCreateSurfaceSIGMA(int atX, int atY, HWND* phWnd, HDC* phDC, int* pPixFmt, PIXELFORMATDESCRIPTOR* pPfd)
{
    afxError err = { 0 };

    DWORD pfdFlags = PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_SWAP_EXCHANGE | PFD_DIRECT3D_ACCELERATED;
    DWORD wndStyles = WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
    DWORD dwExStyle = WS_EX_TOPMOST;

    HWND hWnd = NIL;
    HDC hDC = NIL;
    HGLRC hGLRC = NIL;
    int dcPxlFmt = 0;
    PIXELFORMATDESCRIPTOR dcPfd = { 0 };

    if (!(hWnd = CreateWindowExA(dwExStyle, vgiWndClss.lpszClassName, vgiWndClss.lpszClassName, 
                                wndStyles, atX, atY, 1, 1, NIL, NIL, vgiWndClss.hInstance, NIL)))
    {
        AfxThrowError();
        return err;
    }

    if (!(hDC = GetDC(hWnd)))
    {
        AfxThrowError();
        DestroyWindow(hWnd), hWnd = NIL;
        return err;
    }

    /*
    WGL_SWAP_METHOD_ARB
    If the pixel format supports a back buffer, then this indicates
    how they are swapped. If this attribute is set to
    WGL_SWAP_EXCHANGE_ARB then swapping exchanges the front and back
    buffer contents; if it is set to WGL_SWAP_COPY_ARB then swapping
    copies the back buffer contents to the front buffer; if it is
    set to WGL_SWAP_UNDEFINED_ARB then the back buffer contents are
    copied to the front buffer but the back buffer contents are
    undefined after the operation. If the pixel format does not
    support a back buffer then this parameter is set to
    WGL_SWAP_UNDEFINED_ARB. The <iLayerPlane> parameter is ignored
    if this attribute is specified.
    */

    int pxlAttrPairs[][2] =
    {
        { WGL_SUPPORT_OPENGL_ARB, GL_TRUE },
        { WGL_DRAW_TO_WINDOW_ARB, GL_TRUE },
        { WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB },
        { WGL_DOUBLE_BUFFER_ARB, GL_TRUE },
        { WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB },

        /*
            WGL_COLOR_BITS_ARB
            The number of color bitplanes in each color buffer. For RGBA pixel types,
            it is the size of the color buffer, "excluding the alpha" bitplanes.
            For color-index pixels, it is the size of the color index buffer.
        */
#if 0
        // ARGB8
        { WGL_COLOR_BITS_ARB, AFX_MIN(24, GetDeviceCaps(hDC, BITSPIXEL)) },
#endif
#if 0
        { WGL_RED_BITS_ARB, 8 },
        { WGL_GREEN_BITS_ARB, 8 },
        { WGL_BLUE_BITS_ARB, 8 },
        { WGL_ALPHA_BITS_ARB, 8 },
#endif
#if 0
        { WGL_ALPHA_SHIFT_ARB, 24 },
        { WGL_RED_SHIFT_ARB, 16 },
        { WGL_GREEN_SHIFT_ARB, 8 },
        { WGL_BLUE_SHIFT_ARB, 0 },
#endif
#if 0
        { WGL_DEPTH_BITS_ARB, 0 }, // { WGL_DEPTH_BITS_ARB, 24 },
        { WGL_STENCIL_BITS_ARB, 0 }, // { WGL_STENCIL_BITS_ARB, 8 },
#endif
#if 0
        { WGL_AUX_BUFFERS_ARB, 0 },
        { WGL_ACCUM_BITS_ARB, 0 },
        { WGL_ACCUM_ALPHA_BITS_ARB, 0 },
        { WGL_ACCUM_RED_BITS_ARB, 0 },
        { WGL_ACCUM_GREEN_BITS_ARB, 0 },
        { WGL_ACCUM_BLUE_BITS_ARB, 0 },
#endif
#if 0
        { WGL_TRANSPARENT_ARB, GL_FALSE },//{ WGL_TRANSPARENT_ARB, GL_TRUE },
        //{ WGL_SWAP_METHOD_ARB, WGL_SWAP_EXCHANGE_ARB },
        { WGL_SAMPLE_BUFFERS_ARB, GL_FALSE },
        //{ WGL_SAMPLE_BUFFERS_ARB, GL_TRUE },  // works on Intel, didn't work on Mesa
        { WGL_SAMPLES_ARB, 0 },  // works on Intel, didn't work on Mesa
        //{ WGL_COLORSPACE_EXT, WGL_COLORSPACE_SRGB_EXT }, // WGL_COLORSPACE_LINEAR_EXT // works on Mesa, didn't work on Intel
        //{ WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB, GL_TRUE }, // works on Mesa, didn't work on Intel
#endif
#if 0
        { WGL_SWAP_METHOD_ARB, WGL_SWAP_EXCHANGE_ARB },
#endif
#if 0
        { WGL_NUMBER_OVERLAYS_ARB, 0 },
        { WGL_NUMBER_UNDERLAYS_ARB, 0 },

        { WGL_SHARE_DEPTH_ARB, FALSE },
        { WGL_SHARE_STENCIL_ARB, FALSE },
        { WGL_SHARE_ACCUM_ARB, FALSE },

        { WGL_STEREO_ARB, FALSE },
#endif
        { NIL, NIL },
    };

    UINT formatCount;

    //if (unitIdx == 0)
        //wglLoadWsiSymbolsSIG(opengl32, NIL);

    if (!wglChoosePixelFormatARB(hDC, &pxlAttrPairs[0][0], NIL, 1, &dcPxlFmt, &formatCount))
    {
        AfxThrowError();
        ReleaseDC(hWnd, hDC), hDC = NIL;
        DestroyWindow(hWnd), hWnd = NIL;
        return err;
    }

    AFX_ASSERT(dcPxlFmt);
    AFX_ASSERT(formatCount);
    AfxZero(&dcPfd, sizeof(dcPfd));
    dcPfd.nSize = sizeof(dcPfd);

    if (!wglDescribePixelFormatWIN(hDC, dcPxlFmt, sizeof(dcPfd), &dcPfd))
    {
        if (!DescribePixelFormat(hDC, dcPxlFmt, sizeof(dcPfd), &dcPfd))
        {
            AfxThrowError();
            ReleaseDC(hWnd, hDC), hDC = NIL;
            DestroyWindow(hWnd), hWnd = NIL;
            return err;
        }
        else if (!SetPixelFormat(hDC, dcPxlFmt, &dcPfd))
        {
            AfxThrowError();
            ReleaseDC(hWnd, hDC), hDC = NIL;
            DestroyWindow(hWnd), hWnd = NIL;
            return err;
        }
    }
    else if ((!wglSetPixelFormatWIN(hDC, dcPxlFmt, &dcPfd)) && (!SetPixelFormat(hDC, dcPxlFmt, &dcPfd)))
    {
        AfxThrowError();
        ReleaseDC(hWnd, hDC), hDC = NIL;
        DestroyWindow(hWnd), hWnd = NIL;
        return err;
    }

    AFX_ASSERT(dcPfd.dwFlags & pfdFlags);

    if (_WGL_EXT_swap_control)
    {
        if (_WGL_EXT_swap_control_tear)
        {
            wglSwapIntervalEXT(-1);
        }
        else
        {
            wglSwapIntervalEXT(0);
        }
    }

    *phWnd = hWnd;
    *phDC = hDC;
    *pPixFmt = dcPxlFmt;
    *pPfd = dcPfd;

    return err;
}

_ZGL afxError wglCreateContextSIGMA(HDC hDC, HGLRC hShareCtx, int verMaj, int verMin, afxBool fwd, afxBool robust, afxBool dbg, HGLRC* phGLRC, glVmt* gl, afxBool echo)
{
    afxError err = { 0 };

    AFX_ASSERT(wglGetCurrentDCWIN);
    AFX_ASSERT(wglGetCurrentContextWIN);
    HDC bkpHdc = wglGetCurrentDCWIN();
    HGLRC bkpGlrc = wglGetCurrentContextWIN();

    int ctxAttrPairs[][2] =
    {
        // GL Extensions Viewer 6.0.9.0 benchmark tests appointed 3.2 core as faster profile when 3.1 was not. 4.0 caused major drops.
        //{ WGL_CONTEXT_MAJOR_VERSION_ARB, dpu->verMajor },
        { WGL_CONTEXT_MAJOR_VERSION_ARB, verMaj ? verMaj : 3 },
        //{ WGL_CONTEXT_MINOR_VERSION_ARB, dpu->verMinor },
        { WGL_CONTEXT_MINOR_VERSION_ARB, (!verMin && !verMaj) ? 2 : verMin },
        { WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB }, // WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB
        { WGL_CONTEXT_FLAGS_ARB, GL_NONE
#ifndef ZGL_DONT_USE_FWD_COMPATIBILITY // v3.3+
            | WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB
#endif
#ifndef ZGL_DONT_USE_ROBUST_ACCESS
            | WGL_CONTEXT_ROBUST_ACCESS_BIT_ARB
#endif
#ifdef _AFX_DEBUG
            | WGL_CONTEXT_DEBUG_BIT_ARB
#endif
        },
        { 0, 0 },
        { 0, 0 },
        { 0, 0 },
    };

    if (fwd)
        ctxAttrPairs[3][1] |= WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB;

    if (robust)
        ctxAttrPairs[3][1] |= WGL_CONTEXT_ROBUST_ACCESS_BIT_ARB;

    if (dbg)
    {
        ctxAttrPairs[3][1] |= WGL_CONTEXT_DEBUG_BIT_ARB;
    }
    else if (_WGL_ARB_create_context_no_error)
    {
        /*
            GL_KHR_no_error Extension:
            This extension allows OpenGL to skip error checking for better performance.
            It's useful in production environments where the application is known to be correct and validated.

            When you enable GL_KHR_no_error, the OpenGL driver:
                Skips all internal error checking.
                Does not detect or report errors like invalid enums, invalid states, buffer overflows, etc.
                Will not return error codes via glGetError(); because it doesn't track them anymore.
                Assumes your application is perfect and never makes invalid OpenGL calls.
            It's relying on you, the programmer, to "do everything right."

            Pros: Performance Gains.
                Error checking in OpenGL is not free; especially in driver layers. 
                Skipping it can significantly reduce overhead in performance-critical or high-frequency draw/update calls.

            Cons/Risks: No Safety Net if you accidentally:            
                Bind the wrong buffer,
                Use invalid shaders,
                Use unsupported formats,
                Overrun buffer boundaries...
                
            You may get undefined behavior, crashes, or silent visual bugs. Debugging becomes harder, because glGetError() 
            always returns GL_NO_ERROR and validation tools won't catch mistakes unless you're using external validation layers.
        */
        ctxAttrPairs[3][1] |= GL_CONTEXT_FLAG_NO_ERROR_BIT;
    }

    // hShareCtx can't be active in another thread
    HGLRC hGLRC = NIL;
    AFX_ASSERT(wglCreateContextAttribsARB);
    if (!(hGLRC = wglCreateContextAttribsARB(hDC, hShareCtx, (void*)ctxAttrPairs)))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT(wglMakeCurrentWIN);

    if (!(wglMakeCurrentWIN(hDC, hGLRC)))
    {
        AfxThrowError();
        wglDeleteContextWIN(hGLRC), hGLRC = NIL;
    }

    if (gl)
    {
        AFX_ASSERT(hOpengl32Dll);
        wglLoadSymbolsSIG(hOpengl32Dll, 0, sizeof(*gl) / sizeof(gl->ptr), (void**)gl, echo);
    }
    
    if (err)
        wglDeleteContextWIN(hGLRC), hGLRC = NIL;

    *phGLRC = hGLRC;

    wglMakeCurrentWIN(bkpHdc, bkpGlrc);

    return err;
}

_ZGL void wglDetectDeviceFeaturesSIGMA(glVmt const* gl, HDC hDC, avxFeatures* pFeatures)
{
    afxError err = { 0 };
    avxFeatures features = { 0 };
    
    features.robustness = glHasExtensionSIG(gl, "GL_ARB_robustness");

    features.sampleRateShading = glHasExtensionSIG(gl, "GL_ARB_sample_shading");
    features.shaderRasterGatherExt = glHasExtensionSIG(gl, "GL_ARB_texture_gather");
    features.shaderBufInt64Atomics = glHasExtensionSIG(gl, "GL_ARB_shader_atomic_counters");
    features.vtxPipelineStoresAndAtomics = glHasExtensionSIG(gl, "GL_ARB_shader_atomic_counter_ops"); // glHasExtensionSIG("ARB_shader_atomic_counters");

    features.voidCanvas = glHasExtensionSIG(gl, "GL_ARB_framebuffer_no_attachments");

    features.pipelineStatsQuery = glHasExtensionSIG(gl, "GL_ARB_pipeline_statistics_query"); // glHasExtensionSIG("EXT_pipeline_statistics_query");
    features.occlusionQueryPrecise = glHasExtensionSIG(gl, "GL_ARB_timer_query"); // glHasExtensionSIG("EXT_occlusion_query_boolean");

    // Depth and rasterization features
    features.depthClamp = glHasExtensionSIG(gl, "GL_ARB_depth_clamp");
    features.depthBiasClamp = glHasExtensionSIG(gl, "GL_ARB_polygon_offset_clamp"); // glHasExtensionSIG("EXT_depth_bias_clamp");
    features.depthBounds = glHasExtensionSIG(gl, "GL_EXT_depth_bounds_test");
    features.fillModeNonSolid = glHasExtensionSIG(gl, "GL_ARB_polygon_mode");
    features.wideLines = glHasExtensionSIG(gl, "GL_EXT_wide_line");
    features.largePoints = glHasExtensionSIG(gl, "GL_ARB_point_parameters"); // glHasExtensionSIG(gl, "GL_EXT_large_points");
    features.alphaToOne = glHasExtensionSIG(gl, "GL_EXT_texture_filter_anisotropic");


    features.primShader = glHasExtensionSIG(gl, "GL_ARB_geometry_shader4");
    features.tesselation = glHasExtensionSIG(gl, "GL_ARB_tessellation_shader");

    // texture
    features.dxt = glHasExtensionSIG(gl, "GL_ARB_compressed_texture_pixel_storage"); // glHasExtensionSIG("ARB_texture_compression_dxt1");
    features.etc2 = glHasExtensionSIG(gl, "GL_EXT_texture_compression_s3tc");
    features.astc_LDR = glHasExtensionSIG(gl, "GL_EXT_texture_compression_astc");
    features.rasterCubeArray = glHasExtensionSIG(gl, "GL_ARB_texture_cube_map_array");

    // sampler
    features.samplerAnisotropy = glHasExtensionSIG(gl, "GL_ARB_texture_filter_anisotropic"); // glHasExtensionSIG("EXT_texture_filter_anisotropic");
    features.samplerMirrorClampToEdge = glHasExtensionSIG(gl, "GL_ARB_texture_mirror_clamp_to_edge");
    features.samplerFilterMinMax = glHasExtensionSIG(gl, "GL_ARB_texture_filter_minmax");

    // viewport
    features.viewports = glHasExtensionSIG(gl, "GL_ARB_viewport_array"); // glHasExtensionSIG("EXT_multiview");
    features.cullDist = glHasExtensionSIG(gl, "GL_ARB_cull_distance");

    // MDI
    features.mdi = glHasExtensionSIG(gl, "GL_ARB_multi_draw_indirect");
    features.baseInst = glHasExtensionSIG(gl, "GL_ARB_base_instance");

    // framebuffer ops
    features.independentBlend = glHasExtensionSIG(gl, "GL_EXT_blend_func_separate");
    features.dualSrcBlend = glHasExtensionSIG(gl, "GL_EXT_blend_func_extended");
    features.logicOp = glHasExtensionSIG(gl, "GL_ARB_logic_op");

    /*
    afxBool8 fullDrawIdxUint32;
    afxBool8 independentBlend;
    afxBool8 dualSrcBlend;
    afxBool8 logicOp;
    afxBool8 fillModeNonSolid;
    afxBool8 wideLines;
    afxBool8 alphaToOne;
    afxBool8 etc2; // texture compression.
    afxBool8 astc_LDR; // texture compression.
    afxBool8 vtxPipelineStoresAndAtomics;
    afxBool8 fragStoresAndAtomics;
    afxBool8 shaderTessAndPrimPointSiz;
    afxBool8 shaderRasterGatherExt;
    afxBool8 shaderStorageRasterExtFmts;
    afxBool8 shaderStorageRasterMultisample;
    afxBool8 shaderStorageRasterReadWithoutFmt;
    afxBool8 shaderStorageRasterWriteWithoutFmt;
    afxBool8 shaderUboArrayDynIndexing;
    afxBool8 shaderSampledRasterArrayDynIndexing;
    afxBool8 shaderSsboArrayDynIndexing;
    afxBool8 shaderStorageImageArrayDynIndexing;
    afxBool8 shaderClipDist;
    afxBool8 shaderFloat64;
    afxBool8 shaderInt64;
    afxBool8 shaderInt16;
    afxBool8 shaderRsrcResidency;
    afxBool8 shaderRsrcMinLod;
    afxBool8 sparseBinding;
    afxBool8 sparseResidencyBuffer;
    afxBool8 sparseResidencyRaster2D;
    afxBool8 sparseResidencyRaster3D;
    afxBool8 sparseResidency2Samples;
    afxBool8 sparseResidency4Samples;
    afxBool8 sparseResidency8Samples;
    afxBool8 sparseResidency16Samples;
    afxBool8 sparseResidencyAliased;
    afxBool8 variableMultisampleRate;
    afxBool8 inheritedQueries;

    // compatible with PhysicalDeviceVulkan11Features
    afxBool8 ssbo16bitAccess;
    afxBool8 uboAndSsbo16bitAccess;
    afxBool8 storagePushConst16;
    afxBool8 storageInputOut16;
    afxBool8 multiview;
    afxBool8 multiviewPrimShader;
    afxBool8 multiviewTessShader;
    afxBool8 varPtrsStorageBuf;
    afxBool8 varPtrs;
    afxBool8 protectedMem;
    afxBool8 samplerYcbcrConv;
    afxBool8 shaderDrawParams;

    // compatible with PhysicalDeviceVulkan12Features
    afxBool8 samplerMirrorClampToEdge;
    afxBool8 drawIndirectCnt;
    afxBool8 ssbo8BitAccess;
    afxBool8 uboAndSsbo8bitAccess;
    afxBool8 storagePushConst8;
    afxBool8 shaderBufInt64Atomics;
    afxBool8 shaderSharedInt64Atomics;
    afxBool8 shaderFloat16;
    afxBool8 shaderInt8;
    afxBool8 descIndexing;

    // compatible with PhysicalDeviceDescriptorIndexingFeatures
    afxBool8 shaderInputAttachArrayDynamicIndexing;
    afxBool8 shaderUniformTexelBufArrayDynamicIndexing;
    afxBool8 shaderStorageTexelBufArrayDynamicIndexing;
    afxBool8 shaderUboArrayNonUniformIndexing;
    afxBool8 shaderSampledRasArrayNonUniformIndexing;
    afxBool8 shaderSsboArrayNonUniformIndexing;
    afxBool8 shaderStorageRasArrayNonUniformIndexing;
    afxBool8 shaderInputAttachmentArrayNonUniformIndexing;
    afxBool8 shaderUniformTexelBufArrayNonUniformIndexing;
    afxBool8 shaderStorageTexelBufArrayNonUniformIndexing;
    afxBool8 descBindingUboUpdAfterBind;
    afxBool8 descBindingSampledRasUpdAfterBind;
    afxBool8 descBindingStorageRasUpdAfterBind;
    afxBool8 descBindingSsboUpdAfterBind;
    afxBool8 descBindingUniformTexelBufUpdAfterBind;
    afxBool8 descBindingStorageTexelBufUpdAfterBind;
    afxBool8 descBindingUpdUnusedWhilePending;
    afxBool8 descBindingPartiallyBound;
    afxBool8 descBindingVarDescCnt;
    afxBool8 runtimeDescArray;

    afxBool8 samplerFilterMinMax;
    afxBool8 scalarBlockLayout;
    afxBool8 voidCanvas;
    afxBool8 uniformBufStdLayout;
    afxBool8 shaderSubgroupExtTypes;
    afxBool8 separateDepthStencilLayouts;
    afxBool8 hostQueryReset;
    afxBool8 timelineSemaphore;
    afxBool8 bufDevAddress;
    afxBool8 bufDevAddressCaptureReplay;
    afxBool8 bufDevAddressMultidev;
    afxBool8 vkMemModel;
    afxBool8 vkMemModelDevScope;
    afxBool8 vkMemModelAvailVisibChains;
    afxBool8 shaderOutViewportIdx;
    afxBool8 shaderOutLayer;
    afxBool8 subgroupBroadcastDynId;
    */

    AFX_ASSERT(pFeatures);
    *pFeatures = features;
}

_ZGL void wglDetectDeviceLimitsSIGMA(glVmt const* gl, avxLimits* pLimits)
{
    afxError err = { 0 };
    GLfloat dataf;
    GLfloat dataf2[3];
    GLint datai;
    GLint datai2[3];
    avxLimits limits = { 0 };

#if 0
    afxUnit maxMemAllocCnt;
    afxUnit maxSamplerAllocCnt;

    afxSize sparseAddrSpaceSiz;
    afxUnit maxBoundDescriptorSets;
    afxUnit maxPerStageDescriptorSamplers;
    afxUnit maxPerStageDescriptorUniformBuffers;
    afxUnit maxPerStageDescriptorStorageBuffers;
    afxUnit maxPerStageDescriptorSampledImages;
    afxUnit maxPerStageDescriptorStorageImages;
    afxUnit maxPerStageDescriptorInputAttachments;
    afxUnit maxPerStageResources;

    // the maximum supported texture image units that can be used to access texture maps from the vertex shader and the fragment processor combined.
    // If both the vertex shader and the fragment processing stage access the same texture image unit, then that counts as using two texture image units against this limit.
    // The value must be at least 48.
    //MAX_COMBINED_TEXTURE_IMAGE_UNITS
    afxUnit maxDescriptorSetSamplers;
    afxUnit maxDescriptorSetUniformBuffers;
    afxUnit maxDescriptorSetUniformBuffersDynamic;
    afxUnit maxDescriptorSetStorageBuffers;
    afxUnit maxDescriptorSetStorageBuffersDynamic;
    // the maximum supported texture image units that can be used to access texture maps from the fragment shader. The value must be at least 16.
    //GL_MAX_TEXTURE_IMAGE_UNITS
    afxUnit maxDescriptorSetSampledImages;
    afxUnit maxDescriptorSetStorageImages;
    afxUnit maxDescriptorSetInputAttachments;
    afxUnit maxFragCombinedOutputResources;

    afxUnit subPixelPrecisionBits;
    afxUnit subTexelPrecisionBits;
    afxUnit mipmapPrecisionBits;
    afxUnit maxDrawIndirectCnt;

    afxFlags storageRasterSampleCnts;
    afxBool timestampComputeAndGraphics;
    afxReal timestampPeriod;
    afxUnit discreteQueuePriorities;
    afxBool strictLines;
    afxBool standardSampleLocations; // GL_EXT_framebuffer_multisample, or version is 4.3 or higher.
    afxSize optimalBufCopyOffsetAlign;
    afxSize optimalBufCopyRowPitchAlign;
    afxSize nonCoherentAtomSiz;
#endif

    // the maximum number of uniform blocks per program.The value must be at least 70.
    // GL_MAX_COMBINED_UNIFORM_BLOCKS
    gl->GetIntegerv(GL_MAX_COMBINED_UNIFORM_BLOCKS, &datai);
    limits.maxPerStageUbos = datai;
    limits.maxPerLigaUbos = datai;
    gl->GetIntegerv(GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS, &datai);
    limits.maxPerStageSsbos = datai;
    limits.maxPerLigaSsbos = datai;
    gl->GetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &datai);
    limits.maxPerStageSampledImages = datai;
    limits.maxPerLigaSampledImages = datai;
    limits.maxPerStageSamplers = datai;
    limits.maxPerLigaSamplers = datai;

    // GL_MAX_TEXTURE_IMAGE_UNITS --- the maximum supported texture image units that can be used to access texture maps from the fragment shader. The value must be at least (GL) 16.
    gl->GetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &datai);
    //limits.maxPerLigaSamplers = datai;
    //limits.maxPerLigaSampledImages = datai;
    
    // GL_MAX_UNIFORM_BUFFER_BINDINGS --- the maximum number of uniform buffer binding points on the context, which must be at least 36.
    gl->GetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &datai);
    limits.maxPerLigaUbos = datai;
    // GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS --- the maximum number of shader storage buffer binding points on the context, which must be at least 8.
    gl->GetIntegerv(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS, &datai);
    limits.maxPerLigaSsbos = datai;


    ///////////////////////////////////////////
    // BUFFERIZATION

    // the minimum alignment in basic machine units of pointers returned from glMapBuffer and glMapBufferRange. This value must be a power of two and must be at least 64.
    gl->GetIntegerv(GL_MIN_MAP_BUFFER_ALIGNMENT, &datai);
    limits.minBufMapAlign = datai; // at least 64

    limits.optimalBufCopyOffsetAlign = AFX_SIMD_ALIGNMENT;
    limits.optimalBufCopyRowPitchAlign = AFX_SIMD_ALIGNMENT;
    
    // the minimum required alignment for uniform buffer sizes and offset. The initial value is 1.
    gl->GetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &datai);
    limits.minUboOffsetAlign = datai; // at least 64
    // the maximum size in basic machine units of a uniform block, which must be at least 16384.
    gl->GetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &datai);
    limits.maxUboRange = datai;

    // the minimum required alignment for shader storage buffer sizes and offset. The initial value is 1.
    gl->GetIntegerv(GL_SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT, &datai);
    limits.minSsboOffsetAlign = datai; // at least 64
    gl->GetIntegerv(GL_MAX_SHADER_STORAGE_BLOCK_SIZE, &datai);
    limits.maxSsboRange = datai;
    
    // the minimum required alignment for texture buffer sizes and offset. The initial value is 1.
    gl->GetIntegerv(GL_TEXTURE_BUFFER_OFFSET_ALIGNMENT, &datai);
    limits.minTboOffsetAlign = datai; // at least 64
    // The value gives the maximum number of texels allowed in the texel array of a texture buffer object. Value must be at least 65536.
    gl->GetIntegerv(GL_MAX_TEXTURE_BUFFER_SIZE, &datai);
    limits.maxTboCap = datai;
    // the minimum required alignment for texture buffer sizes and offset. The initial value is 1.
    gl->GetIntegerv(GL_TEXTURE_BUFFER_OFFSET_ALIGNMENT, &datai);
    limits.bufferRasterGranularity = datai;

    // A rough estimate of the largest texture that the GL can handle. The value must be at least 1024.
    gl->GetIntegerv(GL_MAX_TEXTURE_SIZE, &datai);
    limits.maxRasterDim1D = datai; // at least 1024
    limits.maxRasterDim2D = datai; // at least 1024
    // a rough estimate of the largest 3D texture that the GL can handle. The value must be at least 64.
    gl->GetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &datai);
    limits.maxRasterDim3D = datai; // at least 64
    // the maximum number of layers allowed in an array texture, and must be at least 256.
    gl->GetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &datai);
    limits.maxRasterLayers = datai; // at least 256
    // a rough estimate of the largest cube-map texture that the GL can handle. The value must be at least 1024.
    gl->GetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE, &datai);
    limits.maxRasterDimCube = datai; // at least 1024

    //////////////////////////////////////
    // VERTEX PROCESSING

    // the maximum number of 4-component generic vertex attributes accessible to a vertex shader. The value must be at least 16.
    gl->GetIntegerv(GL_MAX_VERTEX_ATTRIBS, &datai);
    limits.maxVtxIns = datai; // at least 16
    // the maximum number of components of output written by a vertex shader, which must be at least 64.
    gl->GetIntegerv(GL_MAX_VERTEX_OUTPUT_COMPONENTS, &datai);
    limits.maxVtxOutCompos = datai; // at least 64
    // the maximum number of vertex buffers that may be bound.
    gl->GetIntegerv(GL_MAX_VERTEX_ATTRIB_BINDINGS, &datai);
    limits.maxVtxInSrcs = datai;
    // the maximum offset that may be added to a vertex binding offset.
    gl->GetIntegerv(GL_MAX_VERTEX_ATTRIB_RELATIVE_OFFSET, &datai);
    limits.maxVtxInOffset = datai;
    // the maximum value of stride parameters to vertex array pointer-setting commands.
    gl->GetIntegerv(GL_MAX_VERTEX_ATTRIB_STRIDE, &datai);
    limits.maxVtxInSrcStride = datai;

    /////////////////////////////////////////////
    // PRIMITIVE PROCESSING

    // the maximum number of application-defined clipping distances. The value must be at least 8.
    gl->GetIntegerv(GL_MAX_CLIP_DISTANCES, &datai);
    limits.maxClipDistances = datai; // at least 8
    gl->GetIntegerv(GL_MAX_CULL_DISTANCES, &datai);
    limits.maxCullDistances = datai; // at least 8
    gl->GetIntegerv(GL_MAX_COMBINED_CLIP_AND_CULL_DISTANCES, &datai);
    limits.maxCombinedClipAndCullDistances = datai; // at least 8

    // the maximum number of simultaneous viewports that are supported. The value must be at least 16.
    gl->GetIntegerv(GL_MAX_VIEWPORTS, &datai);
    limits.maxVpCnt = datai; // at least 16
    // the maximum supported width and height of the viewport. These must be at least as large as the visible dimensions of the display being rendered to.
    gl->GetIntegerv(GL_MAX_VIEWPORT_DIMS, datai2);
    limits.maxVpDimensions[0] = datai2[0];
    limits.maxVpDimensions[1] = datai2[1];
    // the minimum and maximum viewport bounds range. The minimum range should be at least [-32768, 32767].
    gl->GetFloatv(GL_VIEWPORT_BOUNDS_RANGE, dataf2);
    limits.vpBoundsRange[0] = dataf2[0];  // at least [-32768, 32767]
    limits.vpBoundsRange[1] = dataf2[1];  // at least [-32768, 32767]
    // the number of bits of sub-pixel precision which the GL uses to interpret the floating point viewport bounds. The minimum value is 0.
    gl->GetIntegerv(GL_VIEWPORT_SUBPIXEL_BITS, &datai);
    limits.vpSubPixelBits = datai; // at least 0

    // the maximum number of components of inputs read by a geometry shader, which must be at least 64.
    gl->GetIntegerv(GL_MAX_GEOMETRY_INPUT_COMPONENTS, &datai);
    limits.maxPrimInComps = datai; // at least 64
    // the maximum number of components of outputs written by a geometry shader, which must be at least 128.
    gl->GetIntegerv(GL_MAX_GEOMETRY_OUTPUT_COMPONENTS, &datai);
    limits.maxPrimOutComps = datai; // at least 128

    // the implementation-dependent maximum tessellation level (the value of MAX_TESS_GEN_LEVEL).
    gl->GetIntegerv(GL_MAX_TESS_GEN_LEVEL, &datai);
    limits.maxTessGenLvl = datai;
    // the implementation-dependent maximum patch size (the value of MAX_PATCH_VERTICES). The patch size is initially three vertices.
    gl->GetIntegerv(GL_MAX_PATCH_VERTICES, &datai);
    limits.maxTessPatchSiz = datai;
    gl->GetIntegerv(GL_MAX_TESS_CONTROL_INPUT_COMPONENTS, &datai);
    limits.maxTessCtrlPerVtxInComps = datai;
    gl->GetIntegerv(GL_MAX_TESS_CONTROL_OUTPUT_COMPONENTS, &datai);
    limits.maxTessCtrlPerVtxOutComps = datai;
    // The number of components of active per-patch output variables may not exceed the value of MAX_TESS_PATCH_COMPONENTS
    gl->GetIntegerv(GL_MAX_TESS_PATCH_COMPONENTS, &datai);
    limits.maxTessCtrlPerPatchOutComps = datai;
    gl->GetIntegerv(GL_MAX_TESS_CONTROL_TOTAL_OUTPUT_COMPONENTS, &datai);
    limits.maxTessCtrlTotalOutComps = datai;
    gl->GetIntegerv(GL_MAX_TESS_EVALUATION_INPUT_COMPONENTS, &datai);
    limits.maxTessEvalInComps = datai;
    gl->GetIntegerv(GL_MAX_TESS_EVALUATION_OUTPUT_COMPONENTS, &datai);
    limits.maxTessEvalOutComps = datai;

    gl->GetIntegerv(GL_MAX_GEOMETRY_SHADER_INVOCATIONS, &datai);
    limits.maxPrimShadInvocations = datai;
    gl->GetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES, &datai);
    limits.maxPrimOutVertices = datai;
    gl->GetIntegerv(GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS, &datai);
    limits.maxPrimTotalOutComps = datai;

    /////////////////////////////////////
    // CANVAS

    gl->GetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &datai);
    limits.maxColorAttachments = datai;
    // 4.3 --- the maximum width for a framebuffer that has no attachments, which must be at least 16384.
    gl->GetIntegerv(GL_MAX_FRAMEBUFFER_WIDTH, &datai);
    limits.maxCanvasWhd.w = datai; // at least 16384
    // 4.3 --- the maximum height for a framebuffer that has no attachments, which must be at least 16384.
    gl->GetIntegerv(GL_MAX_FRAMEBUFFER_HEIGHT, &datai);
    limits.maxCanvasWhd.h = datai; // at least 16384
    // 4.3 --- the maximum number of layers for a framebuffer that has no attachments, which must be at least 2048.
    gl->GetIntegerv(GL_MAX_FRAMEBUFFER_LAYERS, &datai);
    limits.maxCanvasWhd.d = datai; // at least 2048
     // the maximum samples in a framebuffer that has no attachments, which must be at least 4.
    gl->GetIntegerv(GL_MAX_FRAMEBUFFER_SAMPLES, &datai);
    limits.canvasNoAttachmentsSampleCnts = datai;

    //////////////////////////////
    // SAMPLING

    // the maximum, absolute value of the texture level-of-detail bias. The value must be at least 2.0.
    gl->GetFloatv(GL_MAX_TEXTURE_LOD_BIAS, &dataf);
    limits.maxSamplerLodBias = dataf; // at least 2.0
    gl->GetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &dataf);
    limits.maxSamplerAnisotropy = dataf;

    ////////////////////////////////////////////
    // MULTISAMPLING

    // the maximum number of sample mask words.
    gl->GetIntegerv(GL_MAX_SAMPLE_MASK_WORDS, &datai);
    limits.maxSampleMaskWords = datai;
    // the maximum number of samples supported in integer format multisample buffers.
    gl->GetIntegerv(GL_MAX_INTEGER_SAMPLES, &datai);
    limits.sampledRasterIntegerSampleCnts = datai;
    // the maximum number of samples in a color multisample texture.
    gl->GetIntegerv(GL_MAX_COLOR_TEXTURE_SAMPLES, &datai);
    limits.sampledRasterColorSampleCnts = datai;
    limits.canvasColorSampleCnts = datai;
    // the maximum number of samples in a multisample depth or depth-stencil texture.
    gl->GetIntegerv(GL_MAX_DEPTH_TEXTURE_SAMPLES, &datai);
    limits.sampledRasterDepthSampleCnts = datai;
    limits.sampledRasterStencilSampleCnts = datai;
    limits.canvasDepthSampleCnts = datai;
    limits.canvasStencilSampleCnts = datai;

    ////////////////////////////////////////////
    // RASTERIZATION

    // the smallest and largest supported sizes for antialiased points. The smallest size must be at most 1, and the largest size must be at least 1.
    gl->GetFloatv(GL_POINT_SIZE_RANGE, dataf2);
    limits.pointSizRange[0] = dataf2[0]; // at least 1
    limits.pointSizRange[1] = dataf2[1]; // at least 1
    // the size difference between adjacent supported sizes for antialiased points.
    gl->GetFloatv(GL_POINT_SIZE_GRANULARITY, &dataf);
    limits.pointSizGranularity = dataf;
    // the range of widths supported for smooth (antialiased) lines.
    gl->GetFloatv(GL_LINE_WIDTH_RANGE, dataf2);
    limits.lineWidthRange[0] = dataf2[0];
    limits.lineWidthRange[1] = dataf2[1];
    // the level of quantization applied to smooth line width parameters.
    gl->GetFloatv(GL_LINE_WIDTH_GRANULARITY, &dataf);
    limits.lineWidthGranularity = dataf;

    ///////////////////////////////
    // FRAGMENT PROCESSING

    // the maximum number of components of the inputs read by the fragment shader, which must be at least 128.
    gl->GetIntegerv(GL_MAX_FRAGMENT_INPUT_COMPONENTS, &datai);
    limits.maxFragInComps = datai; // at least 128
    // the maximum number of simultaneous outputs that may be written in a fragment shader. The value must be at least 8.
    gl->GetIntegerv(GL_MAX_DRAW_BUFFERS, &datai);
    limits.maxFragOutAttachments = datai; // at least 8
    // the maximum number of active draw buffers when using dual-source blending. The value must be at least 1.
    gl->GetIntegerv(GL_MAX_DUAL_SOURCE_DRAW_BUFFERS, &datai);
    limits.maxFragDualSrcAttachments = datai; // at least 1

    // the minimum texel offset allowed in a texture lookup, which must be at most -8.
    gl->GetIntegerv(GL_MIN_PROGRAM_TEXEL_OFFSET, &datai);
    limits.minTexelOffset = datai; // at least 1
    // the maximum texel offset allowed in a texture lookup, which must be at least 7.
    gl->GetIntegerv(GL_MAX_PROGRAM_TEXEL_OFFSET, &datai);
    limits.maxTexelOffset = datai; // at least 1
    // For all instructions except textureGather, the limits are the values of MIN_PROGRAM_TEXEL_OFFSET and MAX_PROGRAM_TEXEL_OFFSET. 
    // For the textureGather instruction, the limits are the values of MIN_PROGRAM_TEXTURE_GATHER_OFFSET and MAX_PROGRAM_TEXTURE_GATHER_OFFSET.
    gl->GetIntegerv(GL_MIN_PROGRAM_TEXTURE_GATHER_OFFSET, &datai);
    limits.minTexelGatherOffset = datai; // at least 1
    gl->GetIntegerv(GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET, &datai);
    limits.maxTexelGatherOffset = datai; // at least 1

    // The built-in function interpolateAtOffset will sample variables at a specified(x, y) offset relative to the center of the pixel.The range and granularity of offsets supported by this function is implementation-dependent.
    // If either component of the specified offset is less than the value of MIN_FRAGMENT_INTERPOLATION_OFFSET or greater than the value of MAX_FRAGMENT_INTERPOLATION_OFFSET, the position used to interpolate the variable is undefined.
    // Not all values of offset may be supported; x and y offsets may be rounded to fixed-point values with the number of fraction bits given by the value of the implementation-dependent constant FRAGMENT_INTERPOLATION_OFFSET_BITS.
    gl->GetFloatv(GL_MIN_FRAGMENT_INTERPOLATION_OFFSET, &dataf);
    limits.minInterpolationOffset = dataf; // at least 1
    gl->GetFloatv(GL_MAX_FRAGMENT_INTERPOLATION_OFFSET, &dataf);
    limits.maxInterpolationOffset = dataf; // at least 1
    gl->GetIntegerv(GL_FRAGMENT_INTERPOLATION_OFFSET_BITS, &datai);
    limits.subPixelInterpolationOffsetBits = datai; // at least 1

    ///////////////////////////////////////
    // COMPUTING

    // the number of invocations in a single local work group (i.e., the product of the three dimensions) that may be dispatched to a compute shader.
    gl->GetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &datai);
    limits.maxComputeWarpInvocations = datai;
    // the maximum number of work groups that may be dispatched to a compute shader. Indices 0, 1, and 2 correspond to the X, Y and Z dimensions, respectively.
    gl->GetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_COUNT, datai2);
    limits.maxComputeWarpCnt.w = datai2[0];
    limits.maxComputeWarpCnt.h = datai2[1];
    limits.maxComputeWarpCnt.d = datai2[2];
    // the maximum size of a work groups that may be used during compilation of a compute shader. Indices 0, 1, and 2 correspond to the X, Y and Z dimensions, respectively.
    gl->GetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_SIZE, datai2);
    limits.maxComputeWarpSiz.w = datai2[0];
    limits.maxComputeWarpSiz.h = datai2[1];
    limits.maxComputeWarpSiz.d = datai2[2];

    gl->GetIntegerv(GL_MAX_COMPUTE_SHARED_MEMORY_SIZE, &datai);
    limits.maxComputeSharedMemSiz = datai;

    // the maximum index that may be specified during the transfer of generic vertex attributes to the GL.
    gl->GetIntegerv(GL_MAX_ELEMENT_INDEX, &datai);
    limits.maxDrawIndexedIdxValue = datai;

    // silent any error generated by passing unsupported (effectively invalid) params.
    GLenum glErr = gl->GetError();

    limits.maxBoundPerLigas = 4;
    limits.maxPushConstsSiz = 128;

    limits.maxSamplerAllocCnt = AFX_U32_MAX;
    
    AFX_ASSERT(pLimits);
    *pLimits = limits;
}

BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,  // handle to DLL module
    DWORD fdwReason,     // reason for calling function
    LPVOID lpvReserved)  // reserved
{
    afxError err = { 0 };
    // Perform actions based on the reason for calling.
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        // Initialize once for each new process.
        // Return FALSE to fail DLL load.

        if (!vgiWndClssRegistered)
        {
            AfxZero(&vgiWndClss, sizeof(vgiWndClss));
            vgiWndClss.cbSize = sizeof(vgiWndClss); // only on EX
            vgiWndClss.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
            vgiWndClss.lpfnWndProc = _ZglTmpWndPrcCbW32;
            vgiWndClss.cbClsExtra = 0;
            vgiWndClss.cbWndExtra = 0;
            vgiWndClss.hInstance = hinstDLL;// GetModuleHandleA(NULL);
            vgiWndClss.hIcon = LoadIconA(NULL, IDI_SHIELD);
            vgiWndClss.hCursor = LoadCursorA(NULL, IDC_ARROW);
            vgiWndClss.hbrBackground = NULL;
            vgiWndClss.lpszMenuName = NULL;
            vgiWndClss.lpszClassName = "SIGMA GL/2 Continuous Integration --- Qwadro Video Graphics Infrastructure (c) 2017 SIGMA --- Public Test Build";
            //vgiWndClss.hIconSm = LoadIconA(NULL, IDI_SHIELD);

            if (!(RegisterClassExA(&vgiWndClss)))
            {
                AfxThrowError();
                return FALSE;
            }
            vgiWndClssRegistered = TRUE;
        }
        break;

    case DLL_THREAD_ATTACH:
        // Do thread-specific initialization.
        break;

    case DLL_THREAD_DETACH:
        // Do thread-specific cleanup.
        break;

    case DLL_PROCESS_DETACH:

        if (lpvReserved != NIL)
        {
            break; // do not do cleanup if process termination scenario
        }

        if (vgiWndClssRegistered)
        {
            UnregisterClassA(vgiWndClss.lpszClassName, vgiWndClss.hInstance);
            AfxZero(&vgiWndClss, sizeof(vgiWndClss));
            vgiWndClssRegistered = FALSE;
        }

        // Perform any necessary cleanup.
        break;
    }
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}
