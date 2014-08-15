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
#include "createtaskui.h"
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
    //添加下载的那个窗口模块
    void addTaskWindows();
    //好看的文件大小，下载速度显示
    QString perfectSize(QVariant value);
    //好看的下载任务状态
    QString perfectState(int state);
    Ui::DownLoaderUI *ui;
    TaskListView *taskListView;

    CreateTaskUI *createTaskUI;//创建新任务
//对下载进行操作的三个按钮
    QPushButton *add_button;
    QPushButton *start_button;
    QPushButton *pause_button;
    QPushButton *remove_button;//删除
signals:
    void add_signal(QString args,QString url,QString taskName);
    void start_signal(QString args);
    void pause_signal(QString args);
    void remove_signal(QString args);
public slots:
    void updateTaskView(DownloadTaskStatus taskList );

private slots:
    void addTask(QString url,QString taskName);
    void startTask();
    void pauseTask();
    void removeTask();


};

#endif // DOWNLOADERUI_H
