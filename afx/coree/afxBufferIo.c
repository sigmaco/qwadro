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
 *                               ESTADO-MAIOR DA SEGURIDADE
 *                                 SIGMA TECHNOLOGY GROUP
 *                                        ENGITECH
 */

// This code is part of SIGMA GL/2.
// This software is part of Advanced Video Graphics Extensions.
// This software is part of SIGMA Future Storage.

#define _AFX_CORE_C
#define _AFX_BUFFER_C
#include "afxSystemDDK.h"

#define _AFX_BUFFER_HOSTSIDE_ALWAYS_FULLY_MAPPED TRUE

////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxDumpBuffer(afxBuffer buf, afxUnit opCnt, afxBufferIo const ops[], void* dst, afxMask exuMask, afxFence signal)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    AFX_ASSERT(AfxGetBufferFlags(buf, afxBufferFlag_R));
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ops);
    AFX_ASSERT(dst);

#if AFX_VALIDATION_ENABLED
    // Validate the buffer operations (only happens when validation is enabled)
    for (afxUnit i = 0; i < opCnt; i++)
    {
        afxBufferIo const* op = &ops[i];
        AFX_ASSERT_ALIGNMENT(op->srcOffset, AFX_BUFFER_ALIGNMENT);
        AFX_ASSERT_RANGE(AfxGetBufferCapacity(buf, 0), op->srcOffset, AFX_MAX(1, op->rowCnt) * AFX_MAX(1, op->dstStride));
    }
#endif//AFX_VALIDATION_ENABLED

    // Prepare the transfer structure for the dump operation
    afxTransference transfer = { 0 };
    transfer.exuMask = exuMask;
    transfer.signal = signal;
    transfer.srcFcc = afxFcc_BUF;
    transfer.src.buf = buf;
    transfer.dst.dst = dst;

    // Get the IO system context associated with the buffer
    afxIommu iom = AfxGetBufferHost(buf);
    AFX_ASSERT_OBJECTS(afxFcc_IOM, 1, &iom);

    // Perform the transfer (dump) of data
    if (_AfxIomGetDdi(iom)->transferCb(iom, &transfer, opCnt, ops))
    {
        // If the transfer fails, throw an error
        AfxThrowError();
        return err;
    }

    // Ensure that the base queue index is valid
    AFX_ASSERT(transfer.baseQueIdx != AFX_INVALID_INDEX);
#if 0
    // Wait for the IO queue to finish processing the dump
    if (AfxWaitForIoQueue(iom, AFX_TIMEOUT_INFINITE, portIdx, transfer.baseQueIdx))
        AfxThrowError();
#endif
    // Return the error code (still AFX_ERR_NONE if no error occurred)
    return err;
}

_AFX afxError AfxUpdateBuffer(afxBuffer buf, afxUnit opCnt, afxBufferIo const ops[], void const* src, afxMask exuMask, afxFence signal)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    AFX_ASSERT(AfxGetBufferFlags(buf, afxBufferFlag_W));
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ops);
    AFX_ASSERT(src);

#if AFX_VALIDATION_ENABLED
    // Verify that all operations are valid (this block only runs if validation is enabled)
    for (afxUnit i = 0; i < opCnt; i++)
    {
        afxBufferIo const* op = &ops[i];
        AFX_ASSERT_ALIGNMENT(op->dstOffset, AFX_BUFFER_ALIGNMENT);
        AFX_ASSERT_RANGE(AfxGetBufferCapacity(buf, 0), op->dstOffset, AFX_MAX(1, op->rowCnt) * AFX_MAX(1, op->dstStride));
    }
#endif//AFX_VALIDATION_ENABLED

    // Prepare the transfer structure
    afxTransference transfer = { 0 };
    transfer.exuMask = exuMask;
    transfer.signal = signal;
    transfer.dstFcc = afxFcc_BUF;
    transfer.dst.buf = buf;
    transfer.src.src = src;

    // Get the IO system associated with the buffer
    afxIommu iom = AfxGetBufferHost(buf);
    AFX_ASSERT_OBJECTS(afxFcc_IOM, 1, &iom);

    // Perform the data transfer (buffer update)
    if (_AfxIomGetDdi(iom)->transferCb(iom, &transfer, opCnt, ops))
    {
        // If transfer fails, throw an error
        AfxThrowError();
        return err;
    }

    // Ensure the base queue index is valid
    AFX_ASSERT(transfer.baseQueIdx != AFX_INVALID_INDEX);
