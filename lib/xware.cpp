#include "xware.h"
#include <QSet>
#include <iostream>

using namespace std;
static const QString API_PRELOGIN_URL="http://login.xunlei.com/check";
static const QString API_LOGIN_URL="http://login.xunlei.com/sec2login";
//长点不会死人吧？
static const QString API_AFTERLOGIN_URL="http://stat.login.xunlei.com:1800/report?cnt=1&cmdid=sec2login&errorcode=0&responsetime=1&retrynum=0&serverip=&url=&domain=&b_type=113&platform=1&clientversion=";
static const QString API_HOMEPAGE_URL="http://yuancheng.xunlei.com";
static const QString API_POLL_URL="http://hub.yuancheng.xunlei.com/check/vipcache?callback=jQuery172007442529452964664_1407688441667&_=1407688442337";
Xware::Xware(QObject *parent): QObject(parent)
{
    cookieList = new QList<QNetworkCookie>;
    user = new User;



}

void Xware::login(const QString &userName,const  QString &rawPassword)
{
    user->rawPassword=rawPassword;

    user->userName=userName;

    QString checkUrl=API_PRELOGIN_URL+"?u="+userName+"&business_type=13";
    check(checkUrl);

}


void Xware::check(QString &url)
{
    manager[0] = new QNetworkAccessManager(this);
    connect(manager[0],SIGNAL(finished(QNetworkReply*)),
            this,SLOT(checkReplyFinished(QNetworkReply*)));
    manager[0]->get(QNetworkRequest(QUrl(url)));
}

void Xware::checkReplyFinished(QNetworkReply *reply)
{
    QVariant variantCookies = reply->header(QNetworkRequest::SetCookieHeader);

    QList<QNetworkCookie> cookies = qvariant_cast<QList<QNetworkCookie> >(variantCookies);
    //保存的是所有的cookies
    cookieList->append(cookies);
    for(int i = 0;i<cookies.size();i++)
    {
        if(cookies[i].name()=="check_result")
        {
            QString a;
            a=user->check_result=cookies[i].value();
            QString tmp;
            //提取验证码
            for(int i= 2;i<a.size();i++)
            {
                tmp +=a[i];
            }
            user->verifyKey=tmp;

            qDebug()<<"tmp="<<tmp;
            qDebug()<<a;
            break;
        }
    }
    user->password=encryPassword(user->rawPassword,user->verifyKey);
    qDebug()<<"password="<<user->password;
    connect(this, &Xware::onCheckPost,this,&Xware::signin);
    emit onCheckPost();
    reply->deleteLater();




}

void Xware::signin()
{
    qDebug()<<"正在登录......";
    //request.setHeader(QNetworkRequest::CookieHeader,);
    if(cookieList->empty()) {
        sleep(1);
    }
    if(user->verifyKey=="") {
        abort();
    }


    manager[1]=new QNetworkAccessManager;
    connect(manager[1], SIGNAL(finished(QNetworkReply*)),
            this, SLOT(LoginReplyFinished(QNetworkReply*)));
     QNetworkRequest request;
     request.setHeader(QNetworkRequest::ContentTypeHeader,
                       QVariant("application/x-www-form-urlencoded"));

     QString cookies;
     //一定要设cookie
     cookies=CookieUtil::cookiesToString(*cookieList);
     request.setRawHeader("Cookie",cookies.toLatin1());


     request.setUrl(API_LOGIN_URL);
     QString args = "u="+user->userName+"&p="+user->password+
             "&verifycode="+user->verifyKey.toUpper()+
             "&login_enable=0&token_flag=0&business_type=113";

    manager[1]->post(request,args.toLatin1());
}


void Xware::LoginReplyFinished(QNetworkReply *reply) {

    QVariant varCookies = reply->header(QNetworkRequest::SetCookieHeader);
    QList<QNetworkCookie>cookies=qvariant_cast<QList<QNetworkCookie> >(varCookies);
    cookieList->append(cookies);
    connect(this, &Xware::loginFinish,this,&Xware::checkLoginStatus);//用于检验登录状态
    emit loginFinish();
    reply->deleteLater();
    qDebug()<<"cookies number="<<cookieList->size();


}


void Xware::checkLoginStatus() {
    QString status = CookieUtil::getCookie(*cookieList,"blogresult").value();
    if(status=="0") {
        qDebug()<<"登录成功";
    }else {
        qDebug()<<"登录失败";

    }
    qDebug()<<"blogresult:"<<status;
}



void Xware::status() {
    manager[2] = new QNetworkAccessManager;
    connect(manager[2],SIGNAL(finished(QNetworkReply*)),
            this,SLOT(statusReplyFinished(QNetworkReply*)));
    manager[2]->get(QNetworkRequest(QUrl(API_AFTERLOGIN_URL)));

}

void Xware::statusReplyFinished(QNetworkReply *reply ) {
    qDebug()<<"get status reply";
    QString content = reply->readAll();
    qDebug()<<content;

    emit getStatus();
}
void Xware::goToHomePage() {
    manager[3] = new QNetworkAccessManager;
    connect(manager[3],SIGNAL(finished(QNetworkReply*)),
            this,SLOT(goToHomePageReplyFinished(QNetworkReply*)) );
    QNetworkRequest request;
    request.setUrl(API_HOMEPAGE_URL);

    request.setRawHeader("Cookie",CookieUtil::cookiesToString(*cookieList).toLatin1());
    manager[3]->get(request);

}
//这没有任何用！
void Xware::goToHomePageReplyFinished(QNetworkReply *reply) {
 //QString str = reply->readAll();
 //qDebug()<<str;
//empty

}


QString Xware::encryPassword(const QString &rawPassword,const QString &verCode)
{
    QString p0=rawPassword.trimmed();
    QString v0 = verCode.toUpper();
    QString p1 = QString(QCryptographicHash::hash(p0.toUtf8(),
                                                  QCryptographicHash::Md5).toHex());
    QString p2 = QString(QCryptographicHash::hash(p1.toUtf8(),
                                                  QCryptographicHash::Md5).toHex());
    QString p3 = p2+v0;
    QString password = QString(QCryptographicHash::hash(p3.toUtf8(),
                                                  QCryptographicHash::Md5).toHex());
    return password;
}

void Xware::test()
{
    login("wuzhizhan3333@qq.com","thunderXware");
}
