#ifndef TASKLISTVIEW_H
#define TASKLISTVIEW_H
#include <QTableView>
#include <QAbstractTableModel>
#include <QItemDelegate>
#include <QStringList>
#include <QPainter>
#include <QHeaderView>
#include <QApplication>
#include <QMap>
//委托
class TaskListDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    TaskListDelegate(QObject *parent = 0);
    virtual void paint(QPainter *painter,const QStyleOptionViewItem &option,
                       const QModelIndex &index) const;
};

//Modle
class TaskListModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    TaskListModel(QObject * parent = 0);
    ~TaskListModel(void);
    //继承父类
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData( const QModelIndex &index, const QVariant &value, int role );

    //本类新创建
    void setHorizontalHeaderList(QStringList horizontalHeaderList);
    void setVerticalHeaderList(QStringList verticalHeaderList);
    void setModalDatas(QList<QStringList> * rowList);
    void refrushModel();
    QMap<QString,Qt::CheckState> & getCheckStateMap() {
        return check_state_map ;
    }
    void setCheckStateMap(const QMap<QString,Qt::CheckState> *state) {
        check_state_map = *state;
    }
    void setLocate(const QVector<QString> *l)  {
        locate = *l;
    }
    void setTask(const QMap<QString,QString> *t) {
         task= *t;
    }

signals:
    void updateCount(int count);
private:
    QStringList horizontal_header_list;
    QStringList vertical_header_list;
    QList<QStringList>  *arr_row_list;

    //<id,状态>
    QMap<QString, Qt::CheckState> check_state_map;//记录着复选框的状态

    //<id>
    QVector<QString> locate;
    // <pid,pid_state>,要用pid_state构成url对下载任务进行操作
    QMap<QString,QString> task;
};



//View
class TaskListView : public QTableView
{
    Q_OBJECT
public:
    TaskListView(QWidget *parent = 0);
    ~TaskListView(void);
    void addRow(QStringList rowList);
    int rowCount();
    void updateAllData(const QList<QStringList> *grid_list,
                       const QMap<QString, QString> *task,
                       const QVector<QString> *locate);


signals:
   // void updateCount(int count);
public slots:
    void remove();
    void clear();
    void changeValue();
private:

private:

    void initHeader();

private:

    TaskListModel *model;
    QList< QStringList > grid_data_list;
    TaskListDelegate *taskListDelegate;

};
#endif // TASKLISTVIEW_H
