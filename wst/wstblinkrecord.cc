#include "wstblinkrecord.h"
#include "wstdownloader.h"
#include "wstlog.h"
#include <thread>
#include <chrono>

WstBlinkRecord::WstBlinkRecord() 
{
    _appid = "1234567890abcdefg";
    _uid = "downloaduser";
    _isQuit = false;
}

WstBlinkRecord::~WstBlinkRecord() 
{
    
}

bool WstBlinkRecord::Initialize()
{
    // check dir
    DIR *record, *tmp;
    std::string recordpath = WstConf::Instance().recordpath();
    std::string tmppath = recordpath + "/tmp";
    std::string toolpath = WstConf::Instance().GetBlinkApp();
    record = opendir(recordpath.c_str());
    if (record == NULL) {
        LOGW("record is null.");
        return false;
    }
    tmp = opendir(tmppath.c_str());
    if (tmp == NULL) {
        LOGW("record/tmp is null.");
        return false;
    }
    if (::access(toolpath.c_str(), F_OK) == -1)
    {
        std::stringstream tempstr;
        tempstr << "not found " << toolpath;
        LOGW(tempstr.str());
        return false;
    }
    return true;
}

bool WstBlinkRecord::Start()
{
    std::thread worker(&WstBlinkRecord::Worker, this);
    std::thread downloader(&WstBlinkRecord::Downloader, this);
    worker.join();
    downloader.join();
    return true;
}

bool WstBlinkRecord::Stop()
{

}

void WstBlinkRecord::Destroy()
{
    
}

void WstBlinkRecord::Worker() 
{
    // while (!_isQuit) {
    //     WstBlinkHandler handler;
    //     handler.run();
    //     std::this_thread::sleep_for(std::chrono::seconds(2));
    // }
    
    WstHttpClient::Instance().GetBToken();
    while (!_isQuit) 
    {
        WstHttpClient::Instance().GetDownloader();
        std::this_thread::sleep_for(std::chrono::seconds(60*5));
    }
}

void WstBlinkRecord::Downloader() {

    while (!_isQuit) {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        if (WstHttpClient::Instance()._wstqueue.size() > 0) {
            WstValue value;
            value = WstHttpClient::Instance()._wstqueue.front();
            WstHttpClient::Instance()._wstqueue.pop();
            Handler(value);
        }
        
    }
}

void WstBlinkRecord::Handler(WstValue& value) {
    // if (!ql) {
    //     std::cout << "downloader url: " << value.url << std::endl;
    //     _downloader.run(value.cid, value.url, value.timestamp);
    //     ql = true;
    // }
    std::string outname;
    outname = value.timestamp;
    outname.append("_");
    outname.append(value.cid);
    outname.append(".flv");
    std::string outnamepath;
    outnamepath = WstConf::Instance().recordpath();
    outnamepath.append("/");
    outnamepath.append(outname);

    int pos = value.url.find_last_of("/");
    std::string sname = value.url.substr(pos+1);
    std::cout << "source name: " << sname << std::endl;
    std::string spath = WstConf::Instance().recordpath();
    std::string command;
    command = "wget ";
    command.append(" -P ");
    command.append(spath + "/tmp");
    command.append(" ");
    command.append(value.url);
    system(command.c_str());
    LOGW("wget: " + command);

    std::string convert;
    convert = WstConf::Instance().GetBlinkApp();
    convert.append(" ");
    convert.append(spath + "/tmp/" + sname);
    convert.append(" ");
    convert.append(outnamepath);
    system(convert.c_str());
    LOGW("convert: " + convert);

    FileInfo info;
    info.name = outname;
    info.path = WstConf::Instance().recordpath();
    info.type = "flv";

    std::vector<FileInfo> fileinfo;
    fileinfo.push_back(info);
    WstHttpClient::Instance().ReportFile(fileinfo);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    WstHttpClient::Instance().ReportFile(info);
}