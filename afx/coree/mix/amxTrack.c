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
 *                               (c) 2017 SIGMA FEDERATION
 *                               ESTADO-MAJOR DA SECURIDAD
 *                                SIGMA TECHNOLOGY GROUP
 *                                       ENGITECH
 */

// This software is part of Advanced Multimedia Extensions.

#define _AMX_MIX_C
#define _AMX_BUFFER_C
#define _AMX_TRACK_C
#include "amxIcd.h"

AFX_DEFINE_STRUCT(amxSoundBank)
{
    afxUri      uri;
    amxBuffer   buf;
    // All samples must use an only format to avoid pipeline/state changing.
    amxFormat   fmt;
    afxUnit     freq;
    afxUnit     chCnt;

    // In Amiga AHI, an audio nests a group of audio tracks sharing same amount of channels and frequency.

    struct
    {
        afxSize     offset;
        afxUnit     size;
        afxReal     vol;
        afxReal     pan;
    } tracks[];
};

 // WAV header structure
#pragma pack(push, 1)
typedef struct _WAVHeader
{
    afxChar chunkFcc[4]; // "RIFF"
    afxUnit32 chunkSiz; // 36 + SubChunk2Size
    afxChar audeFcc[4]; // "WAVE"
    afxChar fmtFcc[4]; // "fmt "
    afxUnit32 subchunk1Siz; // 16 for PCM
    afxUnit16 fmt; // PCM = 1
    afxUnit16 chanCnt; // Mono or Stereo
    afxUnit32 freq; // 44100, 48000, etc.
    afxUnit32 byteRate; // (freq * chanCnt * bitsPerSample) / 8
    afxUnit16 blockAlign; // (chanCnt * bitsPerSample) / 8
    afxUnit16 bitsPerSample; // 8, 16, 24, or 32
    afxChar data[4];         // "data"
    afxUnit32 dataSiz; // (num_samples * chanCnt * bitsPerSample) / 8
} _WAVHeader;
#pragma pack(pop)

_AMXINL afxMixSystem AmxGetTrackHost(amxTrack trak)
{
    afxError err;
    AFX_ASSERT_OBJECTS(afxFcc_AUD, 1, &trak);
    afxMixSystem msys = AfxGetHost(trak);
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    return msys;
}

_AMXINL void AmxDescribeTrack(amxTrack trak, amxTrackInfo* desc)
{
    afxError err;
    AFX_ASSERT_OBJECTS(afxFcc_AUD, 1, &trak);
    AFX_ASSERT(desc);
    desc->fmt = trak->fmt;
    desc->chanCnt = trak->chanCnt;
    desc->freq = trak->freq;
    desc->frameCnt = trak->frameCnt;
    //desc->frameCnt = trak->frameCnt;
    desc->udd = trak->udd;
}

_AMXINL amxBuffer AmxGetTrackBuffer(amxTrack trak)
{
    afxError err;
    AFX_ASSERT_OBJECTS(afxFcc_AUD, 1, &trak);
    return trak->buf;
}

_AMXINL afxReal AmxGetTrackDuration(amxTrack trak)
{
    afxError err;
    AFX_ASSERT_OBJECTS(afxFcc_AUD, 1, &trak);
    return trak->frameCnt / trak->freq;
}

_AMX void _AmxSanitizeTrackPeriod(amxTrack trak, amxTrackPeriod const* raw, amxTrackPeriod* san)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_AUD, 1, &trak);
    AFX_ASSERT(raw);
    AFX_ASSERT(san);

    /*
        This code sanitizes a amxTrackPeriod struct to be within the bounds of the audio objects.
    */

    san->baseChan = AFX_MIN(raw->baseChan, trak->chanCnt - 1);
    san->baseFrame = AFX_MIN(raw->baseFrame, trak->frameCnt - 1);

    san->chanCnt = AFX_CLAMP(raw->chanCnt, 1, trak->chanCnt - san->baseChan);
    san->frameCnt = AFX_CLAMP(raw->frameCnt, 1, trak->frameCnt - san->baseFrame);
}

