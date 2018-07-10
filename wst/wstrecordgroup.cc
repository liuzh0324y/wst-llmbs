#include "wstrecordgroup.h"
#include "wstconf.h"
#include "wstrecord.h"
#include "wstclient.h"
#include "wstlog.h"

#include <iomanip>
#include <chrono>

RecorderGroup::RecorderGroup()
{
    std::thread worker(&RecorderGroup::WorkerTime, this);
    worker.detach();
}

RecorderGroup::RecorderGroup(string &appId)
{
    _appid = appId;
    std::thread worker(&RecorderGroup::WorkerTime, this);
    worker.detach();
}

RecorderGroup::~RecorderGroup()
{

}

int  RecorderGroup::start(const string &appId, const string &channelId, const string &username, const string &mixResolution)
{
    string channelKey;
    uint32_t uid = 0;
    uint32_t channelProfile=1;
    
    string decryptionMode;
    string secret;

    int idleLimitSec=WstConf::Instance().idleLimitSec();
    string applitePath=WstConf::Instance().applitepath();
    string appliteLogPath=WstConf::Instance().logspath();
    string recordFileRootDir=WstConf::Instance().recordpath();

    int lowUdpPort = 0;
    int highUdpPort = 0;

    bool isAudioOnly = false;
    bool isMixingEnabled= true;

    RecorderMap::iterator iter = _recorderMap.find(channelId);
    if (iter != _recorderMap.end())
    {
        LOGW("app id already existed!");
        return CHANNELSTARTED; // already 
    }
    if (_recorderMap.size() >= WstHttpClient::Instance().MaxChannel())
    {
        return RECORDCHANNELFULL; // channel full
    }

    LOG(INFO, "uid %" PRIu32 " from vendor %s is joining channel %s",
        uid, appId.c_str(), channelId.c_str());

    _appid = appId;
    Recorder *recorder = new Recorder;
    agora::recording::RecordingConfig config;
    config.idleLimitSec = idleLimitSec;
    config.channelProfile = static_cast<agora::linuxsdk::CHANNEL_PROFILE_TYPE>(channelProfile);

    config.isAudioOnly = isAudioOnly;
    config.isMixingEnabled = isMixingEnabled;
    config.mixResolution = (isMixingEnabled && !isAudioOnly)? const_cast<char*>(mixResolution.c_str()):NULL;;

    config.appliteDir = const_cast<char*>(applitePath.c_str());
    config.recordFileRootDir = const_cast<char*>(recordFileRootDir.c_str());

    config.secret = secret.empty() ? NULL : const_cast<char*>(secret.c_str());
    config.decryptionMode = decryptionMode.empty() ? NULL : const_cast<char*>(decryptionMode.c_str());

    config.lowUdpPort = lowUdpPort;
    config.highUdpPort = highUdpPort;

    config.decodeAudio = agora::linuxsdk::AUDIO_FORMAT_DEFAULT_TYPE; 
    config.decodeVideo = agora::linuxsdk::VIDEO_FORMAT_DEFAULT_TYPE;

    if (!recorder->createChannel(appId, channelKey, channelId, uid, false, false, config))
    {
        LOGW("create agora channel failed!");
        return 303;
    }
    string user = username;
    if (user.empty()){
        user = "unknown";
    }
    // if (user.size() > 27) {
    //     user = user.substr(0, 26);
    // }
    string userinfo;
    userinfo = gettimstamp();
    userinfo.append("_");
    userinfo.append(user);
    _mutex_id.lock();
    _usernameMap.insert(make_pair(channelId, userinfo));
    _recorderMap.insert(make_pair(channelId, recorder));
    _mutex_id.unlock();
    LOGW("create channel succeed!");
    return 0;
}

int RecorderGroup::stop(const string &appId, const string &channelId)
{
    _mutex_id.lock();
    RecorderMap::iterator iter = _recorderMap.find(channelId);
    if (iter != _recorderMap.end())
    {
        if (iter->second->leaveChannel())
        {
            iter->second->release();
            delete iter->second;
            _recorderMap.erase(iter);
            _mutex_id.unlock();
            LOGW("leave channel succeed!");
            return 0;
        }
        else
        {
            LOGW("leave channel failed!");
            _mutex_id.unlock();
            return FREECHANNELFAILED;
        }
    }
    _mutex_id.unlock();
    LOGW("app id not found!");
    return CHANNELNOTFOUND;
}

int  RecorderGroup::stop(const string &channel)
{
    _mutex_id.lock();
    RecorderMap::iterator iter = _recorderMap.find(channel);
    if (iter != _recorderMap.end())
    {
        // std::thread worker(&RecorderGroup::stoprecord_worker, this, channel);
        // worker.detach();
        if (iter->second->leaveChannel())
        {
            iter->second->release();
            std::thread worker(&RecorderGroup::reportfile_worker, this, iter->second->channel(), iter->second->storagepath());
            worker.detach();
            delete iter->second;
            _recorderMap.erase(iter);
            _mutex_id.unlock();
            LOGW("leave channel succeed!");
            return 0;
        }
        else
        {
            LOGW("leave channel failed!");
            _mutex_id.unlock();
            return FREECHANNELFAILED;
        }
    }
    _mutex_id.unlock();
    return CHANNELNOTFOUND;
}

