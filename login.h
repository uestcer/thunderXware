#ifndef LOGIN_H
#define LOGIN_H
#include <QCryptographicHash>
#include <QString>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkCookie>
class Login:public QObject
{
    Q_OBJECT
public:
    Login(QObject *parent=0);
    //登录
    void  signin(QString &userName,QString &rawPassword){}

    void test();
private:
    //登录前的判断
    void check(QString &url);

    //对密码进行加密
    QString encryPassword(QString &password,QString &verCode);

    QNetworkAccessManager *manager;
private slots:
     void checkReplyFinished(QNetworkReply *);

};

#endif // LOGIN_H
