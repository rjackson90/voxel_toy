#ifndef FILEOPS_H
#define FILEOPS_H

#include <string>
#include <fstream>
#include <iostream>

namespace Core
{
    int getLength(const std::string&);
    bool getBlob(const std::string&, char*, int);
    std::string getText(const std::string&);

    template<typename T>
    void blobRead(T &val, char *&buffer)
    {    
        val = *((T*)buffer);
        buffer += sizeof(T);
    }
}
#endif
