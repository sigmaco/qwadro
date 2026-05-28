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
 *                                 Public Test Build
 *                             (c) 2017 SIGMA FEDERATION
 *                      Open sourced under the Qwadro License.
 */

#include "qwadro/afxQwadro.h"

int main(int argc, char const* argv[])
{
    afxError err = { 0 };

    // Boot up the Qwadro (if necessary)

    afxSystemConfig sysc = { 0 };
    AfxConfigureSystem(&sysc, NIL);
    if (AfxBootstrapSystem(&sysc))
        AfxThrowError();

    // Set up the draw system

    afxModule dIcd;
    AvxGetIcd(0, &dIcd);

    afxDrawSystem dsys;
    avxSystemConfig dsyc = { 0 };
    dsyc.caps = avxService_GFX;
    dsyc.accel = afxAcceleration_DPU;
    dsyc.exuCnt = 1;
    AvxConfigureDrawSystem(dIcd, &dsyc);
    if (AvxEstablishDrawSystem(dIcd, &dsyc, &dsys))
        AfxThrowError();
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    // Set up the mix system

    afxModule mIcd;
    AmxGetIcd(0, &mIcd);

    afxMixSystem msys;
    amxSystemConfig msyc = { 0 };
    msyc.caps = amxService_SFX;
    msyc.accel = afxAcceleration_MPU;
    msyc.dsys = dsys; // integrate our draw system
    msyc.exuCnt = 1;
    AmxConfigureMixSystem(mIcd, &msyc);
    if (AmxEstablishMixSystem(mIcd, &msyc, &msys))
        AfxThrowError();
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);

    // Open an environment

    afxModule shIcd;
    AfxGetShell(0, &shIcd);

    afxEnvironment env;
    afxEnvironmentConfig ecfg = { 0 };
    ecfg.dsys = dsys; // integrate our draw system
    //ecfg.msys = msys; // integrate our mix system
    AfxConfigureEnvironment(shIcd, &ecfg);
    if (AfxEstablishEnvironment(shIcd, &ecfg, &env))
        AfxThrowError();
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    // Acquire a drawable surface

    afxWindow wnd;
    afxWindowConfig wcfg = { 0 };
    wcfg.title = AFX_STRING("Amiga Test 1");
    wcfg.dout.dsys = dsys;
    wcfg.dout.ccfg.binCnt = 2;
    wcfg.dout.ccfg.bins[0].fmt = avxFormat_BGRA8v;
    wcfg.dout.ccfg.bins[1].fmt = avxFormat_D32f;
    AfxConfigureWindow(env, &wcfg, NIL, AFX_V3D(0.5, 0.5, 1));
    if (AfxAcquireWindow(env, &wcfg, &wnd))
        AfxThrowError();
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);

    afxSurface dout;
    AfxGetWindowSurface(wnd, &dout);
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

    afxSink sink;
    afxSinkConfig sinkc = { 0 };
    AfxConfigureAudioSink(msys, &sinkc);
    AfxOpenAudioSink(msys, &sinkc, &sink);

    amxBuffer mbuf;
    amxBufferInfo mbufi = { 0 };
    mbufi.fmt = amxFormat_S16i;
    mbufi.size = 1095920;
    mbufi.usage = amxBufferUsage_MIX;
    mbufi.flags = amxBufferFlag_RWXC;
    AmxAcquireBuffers(msys, 1, &mbufi, &mbuf);
    AFX_ASSERT_OBJECTS(afxFcc_MBUF, 1, &mbuf);

    amxBufferedAudio sba = { 0 };
    sba.buf = mbuf;
    sba.range = mbufi.size;
    sba.w = 2;
    sba.h = mbufi.size / (2 * 2);
    sba.d = 1;
    sba.sampRate = 44100;
    sba.fmt = amxFormat_S16i;

    // Run

    afxClock startClock, lastClock;
    AfxGetClock(&startClock);
    lastClock = startClock;

    afxBool readyToRender = TRUE;

    const uint32_t frameSize = 2 /*ch*/ * sizeof(int16_t);

    uint32_t totalFrames = mbufi.size / frameSize;      // frames in your linear buffer
    uint32_t cursor = 0;

    amxContextConfig mcfg = { 0 };
    afxMixContext mixContexts[3];
    AmxAcquireMixContexts(msys, NIL, &mcfg, 3, mixContexts);
    afxUnit submId = 0;

    while (1)
    {
        AfxDoUx(0, AFX_TIMEOUT_INFINITE);

        if (!AfxSystemIsExecuting())
            break;

        afxClock currClock;
        AfxGetClock(&currClock);
        afxReal64 ct = AfxGetSecondsElapsed(&startClock, &currClock);
        afxReal64 dt = AfxGetSecondsElapsed(&lastClock, &currClock);
        lastClock = currClock;

        if (cursor < totalFrames)
        {
            // 1. Query how many frames the sink is ready to accept right now
#if 0
            uint32_t writable;
            sink_get_writable_frames(&writable);

            if (writable == 0)
            {
                sleep_us(1000);          // <-- do NOT busy-spin
                continue;
            }
#else
            uint32_t writable = 512;
#endif

            amxBufferedTrack room = { 0 };
            //if (AfxSucceded(AmxLockSinkBuffer(sink, AFX_TIMEOUT_IGNORED, NIL, writable, &room)))
            {
                writable = room.frameCnt;

                // 2. Amount we can write this iteration
                uint32_t framesToWrite = AFX_MIN(writable, totalFrames - cursor);

                // 3. Lock a region (reserve it)
#if 0
                void* ptr = NULL;
                sink_lock(&ptr, framesToWrite);
#endif
#if 0
                amxBufferIo op = { 0 };
                op.rowCnt = framesToWrite;
                op.srcOffset = cursor * frameSize;
                op.dstOffset = 0;
                op.dstStride = frameSize;
                op.srcStride = frameSize;
                AmxDumpBuffer(mbuf, 1, &op, (void*)room.offset, NIL);
#endif

                afxMixContext mix = mixContexts[submId % 3];

                if (AfxSucceded(AmxPrepareMixCommands(mix, FALSE, amxCmdFlag_ONCE)))
                {

                    amxBufferedAudio dba = { 0 };
                    dba.buf = room.buf;
                    dba.offset = room.offset;
                    dba.range = room.range;
                    dba.range = framesToWrite * 2 * 4;
                    dba.w = 2;
                    dba.h = framesToWrite;
                    dba.d = 1;
                    dba.sampRate = 48000;
                    dba.fmt = amxFormat_S32f;

                    amxAudioPeriod srci = { 0 };
                    srci.h = framesToWrite;
                    srci.d = 1;
                    srci.y = cursor;
                    srci.w = 2;
                    amxAudioPeriod dsti = { 0 };
                    dsti.h = framesToWrite;
                    dsti.d = 1;
                    dsti.w = 2;
                    AmxCmdResampleBufferedAudio(mix, &sba, &dba, &srci, &dsti);

                    // 1c * 1a * 1s --- [s][c][a]
                    // 

                    AmxCompileMixCommands(mix);

                    amxSubmission subm = { 0 };
                    subm.mctx = mix;
                    AmxExecuteMixCommands(msys, 1, &subm);
                    ++submId;

                    AmxWaitForMixSystem(msys, AFX_TIMEOUT_INFINITE);

                    // 4. Copy samples into the reserved region
#if 0
                    memcpy(ptr,
                        &interleavedBuffer[cursor * 2],
                        framesToWrite * frameSize);
#endif

                    // 5. Commit the frames
#if 0
                    sink_unlock(framesToWrite);
#endif
                }

                amxFlush flush = { 0 };
                flush.sink = sink;
                flush.sampleCnt = writable;
                if (AfxSucceded(AmxFlushSinks(msys, 1, &flush, NIL)))
                {

                }

                cursor += framesToWrite;

                if (cursor >= totalFrames)
                    cursor = 0;
            }
        }

        AfxSleep(1);
    }

    AfxDisposeObjects(1, &wnd);
    AfxDisposeObjects(1, &env);
    AfxDisposeObjects(1, &dsys);

    AfxAbolishSystem(0);
    AfxYield();

    return 0;
}
