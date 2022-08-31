#include "ProcessesView.h"

#include <QStandardItemModel>
#include <QHeaderView>
#include <QMenu>
#include <QMouseEvent>
#include <QMutex>

#include "core/InfoProvider.h"
#include "core/Utils.h"

using namespace std;

// columns order
namespace NVSMColumns {
enum {
    Name,
    Type,
    GPUIdx,
    PID,
    FbMem,
    Sm,
    Mem,
    Enc,
    Dec,
    columns_count
};
}

ProcessesView::ProcessesView(QWidget *parent): QTreeView(parent) {
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

void ProcessesView::mousePressEvent(QMouseEvent *event) {
    QTreeView::mousePressEvent(event);
    int row = indexAt(event->pos()).row();

    if (event->button() == Qt::RightButton && row != -1) {
        QString name = model()->data(model()->index(row, NVSMColumns::Name)).toString();
        QString pid = model()->data(model()->index(row, NVSMColumns::PID)).toString();

        QMenu contextMenu(tr("Context menu"), this);

        QAction *kill = contextMenu.addAction("Terminate " + name + " (pid " + pid + ")");
        connect(kill, &QAction::triggered, [pid]() {
            Utils::exec_cmd("kill " + pid);
        });

        contextMenu.addSeparator();

        QPair<const char*, const char*> signalsList[] = {
            {"Suspend", "STOP"},
            {"Continue", "CONT"},
            {"Hangup", "HUP"},
            {"Interrupt", "INT"},
            {"Terminate", "TERM"},
            {"Kill", "KILL"},
            {"User 1", "USR1"},
            {"User 2", "USR2"}
        };

        QMenu *signalMenu = contextMenu.addMenu("Send signal");

        for (auto &sig : signalsList) {
            QAction *act = signalMenu->addAction(QString(sig.first) + " (" + sig.second + ")");
            connect(act, &QAction::triggered, [pid, signal = sig.second]() {
                Utils::exec_cmd("kill -" + QString(signal) + " " + pid);
            });
        }

        contextMenu.exec(mapToGlobal(event->pos()));
    }
}

// https://github.com/congard/nvidia-system-monitor-qt/pull/11/files#diff-cfa97a7a8d28043dcb063d203b8dba3cR130

inline QVariant convertToInt(const QString &str) {
    char *end;
    auto i = strtoll(str.toLocal8Bit().constData(), &end, 10); // check if value can be converted to integer

    if (*end == '\0')  {
        return i;
    } else {
        return str.toLocal8Bit().constData();
    }
}

void ProcessesView::onDataUpdated() {
    // In order to avoid performance and other issues, sorting will be enabled after inserting
    // and updating the items in the tree: https://doc.qt.io/qt-5/qtreeview.html#sortingEnabled-prop
    setSortingEnabled(false);

    const auto &processes = InfoProvider::getProcesses();

    // update existing processes list & remove finished processes
    // loop from more to less in order to delete rows
    for (int i = model()->rowCount() - 1; i >= 0; i--) {
        QString pid = model()->data(model()->index(i, NVSMColumns::PID)).toString();

        int index = -1;

        for (int j = 0; j < processes.size(); ++j) {
            if (processes[j].pid == pid) {
                index = j;
                break;
            }
        }

        if (index == -1) {
            model()->removeRow(i);
        } else {
            const ProcessInfo &processInfo = processes[index];

            // update what may change
            updateItem(i, NVSMColumns::Type, processInfo.type);
            updateItem(i, NVSMColumns::Sm, convertToInt(processInfo.sm));
            updateItem(i, NVSMColumns::Mem, convertToInt(processInfo.mem));
            updateItem(i, NVSMColumns::Enc, convertToInt(processInfo.enc));
            updateItem(i, NVSMColumns::Dec, convertToInt(processInfo.dec));
            updateItem(i, NVSMColumns::FbMem, convertToInt(processInfo.fbmem));
        }
    }

    // add new processes to the list, if there are any
    int rowCount = model()->rowCount();
    for (auto &process : processes) {
        if (getRowIndexByPid(process.pid) == -1) {
            addItem(rowCount, NVSMColumns::Name, process.name);
            addItem(rowCount, NVSMColumns::Type, process.type);
            addItem(rowCount, NVSMColumns::GPUIdx, convertToInt(process.gpuIdx));
            addItem(rowCount, NVSMColumns::PID, convertToInt(process.pid));
            addItem(rowCount, NVSMColumns::Sm, convertToInt(process.sm));
            addItem(rowCount, NVSMColumns::Mem, convertToInt(process.mem));
            addItem(rowCount, NVSMColumns::Enc, convertToInt(process.enc));
            addItem(rowCount, NVSMColumns::Dec, convertToInt(process.dec));
            addItem(rowCount, NVSMColumns::FbMem, convertToInt(process.fbmem));

            rowCount++;
        }
    }

    setSortingEnabled(true);
}

void ProcessesView::addItem(int row, int column, const QVariant &data) {
    auto *qItem = new QStandardItem();
    qItem->setData(data, Qt::DisplayRole);
    qItem->setTextAlignment(Qt::AlignHCenter);
    ((QStandardItemModel*)model())->setItem(row, column, qItem);
}

void ProcessesView::updateItem(int row, int column, const QVariant &data) {
    model()->setData(model()->index(row, column), data, Qt::DisplayRole);
}

int ProcessesView::getRowIndexByPid(const QString &pid) {
    for (int i = 0; i < model()->rowCount(); i++) {
        QString row_pid = model()->data(model()->index(i, NVSMColumns::PID)).toString();

        if (row_pid == pid) {
            return i;
        }
    }

    return -1;
}
