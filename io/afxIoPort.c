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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AFX_IO_BRIDGE_C
#define _AFX_IO_QUEUE_C
#define _AFX_DRAW_OUTPUT_C
#include "afxIoDDK.h"
#include "../exec/afxSystemDDK.h"
#include "qwadro/io/afxIoBridge.h"

/*
    I/O completion ports
    I/O completion ports provide an efficient threading model for processing multiple asynchronous I/O requests on a multiprocessor system. 
    When a process creates an I/O completion port, the system creates an associated queue object for threads whose sole purpose is to 
    service these requests. Processes that handle many concurrent asynchronous I/O requests can do so more quickly and efficiently by using 
    I/O completion ports in conjunction with a pre-allocated thread pool than by creating threads at the time they receive an I/O request.

    How I/O completion ports work
    The CreateIoCompletionPort function creates an I/O completion port and associates one or more file handles with that port. 
    When an asynchronous I/O operation on one of these file handles completes, an I/O completion packet is queued in first-in-first-out (FIFO) 
    order to the associated I/O completion port. One powerful use for this mechanism is to combine the synchronization point for multiple 
    file handles into a single object, although there are also other useful applications. Please note that while the packets are queued in 
    FIFO order they may be dequeued in a different order.

    When a file handle is associated with a completion port, the status block passed in will not be updated until the packet is removed from 
    the completion port. The only exception is if the original operation returns synchronously with an error. A thread (either one created by 
    the main thread or the main thread itself) uses the GetQueuedCompletionStatus function to wait for a completion packet to be queued to 
    the I/O completion port, rather than waiting directly for the asynchronous I/O to complete. Threads that block their execution on an I/O 
    completion port are released in last-in-first-out (LIFO) order, and the next completion packet is pulled from the I/O completion port's 
    FIFO queue for that thread. This means that, when a completion packet is released to a thread, the system releases the last (most recent) 
    thread associated with that port, passing it the completion information for the oldest I/O completion.

    Although any number of threads can call GetQueuedCompletionStatus for a specified I/O completion port, when a specified thread calls 
    GetQueuedCompletionStatus the first time, it becomes associated with the specified I/O completion port until one of three things occurs: 
    The thread exits, specifies a different I/O completion port, or closes the I/O completion port. In other words, a single thread can be 
    associated with, at most, one I/O completion port.

    When a completion packet is queued to an I/O completion port, the system first checks how many threads associated with that port are running. 
    If the number of threads running is less than the concurrency value (discussed in the next section), one of the waiting threads (the most 
    recent one) is allowed to process the completion packet. When a running thread completes its processing, it typically calls 
    GetQueuedCompletionStatus again, at which point it either returns with the next completion packet or waits if the queue is empty.

    Threads can use the PostQueuedCompletionStatus function to place completion packets in an I/O completion port's queue. By doing so, the 
    completion port can be used to receive communications from other threads of the process, in addition to receiving I/O completion packets 
    from the I/O system. The PostQueuedCompletionStatus function allows an application to queue its own special-purpose completion packets to 
    the I/O completion port without starting an asynchronous I/O operation. This is useful for notifying worker threads of external events, 
    for example.

    The I/O completion port handle and every file handle associated with that particular I/O completion port are known as references to the I/O 
    completion port. The I/O completion port is released when there are no more references to it. Therefore, all of these handles must be properly 
    closed to release the I/O completion port and its associated system resources. After these conditions are satisfied, an application should 
    close the I/O completion port handle by calling the CloseHandle function.

    Threads and Concurrency
    The most important property of an I/O completion port to consider carefully is the concurrency value. The concurrency value of a completion 
    port is specified when it is created with CreateIoCompletionPort via the NumberOfConcurrentThreads parameter. This value limits the number 
    of runnable threads associated with the completion port. When the total number of runnable threads associated with the completion port 
    reaches the concurrency value, the system blocks the execution of any subsequent threads associated with that completion port until the 
    number of runnable threads drops below the concurrency value.

    The most efficient scenario occurs when there are completion packets waiting in the queue, but no waits can be satisfied because the port 
    has reached its concurrency limit. Consider what happens with a concurrency value of one and multiple threads waiting in the 
    GetQueuedCompletionStatus function call. In this case, if the queue always has completion packets waiting, when the running thread calls 
    GetQueuedCompletionStatus, it will not block execution because, as mentioned earlier, the thread queue is LIFO. Instead, this thread will 
    immediately pick up the next queued completion packet. No thread context switches will occur, because the running thread is continually 
    picking up completion packets and the other threads are unable to run.

    The best overall maximum value to pick for the concurrency value is the number of CPUs on the computer. If your transaction required a 
    lengthy computation, a larger concurrency value will allow more threads to run. Each completion packet may take longer to finish, but 
    more completion packets will be processed at the same time. You can experiment with the concurrency value in conjunction with profiling 
    tools to achieve the best effect for your application.

    The system also allows a thread waiting in GetQueuedCompletionStatus to process a completion packet if another running thread associated 
    with the same I/O completion port enters a wait state for other reasons, for example the SuspendThread function. When the thread in the 
    wait state begins running again, there may be a brief period when the number of active threads exceeds the concurrency value. However, 
    the system quickly reduces this number by not allowing any new active threads until the number of active threads falls below the concurrency 
    value. This is one reason to have your application create more threads in its thread pool than the concurrency value. Thread pool management 
    is beyond the scope of this topic, but a good rule of thumb is to have a minimum of twice as many threads in the thread pool as there are 
    processors on the system. For additional information about thread pooling, see Thread Pools.

    Supported I/O Functions
    The following functions can be used to start I/O operations that complete by using I/O completion ports. You must pass the function an 
    instance of the OVERLAPPED structure and a file handle previously associated with an I/O completion port (by a call to CreateIoCompletionPort) 
    to enable the I/O completion port mechanism:
        AcceptEx
        ConnectNamedPipe
        DeviceIoControl
        LockFileEx
        ReadDirectoryChangesW
        ReadFile
        TransactNamedPipe
        WaitCommEvent
        WriteFile
        WSASendMsg
        WSASendTo
        WSASend
        WSARecvFrom
        LPFN_WSARECVMSG (WSARecvMsg)
        WSARecv
*/

