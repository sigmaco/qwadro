/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *        Q W A D R O   V I D E O   G R A P H I C S   I N F R A S T R U C T U R E
 *
 *                               (c) 2017 SIGMA FEDERATION
 *                               ESTADO-MAIOR DA SEGURIDADE
 *                                 SIGMA TECHNOLOGY GROUP
 *                                        ENGITECH
 */

  //////////////////////////////////////////////////////////////////////////////
 // QWADRO IOMMU BUFFER                                                      //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2.
// This software is part of Advanced Video Graphics Extensions.
// This software is part of SIGMA Future Storage.

#ifndef AFX_BUFFER_H
#define AFX_BUFFER_H

#include "qwadro/afxMemoryUtil.h"

typedef enum afxFormat afxFormat;

// The value of AFX_BUFFER_SPARSE_PAGE_SIZE is the largest allowed value.
// Smaller values are permitted and it is recommended to implementors that it be a power of two.
#define AFX_BUFFER_SPARSE_PAGE_SIZE 65536

#define AFX_BUFFER_ALIGNMENT 64
#define AFX_ALIGN_BUFFERED(siz_) AFX_ALIGN_SIZE((siz_), AFX_BUFFER_ALIGNMENT)

typedef enum afxBufferUsage
{
    // Can be used as the "source" of a copy operation.
    afxBufferUsage_UPLOAD   = AFX_BITMASK(0),
    // Can be used as the "destination" of a copy or write operation.
    afxBufferUsage_DOWNLOAD = AFX_BITMASK(1),

    // Can be used as a vertex buffer; aka VBO.
    afxBufferUsage_VERTEX   = AFX_BITMASK(2),
    // Can be used as an index buffer; aka IBO.
    afxBufferUsage_INDEX    = AFX_BITMASK(3),

    // Can be used as a uniform buffer; aka UBO.
    afxBufferUsage_UNIFORM  = AFX_BITMASK(4),
    // Can be used as a storage buffer; aka SSBO.
    afxBufferUsage_STORAGE  = AFX_BITMASK(5),

    // Can be used as to store indirect command arguments; aka DBO.
    afxBufferUsage_INDIRECT = AFX_BITMASK(6),
    // Can be used to capture query results; aka QBO.
    afxBufferUsage_QUERY    = AFX_BITMASK(7),

    // Can be used to fetch tensor (usually texels) data; aka TFBO.
    afxBufferUsage_FETCH    = AFX_BITMASK(8),
    // Can be used to read/write tensor data; aka storable TBO.
    afxBufferUsage_TENSOR   = AFX_BITMASK(9),

    // Can be used to store primitive transformation output/feedback.
    afxBufferUsage_FEEDBACK = AFX_BITMASK(10),

    // Can be used as a argumentation buffer; ABO
    afxBufferUsage_ARGUMENT = AFX_BITMASK(11)
} afxBufferUsage;

typedef enum afxBufferFlag
{
    // Mapped range is readable by CPU.
    afxBufferFlag_R     = AFX_BITMASK(0),
    // Mapped range is writeable by CPU.
    afxBufferFlag_W     = AFX_BITMASK(1),
    // Host-visible, mappable storage.
    afxBufferFlag_M     = (afxBufferFlag_R | afxBufferFlag_W),
    // Host-visible, persistently mappable, executable. 
    // Mapped range will be executable by GPU while persistently being read and/or write by CPU.
    afxBufferFlag_X     = AFX_BITMASK(2),
    // Coherent. Data written in mapped range will be immediately visible by GPU and CPU.
    afxBufferFlag_C     = AFX_BITMASK(3),

    // And, to make things compact, there are here convenient combinations frequently used. Thank me later.
    afxBufferFlag_RW = (afxBufferFlag_R | afxBufferFlag_W),
    afxBufferFlag_RX = (afxBufferFlag_R | afxBufferFlag_X),
    afxBufferFlag_RXC = (afxBufferFlag_RX | afxBufferFlag_C),
    afxBufferFlag_WX = (afxBufferFlag_W | afxBufferFlag_X),
    afxBufferFlag_WXC = (afxBufferFlag_WX | afxBufferFlag_C),
    afxBufferFlag_RWX = (afxBufferFlag_RW | afxBufferFlag_X),
    afxBufferFlag_RWXC = (afxBufferFlag_RWX | afxBufferFlag_C),
    afxBufferFlag_ACCESS = (afxBufferFlag_RWX | afxBufferFlag_C),

    // Sparse. The buffer will be backed using sparse memory, and can NOT be mapped.
    afxBufferFlag_S     = AFX_BITMASK(4),
    // Protected. The buffer will be backed using protected memory.
    afxBufferFlag_P     = AFX_BITMASK(5),

    // Hosted. Determine whether storage is local to the client.
    afxBufferFlag_H     = AFX_BITMASK(6),
    // Foreign. The buffer will be backed using (imported) external memory.
    afxBufferFlag_F     = AFX_BITMASK(7),

    // Reserved for SIGMA.
    afxBufferFlag_Z     = AFX_BITMASK(31),
} afxBufferFlags;

