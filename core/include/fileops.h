#ifndef FILEOPS_H
#define FILEOPS_H

#include "dispatch.h"

int fileLength(const std::string&);
bool fileDump(const std::string&, char*, int);

template<typename T>
void blobRead(T &val, char *buffer)
{
    val = *((T*)buffer);
    buffer += sizeof(T);
}

#endif
