#include "filemerge.h"

#include <iostream>
using std::cout;
using std::endl;

FileMerge::FileMerge()
{

}

FileMerge::~FileMerge()
{

}

void FileMerge::merge(string dest, string src1, string src2)
{
    string common;
    common.append("./ffmpeg -i ");
    common.append(src1);
    common.append(" -i ");
    common.append(src2);
    common.append(" ");
    common.append(dest);
    // system("./ffmpeg -i %s -i %s %s", src1.c_str(), src2.c_str(), dest.c_str());
    cout << common << endl;
    system(common.c_str());
}

void FileMerge::merge(string dest, string src, ...)
{

}