typedef enum afxMappingFlag
{
    afxMappingFlag_FLUSH    = AFX_BITMASK(0), // explicitly flushed
    afxMappingFlag_UNSYNC   = AFX_BITMASK(1), // unsynchronized with previous operations

    // Actually these flags seems useless as we changed the behavior of buffer mapping to match Vulkan.
} afxMappingFlags;

AFX_DEFINE_STRUCT(afxBufferedCopy)
// Structure specifying a afxBuffer-backed copy.
{
    afxBuffer dst;

    // is the starting offset in bytes from the start of dstBuffer.
    afxSize dstOffset;

    // is the number of bytes to copy.
    afxUnit range;

    // is the starting offset in bytes from the start of srcBuffer.
    afxSize srcOffset;

    afxBuffer src;
};

#define AFX_BUFFERED_COPY(uSrcOffset, uDstOffset, uRange) \
    (afxBufferedCopy) { .srcOffset = (uSrcOffset), \
                        .dstOffset = (uDstOffset), \
                        .range = (uRange) }

AFX_DEFINE_STRUCT(afxBufferCopy)
// Structure specifying a afxBuffer-backed copy.
{
    // is the starting offset in bytes from the start of dstBuffer.
    afxSize dstOffset;

    // is the starting offset in bytes from the start of srcBuffer.
    afxSize srcOffset;

    // is the number of bytes to copy.
    afxUnit range;
};

#define AFX_BUFFER_COPY(uSrcOffset, uDstOffset, uRange) \
    (afxBufferCopy) {   .srcOffset = (uSrcOffset), \
                        .dstOffset = (uDstOffset), \
                        .range = (uRange) }

AFX_DEFINE_STRUCT(afxBufferIo)
// Structure specifying a afxBuffer-backed transfer.
{
    // is the starting offset in bytes from the start of dstBuffer.
    afxSize dstOffset;

    // [!] only if supported by device, else case it must be 1.
    afxUnit dstStride;

    // is the starting offset in bytes from the start of srcBuffer.
    afxSize srcOffset;

    // [!] only if supported by device, else case it must be 1.
    afxUnit srcStride;

    // is the number of rows to stream in/out.
    afxUnit rowCnt;
};

#define AFX_BUFFER_IO(uSrcOffset, uDstOffset, uSrcStride, uDstStride, uRowCnt) \
    (afxBufferIo) { .srcOffset = (uSrcOffset), \
                    .dstOffset = (uDstOffset), \
                    .srcStride = (uSrcStride), \
                    .dstStride = (uDstStride), \
                    .rowCnt = (uRowCnt) }

AFX_DEFINE_STRUCT(afxBufferedStream)
// Structure specifying a afxBuffer-backed stream.
{
    // A buffer handle.
    afxBuffer buf;

    // The start of buffer.
    afxSize offset;

    // The size in bytes of data from buffer.
    afxUnit range;

    // The byte stride between consecutive elements within the buffer.
    afxUnit stride;
};

#define AFX_BUFFERED_STREAM(hBuf, uOffset, uRange, uStride) \
    (afxBufferedStream) {   .buf = (hBuf), \
                            .offset = (uOffset), \
                            .range = (uRange), \
                            .stride = (uStride) }

