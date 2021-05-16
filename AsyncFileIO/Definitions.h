#pragma once

namespace AsyncFileIO
{

/* Offset for read/write operations*/
typedef __int64 Offset;

const Offset OFFSET_NONE = -1;
const Offset OFFSET_END = LLONG_MAX;

enum class Status {
    /** Operation completed successfully. */
    OK,
    /** Request is currently pending for processing.       */
    PENDING,
    CANCELLATION_PENDING,
    CANCELING,
    PROCESSING,
    ABORT_PENDING,
    ABORTED,
    /** Request successfully processed. */
    COMPLETED,
    /** Request was canceled. */
    CANCELED,
    CLOSED,
    OTHER_FAILURE
};

struct IOStatus
{
    Status status { Status::OTHER_FAILURE };
    size_t transferedBytes { 0 };
    IOStatus() {}
    IOStatus(const Status status, const size_t transferedBytes)
        : status(status)
        , transferedBytes(transferedBytes)
    {
    }
};

}