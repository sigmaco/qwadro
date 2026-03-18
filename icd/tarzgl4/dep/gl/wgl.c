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

/**
 * SPDX-License-Identifier: (WTFPL OR CC0-1.0) AND Apache-2.0
 */
#include "icd_tarzgl4/src/zglInterop_WGL.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "wgl.h"

#ifndef GLAD_IMPL_UTIL_C_
#define GLAD_IMPL_UTIL_C_

#ifdef _MSC_VER
#define GLAD_IMPL_UTIL_SSCANF sscanf_s
#else
#define GLAD_IMPL_UTIL_SSCANF sscanf
#endif

#endif /* GLAD_IMPL_UTIL_C_ */

#ifdef __cplusplus
extern "C" {
#endif



int /*GLAD*/_WGL_VERSION_1_0 = 0;
int /*GLAD*/_WGL_3DFX_multisample = 0;
int /*GLAD*/_WGL_3DL_stereo_control = 0;
int /*GLAD*/_WGL_AMD_gpu_association = 0;
int /*GLAD*/_WGL_ARB_buffer_region = 0;
int /*GLAD*/_WGL_ARB_context_flush_control = 0;
int /*GLAD*/_WGL_ARB_create_context = 0;
int /*GLAD*/_WGL_ARB_create_context_no_error = 0;
int /*GLAD*/_WGL_ARB_create_context_profile = 0;
int /*GLAD*/_WGL_ARB_create_context_robustness = 0;
int /*GLAD*/_WGL_ARB_extensions_string = 0;
int /*GLAD*/_WGL_ARB_framebuffer_sRGB = 0;
int /*GLAD*/_WGL_ARB_make_current_read = 0;
int /*GLAD*/_WGL_ARB_multisample = 0;
int /*GLAD*/_WGL_ARB_pbuffer = 0;
int /*GLAD*/_WGL_ARB_pixel_format = 0;
int /*GLAD*/_WGL_ARB_pixel_format_float = 0;
int /*GLAD*/_WGL_ARB_render_texture = 0;
int /*GLAD*/_WGL_ARB_robustness_application_isolation = 0;
int /*GLAD*/_WGL_ARB_robustness_share_group_isolation = 0;
int /*GLAD*/_WGL_ATI_pixel_format_float = 0;
int /*GLAD*/_WGL_ATI_render_texture_rectangle = 0;
int /*GLAD*/_WGL_EXT_colorspace = 0;
int /*GLAD*/_WGL_EXT_create_context_es2_profile = 0;
int /*GLAD*/_WGL_EXT_create_context_es_profile = 0;
int /*GLAD*/_WGL_EXT_depth_float = 0;
int /*GLAD*/_WGL_EXT_display_color_table = 0;
int /*GLAD*/_WGL_EXT_extensions_string = 0;
int /*GLAD*/_WGL_EXT_framebuffer_sRGB = 0;
int /*GLAD*/_WGL_EXT_make_current_read = 0;
int /*GLAD*/_WGL_EXT_multisample = 0;
int /*GLAD*/_WGL_EXT_pbuffer = 0;
int /*GLAD*/_WGL_EXT_pixel_format = 0;
int /*GLAD*/_WGL_EXT_pixel_format_packed_float = 0;
int /*GLAD*/_WGL_EXT_swap_control = 0;
int /*GLAD*/_WGL_EXT_swap_control_tear = 0;
int /*GLAD*/_WGL_I3D_digital_video_control = 0;
int /*GLAD*/_WGL_I3D_gamma = 0;
int /*GLAD*/_WGL_I3D_genlock = 0;
int /*GLAD*/_WGL_I3D_image_buffer = 0;
int /*GLAD*/_WGL_I3D_swap_frame_lock = 0;
int /*GLAD*/_WGL_I3D_swap_frame_usage = 0;
int /*GLAD*/_WGL_NV_DX_interop = 0;
int /*GLAD*/_WGL_NV_DX_interop2 = 0;
int /*GLAD*/_WGL_NV_copy_image = 0;
int /*GLAD*/_WGL_NV_delay_before_swap = 0;
int /*GLAD*/_WGL_NV_float_buffer = 0;
int /*GLAD*/_WGL_NV_gpu_affinity = 0;
int /*GLAD*/_WGL_NV_multigpu_context = 0;
int /*GLAD*/_WGL_NV_multisample_coverage = 0;
int /*GLAD*/_WGL_NV_present_video = 0;
int /*GLAD*/_WGL_NV_render_depth_texture = 0;
int /*GLAD*/_WGL_NV_render_texture_rectangle = 0;
int /*GLAD*/_WGL_NV_swap_group = 0;
int /*GLAD*/_WGL_NV_vertex_array_range = 0;
int /*GLAD*/_WGL_NV_video_capture = 0;
int /*GLAD*/_WGL_NV_video_output = 0;
int /*GLAD*/_WGL_OML_sync_control = 0;



PFNWGLALLOCATEMEMORYNVPROC /*glad_*/wglAllocateMemoryNV = NULL;
PFNWGLASSOCIATEIMAGEBUFFEREVENTSI3DPROC /*glad_*/wglAssociateImageBufferEventsI3D = NULL;
PFNWGLBEGINFRAMETRACKINGI3DPROC /*glad_*/wglBeginFrameTrackingI3D = NULL;
PFNWGLBINDDISPLAYCOLORTABLEEXTPROC /*glad_*/wglBindDisplayColorTableEXT = NULL;
PFNWGLBINDSWAPBARRIERNVPROC /*glad_*/wglBindSwapBarrierNV = NULL;
PFNWGLBINDTEXIMAGEARBPROC /*glad_*/wglBindTexImageARB = NULL;
PFNWGLBINDVIDEOCAPTUREDEVICENVPROC /*glad_*/wglBindVideoCaptureDeviceNV = NULL;
PFNWGLBINDVIDEODEVICENVPROC /*glad_*/wglBindVideoDeviceNV = NULL;
PFNWGLBINDVIDEOIMAGENVPROC /*glad_*/wglBindVideoImageNV = NULL;
PFNWGLBLITCONTEXTFRAMEBUFFERAMDPROC /*glad_*/wglBlitContextFramebufferAMD = NULL;
PFNWGLCHOOSEPIXELFORMATARBPROC /*glad_*/wglChoosePixelFormatARB = NULL;
PFNWGLCHOOSEPIXELFORMATEXTPROC /*glad_*/wglChoosePixelFormatEXT = NULL;
PFNWGLCOPYIMAGESUBDATANVPROC /*glad_*/wglCopyImageSubDataNV = NULL;
PFNWGLCREATEAFFINITYDCNVPROC /*glad_*/wglCreateAffinityDCNV = NULL;
PFNWGLCREATEASSOCIATEDCONTEXTAMDPROC /*glad_*/wglCreateAssociatedContextAMD = NULL;
PFNWGLCREATEASSOCIATEDCONTEXTATTRIBSAMDPROC /*glad_*/wglCreateAssociatedContextAttribsAMD = NULL;
PFNWGLCREATEBUFFERREGIONARBPROC /*glad_*/wglCreateBufferRegionARB = NULL;
PFNWGLCREATECONTEXTATTRIBSARBPROC /*glad_*/wglCreateContextAttribsARB = NULL;
PFNWGLCREATEDISPLAYCOLORTABLEEXTPROC /*glad_*/wglCreateDisplayColorTableEXT = NULL;
PFNWGLCREATEIMAGEBUFFERI3DPROC /*glad_*/wglCreateImageBufferI3D = NULL;
PFNWGLCREATEPBUFFERARBPROC /*glad_*/wglCreatePbufferARB = NULL;
PFNWGLCREATEPBUFFEREXTPROC /*glad_*/wglCreatePbufferEXT = NULL;
PFNWGLDXCLOSEDEVICENVPROC /*glad_*/wglDXCloseDeviceNV = NULL;
PFNWGLDXLOCKOBJECTSNVPROC /*glad_*/wglDXLockObjectsNV = NULL;
PFNWGLDXOBJECTACCESSNVPROC /*glad_*/wglDXObjectAccessNV = NULL;
PFNWGLDXOPENDEVICENVPROC /*glad_*/wglDXOpenDeviceNV = NULL;
PFNWGLDXREGISTEROBJECTNVPROC /*glad_*/wglDXRegisterObjectNV = NULL;
PFNWGLDXSETRESOURCESHAREHANDLENVPROC /*glad_*/wglDXSetResourceShareHandleNV = NULL;
PFNWGLDXUNLOCKOBJECTSNVPROC /*glad_*/wglDXUnlockObjectsNV = NULL;
PFNWGLDXUNREGISTEROBJECTNVPROC /*glad_*/wglDXUnregisterObjectNV = NULL;
PFNWGLDELAYBEFORESWAPNVPROC /*glad_*/wglDelayBeforeSwapNV = NULL;
PFNWGLDELETEASSOCIATEDCONTEXTAMDPROC /*glad_*/wglDeleteAssociatedContextAMD = NULL;
PFNWGLDELETEBUFFERREGIONARBPROC /*glad_*/wglDeleteBufferRegionARB = NULL;
PFNWGLDELETEDCNVPROC /*glad_*/wglDeleteDCNV = NULL;
PFNWGLDESTROYDISPLAYCOLORTABLEEXTPROC /*glad_*/wglDestroyDisplayColorTableEXT = NULL;
PFNWGLDESTROYIMAGEBUFFERI3DPROC /*glad_*/wglDestroyImageBufferI3D = NULL;
PFNWGLDESTROYPBUFFERARBPROC /*glad_*/wglDestroyPbufferARB = NULL;
PFNWGLDESTROYPBUFFEREXTPROC /*glad_*/wglDestroyPbufferEXT = NULL;
PFNWGLDISABLEFRAMELOCKI3DPROC /*glad_*/wglDisableFrameLockI3D = NULL;
PFNWGLDISABLEGENLOCKI3DPROC /*glad_*/wglDisableGenlockI3D = NULL;
PFNWGLENABLEFRAMELOCKI3DPROC /*glad_*/wglEnableFrameLockI3D = NULL;
PFNWGLENABLEGENLOCKI3DPROC /*glad_*/wglEnableGenlockI3D = NULL;
PFNWGLENDFRAMETRACKINGI3DPROC /*glad_*/wglEndFrameTrackingI3D = NULL;
PFNWGLENUMGPUDEVICESNVPROC /*glad_*/wglEnumGpuDevicesNV = NULL;
PFNWGLENUMGPUSFROMAFFINITYDCNVPROC /*glad_*/wglEnumGpusFromAffinityDCNV = NULL;
PFNWGLENUMGPUSNVPROC /*glad_*/wglEnumGpusNV = NULL;
PFNWGLENUMERATEVIDEOCAPTUREDEVICESNVPROC /*glad_*/wglEnumerateVideoCaptureDevicesNV = NULL;
PFNWGLENUMERATEVIDEODEVICESNVPROC /*glad_*/wglEnumerateVideoDevicesNV = NULL;
PFNWGLFREEMEMORYNVPROC /*glad_*/wglFreeMemoryNV = NULL;
PFNWGLGENLOCKSAMPLERATEI3DPROC /*glad_*/wglGenlockSampleRateI3D = NULL;
PFNWGLGENLOCKSOURCEDELAYI3DPROC /*glad_*/wglGenlockSourceDelayI3D = NULL;
PFNWGLGENLOCKSOURCEEDGEI3DPROC /*glad_*/wglGenlockSourceEdgeI3D = NULL;
PFNWGLGENLOCKSOURCEI3DPROC /*glad_*/wglGenlockSourceI3D = NULL;
PFNWGLGETCONTEXTGPUIDAMDPROC /*glad_*/wglGetContextGPUIDAMD = NULL;
PFNWGLGETCURRENTASSOCIATEDCONTEXTAMDPROC /*glad_*/wglGetCurrentAssociatedContextAMD = NULL;
PFNWGLGETCURRENTREADDCARBPROC /*glad_*/wglGetCurrentReadDCARB = NULL;
PFNWGLGETCURRENTREADDCEXTPROC /*glad_*/wglGetCurrentReadDCEXT = NULL;
PFNWGLGETDIGITALVIDEOPARAMETERSI3DPROC /*glad_*/wglGetDigitalVideoParametersI3D = NULL;
PFNWGLGETEXTENSIONSSTRINGARBPROC /*glad_*/wglGetExtensionsStringARB = NULL;
PFNWGLGETEXTENSIONSSTRINGEXTPROC /*glad_*/wglGetExtensionsStringEXT = NULL;
PFNWGLGETFRAMEUSAGEI3DPROC /*glad_*/wglGetFrameUsageI3D = NULL;
PFNWGLGETGPUIDSAMDPROC /*glad_*/wglGetGPUIDsAMD = NULL;
PFNWGLGETGPUINFOAMDPROC /*glad_*/wglGetGPUInfoAMD = NULL;
PFNWGLGETGAMMATABLEI3DPROC /*glad_*/wglGetGammaTableI3D = NULL;
PFNWGLGETGAMMATABLEPARAMETERSI3DPROC /*glad_*/wglGetGammaTableParametersI3D = NULL;
PFNWGLGETGENLOCKSAMPLERATEI3DPROC /*glad_*/wglGetGenlockSampleRateI3D = NULL;
PFNWGLGETGENLOCKSOURCEDELAYI3DPROC /*glad_*/wglGetGenlockSourceDelayI3D = NULL;
PFNWGLGETGENLOCKSOURCEEDGEI3DPROC /*glad_*/wglGetGenlockSourceEdgeI3D = NULL;
PFNWGLGETGENLOCKSOURCEI3DPROC /*glad_*/wglGetGenlockSourceI3D = NULL;
PFNWGLGETMSCRATEOMLPROC /*glad_*/wglGetMscRateOML = NULL;
PFNWGLGETPBUFFERDCARBPROC /*glad_*/wglGetPbufferDCARB = NULL;
PFNWGLGETPBUFFERDCEXTPROC /*glad_*/wglGetPbufferDCEXT = NULL;
PFNWGLGETPIXELFORMATATTRIBFVARBPROC /*glad_*/wglGetPixelFormatAttribfvARB = NULL;
PFNWGLGETPIXELFORMATATTRIBFVEXTPROC /*glad_*/wglGetPixelFormatAttribfvEXT = NULL;
PFNWGLGETPIXELFORMATATTRIBIVARBPROC /*glad_*/wglGetPixelFormatAttribivARB = NULL;
PFNWGLGETPIXELFORMATATTRIBIVEXTPROC /*glad_*/wglGetPixelFormatAttribivEXT = NULL;
PFNWGLGETSWAPINTERVALEXTPROC /*glad_*/wglGetSwapIntervalEXT = NULL;
PFNWGLGETSYNCVALUESOMLPROC /*glad_*/wglGetSyncValuesOML = NULL;
PFNWGLGETVIDEODEVICENVPROC /*glad_*/wglGetVideoDeviceNV = NULL;
PFNWGLGETVIDEOINFONVPROC /*glad_*/wglGetVideoInfoNV = NULL;
PFNWGLISENABLEDFRAMELOCKI3DPROC /*glad_*/wglIsEnabledFrameLockI3D = NULL;
PFNWGLISENABLEDGENLOCKI3DPROC /*glad_*/wglIsEnabledGenlockI3D = NULL;
PFNWGLJOINSWAPGROUPNVPROC /*glad_*/wglJoinSwapGroupNV = NULL;
PFNWGLLOADDISPLAYCOLORTABLEEXTPROC /*glad_*/wglLoadDisplayColorTableEXT = NULL;
PFNWGLLOCKVIDEOCAPTUREDEVICENVPROC /*glad_*/wglLockVideoCaptureDeviceNV = NULL;
PFNWGLMAKEASSOCIATEDCONTEXTCURRENTAMDPROC /*glad_*/wglMakeAssociatedContextCurrentAMD = NULL;
PFNWGLMAKECONTEXTCURRENTARBPROC /*glad_*/wglMakeContextCurrentARB = NULL;
PFNWGLMAKECONTEXTCURRENTEXTPROC /*glad_*/wglMakeContextCurrentEXT = NULL;
PFNWGLQUERYCURRENTCONTEXTNVPROC /*glad_*/wglQueryCurrentContextNV = NULL;
PFNWGLQUERYFRAMECOUNTNVPROC /*glad_*/wglQueryFrameCountNV = NULL;
PFNWGLQUERYFRAMELOCKMASTERI3DPROC /*glad_*/wglQueryFrameLockMasterI3D = NULL;
PFNWGLQUERYFRAMETRACKINGI3DPROC /*glad_*/wglQueryFrameTrackingI3D = NULL;
PFNWGLQUERYGENLOCKMAXSOURCEDELAYI3DPROC /*glad_*/wglQueryGenlockMaxSourceDelayI3D = NULL;
PFNWGLQUERYMAXSWAPGROUPSNVPROC /*glad_*/wglQueryMaxSwapGroupsNV = NULL;
PFNWGLQUERYPBUFFERARBPROC /*glad_*/wglQueryPbufferARB = NULL;
PFNWGLQUERYPBUFFEREXTPROC /*glad_*/wglQueryPbufferEXT = NULL;
PFNWGLQUERYSWAPGROUPNVPROC /*glad_*/wglQuerySwapGroupNV = NULL;
PFNWGLQUERYVIDEOCAPTUREDEVICENVPROC /*glad_*/wglQueryVideoCaptureDeviceNV = NULL;
PFNWGLRELEASEIMAGEBUFFEREVENTSI3DPROC /*glad_*/wglReleaseImageBufferEventsI3D = NULL;
PFNWGLRELEASEPBUFFERDCARBPROC /*glad_*/wglReleasePbufferDCARB = NULL;
PFNWGLRELEASEPBUFFERDCEXTPROC /*glad_*/wglReleasePbufferDCEXT = NULL;
PFNWGLRELEASETEXIMAGEARBPROC /*glad_*/wglReleaseTexImageARB = NULL;
PFNWGLRELEASEVIDEOCAPTUREDEVICENVPROC /*glad_*/wglReleaseVideoCaptureDeviceNV = NULL;
PFNWGLRELEASEVIDEODEVICENVPROC /*glad_*/wglReleaseVideoDeviceNV = NULL;
PFNWGLRELEASEVIDEOIMAGENVPROC /*glad_*/wglReleaseVideoImageNV = NULL;
PFNWGLRESETFRAMECOUNTNVPROC /*glad_*/wglResetFrameCountNV = NULL;
PFNWGLRESTOREBUFFERREGIONARBPROC /*glad_*/wglRestoreBufferRegionARB = NULL;
PFNWGLSAVEBUFFERREGIONARBPROC /*glad_*/wglSaveBufferRegionARB = NULL;
PFNWGLSENDPBUFFERTOVIDEONVPROC /*glad_*/wglSendPbufferToVideoNV = NULL;
PFNWGLSETDIGITALVIDEOPARAMETERSI3DPROC /*glad_*/wglSetDigitalVideoParametersI3D = NULL;
PFNWGLSETGAMMATABLEI3DPROC /*glad_*/wglSetGammaTableI3D = NULL;
PFNWGLSETGAMMATABLEPARAMETERSI3DPROC /*glad_*/wglSetGammaTableParametersI3D = NULL;
PFNWGLSETPBUFFERATTRIBARBPROC /*glad_*/wglSetPbufferAttribARB = NULL;
PFNWGLSETSTEREOEMITTERSTATE3DLPROC /*glad_*/wglSetStereoEmitterState3DL = NULL;
PFNWGLSWAPBUFFERSMSCOMLPROC /*glad_*/wglSwapBuffersMscOML = NULL;
PFNWGLSWAPINTERVALEXTPROC /*glad_*/wglSwapIntervalEXT = NULL;
PFNWGLSWAPLAYERBUFFERSMSCOMLPROC /*glad_*/wglSwapLayerBuffersMscOML = NULL;
PFNWGLWAITFORMSCOMLPROC /*glad_*/wglWaitForMscOML = NULL;
PFNWGLWAITFORSBCOMLPROC /*glad_*/wglWaitForSbcOML = NULL;


static void /*glad_*/wgl_load_WGL_3DL_stereo_control(GLADuserptrloadfunc load, void *userptr) {
    if(!/*GLAD*/_WGL_3DL_stereo_control) return;
    /*glad_*/wglSetStereoEmitterState3DL = (PFNWGLSETSTEREOEMITTERSTATE3DLPROC) load(userptr, "wglSetStereoEmitterState3DL");
}
static void /*glad_*/wgl_load_WGL_AMD_gpu_association(GLADuserptrloadfunc load, void *userptr) {
    if(!/*GLAD*/_WGL_AMD_gpu_association) return;
    /*glad_*/wglBlitContextFramebufferAMD = (PFNWGLBLITCONTEXTFRAMEBUFFERAMDPROC) load(userptr, "wglBlitContextFramebufferAMD");
    /*glad_*/wglCreateAssociatedContextAMD = (PFNWGLCREATEASSOCIATEDCONTEXTAMDPROC) load(userptr, "wglCreateAssociatedContextAMD");
    /*glad_*/wglCreateAssociatedContextAttribsAMD = (PFNWGLCREATEASSOCIATEDCONTEXTATTRIBSAMDPROC) load(userptr, "wglCreateAssociatedContextAttribsAMD");
    /*glad_*/wglDeleteAssociatedContextAMD = (PFNWGLDELETEASSOCIATEDCONTEXTAMDPROC) load(userptr, "wglDeleteAssociatedContextAMD");
    /*glad_*/wglGetContextGPUIDAMD = (PFNWGLGETCONTEXTGPUIDAMDPROC) load(userptr, "wglGetContextGPUIDAMD");
    /*glad_*/wglGetCurrentAssociatedContextAMD = (PFNWGLGETCURRENTASSOCIATEDCONTEXTAMDPROC) load(userptr, "wglGetCurrentAssociatedContextAMD");
    /*glad_*/wglGetGPUIDsAMD = (PFNWGLGETGPUIDSAMDPROC) load(userptr, "wglGetGPUIDsAMD");
    /*glad_*/wglGetGPUInfoAMD = (PFNWGLGETGPUINFOAMDPROC) load(userptr, "wglGetGPUInfoAMD");
    /*glad_*/wglMakeAssociatedContextCurrentAMD = (PFNWGLMAKEASSOCIATEDCONTEXTCURRENTAMDPROC) load(userptr, "wglMakeAssociatedContextCurrentAMD");
}
static void /*glad_*/wgl_load_WGL_ARB_buffer_region(GLADuserptrloadfunc load, void *userptr) {
    if(!/*GLAD*/_WGL_ARB_buffer_region) return;
    /*glad_*/wglCreateBufferRegionARB = (PFNWGLCREATEBUFFERREGIONARBPROC) load(userptr, "wglCreateBufferRegionARB");
    /*glad_*/wglDeleteBufferRegionARB = (PFNWGLDELETEBUFFERREGIONARBPROC) load(userptr, "wglDeleteBufferRegionARB");
    /*glad_*/wglRestoreBufferRegionARB = (PFNWGLRESTOREBUFFERREGIONARBPROC) load(userptr, "wglRestoreBufferRegionARB");
    /*glad_*/wglSaveBufferRegionARB = (PFNWGLSAVEBUFFERREGIONARBPROC) load(userptr, "wglSaveBufferRegionARB");
}
static void /*glad_*/wgl_load_WGL_ARB_create_context(GLADuserptrloadfunc load, void *userptr) {
    if(!/*GLAD*/_WGL_ARB_create_context) return;
    /*glad_*/wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC) load(userptr, "wglCreateContextAttribsARB");
}
static void /*glad_*/wgl_load_WGL_ARB_extensions_string(GLADuserptrloadfunc load, void *userptr) {
    if(!/*GLAD*/_WGL_ARB_extensions_string) return;
    /*glad_*/wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC) load(userptr, "wglGetExtensionsStringARB");
}
static void /*glad_*/wgl_load_WGL_ARB_make_current_read(GLADuserptrloadfunc load, void *userptr) {
    if(!/*GLAD*/_WGL_ARB_make_current_read) return;
    /*glad_*/wglGetCurrentReadDCARB = (PFNWGLGETCURRENTREADDCARBPROC) load(userptr, "wglGetCurrentReadDCARB");
    /*glad_*/wglMakeContextCurrentARB = (PFNWGLMAKECONTEXTCURRENTARBPROC) load(userptr, "wglMakeContextCurrentARB");
}
static void /*glad_*/wgl_load_WGL_ARB_pbuffer(GLADuserptrloadfunc load, void *userptr) {
    if(!/*GLAD*/_WGL_ARB_pbuffer) return;
    /*glad_*/wglCreatePbufferARB = (PFNWGLCREATEPBUFFERARBPROC) load(userptr, "wglCreatePbufferARB");
    /*glad_*/wglDestroyPbufferARB = (PFNWGLDESTROYPBUFFERARBPROC) load(userptr, "wglDestroyPbufferARB");
    /*glad_*/wglGetPbufferDCARB = (PFNWGLGETPBUFFERDCARBPROC) load(userptr, "wglGetPbufferDCARB");
    /*glad_*/wglQueryPbufferARB = (PFNWGLQUERYPBUFFERARBPROC) load(userptr, "wglQueryPbufferARB");
    /*glad_*/wglReleasePbufferDCARB = (PFNWGLRELEASEPBUFFERDCARBPROC) load(userptr, "wglReleasePbufferDCARB");
}
static void /*glad_*/wgl_load_WGL_ARB_pixel_format(GLADuserptrloadfunc load, void *userptr) {
    if(!/*GLAD*/_WGL_ARB_pixel_format) return;
    /*glad_*/wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC) load(userptr, "wglChoosePixelFormatARB");
    /*glad_*/wglGetPixelFormatAttribfvARB = (PFNWGLGETPIXELFORMATATTRIBFVARBPROC) load(userptr, "wglGetPixelFormatAttribfvARB");
    /*glad_*/wglGetPixelFormatAttribivARB = (PFNWGLGETPIXELFORMATATTRIBIVARBPROC) load(userptr, "wglGetPixelFormatAttribivARB");
}
static void /*glad_*/wgl_load_WGL_ARB_render_texture(GLADuserptrloadfunc load, void *userptr) {
    if(!/*GLAD*/_WGL_ARB_render_texture) return;
    /*glad_*/wglBindTexImageARB = (PFNWGLBINDTEXIMAGEARBPROC) load(userptr, "wglBindTexImageARB");
    /*glad_*/wglReleaseTexImageARB = (PFNWGLRELEASETEXIMAGEARBPROC) load(userptr, "wglReleaseTexImageARB");
    /*glad_*/wglSetPbufferAttribARB = (PFNWGLSETPBUFFERATTRIBARBPROC) load(userptr, "wglSetPbufferAttribARB");
}
static void /*glad_*/wgl_load_WGL_EXT_display_color_table(GLADuserptrloadfunc load, void *userptr) {
    if(!/*GLAD*/_WGL_EXT_display_color_table) return;
    /*glad_*/wglBindDisplayColorTableEXT = (PFNWGLBINDDISPLAYCOLORTABLEEXTPROC) load(userptr, "wglBindDisplayColorTableEXT");
    /*glad_*/wglCreateDisplayColorTableEXT = (PFNWGLCREATEDISPLAYCOLORTABLEEXTPROC) load(userptr, "wglCreateDisplayColorTableEXT");
    /*glad_*/wglDestroyDisplayColorTableEXT = (PFNWGLDESTROYDISPLAYCOLORTABLEEXTPROC) load(userptr, "wglDestroyDisplayColorTableEXT");
    /*glad_*/wglLoadDisplayColorTableEXT = (PFNWGLLOADDISPLAYCOLORTABLEEXTPROC) load(userptr, "wglLoadDisplayColorTableEXT");
}
static void /*glad_*/wgl_load_WGL_EXT_extensions_string(GLADuserptrloadfunc load, void *userptr) {
    if(!/*GLAD*/_WGL_EXT_extensions_string) return;
    /*glad_*/wglGetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC) load(userptr, "wglGetExtensionsStringEXT");
}
static void /*glad_*/wgl_load_WGL_EXT_make_current_read(GLADuserptrloadfunc load, void *userptr) {
    if(!/*GLAD*/_WGL_EXT_make_current_read) return;
    /*glad_*/wglGetCurrentReadDCEXT = (PFNWGLGETCURRENTREADDCEXTPROC) load(userptr, "wglGetCurrentReadDCEXT");
    /*glad_*/wglMakeContextCurrentEXT = (PFNWGLMAKECONTEXTCURRENTEXTPROC) load(userptr, "wglMakeContextCurrentEXT");
}
static void /*glad_*/wgl_load_WGL_EXT_pbuffer(GLADuserptrloadfunc load, void *userptr) {
    if(!/*GLAD*/_WGL_EXT_pbuffer) return;
    /*glad_*/wglCreatePbufferEXT = (PFNWGLCREATEPBUFFEREXTPROC) load(userptr, "wglCreatePbufferEXT");
    /*glad_*/wglDestroyPbufferEXT = (PFNWGLDESTROYPBUFFEREXTPROC) load(userptr, "wglDestroyPbufferEXT");
    /*glad_*/wglGetPbufferDCEXT = (PFNWGLGETPBUFFERDCEXTPROC) load(userptr, "wglGetPbufferDCEXT");
    /*glad_*/wglQueryPbufferEXT = (PFNWGLQUERYPBUFFEREXTPROC) load(userptr, "wglQueryPbufferEXT");
    /*glad_*/wglReleasePbufferDCEXT = (PFNWGLRELEASEPBUFFERDCEXTPROC) load(userptr, "wglReleasePbufferDCEXT");
}
static void /*glad_*/wgl_load_WGL_EXT_pixel_format(GLADuserptrloadfunc load, void *userptr) {
    if(!/*GLAD*/_WGL_EXT_pixel_format) return;
    /*glad_*/wglChoosePixelFormatEXT = (PFNWGLCHOOSEPIXELFORMATEXTPROC) load(userptr, "wglChoosePixelFormatEXT");
    /*glad_*/wglGetPixelFormatAttribfvEXT = (PFNWGLGETPIXELFORMATATTRIBFVEXTPROC) load(userptr, "wglGetPixelFormatAttribfvEXT");
    /*glad_*/wglGetPixelFormatAttribivEXT = (PFNWGLGETPIXELFORMATATTRIBIVEXTPROC) load(userptr, "wglGetPixelFormatAttribivEXT");
}
static void /*glad_*/wgl_load_WGL_EXT_swap_control(GLADuserptrloadfunc load, void *userptr) {
    if(!/*GLAD*/_WGL_EXT_swap_control) return;
    /*glad_*/wglGetSwapIntervalEXT = (PFNWGLGETSWAPINTERVALEXTPROC) load(userptr, "wglGetSwapIntervalEXT");
    /*glad_*/wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC) load(userptr, "wglSwapIntervalEXT");
}
static void /*glad_*/wgl_load_WGL_I3D_digital_video_control(GLADuserptrloadfunc load, void *userptr) {
    if(!/*GLAD*/_WGL_I3D_digital_video_control) return;
    /*glad_*/wglGetDigitalVideoParametersI3D = (PFNWGLGETDIGITALVIDEOPARAMETERSI3DPROC) load(userptr, "wglGetDigitalVideoParametersI3D");
    /*glad_*/wglSetDigitalVideoParametersI3D = (PFNWGLSETDIGITALVIDEOPARAMETERSI3DPROC) load(userptr, "wglSetDigitalVideoParametersI3D");
}
static void /*glad_*/wgl_load_WGL_I3D_gamma(GLADuserptrloadfunc load, void *userptr) {
    if(!/*GLAD*/_WGL_I3D_gamma) return;
    /*glad_*/wglGetGammaTableI3D = (PFNWGLGETGAMMATABLEI3DPROC) load(userptr, "wglGetGammaTableI3D");
    /*glad_*/wglGetGammaTableParametersI3D = (PFNWGLGETGAMMATABLEPARAMETERSI3DPROC) load(userptr, "wglGetGammaTableParametersI3D");
    /*glad_*/wglSetGammaTableI3D = (PFNWGLSETGAMMATABLEI3DPROC) load(userptr, "wglSetGammaTableI3D");
    /*glad_*/wglSetGammaTableParametersI3D = (PFNWGLSETGAMMATABLEPARAMETERSI3DPROC) load(userptr, "wglSetGammaTableParametersI3D");
}
static void /*glad_*/wgl_load_WGL_I3D_genlock(GLADuserptrloadfunc load, void *userptr) {
    if(!/*GLAD*/_WGL_I3D_genlock) return;
    /*glad_*/wglDisableGenlockI3D = (PFNWGLDISABLEGENLOCKI3DPROC) load(userptr, "wglDisableGenlockI3D");
    /*glad_*/wglEnableGenlockI3D = (PFNWGLENABLEGENLOCKI3DPROC) load(userptr, "wglEnableGenlockI3D");
    /*glad_*/wglGenlockSampleRateI3D = (PFNWGLGENLOCKSAMPLERATEI3DPROC) load(userptr, "wglGenlockSampleRateI3D");
    /*glad_*/wglGenlockSourceDelayI3D = (PFNWGLGENLOCKSOURCEDELAYI3DPROC) load(userptr, "wglGenlockSourceDelayI3D");
    /*glad_*/wglGenlockSourceEdgeI3D = (PFNWGLGENLOCKSOURCEEDGEI3DPROC) load(userptr, "wglGenlockSourceEdgeI3D");
    /*glad_*/wglGenlockSourceI3D = (PFNWGLGENLOCKSOURCEI3DPROC) load(userptr, "wglGenlockSourceI3D");
    /*glad_*/wglGetGenlockSampleRateI3D = (PFNWGLGETGENLOCKSAMPLERATEI3DPROC) load(userptr, "wglGetGenlockSampleRateI3D");
    /*glad_*/wglGetGenlockSourceDelayI3D = (PFNWGLGETGENLOCKSOURCEDELAYI3DPROC) load(userptr, "wglGetGenlockSourceDelayI3D");
    /*glad_*/wglGetGenlockSourceEdgeI3D = (PFNWGLGETGENLOCKSOURCEEDGEI3DPROC) load(userptr, "wglGetGenlockSourceEdgeI3D");
    /*glad_*/wglGetGenlockSourceI3D = (PFNWGLGETGENLOCKSOURCEI3DPROC) load(userptr, "wglGetGenlockSourceI3D");
    /*glad_*/wglIsEnabledGenlockI3D = (PFNWGLISENABLEDGENLOCKI3DPROC) load(userptr, "wglIsEnabledGenlockI3D");
    /*glad_*/wglQueryGenlockMaxSourceDelayI3D = (PFNWGLQUERYGENLOCKMAXSOURCEDELAYI3DPROC) load(userptr, "wglQueryGenlockMaxSourceDelayI3D");
}
static void /*glad_*/wgl_load_WGL_I3D_image_buffer(GLADuserptrloadfunc load, void *userptr) {
    if(!/*GLAD*/_WGL_I3D_image_buffer) return;
    /*glad_*/wglAssociateImageBufferEventsI3D = (PFNWGLASSOCIATEIMAGEBUFFEREVENTSI3DPROC) load(userptr, "wglAssociateImageBufferEventsI3D");
    /*glad_*/wglCreateImageBufferI3D = (PFNWGLCREATEIMAGEBUFFERI3DPROC) load(userptr, "wglCreateImageBufferI3D");
    /*glad_*/wglDestroyImageBufferI3D = (PFNWGLDESTROYIMAGEBUFFERI3DPROC) load(userptr, "wglDestroyImageBufferI3D");
    /*glad_*/wglReleaseImageBufferEventsI3D = (PFNWGLRELEASEIMAGEBUFFEREVENTSI3DPROC) load(userptr, "wglReleaseImageBufferEventsI3D");
}
static void /*glad_*/wgl_load_WGL_I3D_swap_frame_lock(GLADuserptrloadfunc load, void *userptr) {
    if(!/*GLAD*/_WGL_I3D_swap_frame_lock) return;
    /*glad_*/wglDisableFrameLockI3D = (PFNWGLDISABLEFRAMELOCKI3DPROC) load(userptr, "wglDisableFrameLockI3D");
    /*glad_*/wglEnableFrameLockI3D = (PFNWGLENABLEFRAMELOCKI3DPROC) load(userptr, "wglEnableFrameLockI3D");
    /*glad_*/wglIsEnabledFrameLockI3D = (PFNWGLISENABLEDFRAMELOCKI3DPROC) load(userptr, "wglIsEnabledFrameLockI3D");
    /*glad_*/wglQueryFrameLockMasterI3D = (PFNWGLQUERYFRAMELOCKMASTERI3DPROC) load(userptr, "wglQueryFrameLockMasterI3D");
}
static void /*glad_*/wgl_load_WGL_I3D_swap_frame_usage(GLADuserptrloadfunc load, void *userptr) {
    if(!/*GLAD*/_WGL_I3D_swap_frame_usage) return;
    /*glad_*/wglBeginFrameTrackingI3D = (PFNWGLBEGINFRAMETRACKINGI3DPROC) load(userptr, "wglBeginFrameTrackingI3D");
    /*glad_*/wglEndFrameTrackingI3D = (PFNWGLENDFRAMETRACKINGI3DPROC) load(userptr, "wglEndFrameTrackingI3D");
    /*glad_*/wglGetFrameUsageI3D = (PFNWGLGETFRAMEUSAGEI3DPROC) load(userptr, "wglGetFrameUsageI3D");
    /*glad_*/wglQueryFrameTrackingI3D = (PFNWGLQUERYFRAMETRACKINGI3DPROC) load(userptr, "wglQueryFrameTrackingI3D");
}
static void /*glad_*/wgl_load_WGL_NV_DX_interop(GLADuserptrloadfunc load, void *userptr) {
    if(!/*GLAD*/_WGL_NV_DX_interop) return;
    /*glad_*/wglDXCloseDeviceNV = (PFNWGLDXCLOSEDEVICENVPROC) load(userptr, "wglDXCloseDeviceNV");
    /*glad_*/wglDXLockObjectsNV = (PFNWGLDXLOCKOBJECTSNVPROC) load(userptr, "wglDXLockObjectsNV");
    /*glad_*/wglDXObjectAccessNV = (PFNWGLDXOBJECTACCESSNVPROC) load(userptr, "wglDXObjectAccessNV");
    /*glad_*/wglDXOpenDeviceNV = (PFNWGLDXOPENDEVICENVPROC) load(userptr, "wglDXOpenDeviceNV");
    /*glad_*/wglDXRegisterObjectNV = (PFNWGLDXREGISTEROBJECTNVPROC) load(userptr, "wglDXRegisterObjectNV");
    /*glad_*/wglDXSetResourceShareHandleNV = (PFNWGLDXSETRESOURCESHAREHANDLENVPROC) load(userptr, "wglDXSetResourceShareHandleNV");
    /*glad_*/wglDXUnlockObjectsNV = (PFNWGLDXUNLOCKOBJECTSNVPROC) load(userptr, "wglDXUnlockObjectsNV");
    /*glad_*/wglDXUnregisterObjectNV = (PFNWGLDXUNREGISTEROBJECTNVPROC) load(userptr, "wglDXUnregisterObjectNV");
}
static void /*glad_*/wgl_load_WGL_NV_copy_image(GLADuserptrloadfunc load, void *userptr) {
    if(!/*GLAD*/_WGL_NV_copy_image) return;
    /*glad_*/wglCopyImageSubDataNV = (PFNWGLCOPYIMAGESUBDATANVPROC) load(userptr, "wglCopyImageSubDataNV");
}
static void /*glad_*/wgl_load_WGL_NV_delay_before_swap(GLADuserptrloadfunc load, void *userptr) {
    if(!/*GLAD*/_WGL_NV_delay_before_swap) return;
    /*glad_*/wglDelayBeforeSwapNV = (PFNWGLDELAYBEFORESWAPNVPROC) load(userptr, "wglDelayBeforeSwapNV");
}
static void /*glad_*/wgl_load_WGL_NV_gpu_affinity(GLADuserptrloadfunc load, void *userptr) {
    if(!/*GLAD*/_WGL_NV_gpu_affinity) return;
    /*glad_*/wglCreateAffinityDCNV = (PFNWGLCREATEAFFINITYDCNVPROC) load(userptr, "wglCreateAffinityDCNV");
    /*glad_*/wglDeleteDCNV = (PFNWGLDELETEDCNVPROC) load(userptr, "wglDeleteDCNV");
    /*glad_*/wglEnumGpuDevicesNV = (PFNWGLENUMGPUDEVICESNVPROC) load(userptr, "wglEnumGpuDevicesNV");
    /*glad_*/wglEnumGpusFromAffinityDCNV = (PFNWGLENUMGPUSFROMAFFINITYDCNVPROC) load(userptr, "wglEnumGpusFromAffinityDCNV");
    /*glad_*/wglEnumGpusNV = (PFNWGLENUMGPUSNVPROC) load(userptr, "wglEnumGpusNV");
}
static void /*glad_*/wgl_load_WGL_NV_present_video(GLADuserptrloadfunc load, void *userptr) {
    if(!/*GLAD*/_WGL_NV_present_video) return;
    /*glad_*/wglBindVideoDeviceNV = (PFNWGLBINDVIDEODEVICENVPROC) load(userptr, "wglBindVideoDeviceNV");
    /*glad_*/wglEnumerateVideoDevicesNV = (PFNWGLENUMERATEVIDEODEVICESNVPROC) load(userptr, "wglEnumerateVideoDevicesNV");
    /*glad_*/wglQueryCurrentContextNV = (PFNWGLQUERYCURRENTCONTEXTNVPROC) load(userptr, "wglQueryCurrentContextNV");
}
static void /*glad_*/wgl_load_WGL_NV_swap_group(GLADuserptrloadfunc load, void *userptr) {
    if(!/*GLAD*/_WGL_NV_swap_group) return;
    /*glad_*/wglBindSwapBarrierNV = (PFNWGLBINDSWAPBARRIERNVPROC) load(userptr, "wglBindSwapBarrierNV");
    /*glad_*/wglJoinSwapGroupNV = (PFNWGLJOINSWAPGROUPNVPROC) load(userptr, "wglJoinSwapGroupNV");
    /*glad_*/wglQueryFrameCountNV = (PFNWGLQUERYFRAMECOUNTNVPROC) load(userptr, "wglQueryFrameCountNV");
    /*glad_*/wglQueryMaxSwapGroupsNV = (PFNWGLQUERYMAXSWAPGROUPSNVPROC) load(userptr, "wglQueryMaxSwapGroupsNV");
    /*glad_*/wglQuerySwapGroupNV = (PFNWGLQUERYSWAPGROUPNVPROC) load(userptr, "wglQuerySwapGroupNV");
    /*glad_*/wglResetFrameCountNV = (PFNWGLRESETFRAMECOUNTNVPROC) load(userptr, "wglResetFrameCountNV");
}
static void /*glad_*/wgl_load_WGL_NV_vertex_array_range(GLADuserptrloadfunc load, void *userptr) {
    if(!/*GLAD*/_WGL_NV_vertex_array_range) return;
    /*glad_*/wglAllocateMemoryNV = (PFNWGLALLOCATEMEMORYNVPROC) load(userptr, "wglAllocateMemoryNV");
    /*glad_*/wglFreeMemoryNV = (PFNWGLFREEMEMORYNVPROC) load(userptr, "wglFreeMemoryNV");
}
static void /*glad_*/wgl_load_WGL_NV_video_capture(GLADuserptrloadfunc load, void *userptr) {
    if(!/*GLAD*/_WGL_NV_video_capture) return;
    /*glad_*/wglBindVideoCaptureDeviceNV = (PFNWGLBINDVIDEOCAPTUREDEVICENVPROC) load(userptr, "wglBindVideoCaptureDeviceNV");
    /*glad_*/wglEnumerateVideoCaptureDevicesNV = (PFNWGLENUMERATEVIDEOCAPTUREDEVICESNVPROC) load(userptr, "wglEnumerateVideoCaptureDevicesNV");
    /*glad_*/wglLockVideoCaptureDeviceNV = (PFNWGLLOCKVIDEOCAPTUREDEVICENVPROC) load(userptr, "wglLockVideoCaptureDeviceNV");
    /*glad_*/wglQueryVideoCaptureDeviceNV = (PFNWGLQUERYVIDEOCAPTUREDEVICENVPROC) load(userptr, "wglQueryVideoCaptureDeviceNV");
    /*glad_*/wglReleaseVideoCaptureDeviceNV = (PFNWGLRELEASEVIDEOCAPTUREDEVICENVPROC) load(userptr, "wglReleaseVideoCaptureDeviceNV");
}
static void /*glad_*/wgl_load_WGL_NV_video_output(GLADuserptrloadfunc load, void *userptr) {
    if(!/*GLAD*/_WGL_NV_video_output) return;
    /*glad_*/wglBindVideoImageNV = (PFNWGLBINDVIDEOIMAGENVPROC) load(userptr, "wglBindVideoImageNV");
    /*glad_*/wglGetVideoDeviceNV = (PFNWGLGETVIDEODEVICENVPROC) load(userptr, "wglGetVideoDeviceNV");
    /*glad_*/wglGetVideoInfoNV = (PFNWGLGETVIDEOINFONVPROC) load(userptr, "wglGetVideoInfoNV");
    /*glad_*/wglReleaseVideoDeviceNV = (PFNWGLRELEASEVIDEODEVICENVPROC) load(userptr, "wglReleaseVideoDeviceNV");
    /*glad_*/wglReleaseVideoImageNV = (PFNWGLRELEASEVIDEOIMAGENVPROC) load(userptr, "wglReleaseVideoImageNV");
    /*glad_*/wglSendPbufferToVideoNV = (PFNWGLSENDPBUFFERTOVIDEONVPROC) load(userptr, "wglSendPbufferToVideoNV");
}
static void /*glad_*/wgl_load_WGL_OML_sync_control(GLADuserptrloadfunc load, void *userptr) {
    if(!/*GLAD*/_WGL_OML_sync_control) return;
    /*glad_*/wglGetMscRateOML = (PFNWGLGETMSCRATEOMLPROC) load(userptr, "wglGetMscRateOML");
    /*glad_*/wglGetSyncValuesOML = (PFNWGLGETSYNCVALUESOMLPROC) load(userptr, "wglGetSyncValuesOML");
    /*glad_*/wglSwapBuffersMscOML = (PFNWGLSWAPBUFFERSMSCOMLPROC) load(userptr, "wglSwapBuffersMscOML");
    /*glad_*/wglSwapLayerBuffersMscOML = (PFNWGLSWAPLAYERBUFFERSMSCOMLPROC) load(userptr, "wglSwapLayerBuffersMscOML");
    /*glad_*/wglWaitForMscOML = (PFNWGLWAITFORMSCOMLPROC) load(userptr, "wglWaitForMscOML");
    /*glad_*/wglWaitForSbcOML = (PFNWGLWAITFORSBCOMLPROC) load(userptr, "wglWaitForSbcOML");
}