#if 0
    // Wait for the IO queue to finish processing (commented out)
    if (AfxWaitForIoQueue(iom, AFX_TIMEOUT_INFINITE, portIdx, transfer.baseQueIdx))
        AfxThrowError();
#endif
    // Return the error code (still AFX_ERR_NONE if no error occurred)
    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxUploadBuffer(afxBuffer buf, afxUnit opCnt, afxBufferIo const ops[], afxStream in, afxMask exuMask, afxFence signal)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    AFX_ASSERT(AfxGetBufferFlags(buf, afxBufferFlag_W));

#if AFX_VALIDATION_ENABLED
    // Validation of the buffer operations (only happens when validation is enabled)
    for (afxUnit i = 0; i < opCnt; i++)
    {
        afxBufferIo const* op = &ops[i];
        AFX_ASSERT_ALIGNMENT(op->dstOffset, AFX_BUFFER_ALIGNMENT);
        AFX_ASSERT_RANGE(AfxGetBufferCapacity(buf, 0), op->dstOffset, AFX_MAX(1, op->rowCnt) * AFX_MAX(1, op->dstStride));
    }
#endif//AFX_VALIDATION_ENABLED

    // Prepare a transfer structure for the operation
    afxTransference transfer = { 0 };
    transfer.exuMask = exuMask;
    transfer.signal = signal;
    transfer.dstFcc = afxFcc_BUF;
    transfer.srcFcc = afxFcc_IOB;
    transfer.dst.buf = buf;
    transfer.src.iob = in;

    // Get the IO system (context) associated with the buffer
    afxIommu iom = AfxGetBufferHost(buf);
    AFX_ASSERT_OBJECTS(afxFcc_IOM, 1, &iom);

    // Perform the actual transfer of data
    if (_AfxIomGetDdi(iom)->transferCb(iom, &transfer, opCnt, ops))
    {
        AfxThrowError();
        return err;
    }

    // Assert that the base queue index is valid
    AFX_ASSERT(transfer.baseQueIdx != AFX_INVALID_INDEX);
#if 0
    // Wait for the IO queue to finish processing
    if (AfxWaitForIoQueue(iom, AFX_TIMEOUT_INFINITE, portIdx, transfer.baseQueIdx))
        AfxThrowError();
#endif
    // Return the error code (still AFX_ERR_NONE if no error occurred)
    return err;
}

_AFX afxError AfxDownloadBuffer(afxBuffer buf, afxUnit opCnt, afxBufferIo const ops[], afxStream out, afxMask exuMask, afxFence signal)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    AFX_ASSERT(AfxGetBufferFlags(buf, afxBufferFlag_R));

#if AFX_VALIDATION_ENABLED
    // Validate the buffer operations (only happens when validation is enabled)
    for (afxUnit i = 0; i < opCnt; i++)
    {
        afxBufferIo const* op = &ops[i];
        AFX_ASSERT_ALIGNMENT(op->srcOffset, AFX_BUFFER_ALIGNMENT);
        AFX_ASSERT_RANGE(AfxGetBufferCapacity(buf, 0), op->srcOffset, AFX_MAX(1, op->rowCnt) * AFX_MAX(1, op->dstStride));
    }