void RecorderGroup::readFileList(string channel, string baseDir)
{
	DIR *dir;
	struct dirent *ptr;
	string base;
	vector<FileInfo> filelist;

	if ((dir = opendir(baseDir.c_str())) == NULL)
	{
		return;
	}

	while ((ptr = readdir(dir)) != NULL)
	{
		if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)
		{
			continue;
		}
		else if (ptr->d_type == 8 || ptr->d_type == 10)
		{
			FileInfo info;
			info.name = ptr->d_name;
			info.path = baseDir;
			if (string::npos != info.name.find(".aac")) info.type = "aac";
			if (string::npos != info.name.find(".webm")) info.type = "webm";
			if (string::npos != info.name.find(".mp4")) info.type = "mp4";
			if (string::npos != info.name.find(".txt")) info.type = "txt";

			if (((string::npos != info.name.find("0_"))) && (string::npos != info.name.find(".aac") || string::npos != info.name.find(".webm") 
				|| string::npos != info.name.find(".mp4") || string::npos != info.name.find(".txt")))
			{
				filelist.push_back(info);
			}
		}
	}
	
	if (filelist.size() > 1)
	{
		// find txt
		string txtfile;
		vector<FileInfo> outfiles;
		vector<FileInfo>::iterator iter = filelist.begin();
		while (iter != filelist.end())
		{
			if ((*iter).type.compare("txt") == 0)
			{
				txtfile = (*iter).name;
			}
			iter++;
		}
		for (iter = filelist.begin(); iter != filelist.end(); iter++)
		{
			if ((*iter).type.compare("txt") == 0) continue;
			FileInfo iterinfo;
			iterinfo = (*iter);
			iterinfo.meta = txtfile;
			iterinfo.channel = channel;
			outfiles.push_back(iterinfo);
		}
		WstHttpClient::Instance().ReportFile(outfiles);
	}

	closedir(dir);
}

void RecorderGroup::reportfile_worker(string channel, string dir)
{
    readFileList(channel, dir);
}

void RecorderGroup::mixmedia_worker(vector<FileInfo> files)
{
    string channel;
    vector<FileInfo>::iterator iter = files.begin();
    if (files.size() != 2)
    {
        return ;
    }

    for (vector<FileInfo>::iterator it = files.begin(); it != files.end(); it++)
    {
        // check support type
        if ( ((*it).type.compare("mp4")!=0) &&
             ((*it).type.compare("aac")!=0) &&
             ((*it).type.compare("webm")!=0) )
        {
            LOGW("not found type.");
            return;
        }

        // check file
        string pathname = (*it).path+"/"+(*it).name;
        channel = (*it).channel;
        if (::access(pathname.c_str(), F_OK) == -1)
        {
            LOGW("not found file.");
            return;
        }
    }

    map<string, string>::iterator itername = _usernameMap.find(channel);
    if (itername == _usernameMap.end())
    {
        LOGW("not found username");
        return;
    }
    // python convert.py inpath outfile metadatafile
    // string mixfile = (*iter).path;
    // mixfile.append("/");
    // mixfile.append((*iter).name);
    // (*iter).name;
        // int pos = (*iter).name.find(".");
        // string tmpstr = (*iter).name.substr(0, pos);
        // string outfile = tmpstr + ".mp4";
    // change mp4 file name.
    string outfile = (*itername).second + ".mp4";
    string out = WstConf::Instance().recordpath() + "/" + outfile;
    string mixpath = "python /opt/llmbs/tools/convert.py ";
    mixpath.append((*iter).path);   // python convert.py inpath
    mixpath.append(" ");
    mixpath.append(out);            // python convert.py inpath outfile
    mixpath.append(" ");            
    mixpath.append((*iter).meta);   // python convert.py inpath outfile metadatafile
    system(mixpath.c_str());
    if (::access(out.c_str(), F_OK) == -1)
    {
        LOGW("merge failed.");
        // httpclient::Instance().reportstatus();
        return;
    }
    // cout << "mixmedia: "<< mixfile << endl;

    // picture
    string picfile = WstConf::Instance().recordpath()+"/"+ (*itername).second + ".jpeg";
    string command_pic = "/opt/llmbs/tools/ffmpeg -i ";
    command_pic.append(out);
    command_pic.append(" -y -f image2 -ss 1 -t 0.001 -s 800x600 ");
    command_pic.append(picfile);
    system(command_pic.c_str());
    if (::access(picfile.c_str(), F_OK) == -1)
    {
        LOGW("get pic failed.");
    }
    FileInfo outinfo;
    outinfo.name = outfile;
    outinfo.pic = (*itername).second +".jpeg";
    outinfo.type = "mp4";
    outinfo.path = WstConf::Instance().recordpath();
    outinfo.tmppath = (*iter).path;
    outinfo.channel = channel;

    LOGW("MIX FILES REPORT FILE.");
    // WstHttpClient::Instance().ReportFile(outinfo);
    WstHttpClient::Instance().ReportFileToGoServer(outinfo);
    // reportfiles(outinfo);
    // for (iter; iter != files.end(); iter++)
    // {
    //     string mixfile = (*iter).path;
    //     mixfile.append("/");
    //     mixfile.append((*iter).name);
    //     string mixpath = "python video_convert.py ";
    //     mixpath.append((*iter).path);
    //     system(mixpath.c_str());
    //     cout << "mixmedia: "<< mixfile << endl;
    // }
    _usernameMap.erase(itername);
}

