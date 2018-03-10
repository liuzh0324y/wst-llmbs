#ifndef _FILE_MERGE_H_
#define _FILE_MERGE_H_

#include "config.h"

class FileMerge
{
public:
    FileMerge();
    ~FileMerge();

    void merge(string dest, string src1, string src2);
    void merge(string dest, string src, ...);
};

#endif // _FILE_MERGE_H_