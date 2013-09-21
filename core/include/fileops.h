#ifndef FILEOPS_H
#define FILEOPS_H

#include <string>
#include <fstream>
#include <iostream>

int getLength(const std::string&);
bool getBlob(const std::string&, char*, int);
std::string getText(const std::string&);

template<typename T>
void blobRead(T &val, char *&buffer)
{
    // Uncomment next line for debugging
    //char* start_addr = buffer;
    
    val = *((T*)buffer);
    buffer += sizeof(T);

    // Uncomment next two lines for debugging
    //std::cout << std::hex << "BlobRead: (" << (int*)start_addr << ":" << (int*)buffer << ") = " <<
    //    (int)val << std::dec << std::endl;
}

#endif