int RecorderGroup::mixmedia(vector<FileInfo> files)
{
    // access file
    vector<FileInfo>::iterator iter = files.begin();
    for (iter; iter != files.end(); iter++)
    {
        string pathname = (*iter).path;
        pathname.append("/");
        pathname.append((*iter).name);
        if (::access(pathname.c_str(), F_OK) == -1)
        {
            return FILENOTFOUND;
        }
    }

    std::thread worker(&RecorderGroup::mixmedia_worker, this, files);
    worker.detach();
    return 0;
}

int RecorderGroup::deletefile(vector<FileInfo> files)
{
    vector<FileInfo>::iterator iter = files.begin();
    for (iter; iter != files.end(); iter++)
    {
        string delfile = (*iter).path;
        delfile.append("/");
        delfile.append((*iter).name);
        if (::access(delfile.c_str(), F_OK) == -1)
        {
            LOGW("not found file.");
            return FILENOTFOUND;
        }
        ::remove(delfile.c_str());
        string out = "delete file: ";
        out.append(delfile);
        LOGW(out);
    }
    return 0;
}

int RecorderGroup::reportfiles(const string basepath)
{
	DIR *dir;
	struct dirent *ptr;
	string base;
	vector<FileInfo> filelist;
	cout << "read file basepath: " << basepath << endl;

	if ((dir = opendir(basepath.c_str())) == NULL)
	{
		cout << "open dir error .. " << endl;
		return 0;
	}

	while ((ptr = readdir(dir)) != NULL)
	{
		if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)
		{
			continue;
		}
		else if (ptr->d_type == 8 || ptr->d_type == 10)
		{
			FileInfo info;
			info.name = ptr->d_name;
			info.path = basepath;
			if (string::npos != info.name.find(".aac")) info.type = "aac";
			if (string::npos != info.name.find(".webm")) info.type = "webm";
			if (string::npos != info.name.find(".mp4")) info.type = "mp4";

			if ((string::npos != info.name.find("av")) && string::npos != info.name.find(".mp4"))
			{
				filelist.push_back(info);
				cout << "found file: " << info.name << ", type: " << info.type << ", path: " << info.path << endl;
			}
		}
	}
	
	if (filelist.size() > 0)
	{
		cout << "file size: " << filelist.size() << endl;
		WstHttpClient::Instance().ReportFile(filelist);
	}

    closedir(dir);
    
    return 0;
}

int RecorderGroup::reportfiles(FileInfo files)
{
    return 0;
}

int RecorderGroup::reportstatus(int code, string description)
{

    // httpclient::Instance()->reportstatus();
    return 0;
}

void RecorderGroup::InitTimer()
{
    struct sigaction act;
    struct itimerval val;

    act.sa_handler = fCheckRecorder;
    act.sa_flags = 0;

    sigemptyset(&act.sa_mask);
    sigaction(SIGPROF, &act, NULL);

    val.it_value.tv_sec = 1;
    val.it_value.tv_usec = 0;

    val.it_interval = val.it_value;

    setitimer(ITIMER_PROF, &val, NULL);
}

string RecorderGroup::gettimstamp()
{
    using std::chrono::system_clock;
    std::time_t tt = system_clock::to_time_t(system_clock::now());

    struct std::tm *ptm = std::localtime(&tt);
    std::stringstream ss;
    ss << std::put_time(ptm, "%Y%m%d%H%M%S");
    return ss.str();
}

void RecorderGroup::fCheckRecorder(int i)
{
    
}

void RecorderGroup::WorkerTime()
{
    while (1)
    {
        sleep(60);
        _mutex_id.lock();

        for (auto iter = _recorderMap.begin(); iter != _recorderMap.end(); iter++)
        {
            if (iter->second->quit())
            {
                delete iter->second;
                _recorderMap.erase(iter);
            }
        }
        // LOGW("Check recorder map.");
        _mutex_id.unlock();
    }
}