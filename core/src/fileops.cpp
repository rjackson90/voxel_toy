#include "fileops.h"

int fileLength(const std::string &path)
{
    using namespace std;
    ifstream file(path, ifstream::binary | ifstream::ate);
    if(!file.good())
    {
        cout << "Failed to open file " << path << endl;
        file.close();
        return -1;
    }
    int length = file.tellg();
    file.close();
    return length;
}

bool fileDump(const std::string &path, char *buffer, int buflen)
{
    using namespace std;

    int file_len = fileLength(path);
    if(file_len < 0)return false;
    if(buflen < file_len)
    {
        cout << "Passed buffer of length " << buflen << " is of insufficient size. "
             << path << " has size " << file_len << endl;
        return false;
    }
    
    ifstream file(path, ifstream::binary);
    if(file.good())
    {
        file.read(buffer, file_len);
        file.close();
        return true;
    }
    else
    {
        cout << "Failed to read file: bad state detected." << endl;
        file.close();
        return false;
    }
}
