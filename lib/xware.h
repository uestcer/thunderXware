#ifndef XWARE_H
#define XWARE_H
#include <QCryptographicHash>
#include <string>
#include <QString>
#include <QVariant>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkCookie>
#include "lib/xware_type.h"
#include <QList>
#include <vector>
#include <list>
#include <unistd.h>
#include "lib/cookieutil.h"
using namespace std;

class Xware:public QObject
{
    Q_OBJECT
public:
    Xware(QObject *parent=0);

    void login(const QString &userName,const QString &rawPassword);


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
     */
    QNetworkAccessManager *manager[4];
    QList<QNetworkCookie>  *cookieList;

    User *user;
signals:
    void onCheckPost();
    void loginFinish();
    void getStatus();

private slots:
    //登录前check接收完成
    void checkReplyFinished(QNetworkReply *);
    //登录接收完成
    void LoginReplyFinished(QNetworkReply *);
    //登录后状态检验完成（还没发现有用）
    void statusReplyFinished(QNetworkReply*);
    void goToHomePageReplyFinished(QNetworkReply*);

};

#endif // LOGIN_H
