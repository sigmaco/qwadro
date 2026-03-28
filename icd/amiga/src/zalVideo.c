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

#include <d3d9.h>

#define _AFX_DRAW_SYSTEM_C
#define _AFX_DRAW_SYSTEM_IMPL
#include "../qwadro/icd/tarzgl4/src/zglCommands.h"

#include "zalInteropWasapi.h"
#include "zalObjects.h"

_ZAL afxError _ZalVidDtorCb(amxVideo vid)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_VID, 1, &vid);

    afxMixSystem msys = AfxGetHost(vid);

    if (_AMX_VID_CLASS_CONFIG.dtor(vid))
    {
        AfxThrowError();
    }

    return err;
}

_ZAL afxError _ZalVidCtorCb(amxVideo vid, void** args, afxUnit invokeNo)
{
    afxResult err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_VID, 1, &vid);

    afxMixSystem msys = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    amxVideoConfig const* cfg = ((amxVideoConfig const *)args[1]) + invokeNo;

    if (_AMX_VID_CLASS_CONFIG.ctor(vid, (void*[]) { msys, (void*)cfg }, 0))
    {
        AfxThrowError();
        return err;
    }
    

    // https://github.com/chyxwzn/fplay/blob/master/ffplay_dxva2.c

#if 0
    afxBool isDecoder = FALSE;

    if (isDecoder)
    {
        GUID *guid_list = NULL;
        unsigned guid_count = 0, i, j;

        HRESULT hr = IDirectXVideoDecoderService_GetDecoderDeviceGuids(msys->decoder_service, &guid_count, &guid_list);
        if (FAILED(hr))
        {
            // "Failed to retrieve decoder device GUIDs\n"
        }

        for (i = 0; dxva2_modes[i].guid; i++)
        {
            D3DFORMAT *target_list = NULL;
            unsigned target_count = 0;
            const dxva2_mode *mode = &dxva2_modes[i];
            if (mode->codec != s->codec_id)
                continue;

            for (j = 0; j < guid_count; j++) {
                if (IsEqualGUID(mode->guid, &guid_list[j]))
                    break;
            }
            if (j == guid_count)
                continue;

            hr = IDirectXVideoDecoderService_GetDecoderRenderTargets(msys->decoder_service, mode->guid, &target_count, &target_list);
            if (FAILED(hr))
            {
                continue;
            }

            for (j = 0; j < target_count; j++)
            {
                const D3DFORMAT format = target_list[j];
                if (format == MKTAG('N', 'V', '1', '2'))
                {
                    target_format = format;
                    break;
                }
            }

            CoTaskMemFree(target_list);

            if (target_format)
            {
                device_guid = *mode->guid;
                break;
            }
        }

        CoTaskMemFree(guid_list);

        if (IsEqualGUID(&device_guid, &GUID_NULL))
        {
            // "No decoder device for codec found\n"
        }

        desc.SampleWidth = s->coded_width;
        desc.SampleHeight = s->coded_height;
        desc.Format = target_format;

        ret = dxva2_get_decoder_configuration(s, &device_guid, &desc, &config);

        if (ret < 0)
        {
            goto fail;
        }

        /* decoding MPEG-2 requires additional alignment on some Intel GPUs,
           but it causes issues for H.264 on certain AMD GPUs..... */
        if (s->codec_id == AV_CODEC_ID_MPEG2VIDEO)
            surface_alignment = 32;
        /* the HEVC DXVA2 spec asks for 128 pixel aligned surfaces to ensure
           all coding features have enough room to work with */
        else if (s->codec_id == AV_CODEC_ID_HEVC)
            surface_alignment = 128;
        else
            surface_alignment = 16;

        /* 4 base work surfaces */
        ctx->num_surfaces = 4;

        /* add surfaces based on number of possible refs */
        if (s->codec_id == AV_CODEC_ID_H264 || s->codec_id == AV_CODEC_ID_HEVC)
            ctx->num_surfaces += 16;
        else
            ctx->num_surfaces += 2;

        /* add extra surfaces for frame threading */
        if (s->active_thread_type & FF_THREAD_FRAME)
            ctx->num_surfaces += s->thread_count;

        ctx->surfaces = av_mallocz(ctx->num_surfaces * sizeof(*ctx->surfaces));
        ctx->surface_infos = av_mallocz(ctx->num_surfaces * sizeof(*ctx->surface_infos));

        if (!ctx->surfaces || !ctx->surface_infos)
        {
            //"Unable to allocate surface arrays\n");
            goto fail;
        }

        hr = IDirectXVideoDecoderService_CreateSurface(ctx->decoder_service,
            FFALIGN(s->coded_width, surface_alignment), FFALIGN(s->coded_height, surface_alignment), ctx->num_surfaces - 1,
            target_format, D3DPOOL_DEFAULT, 0, DXVA2_VideoDecoderRenderTarget, ctx->surfaces, NULL);
        if (FAILED(hr))
        {
            // "Failed to create %d video surfaces\n", ctx->num_surfaces);
            goto fail;
        }

        hr = IDirectXVideoDecoderService_CreateVideoDecoder(msys->decoder_service, &device_guid, &desc, &config, ctx->surfaces, ctx->num_surfaces, &ctx->decoder);
        if (FAILED(hr))
        {
            // "Failed to create DXVA2 video decoder\n");
            goto fail;
        }

        ctx->decoder_guid = device_guid;
        ctx->decoder_config = config;

        dxva_ctx->cfg = &ctx->decoder_config;
        dxva_ctx->decoder = ctx->decoder;
        dxva_ctx->surface = ctx->surfaces;
        dxva_ctx->surface_count = ctx->num_surfaces;

        if (IsEqualGUID(&ctx->decoder_guid, &DXVADDI_Intel_ModeH264_E))
            dxva_ctx->workaround |= FF_DXVA2_WORKAROUND_INTEL_CLEARVIDEO;

    }
#endif

    return err;
}
