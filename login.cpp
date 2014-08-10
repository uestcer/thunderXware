#include "login.h"
#include <QSet>
#include <iostream>

using namespace std;
Login::Login(QObject *parent)
{

}

QString Login::encryPassword(QString &rawPassword,QString &verCode) {
    QString p0=rawPassword.trimmed();
    QString v0 = verCode.toUpper();
    QString p1 = QString(QCryptographicHash::hash(p0.toUtf8(),QCryptographicHash::Md5).toHex());
    QString p2 = QString(QCryptographicHash::hash(p1.toUtf8(),QCryptographicHash::Md5).toHex());
    QString p3 = p2+v0;
    QString password = QString(QCryptographicHash::hash(p3.toUtf8(),QCryptographicHash::Md5).toHex());
    return password;
}

void Login::check(QString &url)
{
    manager = new QNetworkAccessManager(this);
    connect(manager,SIGNAL(finished(QNetworkReply*)),
            this,SLOT(replyFinished(QNetworkReply*)));
    manager->get(QNetworkRequest(QUrl(url)));
}


void Login::checkReplyFinished(QNetworkReply *reply)
{
    QVariant variantCookies = reply->header(QNetworkRequest::SetCookieHeader);
    QList<QNetworkCookie>cookies = qvariant_cast<QList<QNetworkCookie> >(variantCookies);
    //将获取到的cookie存放起来





    qDebug()<<cookies.at(0).domain().toStdString()<<endl;
    //cout<<cookies.at(0).path().toStdString()<<endl;
    qDebug()<<cookies.at(1).value().data()<<endl;



}
void Login::test() {
    QString rawpass= "thunderXware";
    QString verCode = "!XTS";
    cout<<encryPassword(rawpass,verCode).toStdString()<<endl;
    QString url = "http://login.xunlei.com/check?u=wuzhizhan@wuzhizhan.cn&business_type=13";
    check(url);
}
