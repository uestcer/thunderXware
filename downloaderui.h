#ifndef DOWNLOADERUI_H
#define DOWNLOADERUI_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidgetItem>
#include <QPushButton>
#include <unistd.h>
#include <QDebug>
#include <QString>
#include "lib/xware_type.h"
#include "tasklistview.h"
namespace Ui {
class DownLoaderUI;
}
struct PerfectTaskList {
    QString name;//文件名
    quint64 size;//文件大小
    int process;//下载进度
    quint64 remainTime;
    QString state;
};

class DownLoaderUI : public QWidget
{
    Q_OBJECT

public:
    explicit DownLoaderUI(QWidget *parent = 0);
    void test();
    ~DownLoaderUI();

private:
    //好看的文件大小，下载速度显示
    QString perfectSize(QVariant value);
    //好看的下载任务状态
    QString perfectState(int state);
    Ui::DownLoaderUI *ui;
    TaskListView *taskListView;
public slots:
    void updateTaskView(DownloadTaskStatus taskList );

};

#endif // DOWNLOADERUI_H