AFX_DEFINE_STRUCT(afxBufferedMap)
// Structure specifying a afxBuffer-backed memory map.
{
    // A buffer handle.
    afxBuffer buf;

    // The start of buffer.
    afxSize offset;

    // The size in bytes of data from buffer.
    afxUnit range;

    // A bitmask of flags specifying additional parameters of the memory map operation.
    afxFlags flags;
};

#define AFX_BUFFERED_MAP(hBuf, uOffset, uRange, uFlags) \
    (afxBufferedMap) {  .buf = (hBuf), \
                        .offset = (uOffset), \
                        .range = (uRange), \
                        .flags = (uFlags) }

AFX_DEFINE_STRUCT(afxBufferInfo)
{
    // A bitmask specifying allowed usages of the buffer.
    afxBufferUsage usage;

    // A bitmask specifying additional parameters of the buffer.
    afxBufferFlags flags;

    // The size/range in bytes of the buffer to be created.
    afxSize size;

    // A avxFormat describing the format of the data elements in the (FETCH/TENSOR) buffer.
    afxFormat fmt; // required for FETCH/TENSOR buffers.

    // A bitmask of device bridges that will access this buffer.
    afxMask exuMask;

    // Possibly initial data, to avoid boilerplate with unconvenient mappings.
    void* data;

    afxSize dataSiz;

    // Should this buffer be mapped at creation?
    afxBool mapped; // replace for a afxBufferFlag flag?

    // A user-defined data.
    void* udd;

    // A static string specifying a debug tag.
    afxString tag;
};

/*
    The AfxAcquireBuffers() function acquires/allocates buffer resources from the IO system, based on buffer descriptors.

    This function acquires buffers (vertex buffers, uniform buffers, etc.) from a IO system (dsys), 
    based on descriptions provided in infos[].

    It need an handle of the IO system, which manages buffer pools, memory allocation, etc.
*/

AFX afxError AfxAcquireBuffers
(
    // The IO system providing the buffers.
    afxIommu iom, 

    // Number of buffers to acquire.
    afxUnit cnt, 

    // Array of buffer descriptors.
    afxBufferInfo const infos[], 

    // An output array which receives handles of the acquired buffers.
    afxBuffer buffers[]
);

AFX_DEFINE_STRUCT(afxMetabufferInfo)
{
    // A bitmask specifying additional parameters of the buffer.
    afxBufferFlags flags;

    // A avxBuffer on which the view will be created.
    afxBuffer buf;

    // An offset in bytes from the base address of the buffer.
    afxSize bufBase;

    // The size/range in bytes of the buffer to be created.
    afxSize bufRange;

    // A avxFormat describing the format of the data elements in the buffer.
    afxFormat fmt; // used for FETCH/TENSOR buffers.

    // A user-defined data.
    void* udd;

    // A static string specifying a debug tag.
    afxString tag;
};

AFX afxError AfxReacquireBuffers
(
    // The IO system providing the buffers.
    afxIommu iom,

    // Number of buffers to acquire.
    afxUnit cnt,

    // Array of buffer descriptors.
    afxMetabufferInfo const infos[],

    // An output array which receives handles of the acquired buffers.
    afxBuffer buffers[]
);

/*
    The AfxMapBuffers() function maps buffer regions to make them accessible for read/write operations in a IO system. 
    It allows for mapping multiple buffer regions in parallel, managing synchronization, validation, and performing the actual mapping of memory.

    Each buffer mapping is validated, and if it's already mapped, the function ensures the new mapping is within the current mapped region.
    If the buffer has been allocated in host memory (buf->data), the function directly maps the buffer region.
    If the buffer is a GPU-side or managed buffer, the function defers the actual mapping to a later stage using a IO system operation.

    The function uses atomic operations to manage the pending mapping state of each buffer.
    The function waits for any ongoing mapping operations to complete before proceeding.

    The function first checks for a ICD callback function associated with the IO system.
    If the callback is available, it uses it to perform the mapping; otherwise, it falls back to using IO queues and bridges.
    If no callback is available, the function uses a IO queue and IO bridge to handle the buffer remapping.

    The function waits for the IO queue to complete the remapping operation before proceeding.
    After the mapping operations are completed, the function ensures that all buffers have their pendingRemap counters decremented.

    It also performs additional validation (if enabled) to ensure the buffer's mapped state is consistent.
*/

