#ifndef COOKIEUTIL_H
#define COOKIEUTIL_H
#include <QString>
#include <QNetworkCookie>
#include <QList>
class CookieUtil
{
public:
    CookieUtil();

    //将cookies转成string形式
    static QString cookiesToString(const QList<QNetworkCookie> &cookieList);

    //根据cookie名从所有的cookieList中获取cookie
    static QNetworkCookie  getCookie(const QList<QNetworkCookie>&cookieList,
                                     const QString &cookieName) ;

};

#endif // COOKIEUTIL_H
