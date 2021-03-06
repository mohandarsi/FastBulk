#pragma once

#include "IFileStream.h"

namespace spdlog {
    class logger;
}

namespace AsyncFileIO
{


	/** Stream class which represents opened file. */
class FileHandle;
class FileMode;
class IOBuffer;

class FileStream : public IFileStream
{
public:
    FileStream(spdlog::logger& log, const std::string& path, const FileMode& mode);
    virtual ~FileStream();

    void setName(const std::string&);

    std::string  getName() const;

    std::future<IOStatus>
    write(const IOBuffer& buffer, Offset offset = CURRENT_FILE_OFFSET) override;
    
    std::future<IOStatus>
    read(IOBuffer& buffer, Offset offset = CURRENT_FILE_OFFSET) override;
    
    Offset seek(Offset position, RelativePosition relativePosition = RelativePosition::BEGIN) override;
    
    void setSize(size_t length) override;

private:

    spdlog::logger& m_logger;

    friend class FileProcessor;

    std::string m_szName;

    Offset m_currentPos;

    std::unique_ptr<FileHandle> m_ptrFileHandle;

};

}