AFX afxError AfxMapBuffers
(
    // The IO system providing the buffers.
    afxIommu iom,

    // The number of mapping operations.
    afxUnit cnt,

    // An array of afxBufferedMap describing the mapping operations.
    afxBufferedMap maps[], 

    // An array of placeholders for pointers to each mapping operation.
    // After mapping, each entry will point to a region in host memory corresponding to the mapped buffer segment.
    void** placeholders[]
);

/*
    The AfxUnmapBuffers() function unmaps buffers previously mapped in the IO system. 
    It handles synchronization, error checking, and supports both host-side buffers and buffers that are managed by the IO system.

    The function iterates through the list of buffers and waits for any ongoing mapping operations to complete before unmapping the buffers.
    The function uses atomic operations to manage the pending remap state of each buffer and ensure synchronization during unmapping.

    If the IO system provides a ICD callback function, it is used to unmap the buffers.
    If no ICD callback is available, the function uses a IO queue to perform the unmap operation.

    After unmapping the buffers, the function performs final checks (if validation is enabled) and clears the pendingRemap state for each buffer.
*/

AFX afxError AfxUnmapBuffers
(
    // The IO system providing the buffers.
    afxIommu iom,

    // The number of unmapping operations.
    afxUnit cnt,

    // An array of avxBufferedMap describing the unmapping operations.
    afxBufferedMap maps[]
);

/*
    The AfxCohereMappedBuffers() function ensures that any pending operations involving mapped buffers are completed, 
    typically flushing any modifications made to these buffers and synchronizing them with the processor or IO system. 
    The function validates the input, checks buffer states, and either uses a ICD callback or a IO queue to execute the flush operation.

    The function validates the provided buffers and ensures they are part of the correct IO system.

    If no ICD callback is available for flushing the maps, it uses a IO queue and bridge to synchronize the flush operation.

    Error handling and synchronization are key components of this function, ensuring that buffer operations are completed before continuing with other tasks.
*/

AFX afxError AfxCohereMappedBuffers
(
    // The IO system providing the buffers.
    afxIommu iom,

    // A boolean specifying that any host modification must be discarded.
    afxBool invalidate,

    // The number of buffered maps.
    afxUnit cnt,

    // An array of buffered map operations.
    afxBufferedMap const maps[]
);

/*
    The AfxCopyBuffers() function schedules one or more copy operations of IOMMU buffers' data locally or across memory domains.
*/

AFX afxError AfxCopyBuffers
(
    // The IO system providing the buffers.
    afxIommu iom,

    // Mask specifying which execution units should execute the copy. 
    // Think of this as a way to filter or select copy pipelines.
    afxMask exuMask,

    // The number of buffer copy operations to perform.
    afxUnit cnt,

    // An array of afxBufferedCopy structs, each describing one buffer copy operation (src, dst, size, offsets, etc.).
    afxBufferedCopy const ops[],

    afxFence signal
);

////////////////////////////////////////////////////////////////////////////////

AFX afxIommu AfxGetBufferHost
(
    afxBuffer buf
);

/*
    The AfxGetBufferCapacity() function returns the available capacity of a buffer, starting from a specified position. 
    It ensures that the requested capacity doesn't exceed the buffer's total capacity. 
    This function is useful when working with dynamic memory allocation or when you need to determine how much space is left in a buffer from a specific point, 
    helping you manage memory effectively when writing or updating buffer data.
    
    The function returns the available capacity of the buffer starting from the specified position by @from.
    If the @from value is greater than or equal to the buffer's total capacity, the function would return 0, 
    indicating there is no available space starting from that position.
*/

AFX afxSize AfxGetBufferCapacity
(
    // The buffer to be queried.
    afxBuffer buf, 

    // The starting point in the buffer from which capacity is being measured.
    afxSize from
);

// Query an address of a buffer used to access the memory bound to this buffer on the device.

AFX afxSize AfxGetBufferAddress
(
    // The buffer to be queried.
    afxBuffer buf,

    // The starting point in the buffer from which address is being queried.
    afxSize from
);

