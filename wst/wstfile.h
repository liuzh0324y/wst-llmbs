#ifndef WST_FILE_H
#define WST_FILE_H

#include "config.h"

class FileMerge
{
public:
    FileMerge();
    ~FileMerge();

    void merge(string dest, string src1, string src2);
    void merge(string dest, string src, ...);
};

#endif // WST_FILE_H