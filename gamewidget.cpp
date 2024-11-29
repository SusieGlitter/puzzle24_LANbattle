#include "gamewidget.h"
#include "ui_gamewidget.h"

GameWidget::GameWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GameWidget)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(),this->height());

    ui->modeSelectTabWidget->setTabText(0,"自动求解");
    ui->modeSelectTabWidget->setTabText(1,"单人模式");
    ui->modeSelectTabWidget->setTabText(2,"联机模式");
    ui->modeSelectTabWidget->setCurrentIndex(0);

    ui->onlineModeStackedWidget->setCurrentIndex(0);
}

GameWidget::~GameWidget()
{
    delete ui;
}


void GameWidget::sendMessage(MessageType type)
{
    QByteArray data;
    QDataStream out(&data,QIODevice::WriteOnly);
    out<<type;
    switch (type) {
    case test:
        // out<<;
        break;
    default:
        break;
    }
    udpSocket->writeDatagram(data,data.length(),QHostAddress::Broadcast,port);
}

void GameWidget::hearMessage()
{
    while(udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(),datagram.size());
        QDataStream in(&datagram,QIODevice::ReadOnly);
        int type;
        in>>type;
        QString mes;
        switch (type) {
        case test:
            in>>mes;
            // ...
            break;
        default:
            break;
        }
    }
}

void GameWidget::on_modeSelectTabWidget_tabBarClicked(int index)
{

}


void GameWidget::on_solveModeFileButton_clicked()
{

}


void GameWidget::on_solveModeSolveButton_clicked()
{

}


void GameWidget::on_offlineModeGiveupButton_clicked()
{

}


void GameWidget::on_offlineModeCheckButton_clicked()
{

}


void GameWidget::on_onlineModeIDEdit_textChanged()
{

}


void GameWidget::on_onlineModePortSelect_currentIndexChanged(int index)
{

}


void GameWidget::on_onlineModeStartButton_clicked()
{
    ui->onlineModeStackedWidget->setCurrentIndex(1);
}


void GameWidget::on_onlineModeQuitButton_clicked()
{
    ui->onlineModeStackedWidget->setCurrentIndex(0);
}


void GameWidget::on_onlineModeCheckButton_clicked()
{
    //临时
    ui->onlineModeStackedWidget->setCurrentIndex(2);
}


void GameWidget::on_onlineModeSaveButton_clicked()
{

}


void GameWidget::on_onlineModeExitButton_clicked()
{
    ui->onlineModeStackedWidget->setCurrentIndex(0);
}


void GameWidget::on_modeSelectTabWidget_currentChanged(int index)
{

}