/*
    The AfxGetBufferUsage() function retrieves or queries specific usage flags associated with a buffer object. 
    It ensures that the buffer is valid before proceeding, and it allows filtering the buffer’s usage flags using a bitmask if needed. 
    If no bitmask is provided, it simply returns the entire usage flags of the buffer. This function is useful for checking or 
    validating how a buffer is intended to be used in a data pipeline.

    If a @usage pattern is specified, it returns the bitwise AND between buffer's usage and the specified usage. 
    This would allow checking if a specific usage flag is set in the buffer's usage flags.
    If usage is NIL, it simply returns the entire buffer's usage value.

    The function returns either the filtered usage flags (if a specific usage bitmask is specified),
    or, it returns the entire usage flag of the buffer (if no specific usage flag is specified).
*/

AFX afxBufferUsage AfxGetBufferUsage
(
    // The buffer to be queried.
    afxBuffer buf, 

    // A bitmask or a specific usage flag for the buffer.
    afxBufferUsage mask
);

/*
    The AfxGetBufferFlags() function retrieves the access flags of a buffer. 
    By performing a bitwise operation, it checks whether the buffer has certain access permissions (such as read or write access). 
    If the access flag is provided, it returns whether the requested access type is set in the buffer's flags. 
    If no specific flag is requested, it returns the buffer's entire set of access flags. 
    This function is useful for querying buffer access and ensuring the buffer is being used correctly in the data pipeline.
*/

AFX afxBufferFlags AfxGetBufferFlags
(
    // The buffer to be queried.
    afxBuffer buf, 

    // A bitmask representing a specific access flag or a query for specific access flags.
    afxBufferFlags mask
);

/*
    The AfxGetBufferMap() function provides direct access to a mapped region of a buffer. 
    It's used after a buffer has already been mapped using AfxMapBuffers.
    Returns a pointer to a sub-region of a previously mapped buffer.
*/

AFX void* AfxGetBufferMap
(
    // An handle to a previously acquired and mapped buffer.
    afxBuffer buf, 

    // Byte offset into the buffer's memory where your region starts.
    afxSize offset, 

    // Size in bytes of the memory region which the access is wanted.
    afxUnit range
);

/*
    The AfxMapBuffer() method maps a buffer to memory, making it accessible for CPU operations. 
    It validates inputs (buffer validity, alignment, range), prepares the mapping structure, and calls the system's mapping function to map the buffer. 
    The function also includes error handling and validation to ensure that the mapping process is carried out safely and efficiently.

    This function is just a convenient way of calling AfxMapBuffers(), which is called to perform the actual mapping of the buffer,
    for a single buffer. Otherwise, AfxMapBuffers() should be used to avoid contention.
*/

AFX afxError AfxMapBuffer
(
    // The buffer that needs to be mapped.
    afxBuffer buf, 

    // The starting byte offset where the buffer mapping should begin.
    afxSize offset, 

    // The size (in bytes or units) of the portion of the buffer to map, starting from the @offset.
    afxUnit range, 

    // Flags that define the special behaviors for the mapping.
    afxFlags flags, 

    // A pointer to a pointer (void**), which will be used to store the address of the mapped memory.
    // After the mapping, this pointer will point to the memory region that has been mapped.
    void** placeholder
);

/*
    The AfxUnmapBuffer() method unmaps a buffer from memory. 
    It ensures that any changes to the buffer are synchronized before releasing the memory access. 
    The function validates the buffer and I/O system, calls the unmapping process, and handles errors if the unmap operation fails. 
    The wait flag is not used in this function's implementation, but it could be useful for managing synchronization in a more complex implementation.

    This function is just a convenient way of calling AfxUnmapBuffers(), which is called to perform the actual unmapping of the buffer,
    for a single buffer. Otherwise, AvxUnmapBuffers() should be used to avoid contention.
*/

AFX afxError AfxUnmapBuffer
(
    // The buffer to unmap.
    afxBuffer buf, 

    // A flag indicating whether the operation should wait for synchronization.
    afxBool wait
);

