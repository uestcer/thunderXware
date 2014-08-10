#include "cookieutil.h"

CookieUtil::CookieUtil()
{
}
QString  CookieUtil::cookiesToString(const QList<QNetworkCookie> &cookieList) {
    QString cookieStr ;
    int size = cookieList.size();
    for(int i = 0;i<size;i++) {
        cookieStr +=cookieList.at(i).toRawForm()+";";
    }
    return cookieStr;
}

QNetworkCookie CookieUtil::getCookie(const QList<QNetworkCookie>&cookieList,
                                 const QString &cookieName) {
    int size = cookieList.size();
    for(int i = 0;i<size;i++) {
        if(cookieList.at(i).name()==cookieName) {
            return cookieList.at(i);
        }
    }
    return QNetworkCookie(0);

}
