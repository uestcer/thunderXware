#ifndef TASKLISTVIEW_H
#define TASKLISTVIEW_H
#include <QTableView>
#include <QAbstractTableModel>
#include <QItemDelegate>
#include <QStringList>
#include <QPainter>
#include <QHeaderView>
#include <QApplication>
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
    void setHorizontalHeaderList(QStringList horizontalHeaderList);
    void setVerticalHeaderList(QStringList verticalHeaderList);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    void setModalDatas(QList<QStringList> * rowList);
    void refrushModel();
signals:
    void updateCount(int count);
private:
    QStringList horizontal_header_list;
    QStringList vertical_header_list;
    QList<QStringList>  *arr_row_list;
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
    void updateAllData(QList<QStringList> *grid_list);
signals:
    void updateCount(int count);
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