/*
    HANDLE CreateIoCompletionPort(
      [in]           HANDLE    FileHandle,
      [in, optional] HANDLE    ExistingCompletionPort,
      [in]           ULONG_PTR CompletionKey,
      [in]           DWORD     NumberOfConcurrentThreads
    );

    Creates an input/output (I/O) completion port and associates it with a specified file handle, or creates an I/O completion port that is not yet associated with a file handle, allowing association at a later time.

    Associating an instance of an opened file handle with an I/O completion port allows a process to receive notification of the completion of asynchronous I/O operations involving that file handle.

    Parameters
    [in] FileHandle

    An open file handle or INVALID_HANDLE_VALUE.

    The handle must be to an object that supports overlapped I/O.

    If a handle is provided, it has to have been opened for overlapped I/O completion. For example, you must specify the FILE_FLAG_OVERLAPPED flag when using the CreateFile function to obtain the handle.

    If INVALID_HANDLE_VALUE is specified, the function creates an I/O completion port without associating it with a file handle. In this case, the ExistingCompletionPort parameter must be NULL and the CompletionKey parameter is ignored.

    [in, optional] ExistingCompletionPort

    A handle to an existing I/O completion port or NULL.

    If this parameter specifies an existing I/O completion port, the function associates it with the handle specified by the FileHandle parameter. The function returns the handle of the existing I/O completion port if successful; it does not create a new I/O completion port.

    If this parameter is NULL, the function creates a new I/O completion port and, if the FileHandle parameter is valid, associates it with the new I/O completion port. Otherwise no file handle association occurs. The function returns the handle to the new I/O completion port if successful.

    [in] CompletionKey

    The per-handle user-defined completion key that is included in every I/O completion packet for the specified file handle. For more information, see the Remarks section.

    [in] NumberOfConcurrentThreads

    The maximum number of threads that the operating system can allow to concurrently process I/O completion packets for the I/O completion port. This parameter is ignored if the ExistingCompletionPort parameter is not NULL.

    If this parameter is zero, the system allows as many concurrently running threads as there are processors in the system.

    Return value
    If the function succeeds, the return value is the handle to an I/O completion port:

    If the ExistingCompletionPort parameter was NULL, the return value is a new handle.
    If the ExistingCompletionPort parameter was a valid I/O completion port handle, the return value is that same handle.
    If the FileHandle parameter was a valid handle, that file handle is now associated with the returned I/O completion port.
    If the function fails, the return value is NULL. To get extended error information, call the GetLastError function.
    Remarks
    The I/O system can be instructed to send I/O completion notification packets to I/O completion ports, where they are queued. The CreateIoCompletionPort function provides this functionality.

    An I/O completion port and its handle are associated with the process that created it and is not sharable between processes. However, a single handle is sharable between threads in the same process.

    CreateIoCompletionPort can be used in three distinct modes:

    Create only an I/O completion port without associating it with a file handle.
    Associate an existing I/O completion port with a file handle.
    Perform both creation and association in a single call.
    To create an I/O completion port without associating it, set the FileHandle parameter to INVALID_HANDLE_VALUE, the ExistingCompletionPort parameter to NULL, and the CompletionKey parameter to zero (which is ignored in this case). Set the NumberOfConcurrentThreads parameter to the desired concurrency value for the new I/O completion port, or zero for the default (the number of processors in the system).
    The handle passed in the FileHandle parameter can be any handle that supports overlapped I/O. Most commonly, this is a handle opened by the CreateFile function using the FILE_FLAG_OVERLAPPED flag (for example, files, mail slots, and pipes). Objects created by other functions such as socket can also be associated with an I/O completion port. For an example using sockets, see AcceptEx. A handle can be associated with only one I/O completion port, and after the association is made, the handle remains associated with that I/O completion port until it is closed.

    For more information on I/O completion port theory, usage, and associated functions, see I/O Completion Ports.

    Multiple file handles can be associated with a single I/O completion port by calling CreateIoCompletionPort multiple times with the same I/O completion port handle in the ExistingCompletionPort parameter and a different file handle in the FileHandle parameter each time.

    Use the CompletionKey parameter to help your application track which I/O operations have completed. This value is not used by CreateIoCompletionPort for functional control; rather, it is attached to the file handle specified in the FileHandle parameter at the time of association with an I/O completion port. This completion key should be unique for each file handle, and it accompanies the file handle throughout the internal completion queuing process. It is returned in the GetQueuedCompletionStatus function call when a completion packet arrives. The CompletionKey parameter is also used by the PostQueuedCompletionStatus function to queue your own special-purpose completion packets.

    After an instance of an open handle is associated with an I/O completion port, it cannot be used in the ReadFileEx or WriteFileEx function because these functions have their own asynchronous I/O mechanisms.

    It is best not to share a file handle associated with an I/O completion port by using either handle inheritance or a call to the DuplicateHandle function. Operations performed with such duplicate handles generate completion notifications. Careful consideration is advised.

    The I/O completion port handle and every file handle associated with that particular I/O completion port are known as references to the I/O completion port. The I/O completion port is released when there are no more references to it. Therefore, all of these handles must be properly closed to release the I/O completion port and its associated system resources. After these conditions are satisfied, close the I/O completion port handle by calling the CloseHandle function.

    In Windows 8 and Windows Server 2012, this function is supported by the following technologies.
*/

