#include "wstrecord.h"
#include "wstfile.h"
#include "wstclient.h"
#include "wstconf.h"
#include "wstlog.h"

#include <dirent.h>


Recorder::Recorder() : IRecordingEngineEventHandler()
{
	_recorder = NULL;
	_IsQuit = false;
    _stopped.store(false);
}

Recorder::~Recorder()
{
    if (_recorder)
    {
        _recorder->release();
        _recorder = NULL;
    }
}

bool Recorder::stopped() const 
{
    return _stopped;
}

bool Recorder::quit() const
{
	return _IsQuit;
}

bool Recorder::release()
{
    if (_recorder)
    {
		_recorder->release();
		_recorder = NULL;
    }

    return true;
}

bool Recorder::createChannel(const string &appid, const string &channelKey, const string &channelId,
	uint32_t uid, bool decodeAudio, bool decodeVideo, agora::recording::RecordingConfig &config)
{
    if ((_recorder = agora::recording::IRecordingEngine::createAgoraRecordingEngine(appid.c_str(), this)) == NULL)
    {
		LOGW("agora::recording::IRecordingEngine::createAgoraRecordingEngine failed!");
		return false;
    }

	_channel = channelId;
    return (0 == _recorder->joinChannel(channelKey.c_str(), channelId.c_str(), uid, config));
}

bool Recorder::leaveChannel()
{
	if (!_recorder)
	{
		return false;
	}

	if (_recorder->leaveChannel() == 0)
	{
		_stopped = true;
		return true;
	}
	

    return false;
}

int Recorder::setVideoMixLayout()
{
    LOG(INFO, "setVideoMixLayout: user size: %d", _peers.size());

	agora::linuxsdk::VideoMixingLayout layout;
	layout.canvasWidth = 1280;
	layout.canvasHeight = 720;
	layout.backgroundColor = "#23b9dc";

	layout.regionCount = _peers.size();

	if (!_peers.empty())
	{
		LOG(INFO, "setVideoMixLayout: peers not empty");
		agora::linuxsdk::VideoMixingLayout::Region *regionList = new agora::linuxsdk::VideoMixingLayout::Region[_peers.size()];
		
		regionList[0].uid = _peers[0];
		regionList[0].x = 0.f;		
		regionList[0].y = 0.f;
		regionList[0].width = 1280.0;
		regionList[0].height = 720.0;
		regionList[0].zOrder = 0;
		regionList[0].alpha = 1.f;
		regionList[0].renderMode = 2;

		LOG(INFO, "region 0 uid: %d, x: %f, y: %f, width: %f, height: %f, zOrder: %d, alpha: %f",
			regionList[0].uid, regionList[0].x, regionList[0].y, 
			regionList[0].width, regionList[0].height, regionList[0].zOrder,
			regionList[0].alpha);

		float canvasWidth = 1280.0;
		float canvasHeight = 720.0;

		float viewVEdge = 0.3;
		float viewHEdge = 0.025;
		float viewHeight = viewVEdge * (canvasWidth / canvasHeight);
		float viewWidth = viewHEdge * (canvasWidth / canvasHeight);

		// for (int i = 1; i < _peers.size(); i++)
		// {
		// 	if (i >= 7) break;
		// 	regionList[i].uid = _peers[i];

		// 	float xIndex = i % 3;
		// 	float yIndex = i / 3;
		// 	regionList[i].x = xIndex * (viewWidth + viewHEdge) + viewHEdge;
		// 	regionList[i].y = 1 - (yIndex + 1) * (viewHeight + viewVEdge);
		// 	regionList[i].width = viewWidth;
		// 	regionList[i].height = viewHeight;
		// 	regionList[i].zOrder = 0;
		// 	regionList[i].alpha = i + 1;
		// 	regionList[i].renderMode = 0;
		// }

		// regionList[1].x = 0;
		// regionList[1].y = 0;
		// regionList[1].width = canvasWidth;
		// regionList[1].height = canvasHeight;
		// regionList[1].zOrder = 0;
		// regionList[1].alpha = 1;
		// regionList[1].renderMode = 0;
		layout.regions = regionList;
	}
	else
	{
		layout.regions = NULL;
    }

	_recorder->setVideoMixingLayout(layout);

	return 0;
}

