#ifndef LOGINUI_H
#define LOGINUI_H

#include <QDialog>
#include <QPoint>
#include <QMouseEvent>
#include <QToolButton>
#include <QPixmap>
#include <QLabel>
#include <QBitmap>
#include <QPainter>
#include <QPalette>
#include <QApplication>
#include <QDesktopWidget>
#include <QWidget>
namespace Ui {
class LoginUI;
}

class LoginUI : public QDialog
{
    Q_OBJECT

public:
    explicit LoginUI(QWidget *parent = 0);
    ~LoginUI();
signals:
    void login(const QString &userName,const QString &rawPassward);
public slots:
     void loginFail();
private slots:
    void on_pushButtonLogin_clicked();


private:
    Ui::LoginUI *ui;
    QPoint move_point;//移动距离
    bool mouse_press;//鼠标按下
    //鼠标按下事件
    void mousePressEvent(QMouseEvent *event);
    //鼠标释放事件
    void mouseReleaseEvent(QMouseEvent *);
    //鼠标移动事件
    void mouseMoveEvent(QMouseEvent *event);




    QToolButton *closeButton;

    QLabel *background;


};

#endif // LOGINUI_H
