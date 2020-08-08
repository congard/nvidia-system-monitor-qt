#include "ProcessesView.h"

#include <QStandardItemModel>
#include <QHeaderView>
#include <QMenu>
#include <QMouseEvent>
#include <QMutex>

#include "constants.h"
#include "core/Utils.h"

using namespace std;

ProcessesView::ProcessesView(QWidget *parent): QTreeView(parent) {
    worker = new ProcessesWorker();

    auto *model = new QStandardItemModel();

    // Column titles
    QStringList horizontalHeader;
    horizontalHeader.append("Name of Process"); // Longer header increases column width so the process names are visible
    horizontalHeader.append("Type (C/G)");
    horizontalHeader.append("GPU ID");
    horizontalHeader.append("Process ID");
    horizontalHeader.append("FB Mem Usage (MB)");
    horizontalHeader.append("SM Util (%)");
    horizontalHeader.append("GPU Mem Util (%)");
    horizontalHeader.append("Encoding (%)");
    horizontalHeader.append("Decoding (%)");

    model->setHorizontalHeaderLabels(horizontalHeader);

    setModel(model);

    for (int i = 0; i < NVSMColumns::columns_count; i++)
        resizeColumnToContents(i);

    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setAutoScroll(false);
    setRootIsDecorated(false);
}

ProcessesView::~ProcessesView() {
    delete worker;
}

void ProcessesView::mousePressEvent(QMouseEvent *event) {
    QTreeView::mousePressEvent(event);
    int row = indexAt(event->pos()).row();

    if (event->button() == Qt::RightButton && row != -1) {
        QMenu contextMenu(tr("Context menu"), this);

        QString name = model()->data(model()->index(row, NVSMColumns::Name)).toString();
        QString pid = model()->data(model()->index(row, NVSMColumns::PID)).toString();
        QAction action1("Kill " + name + " (pid " + pid + ")", this);

        connect(&action1, &QAction::triggered, this, &ProcessesView::killProcess);
        contextMenu.addAction(&action1);

        contextMenu.exec(mapToGlobal(event->pos()));
    }
}

void ProcessesView::onDataUpdated() {
    QMutexLocker locker(&worker->mutex);

    // update existing processes list & remove finished processes
    // loop from more to less in order to delete rows
    for (int i = model()->rowCount() - 1; i >= 0; i--) {
        string pid = qPrintable(model()->data(model()->index(i, NVSMColumns::PID)).toString());
        int index = worker->processesIndexByPid(pid);

        if (index == -1) {
            model()->removeRow(i);
        } else {
            const ProcessList &processList = worker->processes[index];

            // update what may change
            updateItem(i, NVSMColumns::Type, processList.type);
            updateItem(i, NVSMColumns::Sm, processList.sm);
            updateItem(i, NVSMColumns::Mem, processList.mem);
            updateItem(i, NVSMColumns::Enc, processList.enc);
            updateItem(i, NVSMColumns::Dec, processList.dec);
            updateItem(i, NVSMColumns::FbMem, processList.fbmem);
        }
    }

    // add new processes to the list, if there are any
    int rowCount = model()->rowCount();
    for (auto & process : worker->processes) {
        if (getRowIndexByPid(process.pid) == -1) {
            addItem(rowCount, NVSMColumns::Name, process.name);
            addItem(rowCount, NVSMColumns::Type, process.type);
            addItem(rowCount, NVSMColumns::GPUIdx, process.gpuIdx);
            addItem(rowCount, NVSMColumns::PID, process.pid);
            addItem(rowCount, NVSMColumns::Sm, process.sm);
            addItem(rowCount, NVSMColumns::Mem, process.mem);
            addItem(rowCount, NVSMColumns::Enc, process.enc);
            addItem(rowCount, NVSMColumns::Dec, process.dec);
            addItem(rowCount, NVSMColumns::FbMem, process.fbmem);

            rowCount++;
        }
    }
}

void ProcessesView::addItem(int row, int column, const std::string &data) {
    auto *qItem = new QStandardItem(QString(data.c_str()));
    qItem->setTextAlignment(Qt::AlignHCenter);
    ((QStandardItemModel*)model())->setItem(row, column, qItem);
}

void ProcessesView::updateItem(int row, int column, const std::string &data) {
    model()->setData(model()->index(row, column), QString(data.c_str()));
}

void ProcessesView::killProcess() {
    QModelIndex selectedIndex = currentIndex();
    QModelIndex pidIndex = selectedIndex.sibling(selectedIndex.row(), NVSMColumns::PID);
    QString selectedPid = model()->data(pidIndex).toString();
    Utils::exec("kill " + string(qPrintable(selectedPid)));
}

int ProcessesView::getRowIndexByPid(const std::string &pid) {
    for (size_t i = 0; i < model()->rowCount(); i++) {
        string row_pid = qPrintable(model()->data(model()->index(i, NVSMColumns::PID)).toString());

        if (row_pid == pid) {
            return i;
        }
    }

    return -1;
}
