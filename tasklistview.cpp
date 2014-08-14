#include "tasklistview.h"
#include <QDebug>
#include <QRect>
#include <QStyledItemDelegate>
/*****************delegate***************/
TaskListDelegate::TaskListDelegate(QObject *parent):QItemDelegate(parent)
{
    //empty
}


void TaskListDelegate::paint(QPainter *painter,
                             const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
{
    if(index.column() == 2)
    {
        //进度条
        double progress = index.model ()->data(index, Qt::DisplayRole).toDouble();
        QStyleOptionProgressBarV2 progressBarOption;
        progressBarOption.state = QStyle:: State_Enabled;
        progressBarOption.direction = QApplication:: layoutDirection ();
        progressBarOption.rect = option.rect;
        progressBarOption.fontMetrics = QApplication:: fontMetrics ();
        progressBarOption.minimum = 0;
        progressBarOption.maximum = 100;
        progressBarOption.textAlignment = Qt:: AlignCenter;
        progressBarOption.textVisible = true;
        progressBarOption.progress = progress;

        progressBarOption.text = QString::number(progress)+" %";

        QApplication:: style ()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
    } else {
        return QItemDelegate::paint (painter, option, index);
    }

}


/****************TaskListModel****************/
TaskListModel::TaskListModel(QObject *parent)
    :QAbstractTableModel(parent),arr_row_list(NULL)
{

}
TaskListModel::~TaskListModel()
{
    arr_row_list = NULL;
}

void TaskListModel::setHorizontalHeaderList(QStringList horizontalHeaderList)
{
    horizontal_header_list = horizontalHeaderList;
}

void TaskListModel::setVerticalHeaderList(QStringList verticalHeaderList)
{
    vertical_header_list = verticalHeaderList;
}

int TaskListModel::rowCount(const QModelIndex &parent) const
{
    {
        if(vertical_header_list.size() > 0)
            return vertical_header_list.size();

        if(NULL == arr_row_list)
            return 0;
        else
            return arr_row_list->size();

    }
}

int TaskListModel::columnCount(const QModelIndex &parent) const
{
    if(horizontal_header_list.size() > 0)
        return horizontal_header_list.size();

    if(NULL == arr_row_list)
        return 0;
    else if(arr_row_list->size() < 1)
        return 0;
    else
        return arr_row_list->at(0).size();

}

QVariant TaskListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if(NULL == arr_row_list)
        return QVariant();

    if(arr_row_list->size() < 1)
        return QVariant();

    if (role == Qt::TextAlignmentRole)
    {
        return int(Qt::AlignLeft | Qt::AlignVCenter);
    }
    else if (role == Qt::DisplayRole)
    {
        if(index.row() >= arr_row_list->size())
            return QVariant();
        if(index.column() >= arr_row_list->at(0).size())
            return QVariant();
        return arr_row_list->at(index.row()).at(index.column());

    } else if(role == Qt::CheckStateRole){
        //第7列显示check_box
        if(index.column() == 6) {
            if(check_state_map.contains(index.row()))
                return check_state_map[index.row()] == Qt::Checked ? Qt::Checked : Qt::Unchecked;

            return Qt::Unchecked;
        }
    }
    return QVariant();

}

QVariant TaskListModel::headerData(int section,
                                   Qt::Orientation orientation, int role) const
{
    if(role==Qt::DisplayRole)
    {
        if(orientation==Qt::Horizontal) // 水平表头
        {
            if(horizontal_header_list.size() > section)
                return horizontal_header_list[section];
            else
                return QVariant();
        }
        else
        {
            if(vertical_header_list.size() > section)
                return vertical_header_list[section]; // 垂直表头
            else
                return QVariant();
        }
    }

    return QVariant();

}

Qt::ItemFlags TaskListModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;


    if(index.column()==6)
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;


 // Qt::ItemFlags flag = QAbstractItemModel::flags(index);
 //   return flag;

}

bool TaskListModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
        if(!index.isValid())
                return false;

        if (role == Qt::CheckStateRole && index.column() == 6)
        {
                check_state_map[index.row()] = (value == Qt::Checked ?
                                                    Qt::Checked : Qt::Unchecked);
                qDebug()<<"checkbox";
        }

        return true;
}
void TaskListModel::setModalDatas(QList<QStringList> *rowList)
{
    arr_row_list = rowList;
}

void TaskListModel::refrushModel()
{
    beginResetModel();
    endResetModel();
    emit updateCount(this->rowCount(QModelIndex()));
}


/***************TaskListView***************/
TaskListView::TaskListView(QWidget *parent)
    :QTableView(parent)
{
    this->setAlternatingRowColors(true);
    this->setStyleSheet(( "QTableView{background-color: rgb(250, 250, 115);"
                          "alternate-background-color: rgb(141, 163, 215);}"));
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->horizontalHeader()->setStretchLastSection(true);
    this->horizontalHeader()->setHighlightSections(false);
    this->verticalHeader()->setVisible(false);
    // this->setShowGrid(false);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    // this->setSelectionMode(QAbstractItemView::ExtendedSelection);


    model = new TaskListModel();
    this->setModel(model);
    model->setModalDatas(&grid_data_list);


    taskListDelegate = new TaskListDelegate();
    this->setItemDelegate(taskListDelegate);
    // this->initHeader();



    // connect(model,&TaskListModel::updateCount,this,&TaskListView::updateCount);
    this->initHeader();


}

TaskListView::~TaskListView()
{
    if(taskListDelegate) {
        delete taskListDelegate;
        taskListDelegate = NULL;
    }
    if(model) {
        delete model;
        model = NULL;
    }

    grid_data_list.clear();
}

void TaskListView::addRow(QStringList rowList)
{
    grid_data_list.append(rowList);
    model->refrushModel();

}

void TaskListView::remove()
{
    QModelIndexList model_index_list = this->selectedIndexes();
    int model_count = model_index_list.count();
    if(model_count <= 0)
        return;

    QList<int> list_row;
    for(int i=model_count-1; i>=0; i--)
    {
        QModelIndex model_index = model_index_list.at(i);
        int row = model_index.row();
        if(!list_row.contains(row))
            list_row.append(row);
    }

    if(list_row.isEmpty())
        return;

    qSort(list_row);

    for(int i=list_row.count()-1; i>=0; i--)
    {
        grid_data_list.removeAt(list_row.at(i));
    }

    model->refrushModel();

}
void TaskListView::clear()
{
    grid_data_list.clear();
    model->refrushModel();
}

int TaskListView::rowCount()
{
    return model->rowCount(QModelIndex());
}
void TaskListView::changeValue()
{
    model->refrushModel();
}

//设置水平表头
void TaskListView::initHeader()
{
    QStringList header;
    header<<tr("名称")<<tr("大小")<<tr("进度")<<tr("剩余时间")<<tr("速度")<<tr("状态")<<tr("选择");
    model->setHorizontalHeaderList(header);

}

//更新整个视图
void TaskListView::updateAllData(QList<QStringList> grid_list)
{
    grid_data_list = grid_list;
    model->refrushModel();

}
