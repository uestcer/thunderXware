#include "xware.h"
#include <QSet>
#include <iostream>

using namespace std;
static const QString API_PRELOGIN_URL="http://login.xunlei.com/check";
static const QString API_LOGIN_URL="http://login.xunlei.com/sec2login";
//长点不会死人吧？
static const QString API_AFTERLOGIN_URL="http://stat.login.xunlei.com:1800/report?cnt=1&cmdid=sec2login&errorcode=0&responsetime=1&retrynum=0&serverip=&url=&domain=&b_type=113&platform=1&clientversion=";
static const QString API_HOMEPAGE_URL="http://yuancheng.xunlei.com";

static const QString API_PROTOCOLS_URL="http://homecloud.yuancheng.xunlei.com";
Xware::Xware(QObject *parent): QObject(parent)
{
    //将manager初始化为空指针
    for(size_t i = 0;i<XWARE_MANAGER_ARRAY_SIZE;i++) {
        manager[i]=NULL;
    }
    cookieList = new QList<QNetworkCookie>;
    user = new User;

    connect(this,&Xware::loginSuccess,this,&Xware::listPeer);
    connect(this,&Xware::listPeerFinished,this,&Xware::cycleListPeer);
    connect(this, &Xware::loginFinish,this,&Xware::checkLoginStatus);//用于检验登录状态
    connect(this, &Xware::onCheckPost,this,&Xware::signin);//登录前检验准备
}

void Xware::login(const QString &userName,const  QString &rawPassword)
{
    user->rawPassword=rawPassword;

    user->userName=userName;

    QString checkUrl=API_PRELOGIN_URL+"?u="+userName+"&business_type=13";
    check(checkUrl);

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

void Xware::check(QString &url)
{
    if(manager[0]==NULL) {
        manager[0] = new QNetworkAccessManager(this);

        connect(manager[0],SIGNAL(finished(QNetworkReply*)),
                this,SLOT(checkReplyFinished(QNetworkReply*)));
    }
    manager[0]->get(QNetworkRequest(QUrl(url)));
}

void Xware::checkReplyFinished(QNetworkReply *reply)
{
    QVariant variantCookies = reply->header(QNetworkRequest::SetCookieHeader);

    QList<QNetworkCookie> cookies = qvariant_cast<QList<QNetworkCookie> >(variantCookies);
    //保存的是所有的cookies
    cookieList->append(cookies);
    QString check_result = CookieUtil::getCookie(cookies,"check_result").value();
    if(check_result=="1") {
        //要输入验证码
        emit needCheckResult();
        return ;
    }else {

        QString a;
        user->check_result=check_result;
        QString tmp;
        //提取验证码
        for(int i= 2;i<check_result.size();i++)
        {
            tmp +=check_result[i];
        }
        user->verifyKey=tmp;

        qDebug()<<"verify_code="<<tmp;
        qDebug()<<"raw_verify_code="<<a;


        emit onCheckPost();

    }

    reply->deleteLater();




}

void Xware::signin()
{
    qDebug()<<"signin():正在登录......";

    user->password=encryPassword(user->rawPassword,user->verifyKey);
    qDebug()<<"password="<<user->password;
    if(manager[1]==NULL) {
        manager[1]=new QNetworkAccessManager(this);
        connect(manager[1], SIGNAL(finished(QNetworkReply*)),
                this, SLOT(LoginReplyFinished(QNetworkReply*)));
    }
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

    emit loginFinish();
    reply->deleteLater();
    delete manager[1];
    manager[1]=NULL;


}


void Xware::checkLoginStatus() {
    QString status = CookieUtil::getCookie(*cookieList,"blogresult").value();
    if(status=="0") {
        //保存cookies的成字符串形式，方面后面使用
        cookieString=CookieUtil::cookiesToString(*cookieList);
        //发送登录成功信号
        qDebug()<<"登录成功";
        emit loginSuccess();
    }else {
        //发送登录失败信号
        qDebug()<<"登录失败";
        qDebug()<<"用户名:"<<user->userName<<"原始密码:"<<user->rawPassword;
        //清空原来的cookies
        cookieList->clear();
        emit loginFail();

    }
    qDebug()<<"blogresult:"<<status;
}


void Xware::status() {
    if(manager[2]==NULL) {
        manager[2] = new QNetworkAccessManager(this);
        connect(manager[2],SIGNAL(finished(QNetworkReply*)),
                this,SLOT(statusReplyFinished(QNetworkReply*)));
    }
    manager[2]->get(QNetworkRequest(QUrl(API_AFTERLOGIN_URL)));

}

void Xware::statusReplyFinished(QNetworkReply *reply ) {
    qDebug()<<"get status reply";
    QString content = reply->readAll();
    qDebug()<<content;

    emit getStatus();
}
void Xware::goToHomePage() {
    if(manager[3]==NULL) {
        manager[3] = new QNetworkAccessManager(this);
        connect(manager[3],SIGNAL(finished(QNetworkReply*)),
                this,SLOT(goToHomePageReplyFinished(QNetworkReply*)) );
    }
    QNetworkRequest request;
    request.setUrl(API_HOMEPAGE_URL);

    request.setRawHeader("Cookie",CookieUtil::cookiesToString(*cookieList).toLatin1());
    manager[3]->get(request);

}
//这没有任何用！
void Xware::goToHomePageReplyFinished(QNetworkReply *) {
    //QString str = reply->readAll();
    //qDebug()<<str;
    //empty

}



void Xware::listPeer() {
    QString args="/listPeer?type=0&v=2&ct=0";
    if(manager[4] ==NULL) {

        manager[4]=new QNetworkAccessManager(this);
        connect(manager[4],SIGNAL(finished(QNetworkReply*)),
                this,SLOT(listPeerReplyFinished(QNetworkReply *)) );
    }


    QNetworkRequest request;
    request.setRawHeader("Cookie",cookieString.toLatin1());
    request.setUrl(QUrl(API_PROTOCOLS_URL+args));

    manager[4]->get(request);



}

void Xware::listPeerReplyFinished(QNetworkReply *reply)
{
    qDebug()<<"receive listPeer ";
    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QString all = codec->toUnicode(reply->readAll());

    QJsonParseError parseerr;
    QVariant result = QJsonDocument::fromJson(all.toUtf8(), &parseerr).toVariant();

    if (parseerr.error == QJsonParseError::NoError) {
        QVariantMap obj = result.toMap();
        QList<PeerList> peers;
        if (obj["rtn"].toInt() == 0) {
            //有下载器
            QVariantList  peerList = obj["peerList"].toList();
            foreach(const QVariant& item, peerList) {
                QVariantMap peer = item.toMap();
                PeerList s;
                s.accesscode=peer["accesscode"].toString();
                s.category = peer["category"].toString();
                s.company =peer["company"].toString();
                s.deviceVersion=peer["deviceVersion"].toUInt();
                s.lastLoginTime=peer["lastLoginTime"].toULongLong();
                s.localIP = peer["localIP"].toString();
                s.name = peer["name"].toString();
                s.online = peer["online"].toBool();
                s.path_list = peer["path_list"].toString();
                s.pid = peer["pid"].toString();
                peers.push_back(s);

            }


            emit hasDownloader(peers);
        }
        else {
            //没有配置下载器
            emit noDownloader();
        }
    }
    emit listPeerFinished();
    reply->deleteLater();

}

void Xware::cycleListPeer() {



    QTimer::singleShot(2000, this, SLOT(listPeer()));


}

void Xware::test()
{




    login("wuzhizhan3333@qq.com","thunderXware");
    qDebug()<<"hello";



}