_AFX afxError _AfxXpuStdWorkCallbackCb(afxIoBridge exu, afxStdWork* work)
{
    afxError err = { 0 };
    work->Callback.f(exu, work->Callback.udd);
    return err;
}

_AFX afxError _AfxXpuStdWorkExecuteCb(afxIoBridge exu, afxStdWork* work)
{
    afxError err = { 0 };

    for (afxUnit i = 0; i < work->Execute.cmdbCnt; i++)
    {
        afxStream iob = work->Execute.cmdbs[i];
        AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &iob);
        _XpuRollStreams(exu, 1, &iob);
    }
    return err;
}

_AFX afxStdWorkList _AfxStdXpuWorkCbVmt =
{
    .Callback = _AfxXpuStdWorkCallbackCb,
    .Execute = _AfxXpuStdWorkExecuteCb,
};

_AFX afxBool _AfxExuStdIoProcCb(afxIoBridge exu, afxThread thr)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_EXU, 1, &exu);
    AFX_ASSERT_OBJECTS(afxFcc_THR, 1, &thr);

    for (afxUnit queIdx = 0; queIdx < exu->queCnt; queIdx++)
    {
        afxIoQueue xque;
        if (!AfxGetIoQueues(exu, queIdx, 1, &xque) || !xque)
        {
            AfxThrowError();
            continue;
        }
        AFX_ASSERT_OBJECTS(afxFcc_XQUE, 1, &xque);

        if (AfxTryLockMutex(&xque->workChnMtx))
        {
            afxStdWork* work;
            AFX_ITERATE_CHAIN_B2F(work, hdr.chain, &xque->workChn)
            {
                AFX_ASSERT(xque->workChn.cnt);
                AfxGetTime(&work->hdr.pullTime);

                exu->workExecFn[work->hdr.id](exu, work);

                AfxGetTime(&work->hdr.complTime);
                _AfxXquePopWork(xque, work);
            }
            AfxSignalCondition(&xque->idleCnd);
            AfxUnlockMutex(&xque->workChnMtx);
        }
    }
    return TRUE;
}

