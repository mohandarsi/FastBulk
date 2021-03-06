#pragma once
#include<string>

namespace AsyncFileIO
{

class FileMode
{
public:
    /** Read access. */
    bool read : 1,

        /** Write access.*/
        write : 1,

        /** Append.*/
        append : 1,

        /** Extended access (corresponds to "+" in opening mode string.)*/
        extended : 1;

    /*Parses the mode */
    explicit FileMode(const std::string& mode);
    ~FileMode() = default;
};

}