static void /*glad_*/wgl_resolve_aliases(void) {
}

static int /*glad_*/wgl_has_extension(HDC hdc, const char *ext) {
    const char *terminator;
    const char *loc;
    const char *extensions;

    if(wglGetExtensionsStringEXT == NULL && wglGetExtensionsStringARB == NULL)
        return 0;

    if(wglGetExtensionsStringARB == NULL || hdc == INVALID_HANDLE_VALUE)
        extensions = wglGetExtensionsStringEXT();
    else
        extensions = wglGetExtensionsStringARB(hdc);

    if(extensions == NULL || ext == NULL)
        return 0;

    while(1) {
        loc = strstr(extensions, ext);
        if(loc == NULL)
            break;

        terminator = loc + strlen(ext);
        if((loc == extensions || *(loc - 1) == ' ') &&
            (*terminator == ' ' || *terminator == '\0'))
        {
            return 1;
        }
        extensions = terminator;
    }

    return 0;
}

static GLADapiproc /*glad_*/wgl_get_proc_from_userptr(void *userptr, const char* name) {
    return (GLAD_GNUC_EXTENSION (GLADapiproc (*)(const char *name)) userptr)(name);
}

static int /*glad_*/wgl_find_extensions_wgl(HDC hdc) {
    /*GLAD*/_WGL_3DFX_multisample = /*glad_*/wgl_has_extension(hdc, "WGL_3DFX_multisample");
    /*GLAD*/_WGL_3DL_stereo_control = /*glad_*/wgl_has_extension(hdc, "WGL_3DL_stereo_control");
    /*GLAD*/_WGL_AMD_gpu_association = /*glad_*/wgl_has_extension(hdc, "WGL_AMD_gpu_association");
    /*GLAD*/_WGL_ARB_buffer_region = /*glad_*/wgl_has_extension(hdc, "WGL_ARB_buffer_region");
    /*GLAD*/_WGL_ARB_context_flush_control = /*glad_*/wgl_has_extension(hdc, "WGL_ARB_context_flush_control");
    /*GLAD*/_WGL_ARB_create_context = /*glad_*/wgl_has_extension(hdc, "WGL_ARB_create_context");
    /*GLAD*/_WGL_ARB_create_context_no_error = /*glad_*/wgl_has_extension(hdc, "WGL_ARB_create_context_no_error");
    /*GLAD*/_WGL_ARB_create_context_profile = /*glad_*/wgl_has_extension(hdc, "WGL_ARB_create_context_profile");
    /*GLAD*/_WGL_ARB_create_context_robustness = /*glad_*/wgl_has_extension(hdc, "WGL_ARB_create_context_robustness");
    /*GLAD*/_WGL_ARB_extensions_string = /*glad_*/wgl_has_extension(hdc, "WGL_ARB_extensions_string");
    /*GLAD*/_WGL_ARB_framebuffer_sRGB = /*glad_*/wgl_has_extension(hdc, "WGL_ARB_framebuffer_sRGB");
    /*GLAD*/_WGL_ARB_make_current_read = /*glad_*/wgl_has_extension(hdc, "WGL_ARB_make_current_read");
    /*GLAD*/_WGL_ARB_multisample = /*glad_*/wgl_has_extension(hdc, "WGL_ARB_multisample");
    /*GLAD*/_WGL_ARB_pbuffer = /*glad_*/wgl_has_extension(hdc, "WGL_ARB_pbuffer");
    /*GLAD*/_WGL_ARB_pixel_format = /*glad_*/wgl_has_extension(hdc, "WGL_ARB_pixel_format");
    /*GLAD*/_WGL_ARB_pixel_format_float = /*glad_*/wgl_has_extension(hdc, "WGL_ARB_pixel_format_float");
    /*GLAD*/_WGL_ARB_render_texture = /*glad_*/wgl_has_extension(hdc, "WGL_ARB_render_texture");
    /*GLAD*/_WGL_ARB_robustness_application_isolation = /*glad_*/wgl_has_extension(hdc, "WGL_ARB_robustness_application_isolation");
    /*GLAD*/_WGL_ARB_robustness_share_group_isolation = /*glad_*/wgl_has_extension(hdc, "WGL_ARB_robustness_share_group_isolation");
    /*GLAD*/_WGL_ATI_pixel_format_float = /*glad_*/wgl_has_extension(hdc, "WGL_ATI_pixel_format_float");
    /*GLAD*/_WGL_ATI_render_texture_rectangle = /*glad_*/wgl_has_extension(hdc, "WGL_ATI_render_texture_rectangle");
    /*GLAD*/_WGL_EXT_colorspace = /*glad_*/wgl_has_extension(hdc, "WGL_EXT_colorspace");
    /*GLAD*/_WGL_EXT_create_context_es2_profile = /*glad_*/wgl_has_extension(hdc, "WGL_EXT_create_context_es2_profile");
    /*GLAD*/_WGL_EXT_create_context_es_profile = /*glad_*/wgl_has_extension(hdc, "WGL_EXT_create_context_es_profile");
    /*GLAD*/_WGL_EXT_depth_float = /*glad_*/wgl_has_extension(hdc, "WGL_EXT_depth_float");
    /*GLAD*/_WGL_EXT_display_color_table = /*glad_*/wgl_has_extension(hdc, "WGL_EXT_display_color_table");
    /*GLAD*/_WGL_EXT_extensions_string = /*glad_*/wgl_has_extension(hdc, "WGL_EXT_extensions_string");
    /*GLAD*/_WGL_EXT_framebuffer_sRGB = /*glad_*/wgl_has_extension(hdc, "WGL_EXT_framebuffer_sRGB");
    /*GLAD*/_WGL_EXT_make_current_read = /*glad_*/wgl_has_extension(hdc, "WGL_EXT_make_current_read");
    /*GLAD*/_WGL_EXT_multisample = /*glad_*/wgl_has_extension(hdc, "WGL_EXT_multisample");
    /*GLAD*/_WGL_EXT_pbuffer = /*glad_*/wgl_has_extension(hdc, "WGL_EXT_pbuffer");
    /*GLAD*/_WGL_EXT_pixel_format = /*glad_*/wgl_has_extension(hdc, "WGL_EXT_pixel_format");
    /*GLAD*/_WGL_EXT_pixel_format_packed_float = /*glad_*/wgl_has_extension(hdc, "WGL_EXT_pixel_format_packed_float");
    /*GLAD*/_WGL_EXT_swap_control = /*glad_*/wgl_has_extension(hdc, "WGL_EXT_swap_control");
    /*GLAD*/_WGL_EXT_swap_control_tear = /*glad_*/wgl_has_extension(hdc, "WGL_EXT_swap_control_tear");
    /*GLAD*/_WGL_I3D_digital_video_control = /*glad_*/wgl_has_extension(hdc, "WGL_I3D_digital_video_control");
    /*GLAD*/_WGL_I3D_gamma = /*glad_*/wgl_has_extension(hdc, "WGL_I3D_gamma");
    /*GLAD*/_WGL_I3D_genlock = /*glad_*/wgl_has_extension(hdc, "WGL_I3D_genlock");
    /*GLAD*/_WGL_I3D_image_buffer = /*glad_*/wgl_has_extension(hdc, "WGL_I3D_image_buffer");
    /*GLAD*/_WGL_I3D_swap_frame_lock = /*glad_*/wgl_has_extension(hdc, "WGL_I3D_swap_frame_lock");
    /*GLAD*/_WGL_I3D_swap_frame_usage = /*glad_*/wgl_has_extension(hdc, "WGL_I3D_swap_frame_usage");
    /*GLAD*/_WGL_NV_DX_interop = /*glad_*/wgl_has_extension(hdc, "WGL_NV_DX_interop");
    /*GLAD*/_WGL_NV_DX_interop2 = /*glad_*/wgl_has_extension(hdc, "WGL_NV_DX_interop2");
    /*GLAD*/_WGL_NV_copy_image = /*glad_*/wgl_has_extension(hdc, "WGL_NV_copy_image");
    /*GLAD*/_WGL_NV_delay_before_swap = /*glad_*/wgl_has_extension(hdc, "WGL_NV_delay_before_swap");
    /*GLAD*/_WGL_NV_float_buffer = /*glad_*/wgl_has_extension(hdc, "WGL_NV_float_buffer");
    /*GLAD*/_WGL_NV_gpu_affinity = /*glad_*/wgl_has_extension(hdc, "WGL_NV_gpu_affinity");
    /*GLAD*/_WGL_NV_multigpu_context = /*glad_*/wgl_has_extension(hdc, "WGL_NV_multigpu_context");
    /*GLAD*/_WGL_NV_multisample_coverage = /*glad_*/wgl_has_extension(hdc, "WGL_NV_multisample_coverage");
    /*GLAD*/_WGL_NV_present_video = /*glad_*/wgl_has_extension(hdc, "WGL_NV_present_video");
    /*GLAD*/_WGL_NV_render_depth_texture = /*glad_*/wgl_has_extension(hdc, "WGL_NV_render_depth_texture");
    /*GLAD*/_WGL_NV_render_texture_rectangle = /*glad_*/wgl_has_extension(hdc, "WGL_NV_render_texture_rectangle");
    /*GLAD*/_WGL_NV_swap_group = /*glad_*/wgl_has_extension(hdc, "WGL_NV_swap_group");
    /*GLAD*/_WGL_NV_vertex_array_range = /*glad_*/wgl_has_extension(hdc, "WGL_NV_vertex_array_range");
    /*GLAD*/_WGL_NV_video_capture = /*glad_*/wgl_has_extension(hdc, "WGL_NV_video_capture");
    /*GLAD*/_WGL_NV_video_output = /*glad_*/wgl_has_extension(hdc, "WGL_NV_video_output");
    /*GLAD*/_WGL_OML_sync_control = /*glad_*/wgl_has_extension(hdc, "WGL_OML_sync_control");
    return 1;
}

