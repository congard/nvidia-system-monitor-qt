#include "ProcessesTableView.h"

#include <QStandardItemModel>
#include <QHeaderView>
#include <QMenu>
#include <QMouseEvent>
#include <QMutex>

#include "constants.h"
#include "core/Utils.h"

ProcessesTableView::ProcessesTableView(QWidget *parent) : QTableView(parent) {
    worker = new ProcessesWorker;

    auto *model = new QStandardItemModel;

    // Column titles
    QStringList horizontalHeader;
    horizontalHeader.append("Name of Process"); // Longer header increases column width so the process names are visible
    horizontalHeader.append("Type (C/G)");
    horizontalHeader.append("GPU ID");
    horizontalHeader.append("Process ID");
    horizontalHeader.append("SM Util (%)");
    horizontalHeader.append("GPU Mem Util (%)");
    horizontalHeader.append("Encoding (%)");
    horizontalHeader.append("Decoding (%)");
    horizontalHeader.append("FB Mem Usage (MB)");

    model->setHorizontalHeaderLabels(horizontalHeader);

    setModel(model);
    resizeRowsToContents();
    resizeColumnsToContents();
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    verticalHeader()->hide();
    setAutoScroll(false);
}

ProcessesTableView::~ProcessesTableView() {
    delete worker;
}

void ProcessesTableView::mousePressEvent(QMouseEvent *event) {
    QTableView::mousePressEvent(event);
    int row = indexAt(event->pos()).row();

    if (row != -1) {
        QMutexLocker locker(&worker->mutex);
        setCurrentIndex(model()->index(row, 0));
        selectedPid = worker->processes[row].pid;
    } else {
        selectedPid = "";
    }

    if (event->button() == Qt::RightButton && row != -1) {
        QMenu contextMenu(tr("Context menu"), this);

        worker->mutex.lock();
        QAction action1(
                ("Kill " + worker->processes[row].name + " (pid " + worker->processes[row].pid + ")").c_str(),
                this
        );
        worker->mutex.unlock();

        connect(&action1, &QAction::triggered, this, &ProcessesTableView::killProcess);
        contextMenu.addAction(&action1);

        contextMenu.exec(mapToGlobal(event->pos()));
    }
}

#define setItemExt(row, column, str, extra) \
    { \
        auto *qItem = new QStandardItem(QString((worker->processes[i].str + extra).c_str())); \
        qItem->setTextAlignment(Qt::AlignHCenter); \
        ((QStandardItemModel*)model())->setItem(row, column, qItem); \
    }

#define setItem(row, column, str) setItemExt(row, column, str, "")

void ProcessesTableView::onDataUpdated() {
    model()->removeRows(0, model()->rowCount());
    QMutexLocker locker(&worker->mutex);

    for (size_t i = 0; i < worker->processes.size(); i++) {
        setItem(i, NVSM_NAME, name)
        setItem(i, NVSM_TYPE, type)
        setItem(i, NVSM_GPUIDX, gpuIdx)
        setItem(i, NVSM_PID, pid)
        setItem(i, NVSM_SM, sm)
        setItem(i, NVSM_MEM, mem)
        setItem(i, NVSM_ENC, enc)
        setItem(i, NVSM_DEC, dec)
        setItem(i, NVSM_FBMEM, fbmem)
    }

    int index = worker->processesIndexByPid(selectedPid);
    if (index != -1)
        setCurrentIndex(model()->index(index, 0));
}

void ProcessesTableView::killProcess() {
    QMutexLocker locker(&worker->mutex);
    if (worker->processesIndexByPid(selectedPid) != -1) {
        Utils::exec("kill " + selectedPid);
        selectedPid = "";
    }
}
