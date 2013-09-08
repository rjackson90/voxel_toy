#include "fileops.h"

int getLength(const std::string &path)
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

bool getBlob(const std::string &path, char *buffer, int buflen)
{
    using namespace std;

    int file_len = getLength(path);
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

std::string getText(const std::string &path)
{
    using namespace std;

    ifstream file(path);
    if(file.good())
    {
        // The seemingly-redundant parentheses below are REQUIRED. DO NOT REMOVE
        string text((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
        return text;
    }

    cout << "Failed to open file " << path << endl;
    return string();
}