#endif//AFX_VALIDATION_ENABLED

    // Prepare the transfer structure for the download operation
    afxTransference transfer = { 0 };
    transfer.exuMask = exuMask;
    transfer.signal = signal;
    transfer.srcFcc = afxFcc_BUF;
    transfer.dstFcc = afxFcc_IOB;
    transfer.src.buf = buf;
    transfer.dst.iob = out;

    // Get the IO system context associated with the buffer
    afxIommu iom = AfxGetBufferHost(buf);
    AFX_ASSERT_OBJECTS(afxFcc_IOM, 1, &iom);

    // Perform the transfer (download) of data
    if (_AfxIomGetDdi(iom)->transferCb(iom, &transfer, opCnt, ops))
    {
        AfxThrowError();
        return err;
    }

    // Ensure that the base queue index is valid
    AFX_ASSERT(transfer.baseQueIdx != AFX_INVALID_INDEX);
#if 0
    if (AfxWaitForIoQueue(iom, AFX_TIMEOUT_INFINITE, portIdx, transfer.baseQueIdx))
        AfxThrowError();
#endif
    // Return the error code (still AFX_ERR_NONE if no error occurred)
    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxMapBuffers(afxIommu iom, afxUnit cnt, afxBufferedMap maps[], void** placeholders[])
{
    afxError err = { 0 };
    // @iom must be a valid afxIommu handle.
    AFX_ASSERT_OBJECTS(afxFcc_IOM, 1, &iom);

    // Ensure that the operation count doesn't exceed 32
    AFX_ASSERT(cnt <= AFX_MAX_VERTEX_SOURCES);
    // Temporary array for mapping operations
    _afxBufferRemapping remaps2[AFX_MAX_VERTEX_SOURCES];
    // Count of operations to be processed
    afxUnit opCnt = 0;

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxBufferedMap const* map = &maps[i];
        afxBuffer buf = map->buf;
        
        // Skip if the buffer is NULL
        if (!buf) continue;
        AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);

        // Get the buffer's capacity
        afxSize bufCap = AfxGetBufferCapacity(buf, 0);

        afxUnit range = map->range;
        // range always is SIMD aligned.
        range = AFX_ALIGN_SIZE(range, AFX_SIMD_ALIGNMENT);
        AFX_ASSERT_ALIGNMENT(range, AFX_SIMD_ALIGNMENT);
        
        afxSize offset = map->offset;
        AFX_ASSERT_ALIGNMENT(offset, AFX_BUFFER_ALIGNMENT);
        // offset must be aligned to AFX_BUFFER_ALIGNMENT.

        // Perform validation if enabled
#ifdef AFX_VALIDATION_ENABLED
        AFX_ASSERT(AfxGetBufferFlags(buf, afxBufferFlag_RW));

        if (bufCap < map->offset + map->range)
        {
            AFX_ASSERT_RANGE(bufCap, map->offset, map->range);
            AfxThrowError();
            break;
        }
        
        // Check that the buffer context matches the IO system.
        afxIommu dsys2 = AfxGetBufferHost(buf);
        if (dsys2 != iom)
        {
            AFX_ASSERT_OBJECTS(afxFcc_IOM, 1, &dsys2);
            AFX_ASSERT(dsys2 == iom);
            AfxThrowError();
            break;
        }

        // Validate offset and range
        AFX_ASSERT_RANGE(bufCap, offset, range);

