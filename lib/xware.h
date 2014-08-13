#ifndef XWARE_H
#define XWARE_H
#include <QCryptographicHash>
#include <QTextCodec>
#include <QJsonDocument>
#include <QVariantMap>
#include <string>
#include <QString>
#include <QVariant>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkCookie>
#include <QList>
#include <vector>
#include <list>
#include <unistd.h>
#include <QTimer>
#include <QEventLoop>
#include <QNetworkCookieJar>

#include "lib/cookieutil.h"
#include "lib/xware_type.h"
using namespace std;

class Xware:public QObject
{
    Q_OBJECT
public:
    Xware(QObject *parent=0);

    void login(const QString &userName,const QString &rawPassword);

    QList<PeerList> &getPeerList() { return peerList  ;}
    void setPeerList(const QList<PeerList> &peers) {
        peerList.clear();
        peerList.append(peers);
    }
    void test();

private:
    //登录前的判断
    void check(QString &url);
    //登录
    void  signin();
    //对密码进行加密
    QString encryPassword(const QString &password,const QString &verCode);
    //登录后检验(不知道有什么用)
    void status();
    //访问首页

    void goToHomePage();

    //检验登录状态
    void checkLoginStatus();


    /**
     * 0:登录前检查
     * 1：登录
     * 2.登录后检验
     * 3.访问首页
     * 4.listPeer检查下载器的状态
     */
    static const size_t XWARE_MANAGER_ARRAY_SIZE = 5;
    QNetworkAccessManager *manager[XWARE_MANAGER_ARRAY_SIZE];
    QList<QNetworkCookie>  *cookieList;
    QString cookieString;
    User *user;
    QList<PeerList> peerList;


signals:
    void onCheckPost();
    void loginFinish();
    void getStatus();
    void loginSuccess();//登录成功
    void loginFail();//登录失败
    void hasDownloader(QList<PeerList> &peerList);
    void noDownloader();
    void listPeerFinished();//处理完peerlist的json数据;
    void needCheckResult(); //要验证码
private slots:
    //登录前check接收完成
    void checkReplyFinished(QNetworkReply *);
    //登录接收完成
    void LoginReplyFinished(QNetworkReply *);
    //登录后状态检验完成（还没发现有用）
    void statusReplyFinished(QNetworkReply*);
    void goToHomePageReplyFinished(QNetworkReply*);
    void listPeerReplyFinished(QNetworkReply *reply);
    void listPeer();
    void cycleListPeer();//启动周期性listPeer();


};

#endif // LOGIN_H