_AMX void _AmxSanitizeTrackCopy(amxTrack src, amxTrack dst, amxTrackCopy const* raw, amxTrackCopy* san)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_AUD, 1, &src);
    AFX_ASSERT_OBJECTS(afxFcc_AUD, 1, &dst);
    AFX_ASSERT(raw);
    AFX_ASSERT(san);

    /*
        This code sanitizes a amxTrackCopy struct to be within the bounds of the audio objects.
    */

    san->dstBaseChan = AFX_MIN(raw->dstBaseChan, src->chanCnt - 1);
    san->dstBaseFrame = AFX_MIN(raw->dstBaseFrame, src->frameCnt - 1);
    san->src.baseChan = AFX_MIN(raw->src.baseChan, dst->chanCnt - 1);
    san->src.baseFrame = AFX_MIN(raw->src.baseFrame, dst->frameCnt - 1);

    san->src.chanCnt = AFX_CLAMP(raw->src.chanCnt, 1, AFX_MIN(src->chanCnt - san->dstBaseChan, dst->chanCnt - san->src.baseChan));
    san->src.frameCnt = AFX_CLAMP(raw->src.frameCnt, 1, AFX_MIN(src->frameCnt - san->dstBaseFrame, dst->frameCnt - san->src.baseFrame));
}

////////////////////////////////////////////////////////////////////////////////

_AMX afxError AmxUpdateTrack(amxTrack trak, afxUnit opCnt, amxTrackIo const ops[], void const* src, amxFence signal)
{
    afxResult err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_AUD, 1, &trak);

#if _AFX_DEBUG
    for (afxUnit i = 0; i < opCnt; i++)
    {
        amxTrackIo const* op = &ops[i];
        //AFX_ASSERT_RANGE(ras->mipCnt, op->rgn.lodIdx, 1);
        //AFX_ASSERT_RANGE_WHD(ras->whd, op->rgn.origin, op->rgn.whd);
    }
#endif

    amxTransference transfer = { 0 };
    transfer.dstFcc = afxFcc_AUD;
    transfer.dst.aud = trak;
    transfer.src.src = src;

    afxMixSystem msys = AmxGetTrackHost(trak);
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);

    if (_AmxMsysGetDdi(msys)->transferCb(msys, &transfer, opCnt, ops))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT(transfer.baseQueIdx != AFX_INVALID_INDEX);

    return err;
}

_AMX afxError AmxDumpTrack(amxTrack trak, afxUnit opCnt, amxTrackIo const ops[], void* dst, amxFence signal)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_AUD, 1, &trak);
    AFX_ASSERT(((trak->bufFlags & avxBufferFlag_ACCESS) & avxBufferFlag_R));
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ops);
    AFX_ASSERT(dst);

#if _AFX_DEBUG
    for (afxUnit i = 0; i < opCnt; i++)
    {
        amxTrackIo const* op = &ops[i];
    }
#endif

    amxTransference transfer = { 0 };
    transfer.srcFcc = afxFcc_AUD;
    transfer.src.aud = trak;
    transfer.dst.dst = dst;

    afxMixSystem msys = AmxGetTrackHost(trak);
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);

    if (_AmxMsysGetDdi(msys)->transferCb(msys, &transfer, opCnt, ops))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT(transfer.baseQueIdx != AFX_INVALID_INDEX);

    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AMX afxError AmxUploadTrack(amxTrack trak, afxUnit opCnt, amxTrackIo const ops[], afxStream in, amxFence signal)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_AUD, 1, &trak);

#if _AFX_DEBUG
    for (afxUnit i = 0; i < opCnt; i++)
    {
        amxTrackIo const* op = &ops[i];
    }
#endif

    amxTransference transfer = { 0 };
    transfer.dstFcc = afxFcc_AUD;
    transfer.srcFcc = afxFcc_IOB;
    transfer.dst.aud = trak;
    transfer.src.iob = in;

    afxMixSystem msys = AmxGetTrackHost(trak);
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);

    if (_AmxMsysGetDdi(msys)->transferCb(msys, &transfer, opCnt, ops))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT(transfer.baseQueIdx != AFX_INVALID_INDEX);

    return err;
}

_AMX afxError AmxDownloadTrack(amxTrack trak, afxUnit opCnt, amxTrackIo const ops[], afxStream out, amxFence signal)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_AUD, 1, &trak);

