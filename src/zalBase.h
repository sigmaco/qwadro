/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#ifndef ZAL_BASE_H
#define ZAL_BASE_H

#include "../../../dep_/miniaudio/miniaudio.h"

#define _AFX_CORE_C
#define _AFX_SYSTEM_C
#define _AFX_DEVICE_C
#define _AFX_CONTEXT_C
#define _AVX_FENCE_C
#define _AVX_FENCE_IMPL
#define _AFX_SEMAPHORE_C
#define _AFX_SEMAPHORE_IMPL

#define _AMX_MIX_C
#define _AMX_MIX_SYSTEM_C
#define _AMX_MIX_DEVICE_C
#define _AMX_MIX_BRIDGE_C
#define _AMX_MIX_BRIDGE_IMPL
#define _AMX_MIX_QUEUE_C
#define _AMX_MIX_QUEUE_IMPL
#define _AMX_MIX_DEVICE_IMPL
#define _AMX_MIX_SYSTEM_C
#define _AMX_MIX_SYSTEM_IMPL
#define _AMX_SINK_C
#define _AMX_SINK_IMPL
#define _AMX_SOUND_BUFFER_C
#define _AMX_SOUND_BUFFER_IMPL
#define _AMX_SOUND_IMPL
#define _AMX_AUDIO_C
#define _AMX_WAVEFORM_IMPL
#define _AMX_TRACK_C
#define _AMX_TRACK_IMPL
#define _AMX_MIX_CONTEXT_C
#define _AMX_MIX_CONTEXT_IMPL
#define _AMX_BROKER_C
#define _AMX_BROKER_IMPL
#define _AMX_AUDIENT_C
#define _AMX_AUDIENT_IMPL


#include "qwadro/inc/mix/afxMixSystem.h"
#include "qwadro/inc/exec/afxSystem.h"
#include "afx/src/mix/impl/amxImplementation.h"

#ifndef AMX_DRV_SRC
#   ifdef _DEBUG
#       define ZAL DLLIMPORT extern 
#       define ZALINL DLLIMPORT EMBED
#   else
#       define ZAL DLLIMPORT extern 
#       define ZALINL DLLIMPORT EMBED
#   endif
#else
#   ifdef _DEBUG
#       define _ZAL DLLEXPORT
#       define ZAL DLLEXPORT extern 
#       define _ZALINL DLLEXPORT INLINE
#       define ZALINL DLLEXPORT EMBED
#   else
#       define _ZAL DLLEXPORT
#       define ZAL DLLEXPORT extern 
#       define _ZALINL DLLEXPORT INLINE
#       define ZALINL DLLEXPORT EMBED
#   endif
#endif

#endif//ZAL_BASE_H
