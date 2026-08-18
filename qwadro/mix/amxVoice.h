/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *            Q W A D R O   M U L T I M E D I A   I N F R A S T R U C T U R E
 *
 *                               (c) 2017 SIGMA FEDERATION
 *                               ESTADO-MAJOR DA SECURIDAD
 *                                SIGMA TECHNOLOGY GROUP
 *                                       ENGITECH
 */

// This software is part of Advanced Multimedia Extensions.

// Voice
// each of the notes or sounds able to be produced simultaneously by a musical instrument (especially an electronic one) or a computer.
// (in an electronic musical instrument) each of a number of preset or programmable tones.

#ifndef AMX_VOICE_H
#define AMX_VOICE_H

#include "qwadro/mix/afxMixDefs.h"

typedef enum amxVoiceCap
{
    amxVoiceCap_REVERB,
    amxVoiceCap_PHASE,
    amxVoiceCap_GAIN,
    amxVoiceCap_ANALOG,
    amxVoiceCap_FLANGE,
    amxVoiceCap_COMPRESS,

} amxVoiceCaps;

AFX_DEFINE_STRUCT(amxVoicingInfo)
// Represents an audio data buffer.
{
    // Flags that provide additional information about the audio buffer.
    afxFlags        flags;
    amxBufferedStream   src;
    // Sample rate of the fed data.
    afxUnit         sampleRate;
    // First sample in the buffer that should be played.
    afxUnit         playBegin;
    // Length of the region to be played, in samples. 
    // A value of zero means to play the entire buffer, and, in this case, @playBegin must be zero as well.
    afxUnit         playLen;
    // Number of times to loop through the loop region. This value can be between 0 and AFX_U32_MAX. 
    // If @iterCnt is zero no looping is performed and @iterBegin and @iterLen must be 0. To loop forever, 
    // set @iterCnt to AFX_U32_MAX.
    afxUnit         iterCnt;
    // First sample of the region to be looped. The value of @iterBegin must be less than @playBegin + @playLen. 
    // @iterBegin can be less than @playBegin. @iterBegin must be 0 if LoopCount is 0.
    afxUnit         iterBegin;
    // Length of the loop region, in samples. The value of @iterBegin+@iterLen must be greater than @playBegin 
    // and less than @playBegin + @playLen. @iterLen must be zero if @iterCnt is 0. 
    // If @iterCnt is not 0 then a loop length of zero indicates the entire sample should be looped.
    afxUnit         iterLen;
    // User-defined data.
    void*           udd;
    // Debugging string.
    afxString       tag;
};

// Adds a new audio buffer to the voice queue.

AMX afxError AmxFeedVoice(amxVoice vox, amxVoicingInfo const* info);

// Removes all pending audio buffers from the voice queue.

AMX afxError AmxPurgeVoice(amxVoice vox, afxFlags flags);

// Stops looping the voice when it reaches the end of the current loop region.

AMX afxError AmxBreakVoiceLoop(amxVoice vox, afxFlags flags);

// Starts consumption and processing of audio by the voice. 
// Delivers the result to any connected submix or mastering voices, or to the output device.

AMX afxError AmxPauseVoice(amxVoice vox, afxBool suspend, afxFlags flags);

// Reconfigures the voice to consume source data at a different sample rate than the rate specified when the voice was created.

AMX afxError AmxSetVoiceSampleRate(amxVoice vox, afxUnit sampRate);

// Sets the frequency adjustment ratio of the voice.

AMX afxError AmxSetVoiceFrequencyRatio(amxVoice vox, afxReal ratio);

// Returns the frequency adjustment ratio of the voice.

AMX afxError AmxGetVoiceFrequencyRatio(amxVoice vox, afxReal* ratio);

// Returns the voice's current state and cursor position data.

AMX afxError AmxQueryVoiceState(amxVoice vox, amxBuffer* buf, afxUnit* bufsQueued, afxUnit64* samplesPlayed);

////////////////////////////////////////////////////////////////////////////////

AMX afxError AmxAcquireVoices
(
    afxMixSystem, 
    amxVoiceCaps caps, 
    afxUnit cnt, 
    amxVoice voices[]
);

#endif//AMX_VOICE_H