#endif//AFX_VALIDATION_ENABLED

        // If range is 0, calculate it based on the buffer's capacity
        if (!range) range = AFX_MIN(bufCap, bufCap - offset);

        // Wait for any pending mapping operation on the buffer
        while (AfxLoadAtom32(&buf->storage[0].pendingRemap)) AfxYield();
        // Signal an ongoing mapping operation
        AfxIncAtom32(&buf->storage[0].pendingRemap);

        // If the buffer has already been mapped, check if the region is valid
        if (buf->storage[0].mapPtr)
        {
            AFX_ASSERT(buf->storage[0].mapRefCnt);

            AFX_ASSERT_RANGE(buf->reqSiz, buf->storage[0].mapOffset, buf->storage[0].mapRange);

            // Validate if the requested region is within the currently mapped region
            if ((offset < buf->storage[0].mapOffset) || ((offset + range) > (buf->storage[0].mapOffset + buf->storage[0].mapRange)))
            {
                AfxThrowError();
                AfxDecAtom32(&buf->storage[0].pendingRemap);
                break;
            }
            else
            {
                // If the region is valid, set the placeholder to the corresponding address
                AFX_ASSERT(buf->storage[0].mapOffset + buf->storage[0].mapRange >= offset + range);
                AFX_ASSERT(buf->storage[0].mapOffset <= offset);
                ++buf->storage[0].mapRefCnt;
                afxUnit offDiff = offset - buf->storage[0].mapOffset;
                AFX_ASSERT(placeholders);
                *placeholders[i] = &buf->storage[0].mapPtr[offDiff];
            }

            // Decrement the pending remap count as no new operation was added
            AfxDecAtom32(&buf->storage[0].pendingRemap);
        }
        // If buffer is host-side allocated, map directly (just do it here)
        else
        {
            AFX_ASSERT(buf->storage[0].mapRefCnt == 0);

            if (buf->storage[0].host.addr)
            {
#ifndef _AFX_BUFFER_HOSTSIDE_ALWAYS_FULLY_MAPPED
                buf->storage[0].mapOffset = offset;
                buf->storage[0].mapRange = range;
                buf->storage[0].mapFlags = map->flags;
                buf->storage[0].mapPtr = &buf->storage[0].hostedAlloc.bytemap[offset];

                AFX_ASSERT(placeholders);
                *placeholders[i] = buf->storage[0].mapPtr;
#else
                // Always map the whole buffer when host-side allocated.
                buf->storage[0].mapRefCnt = 1;
                buf->storage[0].mapOffset = 0;
                buf->storage[0].mapRange = buf->reqSiz;
                buf->storage[0].mapFlags = map->flags;
                buf->storage[0].mapPtr = buf->storage[0].host.bytemap;

                AFX_ASSERT(placeholders);
                *placeholders[i] = &buf->storage[0].mapPtr[map->offset];
#endif
                // Decrement the pending remap count as no new operation was added
                AfxDecAtom32(&buf->storage[0].pendingRemap);
            }
            else
            {
                // Buffer needs to be mapped through a IO system operation
                remaps2[opCnt].buf = buf;
                remaps2[opCnt].offset = offset;
                remaps2[opCnt].range = range;
                remaps2[opCnt].flags = map->flags;
                remaps2[opCnt].unmap = FALSE;
                remaps2[opCnt].placeholder = placeholders[i];
                AFX_ASSERT(remaps2[opCnt].placeholder);
                opCnt++;

                //AfxDecAtom32(&buf->pendingRemap);
            }
        }
    }

    // If an error occurred or no operations are left to process, return the error
    if (err || !opCnt)
        return err;

    if (_AfxIomGetDdi(iom)->remapCb(iom, FALSE, opCnt, remaps2))
    {
        AfxThrowError();
    }

    // Final validation and cleaning up pending remaps for each buffer.
    for (afxUnit i = 0; i < opCnt; i++)
    {
        afxBuffer buf = remaps2[i].buf;
        AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);

#if AFX_VALIDATION_ENABLED
        // Perform final validation on the buffer after remapping.
        AFX_ASSERT(buf->storage[0].mapPtr);
        AFX_ASSERT(buf->storage[0].mapRange >= remaps2[i].range);
        AFX_ASSERT(buf->storage[0].mapOffset >= remaps2[i].offset);
        AFX_ASSERT((buf->storage[0].mapOffset + buf->storage[0].mapRange) >= (remaps2[i].offset + remaps2[i].range));
        AFX_ASSERT_RANGE(buf->reqSiz, buf->storage[0].mapOffset, buf->storage[0].mapRange);
#endif//AFX_VALIDATION_ENABLED
        
        // Decrement the pending remap counter for each buffer.
        AfxDecAtom32(&buf->storage[0].pendingRemap);
    }

    // Return the error code (still AFX_ERR_NONE if no error occurred).
    return err;
}