#if _AFX_DEBUG
    for (afxUnit i = 0; i < opCnt; i++)
    {
        amxTrackIo const* op = &ops[i];
    }
#endif

    amxTransference transfer = { 0 };
    transfer.srcFcc = afxFcc_AUD;
    transfer.dstFcc = afxFcc_IOB;
    transfer.src.aud = trak;
    transfer.dst.iob = out;

    afxMixSystem msys = AmxGetTrackHost(trak);
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);

    if (_AmxMsysGetDdi(msys)->transferCb(msys, &transfer, opCnt, ops))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT(transfer.baseQueIdx != AFX_INVALID_INDEX);

    return err;
}

_AMX afxError AmxPrintTrack(amxTrack trak, amxTrackPeriod const* op, afxUri const* uri)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_AUD, 1, &trak);

    if (!uri || AfxIsUriBlank(uri))
    {
        AfxThrowError();
        return err;
    }

    afxStream file;
    if (AfxOpenFile(uri, afxFileFlag_W, &file))
    {
        AfxThrowError();
        return err;
    }

    amxTrackIo op2 = { 0 };

    if (op) _AmxSanitizeTrackPeriod(trak, op, &op2.period);
    else
    {
        op2.period.frameCnt = trak->frameCnt;
        op2.period.chanCnt = trak->chanCnt;
    }

    // Prepare the WAV header
    _WAVHeader hdr;
    hdr.chunkFcc[0] = 'R';
    hdr.chunkFcc[1] = 'I';
    hdr.chunkFcc[2] = 'F';
    hdr.chunkFcc[3] = 'F';
    hdr.audeFcc[0] = 'W';
    hdr.audeFcc[1] = 'A';
    hdr.audeFcc[2] = 'V';
    hdr.audeFcc[3] = 'E';
    hdr.fmtFcc[0] = 'f';
    hdr.fmtFcc[1] = 'm';
    hdr.fmtFcc[2] = 't';
    hdr.fmtFcc[3] = ' ';
    hdr.fmt = 1;  // PCM
    hdr.chanCnt = op2.period.chanCnt;  // Mono
    hdr.freq = trak->freq;
    hdr.data[0] = 'd';
    hdr.data[1] = 'a';
    hdr.data[2] = 't';
    hdr.data[3] = 'a';

    // Number of samples in the audio
    afxUnit numSamples = trak->frameCnt;

    switch (trak->fmt)
    {
    case amxFormat_S32f:
    {
        hdr.fmt = 3; // 3 for IEEE float
        hdr.bitsPerSample = 32;  // 32-bit floating point
        hdr.byteRate = hdr.freq * hdr.chanCnt * 4; // 4 bytes per sample (32-bit float)
        hdr.blockAlign = hdr.chanCnt * 4; // 4 bytes per sample
        hdr.dataSiz = numSamples * hdr.chanCnt * 4;  // data size
        hdr.chunkSiz = 36 + hdr.dataSiz;  // total file size minus 8 bytes

        op2.framesPerChan = hdr.chanCnt;
        op2.chansPerSample = numSamples;
        break;
    }
    case amxFormat_M32f:
    {
        hdr.fmt = 3; // 3 for IEEE float
        hdr.bitsPerSample = 32;  // 32-bit floating point
        hdr.byteRate = hdr.freq * hdr.chanCnt * 4; // 4 bytes per sample (32-bit float)
        hdr.blockAlign = 4; // 4 bytes per sample
        hdr.dataSiz = numSamples * hdr.chanCnt * 4;  // data size
        hdr.chunkSiz = 36 + hdr.dataSiz;  // total file size minus 8 bytes

        op2.framesPerChan = numSamples;
        op2.chansPerSample = hdr.chanCnt;
        break;
    }
    case amxFormat_S16i:
    {
        hdr.fmt = 1;
        hdr.bitsPerSample = 16;  // 16-bit audio
        hdr.byteRate = hdr.freq * hdr.chanCnt * 2; // 2 bytes per sample (16-bit audio)
        hdr.blockAlign = hdr.chanCnt * 2; // 2 bytes per sample
        hdr.dataSiz = numSamples * hdr.chanCnt * 2;  // data size
        hdr.chunkSiz = 36 + hdr.dataSiz;  // total file size minus 8 bytes

        op2.framesPerChan = hdr.chanCnt;
        op2.chansPerSample = numSamples;
        break;
    }
    case amxFormat_M16i:
    {
        hdr.fmt = 1;
        hdr.bitsPerSample = 16;  // 16-bit audio
        hdr.byteRate = hdr.freq * hdr.chanCnt * 2; // 2 bytes per sample (16-bit audio)
        hdr.blockAlign = 2; // 2 bytes per sample
        hdr.dataSiz = numSamples * hdr.chanCnt * 2;  // data size
        hdr.chunkSiz = 36 + hdr.dataSiz;  // total file size minus 8 bytes

        op2.framesPerChan = numSamples;
        op2.chansPerSample = numSamples;
        break;
    }
    default: AfxThrowError(); break;
    }

    // Write the header to the file
    if (AfxWriteStream(file, sizeof(_WAVHeader), 1, &hdr))
    {
        AfxThrowError();
        AfxDisposeObjects(1, &file);
        return err;
    }

    op2.offset = AfxAskStreamPosn(file);

    if (AmxDownloadTrack(trak, 1, &op2, file, 0))
    {
        AfxThrowError();
        AfxDisposeObjects(1, &file);
        return err;
    }

    afxMixSystem msys = AmxGetTrackHost(trak);
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    if (AmxWaitForMixBridges(msys, 0, 0))
        AfxThrowError();

    AfxDisposeObjects(1, &file);

    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AMX afxError _AmxTrakDtorCb(amxTrack trak)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_AUD, 1, &trak);

    afxMixSystem msys = AmxGetTrackHost(trak);

    if (trak->buf)
        AfxDisposeObjects(1, &trak->buf);

    return err;
}