static int /*glad_*/wgl_find_core_wgl(void) {
    int major = 1, minor = 0;
    _WGL_VERSION_1_0 = (major == 1 && minor >= 0) || major > 1;
    return GLAD_MAKE_VERSION(major, minor);
}

int gladLoadWGLUserPtr(HDC hdc, GLADuserptrloadfunc load, void *userptr) {
    int version;
    wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC) load(userptr, "wglGetExtensionsStringARB");
    wglGetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC) load(userptr, "wglGetExtensionsStringEXT");
    if(wglGetExtensionsStringARB == NULL && wglGetExtensionsStringEXT == NULL) return 0;
    version = /*glad_*/wgl_find_core_wgl();


    if (!/*glad_*/wgl_find_extensions_wgl(hdc)) return 0;
    /*glad_*/wgl_load_WGL_3DL_stereo_control(load, userptr);
    /*glad_*/wgl_load_WGL_AMD_gpu_association(load, userptr);
    /*glad_*/wgl_load_WGL_ARB_buffer_region(load, userptr);
    /*glad_*/wgl_load_WGL_ARB_create_context(load, userptr);
    /*glad_*/wgl_load_WGL_ARB_extensions_string(load, userptr);
    /*glad_*/wgl_load_WGL_ARB_make_current_read(load, userptr);
    /*glad_*/wgl_load_WGL_ARB_pbuffer(load, userptr);
    /*glad_*/wgl_load_WGL_ARB_pixel_format(load, userptr);
    /*glad_*/wgl_load_WGL_ARB_render_texture(load, userptr);
    /*glad_*/wgl_load_WGL_EXT_display_color_table(load, userptr);
    /*glad_*/wgl_load_WGL_EXT_extensions_string(load, userptr);
    /*glad_*/wgl_load_WGL_EXT_make_current_read(load, userptr);
    /*glad_*/wgl_load_WGL_EXT_pbuffer(load, userptr);
    /*glad_*/wgl_load_WGL_EXT_pixel_format(load, userptr);
    /*glad_*/wgl_load_WGL_EXT_swap_control(load, userptr);
    /*glad_*/wgl_load_WGL_I3D_digital_video_control(load, userptr);
    /*glad_*/wgl_load_WGL_I3D_gamma(load, userptr);
    /*glad_*/wgl_load_WGL_I3D_genlock(load, userptr);
    /*glad_*/wgl_load_WGL_I3D_image_buffer(load, userptr);
    /*glad_*/wgl_load_WGL_I3D_swap_frame_lock(load, userptr);
    /*glad_*/wgl_load_WGL_I3D_swap_frame_usage(load, userptr);
    /*glad_*/wgl_load_WGL_NV_DX_interop(load, userptr);
    /*glad_*/wgl_load_WGL_NV_copy_image(load, userptr);
    /*glad_*/wgl_load_WGL_NV_delay_before_swap(load, userptr);
    /*glad_*/wgl_load_WGL_NV_gpu_affinity(load, userptr);
    /*glad_*/wgl_load_WGL_NV_present_video(load, userptr);
    /*glad_*/wgl_load_WGL_NV_swap_group(load, userptr);
    /*glad_*/wgl_load_WGL_NV_vertex_array_range(load, userptr);
    /*glad_*/wgl_load_WGL_NV_video_capture(load, userptr);
    /*glad_*/wgl_load_WGL_NV_video_output(load, userptr);
    /*glad_*/wgl_load_WGL_OML_sync_control(load, userptr);

    /*glad_*/wgl_resolve_aliases();

    return version;
}

