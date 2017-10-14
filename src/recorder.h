#ifndef _AGORA_RECORDER_H_
#define _AGORA_RECORDER_H_

#include "config.h"

#include <csignal>
#include <cstdint>
#include <cstring>
#include <algorithm>

#include "base/atomic.h"
#include "base/log.h"

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
    ~Recorder();

    bool createChannel(const string &appid, const string &channelKey, const string &channelId, uid_t uid,
            bool decodeAudio, bool decodeVideo, agora::recording::RecordingConfig &config);

    int setVideoMixLayout();

    bool leaveChannel();
    bool release();

    bool stopped() const;

    int reportfiles();
    string channel();
    string storagepath();
    
private:
    virtual void onError(int error);
    virtual void onWarning(int warn);

    virtual void onJoinChannelSuccess(const char *channelId, uid_t uid);
    virtual void onLeaveChannel();

    virtual void onUserJoined(uid_t uid, agora::recording::UserJoinInfos &infos);
    virtual void onUserOffline(uid_t uid, agora::recording::USER_OFFLINE_REASON_TYPE reason);

    virtual const void audioFrameReceived(unsigned int uid, const agora::recording::AudioFrame *frame);
    virtual const void videoFrameReceived(unsigned int uid, const agora::recording::VideoFrame *frame);

    void readFileList(string uid, string baseDir);

private:
    atomic_bool_t _stopped;
    agora::recording::IRecordingEngine *_recorder;

    vector<agora::recording::uid_t> _peers;
    map<string, string> _userinfo;

    string _channel;
};

template <class T>
string convertToStdString(T value)
{
    stringstream ss;
    ss << value;
    return ss.str();
}

#endif // _AGORA_RECORDER_H_