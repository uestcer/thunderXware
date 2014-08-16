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
    void list();//轮询当前下载器的任务，这里假设只有一个下载器
   /* QList<PeerList> &getPeerList() { return peerList  ;}
    void setPeerList(const QList<PeerList> &peers) {
        peerList.clear();
        peerList.append(peers);
    }*/
    //args是要传入的参数
    void operateTask(QString args);
    void createTask(QString args,QString url,QString taskName );
    DownloadTaskStatus downloadTaskStatus;
    void test();
signals:
    void onCheckPost();
    void loginFinish();
    void getStatus();
    void loginSuccess();//登录成功
    void loginFail();//登录失败
    void hasDownloader();
    void noDownloader();
    void listPeerFinished();//处理完peerlist的json数据;
    void needCheckResult(); //要验证码
    void listFinished(DownloadTaskStatus );//list解析完成
    void operateTaskFinish( );//操作完成
    void createTaskFailSignal(const QString &info);
private slots:
    //登录前check接收完成
    void checkReplyFinished(QNetworkReply *);
    //登录接收完成
    void LoginReplyFinished(QNetworkReply *);
    //登录后状态检验完成（还没发现有用）
    void statusReplyFinished(QNetworkReply*);
    void goToHomePageReplyFinished(QNetworkReply*);
    void listPeer();
    void listPeerReplyFinished(QNetworkReply *reply);
    void cycleListPeer();//启动周期性listPeer();

    void listReplyFinished(QNetworkReply *reply);
    void operateTaskReplyFinished(QNetworkReply *reply);
    void createTaskReplyFinished(QNetworkReply *reply);
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
     * 5.list检查下载器任务
     * 6.对任务进行各种操作（开始暂停的任务，删除，暂停任务等）
     * 7.创建新下载任务
     */
    static const size_t XWARE_MANAGER_ARRAY_SIZE = 8;
    QNetworkAccessManager *manager[XWARE_MANAGER_ARRAY_SIZE];
    QList<QNetworkCookie>  *cookieList;
    QString cookieString;
    User *user;
    QList<PeerList> peerList;



};

#endif // LOGIN_H
