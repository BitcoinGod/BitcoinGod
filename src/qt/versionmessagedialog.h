#ifndef VERSIONMESSAGEDIALOG_H
#define VERSIONMESSAGEDIALOG_H

#include <QDialog>

namespace Ui {
class VersionMessageDialog;
}

class VersionMessageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VersionMessageDialog(QWidget *parent = 0);
    ~VersionMessageDialog();
    void setContent(QString content);

private Q_SLOTS:
    void on_okBtn_clicked();

private:
    Ui::VersionMessageDialog *ui;
};

#endif // VERSIONMESSAGEDIALOG_H
