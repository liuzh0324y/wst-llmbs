#ifndef WST_RECORD_H
#define WST_RECORD_H

#include "config.h"

#include <csignal>
#include <cstdint>
#include <cstring>
#include <algorithm>

#include <base/atomic.h>
#include <base/log.h>

#include "IAgoraRecordingEngine.h"

using std::stringstream;
using std::string;
using std::cout;
using std::cerr;
using std::endl;

using std::vector;
using std::map;


class Recorder : public agora::recording::IRecordingEngineEventHandler
{
public:
    Recorder();
    virtual ~Recorder();

    bool createChannel(const string &appid, const string &channelKey, const string &channelId, uid_t uid,
            bool decodeAudio, bool decodeVideo, agora::recording::RecordingConfig &config);

    int setVideoMixLayout();

    bool leaveChannel();
    bool release();

    bool stopped() const;
    bool quit() const;

    int reportfiles();
    string channel();
    string storagepath();

private:
    virtual void onError(int error, agora::linuxsdk::STAT_CODE_TYPE stat_code);
    virtual void onWarning(int warn);

    virtual void onJoinChannelSuccess(const char *channelId, uid_t uid);
    virtual void onLeaveChannel(agora::linuxsdk::LEAVE_PATH_CODE code);

    virtual void onUserJoined(uid_t uid, agora::linuxsdk::UserJoinInfos &infos);
    virtual void onUserOffline(uid_t uid, agora::linuxsdk::USER_OFFLINE_REASON_TYPE reason);

    virtual void audioFrameReceived(unsigned int uid, const agora::linuxsdk::AudioFrame *frame) const;
    virtual void videoFrameReceived(unsigned int uid, const agora::linuxsdk::VideoFrame *frame) const;

    void readFileList(string uid, string baseDir);

private:
    atomic_bool_t _stopped;
    agora::recording::IRecordingEngine *_recorder;

    vector<agora::linuxsdk::uid_t> _peers;
    map<string, string> _userinfo;

    string _channel;
    string _username;
    bool   _IsQuit;
};

template <class T>
string convertToStdString(T value)
{
    stringstream ss;
    ss << value;
    return ss.str();
}

#endif // WST_RECORD_H