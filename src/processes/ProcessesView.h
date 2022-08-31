#ifndef QNVSM_PROCESSESVIEW_H
#define QNVSM_PROCESSESVIEW_H

#include <QTreeView>

class ProcessesView: public QTreeView {
    Q_OBJECT

public:
    explicit ProcessesView(QWidget *parent = nullptr);

    void mousePressEvent(QMouseEvent *event) override;

public slots:
    void onDataUpdated();

private:
    void addItem(int row, int column, const QVariant &data);
    void updateItem(int row, int column, const QVariant &data);
    int getRowIndexByPid(const QString &pid);
};

#endif //QNVSM_PROCESSESVIEW_H
