#ifndef FILEOPS_H
#define FILEOPS_H

#include <string>
#include <fstream>
#include <iostream>

int fileLength(const std::string&);
bool fileDump(const std::string&, char*, int);

template<typename T>
void blobRead(T &val, char *buffer)
{
    val = *((T*)buffer);
    buffer += sizeof(T);
}

#endif
