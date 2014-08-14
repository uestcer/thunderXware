#ifndef XWARE_TYPE_H
#define XWARE_TYPE_H
#include<QNetworkCookie>
#include<map>
#include <string>
#include <list>
using namespace  std;
struct User
{
    QString userName;
    QString rawPassword;
    QString password;//经加密后的密码
    QString verifyKey; //验证码
    QString check_result;//从chcek GET请求中获得的验证吗，不是我们想要的！
    bool isLogin;//登录状态，true表示登录，false表示没登录

};

struct PeerList
{
    QString pid; // "1C776A359DCE217X0001"
    QString category;
    QString status;
    QString name;
    quint32 vodPort;
    QString company;

    quint64 lastLoginTime;
    QString accesscode;
    QString localIP;
    bool online;
    QString path_list;
    qint32 type;
    quint32 deviceVersion;



};

//内容有点多，我们用到什么再填什么算了
struct DownloadTask {
    int failCode;
    struct Channel_struct {
        bool available;
        int failCode;
        int opened;
        int type;
        quint64 dlBytes;
        quint64 speed;

    };
    Channel_struct vipChannel_struct;
    QString name;//下载文件名
    QString url;//下载文件的下载地址
    quint64 speed;
    Channel_struct lixianChannel;
    quint64 downTime;
    //sublist 不知具体内容
    quint64 createTime;
    int state;
    quint64 remainTime;
    int progress;//进度
    QString path;//映射地址
    int type;
    QString id;
    quint64 completeTime;
    quint64 size;//文件大小(字节）

};

struct DownloadTaskStatus {

    int recycleNum;
    int serverFailNum;
    int completeNum;//下载完成数量
    int sync;
    QList<DownloadTask> tasks;
    int dlNum;//还没下载完完成任务的任务数


};

#endif // XWARE_TYPE_H
