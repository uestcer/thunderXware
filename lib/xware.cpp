#include "xware.h"
#include <QSet>
#include <iostream>

using namespace std;
static const QString API_PRELOGIN_URL="http://login.xunlei.com/check";
static const QString API_LOGIN_URL="http://login.xunlei.com/sec2login";
//长点不会死人吧？
static const QString API_AFTERLOGIN_URL="http://stat.login.xunlei.com:1800/report?cnt=1&cmdid=sec2login&errorcode=0&responsetime=1&retrynum=0&serverip=&url=&domain=&b_type=113&platform=1&clientversion=";
static const QString API_HOMEPAGE_URL="http://yuancheng.xunlei.com";
static const QString API_PROTOCOLS_URL_REMOTE="http://homecloud.yuancheng.xunlei.com";
static const QString API_PROTOCOLS_URL="http://homecloud.yuancheng.xunlei.com";
//static const QString API_PROTOCOLS_URL = "http://localhost:1111";
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
    connect(this,&Xware::hasDownloader,this,&Xware::list);//下载器任务
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
        qDebug()<<"需要验证码";
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




        emit onCheckPost();

    }

    reply->deleteLater();




}

void Xware::signin()
{
    qDebug()<<"signin():正在登录......";

    user->password=encryPassword(user->rawPassword,user->verifyKey);

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

        //清空原来的cookies
        cookieList->clear();
        emit loginFail();

    }

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

    QString content = reply->readAll();


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

void Xware::cycleListPeer() {



    QTimer::singleShot(2000, this, SLOT(listPeer()));


}

void Xware::listPeer() {
    QString args="/listPeer?v=2&ct=0&type=0";
    if(manager[4] ==NULL) {

        manager[4]=new QNetworkAccessManager(this);
        connect(manager[4],SIGNAL(finished(QNetworkReply*)),
                this,SLOT(listPeerReplyFinished(QNetworkReply *)) );
    }


    QNetworkRequest request;
    request.setRawHeader("Cookie",cookieString.toLatin1());
    //request.setUrl(QUrl(API_PROTOCOLS_URL+args));
    request.setUrl(QUrl(API_PROTOCOLS_URL_REMOTE+args));
    manager[4]->get(request);



}

