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

// This code is part of SIGMA Future Storage <https://sigmaco.org/future-storage>

#define _CRT_SECURE_NO_WARNINGS 1
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dep/pdjson/pdjson.h"

#define _AFX_JSON_C
#include "../io/afxIoDDK.h"

AFX_DEFINE_STRUCT(afxNotation)
{
    json_stream json;
};

afxError AfxMakeNotation(afxNotation* nota, afxString const* data)
{
    //json_open_buffer(&nota->json, data->start, data->len);
    //json_set_streaming(&nota->json, FALSE);
}

afxUnit AfxFindNotationItem(afxNotation* nota, afxString const* tag, afxUnit first)
{

}

afxUnit AfxIterateNotation(afxNotation* nota, afxUnit first, afxUnit cnt)
{

}

