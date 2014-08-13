#include "downloaderui.h"
#include "ui_downloaderui.h"

DownLoaderUI::DownLoaderUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DownLoaderUI)
{
    ui->setupUi(this);
}

DownLoaderUI::~DownLoaderUI()
{
    delete ui;
}
void DownLoaderUI::test() {
    ui->listWidgetTasks->setResizeMode(QListView::Adjust);
    ui->listWidgetTasks->setAutoScroll(true);
    QWidget *wContainer = new QWidget(ui->listWidgetTasks);
    QHBoxLayout *hLayout = new QHBoxLayout(wContainer);

    //QLabel *alarmIcon = new QLabel(tr("beih"));
    QPushButton *button = new QPushButton("下载");
    QLabel *placeLabel = new QLabel(tr("北京"));
    QLabel *videoNumLabel = new QLabel(tr("8"));
    QLabel *dateLabel = new QLabel(tr("2013-4-16"));
    QLabel *alarmMsgLabel = new QLabel(tr("违章搭建"));
    //QPushButton *pDeleteBtn = new QPushButton(QIcon(),tr("delete"));

    hLayout->addWidget(button);
    hLayout->addStretch(1);//将空白没有widget的地方分成了若干份，按比例分配
    hLayout->addWidget(placeLabel);
    hLayout->addStretch(1);
    hLayout->addWidget(videoNumLabel);
    hLayout->addStretch(1);
    hLayout->addWidget(dateLabel);
    hLayout->addStretch(1);//将空白没有widget的地方分成了若干份，按比例分配
    hLayout->addWidget(alarmMsgLabel);
    hLayout->setContentsMargins(5,0,0,5);//关键代码，如果没有很可能显示不出来
    // wContainer->setLayout(hLayout);//如果layout在创建时就已经将父窗口指针当参数，那就不用setlayout
    wContainer->resize(350,50);
    // wContainer->show();

    QListWidgetItem *alarmItem = new QListWidgetItem(ui->listWidgetTasks);

    ui->listWidgetTasks->setItemWidget(alarmItem,wContainer);
    QPushButton *b = new  QPushButton("好的");

    QListWidgetItem *item1 = new QListWidgetItem(ui->listWidgetTasks);
    ui->listWidgetTasks->setItemWidget(item1,b);
    QPushButton *b1 = new  QPushButton("不好啊");

    QListWidgetItem *item2 = new QListWidgetItem(ui->listWidgetTasks);


     qDebug()<<ui->listWidgetTasks->count();
     b->setText("你妹");

}