_AMX afxError _AmxTrakCtorCb(amxTrack trak, void** args, afxUnit invokeNo)
{
    afxResult err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_AUD, 1, &trak);

    afxMixSystem msys = AmxGetTrackHost(trak);
    amxTrackConfig const *spec = ((amxTrackConfig const *)args[1]) + invokeNo;
    AFX_ASSERT(spec);
    afxBool allocOnSysMem = *(afxBool*)(args[2]);

    if (!spec->fmt)
    {
        AfxThrowError();
        return err;
    }

    if (!spec->freq)
    {
        AfxThrowError();
        return err;
    }

    if (!spec->frameCnt)
    {
        AfxThrowError();
        return err;
    }

    afxUnit bps = 0;
    switch (spec->fmt)
    {
    case amxFormat_M32f: bps = 32; break;
    case amxFormat_S32f: bps = 32; break;
    case amxFormat_M32i: bps = 32; break;
    case amxFormat_S32i: bps = 32; break;
    case amxFormat_M24i: bps = 24; break;
    case amxFormat_S24i: bps = 24; break;
    case amxFormat_M16i: bps = 16; break;
    case amxFormat_S16i: bps = 16; break;
    case amxFormat_M8i: bps = 8; break;
    case amxFormat_S8i: bps = 8; break;
    default: AfxThrowError();  break;
    }
    trak->fmt = spec->fmt;
    trak->fmtBps = bps;
    trak->fmtStride = bps / AFX_BYTE_SIZE;

    afxUnit segCnt = AFX_MAX(1, spec->segCnt);
    afxUnit frameCnt = AFX_MAX(1, spec->frameCnt);
    afxUnit chanCnt = AFX_MAX(1, spec->chanCnt);
    afxUnit freq = AFX_MAX(1, spec->freq);
    
    trak->frameCnt = frameCnt;
    trak->chanCnt = chanCnt;
    trak->segCnt = segCnt;
    trak->freq = freq;

    afxUnit siz = AFX_ALIGN_SIZE((trak->fmtStride * frameCnt * chanCnt), AFX_SIMD_ALIGNMENT) * segCnt;
    trak->bufCap = AFX_ALIGN_SIZE(siz, AFX_SIMD_ALIGNMENT);
    
    trak->tag = spec->tag;
    trak->udd = spec->udd;

    amxBuffer buf;
    amxBufferInfo bufi = { 0 };
    bufi.fmt = trak->fmt;
    bufi.size = trak->bufCap;
    bufi.usage = amxBufferUsage_MIX;
    bufi.flags = amxBufferFlag_RW;
    AmxAcquireBuffers(msys, 1, &bufi, &buf);

    trak->buf = buf;

    return err;
}

