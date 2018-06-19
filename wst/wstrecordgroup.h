#ifndef WST_RECORDGROUP_H
#define WST_RECORDGROUP_H

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

    int  start(const string &appId, const string &channelId, const string &username, const string &mixresolution);
    int  bstart(const string &appId, const string &channelId, const string &username, const string &mixresolution);
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
    void WorkerTime();
    void mixmedia_worker(vector<FileInfo> files);
    void reportfile_worker(string channel, string dir);
    void readFileList(string channel, string baseDir);
    static void fCheckRecorder(int i);

private:
    void InitTimer();
    string gettimstamp();
private:
    // recorder objs
    RecorderMap _recorderMap;
    // recorder mutex
    mutex _mutex_id;
    // appid
    string _appid;
    map<string,string> _usernameMap;
};

#endif // WST_RECORDGROUP_H