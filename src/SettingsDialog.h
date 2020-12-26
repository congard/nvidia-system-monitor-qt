#ifndef QNVSM_SETTINGSDIALOG_H
#define QNVSM_SETTINGSDIALOG_H

#include <QDialog>

class SettingsDialog: public QDialog {
public:
    SettingsDialog();

    void closeEvent(QCloseEvent *) override;
};

#endif //QNVSM_SETTINGSDIALOG_H