int gladLoadWGL(HDC hdc, GLADloadfunc load) {
    return gladLoadWGLUserPtr(hdc, /*glad_*/wgl_get_proc_from_userptr, GLAD_GNUC_EXTENSION (void*) load);
}

#include "../../src/zglInterop_WGL.h"

int(WINAPI*wglChoosePixelFormatWIN)(HDC hdc, CONST PIXELFORMATDESCRIPTOR *ppfd);  // "wglChoosePixelFormat" funciona com Intel mas não com AMD.
int(WINAPI*wglDescribePixelFormatWIN)(HDC hdc, int iPixelFormat, UINT nBytes, LPPIXELFORMATDESCRIPTOR ppfd);
BOOL(WINAPI*wglSetPixelFormatWIN)(HDC hdc, int format, CONST PIXELFORMATDESCRIPTOR * ppfd); // "wglSetPixelFormat" funciona com Intel mas não com AMD.
int(WINAPI*wglGetPixelFormatWIN)(HDC hdc);

BOOL(WINAPI*wglCopyContextWIN)(HGLRC, HGLRC, UINT);
HGLRC(WINAPI*wglCreateContextWIN)(HDC);
HGLRC(WINAPI*wglCreateLayerContextWIN)(HDC, int);
BOOL(WINAPI*wglDeleteContextWIN)(HGLRC);
HGLRC(WINAPI*wglGetCurrentContextWIN)(VOID);
HDC(WINAPI*wglGetCurrentDCWIN)(VOID);
PROC(WINAPI*wglGetProcAddressWIN)(LPCSTR);
BOOL(WINAPI*wglMakeCurrentWIN)(HDC, HGLRC);
BOOL(WINAPI*wglSwapBuffersWIN)(HDC);// wglSwapBuffers;
BOOL(WINAPI*wglSwapLayerBuffersWIN)(HDC, UINT);

