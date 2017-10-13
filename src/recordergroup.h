#ifndef _RECORDER_GROUP_H_
#define _RECORDER_GROUP_H_

#include "config.h"
#include <iostream>
#include <string>
#include <map>
#include <mutex>
#include <vector>

using std::string;
using std::map;
using std::mutex;
using std::cout;
using std::endl;
using std::cerr;
using std::vector;

class Recorder;

typedef map<string,Recorder*> RecorderMap;
class RecorderGroup 
{
public:
    RecorderGroup();
    RecorderGroup(string &appId);

    ~RecorderGroup();

    int  start(const string &appId, const string &channelId);
    int  stop(const string &appId, const string &channelId);
    // Time delay mode
    int  stop(const string &channel);

    int mixmedia(vector<FileInfo> files);
    int deletefile(vector<FileInfo> files);
    int reportfiles(const string basepath);
    int reportfiles(FileInfo files);
    int reportstatus(int code, string description);
    
    int addRecordInfoQueue(const string channel);

protected:
    void mixmedia_worker(vector<FileInfo> files);
    void reportfile_worker(string channel, string dir);
    void readFileList(string channel, string baseDir);
private:
    // recorder objs
    RecorderMap _recorderMap;
    // recorder mutex
    mutex _mutex_id;
    // appid
    string _appid;
};

#endif // _RECORDER_GROUP_H_