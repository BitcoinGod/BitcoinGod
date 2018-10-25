#include "miningpage.h"
#include "ui_miningpage.h"

static bool ismining =false;
MiningPage::MiningPage(const PlatformStyle *platformStyle,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MiningPage),
    walletModel(0)
{
    ui->setupUi(this);
}

MiningPage::~MiningPage()
{
    delete ui;
}

void MiningPage::setWalletModel(WalletModel *model)
{
    this->walletModel = model;
}

void MiningPage::on_miningControl_clicked()
{
    ismining = !ismining;
    changeMiningStatus(ismining);
}

void MiningPage::changeMiningStatus(bool isMining){
    if(ismining){
        ui->minStatus->setText(QString("正在挖矿"));
        ui->miningControl->setText(QString("停止挖矿"));
    }else{
        ui->minStatus->setText(QString("停止挖矿"));
        ui->miningControl->setText(QString("进行挖矿"));
    }
}
