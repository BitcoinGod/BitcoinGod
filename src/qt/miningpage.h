#ifndef MININGPAGE_H
#define MININGPAGE_H

#include  <QWidget>
#include "walletmodel.h"

namespace Ui {
class MiningPage;
}
class MiningPage : public QWidget
{
    Q_OBJECT

public:
    explicit MiningPage(const PlatformStyle *platformStyle, QWidget *parent = 0);
    ~MiningPage();

    void setWalletModel(WalletModel *walletModel);

private Q_SLOTS:
    void on_miningControl_clicked();

private:
    Ui::MiningPage *ui;
    WalletModel *walletModel;
    void changeMiningStatus(bool isMining);
};

#endif // MININGPAGE_H
