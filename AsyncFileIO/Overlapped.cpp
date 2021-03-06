#include "windows.h"

#include "Utils.h"
#include "Overlapped.h"
#include "spdlog/spdlog.h"

namespace AsyncFileIO
{

Overlapped::Overlapped(const std::weak_ptr<void> handle, const int64_t fileOffset)
    : fileHandle(handle)
{
    SPDLOG_TRACE("Overlapped");
    std::memset(static_cast<OVERLAPPED *>(this), 0, sizeof(OVERLAPPED));

    Offset = static_cast<DWORD>(fileOffset);
    OffsetHigh = static_cast<DWORD>(fileOffset >> 32);
}

Overlapped::~Overlapped()
{
    SPDLOG_TRACE("~Overlapped");
    fileHandle.reset();
}

VOID CALLBACK Overlapped::callback(const DWORD errorCode, const DWORD numberOfBytesTransferred, const LPOVERLAPPED lpOverlapped)
{
    std::unique_ptr<Overlapped> overlapped(static_cast<Overlapped*>(lpOverlapped));

    const auto handle = overlapped->fileHandle.lock();
    SPDLOG_TRACE("Handle {} and overlapped operation completed with result  {} and bytes transferred {}",handle, 
        getErrorDescription(dwErrorCode), numberOfBytesTransferred);

    overlapped->status.set_value({ MapError(errorCode),numberOfBytesTransferred });
}

}