#include "versionmessagedialog.h"
#include "ui_versionmessagedialog.h"

VersionMessageDialog::VersionMessageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VersionMessageDialog)
{
    ui->setupUi(this);
}

VersionMessageDialog::~VersionMessageDialog()
{
    delete ui;
}

void VersionMessageDialog::setContent(QString content){
    ui->content->setText(content);
    ui->content->setTextFormat(Qt::RichText);
    ui->content->setTextInteractionFlags(Qt::TextBrowserInteraction);
    ui->content->setOpenExternalLinks(true);
}

void VersionMessageDialog::on_okBtn_clicked()
{
    QDialog::accept();
}