_AFX afxError AfxUnmapBuffers(afxIommu iom, afxUnit cnt, afxBufferedMap maps[])
{
    afxError err = { 0 };
    // @iom must be a valid afxIommu handle.
    AFX_ASSERT_OBJECTS(afxFcc_IOM, 1, &iom);

    /*
        This function does not accept flags. Instead, any behavior wanted in unmapping must be specified in mapping operation.
    */

    // Ensure that the operation count doesn't exceed 32
    AFX_ASSERT(cnt <= AFX_MAX_VERTEX_SOURCES);
    _afxBufferRemapping unmaps2[AFX_MAX_VERTEX_SOURCES];
    // Count of buffers to unmap.
    afxUnit opCnt = 0;

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxBufferedMap const* map = &maps[i];
        afxBuffer buf = map->buf;

        // Skip if the buffer is NULL
        if (!buf) continue;
        AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);

        // Can not be unmapped.
        if (buf->storage[0].permanentlyMapped) continue;

#if AFX_VALIDATION_ENABLED
        AFX_ASSERT(AfxGetBufferFlags(buf, afxBufferFlag_RW));

        // Validate the IO system context of the buffer
        // Ensure the buffer is associated with the current IO system
        afxIommu dsys2 = AfxGetBufferHost(buf);
        AFX_ASSERT_OBJECTS(afxFcc_IOM, 1, &dsys2);

        if (dsys2 != iom)
        {
            AFX_ASSERT(dsys2 == iom);
            // If the buffer is not associated with the correct IO system, throw an error
            AfxThrowError();
            break;
        }
#endif//AFX_VALIDATION_ENABLED

        // Wait for any ongoing mapping operation to complete before unmapping.
        // Yield if there are pending remaps.
        while (AfxLoadAtom32(&buf->storage[0].pendingRemap)) AfxYield();
        // Indicate that an unmap operation is ongoing.
        AfxIncAtom32(&buf->storage[0].pendingRemap);

        if (buf->storage[0].mapPtr)
        {
            AFX_ASSERT(buf->storage[0].mapRefCnt);

            // If the buffer is host-side allocated (i.e., directly accessible in CPU memory)
            if (buf->storage[0].host.addr)
            {
                --buf->storage[0].mapRefCnt;

                if (0 == buf->storage[0].mapRefCnt)
                {
                    AFX_ASSERT(0 == buf->storage[0].mapRefCnt);
                    // Clear the mapped state for host-side buffers
                    buf->storage[0].mapOffset = 0;
                    buf->storage[0].mapRange = 0;
                    buf->storage[0].mapFlags = NIL;
                    buf->storage[0].mapPtr = NIL;
                    // Decrement the pending remap counter
                    AfxDecAtom32(&buf->storage[0].pendingRemap);
                }
            }
            else
            {
                if (1 == buf->storage[0].mapRefCnt)
                {
                    AFX_ASSERT(1 == buf->storage[0].mapRefCnt);

                    // If the buffer is not host-side allocated, prepare it for an unmap operation
                    unmaps2[opCnt].buf = buf;
                    unmaps2[opCnt].offset = 0;
                    unmaps2[opCnt].range = 0;
                    unmaps2[opCnt].flags = NIL;
                    unmaps2[opCnt].unmap = TRUE;
                    unmaps2[opCnt].placeholder = NIL;
                    opCnt++;
                }
                else
                {
                    --buf->storage[0].mapRefCnt;
                }
            }
        }
        else
        {
            AFX_ASSERT(buf->storage[0].mapRefCnt == 0);
        }
    }

    // If no buffers need to be unmapped or if an error occurred, return the error code
    if (err || !opCnt)
        return err;

    if (_AfxIomGetDdi(iom)->remapCb(iom, TRUE, cnt, unmaps2))
    {
        AfxThrowError();
    }

    // Final cleanup for unmapped buffers
    for (afxUnit i = 0; i < opCnt; i++)
    {
        afxBuffer buf = unmaps2[i].buf;
        AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);

#if AFX_VALIDATION_ENABLED
        // Some final validation checks (if validation is enabled)

#if 0 // if no waited, these assertions will always fail.
        // some assertions
        AFX_ASSERT(!buf->bytemap);
        AFX_ASSERT(!buf->mappedRange);
        AFX_ASSERT(!buf->mappedOffset);