void Recorder::onError(int error, agora::linuxsdk::STAT_CODE_TYPE stat_code)
{
	string out = "AgoraRecorder::Error: ";
	out.append(T_as_string(error));
	LOGW(out);

	switch (error)
	{
	case 1:
		{
			httpclient::Instance().reportstatus(AGORAFAILED, "agora filed");
		}
		break;
	case 2:
		{
			httpclient::Instance().reportstatus(AGORAINVAILDAGUMENT, "agora invaild agument");
		}
		break;
	case 3:
		{
			httpclient::Instance().reportstatus(AGORAINTERNALFAILED, "agora internal failed");
		}
		break;
	default:
		break;
	}
	_IsQuit = true;
	leaveChannel();
}

void Recorder::onWarning(int warn)
{
	string out = "AgoraRecorder::Warn: ";
	out.append(T_as_string(warn));
	LOGW(out);
	//leaveChannel();
}

void Recorder::onJoinChannelSuccess(const char *channelId, uid_t uid)
{
	string out = "join channel Id: ";
	out.append(channelId);
	out.append(", with uid: ");
	out.append(T_as_string(uid));
	LOGW(out);
}

void Recorder::onLeaveChannel(agora::linuxsdk::LEAVE_PATH_CODE code)
{
	LOGW("leave channel");
}

void Recorder::onUserJoined(unsigned uid, agora::linuxsdk::UserJoinInfos &infos)
{
	// cout << "User " << uid << " joined, RecordingDir:" << (infos.recordingDir ? infos.recordingDir : "NULL") << endl;
	string out = "User ";
	out.append(T_as_string(uid));
	out.append(" joined, storageDir:");
	out.append(infos.storageDir ? infos.storageDir : "NULL");
	LOGW(out);
	_peers.push_back(uid);
	_userinfo.insert(make_pair(convertToStdString(uid),infos.storageDir));
	setVideoMixLayout();
}

void Recorder::onUserOffline(unsigned uid, agora::linuxsdk::USER_OFFLINE_REASON_TYPE reason)
{
	// When the user is offline after reporting file information.
	string out = "User ";
	out.append(T_as_string(uid));
	out.append(" offline, reason: ");
	out.append(T_as_string(reason));
	LOGW(out);

	_peers.erase(remove(_peers.begin(), _peers.end(), uid), _peers.end());
	// map<string,string>::iterator iter = _userinfo.find(convertToStdString(uid));
	// if (iter != _userinfo.end())
	// {
	// 	readFileList(iter->first, iter->second);
	// 	_userinfo.erase(iter);
	// }
	
	setVideoMixLayout();
}

void Recorder::audioFrameReceived(unsigned int uid, const agora::linuxsdk::AudioFrame *frame) const
{

}

void Recorder::videoFrameReceived(unsigned int uid, const agora::linuxsdk::VideoFrame *frame) const
{

}

string Recorder::channel()
{
	return _channel;
}

string Recorder::storagepath()
{
	if (!_userinfo.size())
	{
		LOGW("no user joined.");
		return string(); // 沒有用戶加入
	}

	map<string, string>::iterator iter = _userinfo.begin();
	string dir = iter->second;
	LOGW(iter->second);
	return dir;
}
int Recorder::reportfiles()
{
	if (!_userinfo.size())
	{
		LOGW("no user joined.");
		return -1; // 沒有用戶加入
	}

	map<string, string>::iterator iter = _userinfo.begin();
	string dir = iter->second;
	LOGW(iter->second);
	readFileList(iter->first, iter->second);
}

// read local file information.
void Recorder::readFileList(string uid, string baseDir)
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

			if (((string::npos != info.name.find(uid)) || (string::npos != info.name.find("0_"))) && (string::npos != info.name.find(".aac") || string::npos != info.name.find(".webm") 
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
			iterinfo.channel = _channel;
			outfiles.push_back(iterinfo);
		}
		httpclient::Instance().reportfile(outfiles);
	}

	closedir(dir);
}






