_AMX afxClassConfig const _AMX_TRAK_CLASS_CONFIG =
{
    .fcc = afxFcc_AUD,
    .name = "Track",
    .desc = "Tracked/Formatted Media Buffer",
    .fixedSiz = sizeof(AFX_OBJECT(amxTrack)),
    .ctor = (void*)_AmxTrakCtorCb,
    .dtor = (void*)_AmxTrakDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AMX afxError AmxAcquireTracks(afxMixSystem msys, afxUnit cnt, amxTrackConfig const cfgs[], amxTrack tracks[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    AFX_ASSERT(tracks);
    AFX_ASSERT(cfgs);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)_AmxMsysGetAudClass(msys);
    AFX_ASSERT_CLASS(cls, afxFcc_AUD);

    afxBool allocOnSysMem = TRUE;

    if (AfxAcquireObjects(cls, cnt, (afxObject*)tracks, (void const*[]) { msys, (void*)cfgs, &allocOnSysMem }))
        AfxThrowError();

    return err;
}
#if 0
_AMX afxError AmxLoadTracks(afxMixSystem msys, afxUnit cnt, afxUri const uris[], amxTrack tracks[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    AFX_ASSERT(tracks);
    AFX_ASSERT(uris);
    AFX_ASSERT(cnt);
    
    /*
        In a .wav file, the interleaving of audio samples (whether the file is stereo with interleaved or 
        deinterleaved channels) is not explicitly stored as a flag in the header. However, you can infer whether 
        the audio is interleaved or deinterleaved based on how the data is structured, which can be derived from 
        the num_channels and block_align fields in the WAV header.
    */

    //  if (hdr.blockAlign == hdr.chanCnt * hdr.bitsPerSample / AFX_BYTE_SIZE)

    for (afxUnit fIdx = 0; fIdx < cnt; fIdx++)
    {
        afxStream iob;
        AfxOpenFile(&uris[fIdx], afxFileFlag_R, &iob);

        _WAVHeader hdr;
        AfxReadStream(iob, sizeof(hdr), 0, &hdr);
        amxFormat fmt = NIL;

        switch (hdr.fmt)
        {
        case 1:
        {
            fmt = amxFormat_S16i;
            break;
        }
        case 3:
        {
            fmt = amxFormat_S32f;
            break;
        }
        default: AfxThrowError(); break;
        }

        afxUnit totalSamplesPerChan = hdr.dataSiz / (hdr.chanCnt * hdr.bitsPerSample / 8);

        amxTrackInfo audi = { 0 };
        audi.fmt = fmt;
        audi.segCnt = 1;
        audi.frameCnt = totalSamplesPerChan; // our channel will contain a fraction of the frequency.
        audi.chanCnt = hdr.chanCnt; // our channels will be deinterlaved for that fraction.
        audi.freq = hdr.freq;

        if (AmxAcquireTracks(msys, 1, &audi, &tracks[fIdx]))
            AfxThrowError();

        void* data;
        AfxAllocate(AfxHere(), hdr.dataSiz, 0, (void**)&data);

        AfxReadStream(iob, hdr.dataSiz, 0, data);

        amxTrackIo iop = { 0 };
        iop.offset = 0;// AfxAskStreamPosn(iob);
        iop.framesPerChan = hdr.chanCnt; // interleaved
        iop.chansPerSample = audi.frameCnt;
        iop.period.segCnt = 1;
        iop.period.chanCnt = audi.chanCnt;
        iop.period.frameCnt = audi.frameCnt;
        //iop.fmt = fmt;
        //AmxUploadTrack(buffers[fIdx], 1, &iop, iob, 0);
        //_AmxUploadTrack(buffers[fIdx], &iop, iob);
        AmxWaitForMixBridges(msys, 0, 0);
        
        _AmxUpdateTrack(tracks[fIdx], &iop, data);
        AfxDeallocate(AfxHere(), (void**)&data);

        AmxPrintTrack(tracks[fIdx], &iop.period, AfxUri("../test.wav"));

        AfxDisposeObjects(1, &iob);
    }
    return err;
}
#endif