_AFX afxInt _AfxExuStdIoThreadProc(afxIoBridge exu)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_EXU, 1, &exu);

    afxDevice ddev = AfxGetIoBridgeDevice(exu);
    afxDevLink ctx = AfxGetIoBridgeContext(exu);
    afxUnit portId = exu->portId;

    afxThread thr;
    AfxGetThread(&thr);
    AFX_ASSERT(thr == exu->worker);

    do
    {
        AfxLockMutex(&exu->schedCndMtx);

        while (!exu->schedCnt)
            AfxWaitCondition(&exu->schedCnd, &exu->schedCndMtx);

        exu->schedCnt = 0;
        AfxUnlockMutex(&exu->schedCndMtx);

        exu->procCb(exu, exu->worker);

        if (AfxShouldThreadBeInterrupted())
            break;

    } while (1);

    return 0;
}

_AFX afxError _AfxExuStdIoPingCb(afxIoBridge exu, afxUnit queIdx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_EXU, 1, &exu);
    exu->schedCnt += 1;
    AfxSignalCondition(&exu->schedCnd);
    return err;
}

_AFX afxError _AfxExuStdIoCtrlCb(afxIoBridge exu, afxInt code)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_EXU, 1, &exu);

    switch (code)
    {
    case 0: // start
    {
        if (!AfxIsThreadRunning(exu->worker))
        {
            AfxRunThread(exu->worker, (void*)exu->workerProc, exu);
        }
        break;
    }
    case 1: // pause/suspend
    {
        break;
    }
    case 2: // resume
    {
        break;
    }
    case 3: // stop
    {
        afxInt exitCode = 0;
        do
        {
            AfxRequestThreadInterruption(exu->worker);
            _AfxWaitForIdleIoBridge(exu, AFX_TIMEOUT_INFINITE);
        } while (!AfxWaitForThreadExit(exu->worker, &exitCode));

        break;
    }
    default: AfxThrowError(); break;
    }
    return err;
}
