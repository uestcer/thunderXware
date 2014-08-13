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

#endif // XWARE_TYPE_H
