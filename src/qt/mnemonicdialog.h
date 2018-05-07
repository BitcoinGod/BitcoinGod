#ifndef MNEMONICDIALOG_H
#define MNEMONICDIALOG_H

#include <QDialog>

namespace Ui {
class MnemonicDialog;
}

class MnemonicDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MnemonicDialog(QWidget *parent = 0);
    ~MnemonicDialog();
    void importMnemonic(bool forceRescan);
    void importWallet(bool forceRescan);
    void importPrivatekey(bool forceRescan);
    
Q_SIGNALS:
    void cmdToConsole(const QString &command);  

private Q_SLOTS:
    void on_importPrivatekey_clicked();
    void on_reimportPrivatekey_clicked();
    void on_importMnemonic_clicked();
    void on_reimportMnemonic_clicked();
    void on_importWallet_clicked();
    void on_reimportWallet_clicked();
    void on_fileButton_clicked();
private:
    Ui::MnemonicDialog *ui;
};

#endif // MNEMONICDIALOG_H
