#include "loginui.h"
#include "ui_loginui.h"
#include <QDebug>
LoginUI::LoginUI(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginUI),mouse_press(false)
{
    ui->setupUi(this);
    //美化窗体外观
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    this->setMouseTracking(false);


    closeButton = new QToolButton(this);
    QPixmap closePix(":/icons/res/loginUI/close.png");
    closeButton->setIcon(closePix);
    closeButton->setGeometry(width()-25,5,20,20);

    closeButton->setToolTip(tr("关闭"));
    closeButton->setStyleSheet("background-color:transparent;");
    connect(closeButton,SIGNAL(clicked()),this,SLOT(close()));



    QPixmap pixmap = QPixmap(":/icons/res/loginUI/afternoon.jpg").scaled(this->size());
    QPalette palette(this->palette());
    palette.setBrush(QPalette::Background, QBrush(pixmap));
    this->setPalette(palette);
    //中央显示
    move ((QApplication::desktop()->width() - width())/2,
          (QApplication::desktop()->height() - height())/2);



}

LoginUI::~LoginUI()
{
    delete ui;
}

void LoginUI::on_pushButtonLogin_clicked()
{
    ui->errMessage->setText("");
    qDebug()<<"loginUI";
    emit login(ui->lineEdituserName->text().trimmed(),
               ui->lineEditPassword->text().trimmed());
    ui->pushButtonLogin->setDisabled(true);
}

void LoginUI::mousePressEvent(QMouseEvent *event)
{



   if(event->button() == Qt::LeftButton) {
        mouse_press = true;
        move_point = event->pos();
    }

}
void LoginUI::mouseMoveEvent(QMouseEvent *event)
{

    if(mouse_press)
    {
        QPoint move_pos = event->globalPos();
        this->move(move_pos-move_point);
    }

}

void LoginUI::mouseReleaseEvent(QMouseEvent *)
{
    //mouse_press = false;
}


void LoginUI::loginFail()
{
    ui->errMessage->setText("<span style=\" color:#f30808;\">密码或账号错误!</span>");


    ui->pushButtonLogin->setEnabled(true);

}