#ifdef GLAD_WGL

static GLADapiproc /*glad_*/wgl_get_proc(void *vuserptr, const char* name) {
    GLAD_UNUSED(vuserptr);
    return GLAD_GNUC_EXTENSION (GLADapiproc) wglGetProcAddressWIN(name);
}

int gladLoaderLoadWGL(HDC hdc) {
    return gladLoadWGLUserPtr(hdc, /*glad_*/wgl_get_proc, NULL);
}


#endif /* GLAD_WGL */

_ZGL HMODULE hOpengl32Dll = NIL;

_ZGL afxError wglLoadModule(HMODULE opengl32, afxUnit* verMajor, afxUnit* verMinor, afxUnit* verPatch)
{
    afxError err = { 0 };
    static afxBool loaded = FALSE;
    static afxUnit loadedVerMajor = 0;
    static afxUnit loadedVerMinor = 0;
    static afxUnit loadedVerPatch = 0;

    if (!loaded)
    {
        hOpengl32Dll = opengl32;

        wglGetProcAddressWIN = (void*)GetProcAddress(opengl32, "wglGetProcAddress");

        wglCreateContextWIN = (void*)GetProcAddress(opengl32, "wglCreateContext");
        wglDeleteContextWIN = (void*)GetProcAddress(opengl32, "wglDeleteContext");
        wglGetCurrentContextWIN = (void*)GetProcAddress(opengl32, "wglGetCurrentContext");
        wglGetCurrentDCWIN = (void*)GetProcAddress(opengl32, "wglGetCurrentDC");
        wglMakeCurrentWIN = (void*)GetProcAddress(opengl32, "wglMakeCurrent");
        wglChoosePixelFormatWIN = (void*)GetProcAddress(opengl32, "wglChoosePixelFormat");
        wglSetPixelFormatWIN = (void*)GetProcAddress(opengl32, "wglSetPixelFormat");
        wglDescribePixelFormatWIN = (void*)GetProcAddress(opengl32, "wglDescribePixelFormat");
        wglGetPixelFormatWIN = (void*)GetProcAddress(opengl32, "wglGetPixelFormat");
        wglSwapBuffersWIN = (void*)GetProcAddress(opengl32, "wglSwapBuffers");
        wglSwapLayerBuffersWIN = (void*)GetProcAddress(opengl32, "wglSwapLayerBuffers");

        DWORD pfdFlags = PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;// | PFD_DIRECT3D_ACCELERATED | PFD_SWAP_EXCHANGE;
        DWORD dwExStyle = WS_EX_APPWINDOW;
        DWORD wndStyles = WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

        HDC tmpHdc = NIL;
        HWND tmpHwnd = NIL;
        HGLRC tmpHrc = NIL;

        if (!(tmpHwnd = CreateWindowExA(dwExStyle, vgiWndClss.lpszClassName, "", wndStyles, 0, 0, 1, 1, NIL, NIL, vgiWndClss.hInstance, NIL)))
        {
            AfxThrowError();
            return err;
        }

        if (!(tmpHdc = GetDC(tmpHwnd)))
        {
            AfxThrowError();
            DestroyWindow(tmpHwnd), tmpHwnd = NIL;
            return err;
        }

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

        {
            HDC hScreenDc = GetDC(NULL);
            pxlfmt = GetPixelFormat(hScreenDc);

            if (pxlfmt)
                DescribePixelFormat(hScreenDc, pxlfmt, sizeof(pfd), &pfd);

            ReleaseDC(NULL, hScreenDc);
        }

        if (!(pxlfmt = ChoosePixelFormat(tmpHdc, &pfd))) AfxThrowError();
        else if (!(DescribePixelFormat(tmpHdc, pxlfmt, sizeof(pfd), &pfd))) AfxThrowError();
        else if (!SetPixelFormat(tmpHdc, pxlfmt, &pfd))
            AfxThrowError();

        if (err)
        {
            ReleaseDC(tmpHwnd, tmpHdc), tmpHdc = NIL;
            DestroyWindow(tmpHwnd), tmpHwnd = NIL;
            return err;
        }

        if (!(tmpHrc = wglCreateContextWIN(tmpHdc)))
        {
            AfxThrowError();
            ReleaseDC(tmpHwnd, tmpHdc), tmpHdc = NIL;
            DestroyWindow(tmpHwnd), tmpHwnd = NIL;
            return err;
        }

        if (!(wglMakeCurrentWIN(tmpHdc, tmpHrc)))
        {
            AfxThrowError();
            wglDeleteContextWIN(tmpHrc), tmpHrc = NIL;
            ReleaseDC(tmpHwnd, tmpHdc), tmpHdc = NIL;
            DestroyWindow(tmpHwnd), tmpHwnd = NIL;
            return err;
        }

        afxString ver;
        afxUnit vMajor, vMinor, vPatch;
        PFNGLGETSTRINGPROC _glGetString = (void*)wglGetProcAddressWIN("glGetString");
        AFX_ASSERT(_glGetString);
        AfxMakeString(&ver, 0, (afxChar const*)_glGetString(GL_VERSION), 0);
        AfxScanString(&ver, "%u.%u.%u", &vMajor, &vMinor, &vPatch);

        loadedVerMajor = vMajor;
        loadedVerMinor = vMinor;
        loadedVerPatch = vPatch;

        gladLoaderLoadWGL(tmpHdc);

        wglDeleteContextWIN(tmpHrc), tmpHrc = NIL;
        ReleaseDC(tmpHwnd, tmpHdc), tmpHdc = NIL;
        DestroyWindow(tmpHwnd), tmpHwnd = NIL;

        if (!err)
            loaded = TRUE;
    }

    if (verMajor)
        *verMajor = loadedVerMajor;
    if (verMinor)
        *verMinor = loadedVerMinor;
    if (verPatch)
        *verPatch = loadedVerPatch;

    return err;
}

#ifdef __cplusplus
}
#endif