#endif

#endif//AFX_VALIDATION_ENABLED

        // Remove the pending remap state for the unmapped buffer
        AfxDecAtom32(&buf->storage[0].pendingRemap);
    }

    // Return the error code (AFX_ERR_NONE if successful)
    return err;
}

_AFX afxError AfxCohereMappedBuffers(afxIommu iom, afxBool invalidate, afxUnit cnt, afxBufferedMap const maps[])
{
    afxError err = { 0 };
    // @iom must be a valid afxIommu handle.
    AFX_ASSERT_OBJECTS(afxFcc_IOM, 1, &iom);

#if AFX_VALIDATION_ENABLED
    // Perform validation for each buffer in the maps
    for (afxUnit i = 0; i < cnt; i++)
    {
        afxBufferedMap const* map = &maps[i];
        afxBuffer buf = map->buf;

        // Skip if the buffer is invalid
        if (!buf) continue;
        AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);

        // Ensure the map's range is within the buffer's capacity
        afxSize bufCap = AfxGetBufferCapacity(buf, 0);
        if (bufCap < map->offset + map->range)
        {
            // If the range is out of bounds, throw an error
            AFX_ASSERT_RANGE(bufCap, map->offset, map->range);
            AfxThrowError();
            break;
        }

        // Ensure the buffer belongs to the same IO system as the provided one
        afxIommu dsys2 = AfxGetBufferHost(buf);
        if (dsys2 != iom)
        {
            // If the buffer belongs to a different system, throw an error
            AFX_ASSERT_OBJECTS(afxFcc_IOM, 1, &dsys2);
            AFX_ASSERT(dsys2 == iom);
            AfxThrowError();
            break;
        }
    }

    // If any error has occurred during validation, return the error code
    if (err)
        return err;
#endif//AFX_VALIDATION_ENABLED

    // If no ICD callback is provided, use a IO queue to perform the flush/invalidate operation.
    if (_AfxIomGetDdi(iom)->cohereCb(iom, invalidate, cnt, maps))
    {
        // If flushing fails, throw an error
        AfxThrowError();
    }
    // Return the error code (AFX_ERR_NONE if successful)
    return err;
}

_AFX afxError AfxCopyBuffers(afxIommu iom, afxMask exuMask, afxUnit cnt, afxBufferedCopy const ops[], afxFence signal)
{
    afxError err = { 0 };
    // @iom must be a valid afxIommu handle.
    AFX_ASSERT_OBJECTS(afxFcc_IOM, 1, &iom);

    AFX_ASSERT(cnt);
    AFX_ASSERT(ops);
    
    // TODO: Use command context?

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxBufferedCopy const* op = &ops[i];
        AFX_ASSERT_ALIGNMENT(op->dstOffset, AFX_BUFFER_ALIGNMENT);
        AFX_ASSERT_ALIGNMENT(op->srcOffset, AFX_BUFFER_ALIGNMENT);

        afxBuffer dst = op->dst;
        AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &dst);
        AFX_ASSERT_RANGE(AfxGetBufferCapacity(dst, 0), op->dstOffset, op->range);

        afxBuffer src = op->src;
        AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &src);
        AFX_ASSERT_RANGE(AfxGetBufferCapacity(src, 0), op->srcOffset, op->range);

        // Prepare the transfer structure
        afxTransference transfer = { 0 };
        transfer.exuMask = exuMask;
        transfer.signal = signal;
        transfer.dstFcc = afxFcc_BUF;
        transfer.srcFcc = afxFcc_BUF;
        transfer.dst.buf = dst;
        transfer.src.src = src;

        afxBufferCopy op2 = { 0 };
        op2.dstOffset = op->dstOffset;
        op2.srcOffset = op->srcOffset;
        op2.range = op->range;

        // Perform the data transfer (buffer update)
        if (_AfxIomGetDdi(iom)->transferCb(iom, &transfer, 1, &op2))
        {
            // If transfer fails, throw an error
            AfxThrowError();
            //return err;
        }
    }
    return err;
}
