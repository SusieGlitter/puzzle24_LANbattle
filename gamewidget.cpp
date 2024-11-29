#include "gamewidget.h"
#include "ui_gamewidget.h"

GameWidget::GameWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GameWidget)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(),this->height());

    ui->modeSelectTabWidget->setCurrentIndex(0);
    ui->onlineModeStackedWidget->setCurrentIndex(0);

    udpSocket=new QUdpSocket(this);
    solver=new Solver();
    solver->solveAll();

    offlinePoints=0;

    port=ui->onlineModePortSelect->currentIndex()+1145;
    udpSocket->bind(port,QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint);
    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(hearMessage()));
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

void GameWidget::getRandomNums()
{
    do
    {
        for(int i=0;i<4;i++)
            randomNums[i]=QRandomGenerator::global()->bounded(1, 14);
    }
    while("No solution"==solver->getAns(randomNums[0],randomNums[1],randomNums[2],randomNums[3]));
}

void GameWidget::offlinePointShowcasing()
{
    ui->offlineModePointShowcase->setPlainText("得分: "+QString::number(offlinePoints));
}

void GameWidget::offlineNumShowcaseing()
{
    ui->offlineModeNumShowcase1->setPlainText(QString::number(randomNums[0]));
    ui->offlineModeNumShowcase2->setPlainText(QString::number(randomNums[1]));
    ui->offlineModeNumShowcase3->setPlainText(QString::number(randomNums[2]));
    ui->offlineModeNumShowcase4->setPlainText(QString::number(randomNums[3]));
}

void GameWidget::on_modeSelectTabWidget_tabBarClicked(int index)
{

}


void GameWidget::on_solveModeFileButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("选择文件"),"",tr("TXT(*.txt)"));
    if (fileName.isEmpty())
        return;
    ui->solveModeTextEdit->clear();
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        while (!file.atEnd())
        {
            QByteArray line = file.readLine();
            QString str(line);
            ui->solveModeTextEdit->insertPlainText(str);
        }
        file.close();
    }
}


void GameWidget::on_solveModeSolveButton_clicked()
{
    ui->solveModeTextShowcase->clear();
    int numints[4];
    QStringList numstrs;
    for(QString line:ui->solveModeTextEdit->toPlainText().split("\n"))
    {
        numstrs=line.split(" ");
        numstrs.removeAll("");
        if(numstrs.length()!=4)
        {
            ui->solveModeTextShowcase->append("error");
            continue;
        }
        bool err=false;
        for(int i=0;i<4;i++)
        {
            numints[i]=numstrs[i].toInt();
            if(numints[i]<=0||numints[i]>=14)
            {
                err=true;
                break;
            }
        }
        if(err)
        {
            ui->solveModeTextShowcase->append("error");
            continue;
        }
        QString ans=solver->getAns(numints[0],numints[1],numints[2],numints[3]);
        ui->solveModeTextShowcase->append(ans);
    }
}


void GameWidget::on_offlineModeGiveupButton_clicked()
{
    offlinePoints-=2;
    offlinePointShowcasing();
    QString ans=solver->getAns(randomNums[0],randomNums[1],randomNums[2],randomNums[3]);
    ui->offlineModeTextEdit->clear();
    ui->offlineModeTextEdit->setPlainText(ans);
}


void GameWidget::on_offlineModeCheckButton_clicked()
{
    Expression exp;
    exp.setExpression(ui->offlineModeTextEdit->toPlainText());

    // if(exp.calculate()==Frac(24,1))
    {
        offlinePoints+=1;
        offlinePointShowcasing();
        ui->offlineModeTextEdit->clear();

        getRandomNums();
        offlineNumShowcaseing();
    }
}


void GameWidget::on_onlineModeIDEdit_textChanged()
{
    ID=ui->onlineModeIDEdit->toPlainText().split("\n")[0];
}


void GameWidget::on_onlineModePortSelect_currentIndexChanged(int index)
{
    udpSocket->disconnect();
    udpSocket->abort();
    port=index+1145;
    udpSocket->bind(port,QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint);
    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(hearMessage()));
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
    if(index==1)
    {
        getRandomNums();
        offlinePointShowcasing();
        offlineNumShowcaseing();
    }
}