void Xware::listPeerReplyFinished(QNetworkReply *reply)
{

    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QString all = codec->toUnicode(reply->readAll());
    qDebug()<<"Xware::listPeer"<<all;
    QJsonParseError parseerr;
    QVariant result = QJsonDocument::fromJson(all.toUtf8(), &parseerr).toVariant();

    if (parseerr.error == QJsonParseError::NoError) {
        QVariantMap obj = result.toMap();
        QList<PeerList> peers;
        if (obj["rtn"].toInt() == 0) {
            //有下载器
            QVariantList  QvarPeerList = obj["peerList"].toList();
            foreach(const QVariant& item, QvarPeerList) {
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

            peerList = peers;


            emit hasDownloader();

        }
        else {
            //没有配置下载器
            emit noDownloader();

        }
    }
    emit listPeerFinished();
    reply->deleteLater();

}


QString listType = "0";
QString downloaderPID;
void Xware::list() {
    if(peerList.size()<=0)
        return;

    if(downloaderPID=="") {
        downloaderPID = peerList.at(0).pid;
    }

    //假设只有一个下载器
    QString args="/list?&pos=0&number=8&needUrl=1&v=2&ct=0&type="+listType
            +"&pid="+downloaderPID;
           // peerList.at(0).pid;

    if(manager[5] ==NULL) {

        manager[5]=new QNetworkAccessManager(this);
        connect(manager[5],SIGNAL(finished(QNetworkReply*)),
                this,SLOT(listReplyFinished(QNetworkReply *)) );
    }


    QNetworkRequest request;
    request.setRawHeader("Cookie",cookieString.toLatin1());
    request.setUrl(QUrl(API_PROTOCOLS_URL+args));

    manager[5]->get(request);
}
void Xware::listReplyFinished(QNetworkReply *reply) {

    //qDebug()<<"Xware::listReplyFinished()";
    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QString all = codec->toUnicode(reply->readAll());
    // qDebug()<<"listReply:"<<all;
    QJsonParseError parseer;
    QVariant result = QJsonDocument::fromJson(all.toUtf8(),&parseer).toVariant();
    if(parseer.error == QJsonParseError::NoError) {
        QVariantMap obj = result.toMap();
        DownloadTaskStatus task;
        task.completeNum=obj["completeNum"].toInt();
        task.dlNum = obj["dlNum"].toInt();
        task.recycleNum=obj["recycleNum"].toInt();
        task.serverFailNum=obj["serverFailNum"].toInt();
        task.sync = obj["sync"].toInt();
        QVariantList taskList = obj["tasks"].toList();

        foreach(const QVariant &item,taskList) {
            //vip和离线通道没有赋值
            QVariantMap m = item.toMap();
            DownloadTask dt;
            dt.completeTime = m["completeTime"].toULongLong();
            dt.createTime = m["createTime"].toULongLong();
            dt.downTime = m["downTime"].toULongLong();
            dt.failCode = m["failCode"].toInt();
            dt.id = m["id"].toString();
            dt.name = m["name"].toString();//下载文件名
            dt.path = m["path"].toString();//下载文件地址
            dt.progress = m["progress"].toInt();
            dt.size = m["size"].toULongLong();
            dt.speed = m["speed"].toULongLong();
            dt.state = m["state"].toInt();
            dt.type = m["type"].toInt();
            dt.url = m["url"].toString();
            task.tasks.append(dt);


        }
        downloadTaskStatus = task;

        emit listFinished(task);

    }
    reply->deleteLater();
}


void Xware::operateTask(QString args) {


    QString CompleArgs=args+"&v=2&ct=0";
    qDebug()<<"Xware::operateTask()"<<CompleArgs;
    if(manager[6] ==NULL) {

        manager[6]=new QNetworkAccessManager(this);
        connect(manager[6],SIGNAL(finished(QNetworkReply*)),
                this,SLOT(operateTaskReplyFinished(QNetworkReply*)));
    }


    QNetworkRequest request;
    request.setRawHeader("Cookie",cookieString.toLatin1());
    request.setUrl(QUrl(API_PROTOCOLS_URL+CompleArgs));

    manager[6]->get(request);
}
void Xware::operateTaskReplyFinished(QNetworkReply *reply) {

    Q_UNUSED(reply);
    emit operateTaskFinish();
    //手工调用更新列表
    list();
}


//用post方法创建
void Xware::createTask(QString args, QString url, QString taskName)
{


    if(peerList.size()<=0)
        return;
    QString CompleArgs=args+"v=2&ct=0&pid="+peerList.at(0).pid;

    if(manager[7] ==NULL) {

        manager[7]=new QNetworkAccessManager(this);
        connect(manager[7],SIGNAL(finished(QNetworkReply*)),
                this,SLOT(createTaskReplyFinished(QNetworkReply*)));
    }


    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      QVariant("application/x-www-form-urlencoded"));
    request.setRawHeader("Cookie",cookieString.toLatin1());
    request.setUrl(QUrl(API_PROTOCOLS_URL_REMOTE+CompleArgs));

    /*
     * name:json
     * param: path "C:/TDDOWNLOAD/"
     * tasks:
     * [{
     *  "url":"http://....",
     *  "name":"qq.ext",
     *  "gcid":"",
     *  "cid":"",
     *  "file":0
     * }]
     *
     */
    //手工构造json
    QString postData="json={";
    postData+="\"path\":\"C:/TDDOWNLOAD/\",";
    postData+="\"tasks\":[{\"url\":\""+url+"\",";
    postData+="\"name\":\""+taskName+"\",";
    postData+="\"gcid\":\"\",\"cid\":\"\",";
    postData+="\"filesize\":0";
    postData+="}]}";

    manager[7]->post(request,postData.toUtf8());
}
void Xware::createTaskReplyFinished(QNetworkReply *reply)
{
    QString all = reply->readAll();
    QJsonParseError parseerr;
    QVariant result =  QJsonDocument::fromJson(all.toUtf8(),&parseerr).toVariant();
    if(parseerr.error == QJsonParseError::NoError) {
        QVariantMap obj = result.toMap();
        if(obj["rnt"].toInt()==0) {
            QVariantList tasks = obj["tasks"].toList();
            //只进行一项任务
            foreach (const QVariant &item,tasks) {
                QVariantMap task  = item.toMap();
                if(task["result"].toInt()==0) {
                    //emit createTaskResultSignal(tr("创建成功"));

                }else if(task["result"].toInt()==202) {
                    emit createTaskFailSignal(tr("已存在相同任务"));//重复

                }else {
                    emit createTaskFailSignal(tr("创建失败"));
                }

            }
        }
    }else {
        emit createTaskFailSignal(tr("创建失败"));
    }

    //手工调用更新列表
    list();
}

void Xware::test()
{




    login("wuzhizhan3333@qq.com","thunderXware");
    qDebug()<<"hello";



}