/*
    The AfxCohereMappedBuffer() method performs any coherence in the mapped range. 
    When @sync is FALSE, it ensures any changes to a buffer's data are committed or flushed to memory.
    When @sync is TRUE, it synchronizes a buffer's state to ensure that it is ready for further operations (such as reading or writing).
    And if any errors occur during this process, they are raised and returned.

    This function is just a convenient way of calling AfxCohereMappedBuffers(), which carries out the synchronization, for a single buffer.
    Otherwise, AfxCohereMappedBuffers() should be used to avoid contention.
*/

AFX afxError AfxCohereMappedBuffer
(
    // The buffer that needs to be flushed.
    afxBuffer buf,

    // The starting point (in bytes) of the portion of the buffer that should be flushed.
    afxSize offset,

    // The size (in bytes or units) of the portion of the buffer to flush, starting from the @offset.
    afxUnit range,

    // The optional flags.
    afxFlags flags,

    // A boolean specifying if any host modification should be discarded.
    afxBool invalidate
);

/*
    The AfxUploadBuffer() method uploads data to a buffer. 
    It validates the operations, prepares a transfer structure, and then performs the actual data upload. 
    The function handles synchronization and ensures that errors are caught and thrown appropriately. 
    The use of execution units allows for parallel or distributed uploads, such as using a GPU to handle the operations.
*/

AFX afxError AfxUploadBuffer
(
    // The destination buffer to which data will be uploaded.
    afxBuffer buf, 

    // The number of operations to be performed on the buffer.
    afxUnit opCnt, 

    // An array of operations (@opCnt number of operations) to be performed on the buffer.
    // Each operation specifies where the data will be uploaded within the buffer and how the data should be handled.
    afxBufferIo const ops[],

    // The source stream from which the data will be transferred into the buffer.
    afxStream in,

    // The execution unit index. This typically refers to the specific processor or compute unit that will handle the operation.
    afxMask exuMask,

    afxFence signal
);

/*
    The AfxDownloadBuffer() method downloads data from a buffer. 
    It validates the operations, prepares a transfer structure, and then performs the actual data transfer. 
    It also handles synchronization and error handling, ensuring that the data is downloaded to the specified output stream. 
    The function is optimized for use in parallel computing systems, with the ability to specify the execution unit responsible for the operation.
*/

AFX afxError AfxDownloadBuffer
(
    // The source buffer from which data will be downloaded.
    afxBuffer buf, 

    // The number of operations to be performed on the buffer.
    afxUnit opCnt, 

    // An array of operations to be performed on the buffer. 
    // Each operation specifies how data should be extracted from the buffer.
    afxBufferIo const ops[],

    // The destination stream where the data from the buffer will be transferred.
    afxStream out,

    // The execution unit index. This refers to the specific processor or compute unit responsible for the operation.
    afxMask exuMask,

    afxFence signal
);

/*
    The AfxDumpBuffer() method dumps data from a buffer to a specified destination. 
    It validates the operations, prepares a transfer structure, and then performs the actual data transfer. 
    It also handles synchronization and error handling, ensuring that the data is dumped to the specified destination. 
    The function is optimized for use in parallel computing systems, with the ability to specify 
    the execution unit responsible for the operation.
*/

AFX afxError AfxDumpBuffer
(
    // The source buffer from which data will be dumped.
    afxBuffer buf, 

    // The number of operations to be performed on the buffer.
    afxUnit opCnt, 

    // An array of operations to be performed on the buffer. 
    // Each operation specifies how data should be dumped from the buffer.
    afxBufferIo const ops[],

    // A pointer to the destination where the buffer data will be dumped.
    void* dst,

    // The execution unit index (e.g., device, processor or compute unit) responsible for the operation.
    afxMask exuMask,

    afxFence signal
);

AFX afxError AfxUpdateBuffer
(
    // The target buffer that will be updated with new data.
    afxBuffer buf, 

    // The number of operations to be performed on the buffer.
    afxUnit opCnt, 

    // An array of operations to be performed on the buffer. 
    // Each operation specifies how the buffer should be updated.
    afxBufferIo const ops[],

    // A pointer to the source data from which the buffer will be updated.
    void const* src,

    // The execution unit index (e.g., device, processor or compute unit) that will perform the transfer.
    afxMask exuMask,

    afxFence signal
);

#endif//AFX_BUFFER_H
