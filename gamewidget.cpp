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

    setStage(resting);
    changeID();

    IDList.clear();
    pointList.clear();

    ui->onlineModeRankTabel->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

GameWidget::~GameWidget()
{
    delete ui;
}


void GameWidget::sendMessage(MessageType type,QString desID)
{
    QByteArray data;
    QDataStream out(&data,QIODevice::WriteOnly);
    out<<type;
    switch (type) {
    case questRoom:
        out<<ID;
        break;
    case ansRoomNameRepeat:
        out<<desID;
        break;
    case ansRoomAccept:
        out<<desID<<IDList;
        break;
    case noticeQuitRoom:
        out<<ID;
        break;
    case noticeNewQuestion:
        out<<randomNums[0]<<randomNums[1]<<randomNums[2]<<randomNums[3];
        break;
    case noticeCorrect:
        out<<ID<<ui->onlineModeTextEdit->toPlainText();
        break;
    case noticeGameQuit:
        out<<ID;
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

        QString mes1,mes2;
        QStringList mesList;

        switch (type) {
        case questRoom:
            in>>mes1;
            if(!host)break;
            if(stage==playingGame)
                sendMessage(ansRoomGaming);
            else if(IDList.contains(mes1))
                sendMessage(ansRoomNameRepeat,mes1);
            else if(stage==waitingStart)
                sendMessage(ansRoomAccept,mes1);
            break;
        case ansRoomGaming:
            if(stage==findingRoom)
            {
                setStage(resting);
                ui->onlineModeDebugLabel->setText("当前有正在进行的游戏");
            }
            break;
        case ansRoomNameRepeat:
            in>>mes1;
            if(mes1==ID&&stage==findingRoom)
            {
                setStage(resting);
                ui->onlineModeDebugLabel->setText("ID被占用");
            }
            break;
        case ansRoomAccept:
            in>>mes1>>mesList;
            if(mes1==ID&&stage==findingRoom)
            {
                host=false;
                setStage(waitingStart);
                IDList=mesList;
                ui->onlineModeDebugLabel->setText("成功加入房间");
            }
            if(stage==waitingStart)
                IDList.append(mes1);
            break;
        case noticeQuitRoom:
            in>>mes1;
            if(stage==waitingStart)
            {
                if(mes1==ID)
                {
                    setStage(resting);
                    ui->onlineModeDebugLabel->setText("退出房间");
                }
                else
                {
                    IDList.removeAll(mes1);
                }
            }
            break;
        case noticeKillRoom:
            if(stage==waitingStart)
            {
                host=false;
                setStage(resting);
                ui->onlineModeDebugLabel->setText("房间被解散了");
            }
            break;
        case noticeGameStart:
            if(stage==waitingStart)
            {
                ui->onlineModeTextEdit->clear();
                setStage(playingGame);
                for(int i=0;i<IDList.length();i++)
                    pointList.push_back(0);
            }
            break;
        case noticeNewQuestion:
            if(stage==playingGame)
            {
                in>>randomNums[0]>>randomNums[1]>>randomNums[2]>>randomNums[3];
                onlineNumShowcaseing();
                ui->onlineModeTextEdit->clear();
                ui->onlineModeCheckButton->setDisabled(false);
                ui->onlineModeCheckButton->setVisible(true);
                ui->onlineModeCheckButton->setText("提交");

            }
            break;
        case noticeCorrect:
            if(stage==playingGame)
            {
                in>>mes1>>mes2;
                setStage(playingGame);
                ui->onlineModeTextEdit->setPlainText(mes1+" has got 24!\n"+mes2);
                for(int i=0;i<IDList.length();i++)
                    if(mes1==IDList[i])
                    {
                        pointList[i]++;
                        break;
                    }
            }
            break;
        case noticeGameQuit:
            in>>mes1;
            if(stage==playingGame)
            {
                if(mes1==ID)
                {
                    IDList.clear();
                    pointList.clear();
                    setStage(resting);
                    ui->onlineModeDebugLabel->setText("退出了游戏");
                }
                else
                {
                    // 还没确定要不要把退出的人除名,暂时不除名
                }
            }
            break;
        case noticeGameEnd:
            if(stage==playingGame)
            {
                setStage(lookingRank);
            }
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
void GameWidget::onlineNumShowcaseing()
{
    ui->onlineModeNumShowcase1->setPlainText(QString::number(randomNums[0]));
    ui->onlineModeNumShowcase2->setPlainText(QString::number(randomNums[1]));
    ui->onlineModeNumShowcase3->setPlainText(QString::number(randomNums[2]));
    ui->onlineModeNumShowcase4->setPlainText(QString::number(randomNums[3]));
}

void GameWidget::changeID()
{
    ID=ui->onlineModeIDEdit->toPlainText().split("\n")[0];
    if(ID=="")
        ID=QString::number(QRandomGenerator::global()->bounded(1, 114514));
}

void GameWidget::setStage(Stage stage)
{
    this->stage=stage;
    switch (stage) {
    case resting:
        ui->modeSelectTabWidget->setTabEnabled(0,true);
        ui->modeSelectTabWidget->setTabEnabled(1,true);
        ui->onlineModeStartButton->setText("寻找房间");
        ui->onlineModeIDEdit->setDisabled(false);
        ui->onlineModePortSelect->setDisabled(false);
        ui->onlineModeStartButton->setDisabled(false);
        ui->onlineModeStartButton->setVisible(true);
        ui->onlineModeQuitRoomButton->setDisabled(true);
        ui->onlineModeQuitRoomButton->setVisible(false);
        ui->onlineModeStackedWidget->setCurrentIndex(0);
        break;
    case findingRoom:
        ui->modeSelectTabWidget->setTabEnabled(0,false);
        ui->modeSelectTabWidget->setTabEnabled(1,false);
        ui->onlineModeStartButton->setText("寻找中...");
        ui->onlineModeIDEdit->setDisabled(true);
        ui->onlineModePortSelect->setDisabled(true);
        break;
    case waitingStart:
        ui->onlineModeStartButton->setText("开始游戏");
        if(host)
        {
            ui->onlineModeQuitRoomButton->setText("解散房间");
        }
        else
        {
            ui->onlineModeQuitRoomButton->setText("退出房间");
            ui->onlineModeStartButton->setDisabled(true);
            ui->onlineModeStartButton->setVisible(false);
        }
        ui->onlineModeQuitRoomButton->setDisabled(false);
        ui->onlineModeQuitRoomButton->setVisible(true);
        break;
    case playingGame:
        if(host)
        {
            ui->onlineModeCheckButton->setText("加载题目");
        }
        else
        {
            ui->onlineModeCheckButton->setDisabled(true);
            ui->onlineModeCheckButton->setVisible(false);
        }
        ui->onlineModeStackedWidget->setCurrentIndex(1);
        break;
    case lookingRank:
        ui->onlineModeRankTabel->clearContents();
        ui->onlineModeRankTabel->setRowCount(IDList.length());

        ui->onlineModeStackedWidget->setCurrentIndex(2);
        for(int i=0;i<IDList.length();i++)
        {
            QTableWidgetItem *IDItem=new QTableWidgetItem(IDList[i]);
            QTableWidgetItem *PointsItem=new QTableWidgetItem(QString::number(pointList[i]));
            ui->onlineModeRankTabel->setItem(i,0,IDItem);
            ui->onlineModeRankTabel->setItem(i,1,PointsItem);
        }
        break;
    default:
        break;
    }
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
    exp.calculate();
    Frac res=exp.res;
    bool accept=!exp.err;
    bool used[4]={false,false,false,false};
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            if(!used[j]&&exp.usedNums[i]==randomNums[j])
            {
                used[j]=true;
                break;
            }
    for(int j=0;j<4;j++)
        if(!used[j])
            accept=false;

    ui->offlineModeTextEdit->append(QString::number(res.num)+"/"+QString::number(res.den));

    if(accept==true&&res==Frac(24))
    {
        offlinePoints+=1;
        offlinePointShowcasing();
        // ui->offlineModeTextEdit->clear();

        getRandomNums();
        offlineNumShowcaseing();
    }
}

void GameWidget::on_onlineModeQuitRoomButton_clicked()
{
    if(stage==waitingStart)
    {
        if(host)
            sendMessage(noticeKillRoom);
        else
            sendMessage(noticeQuitRoom);
    }
}

void GameWidget::on_onlineModeIDEdit_textChanged()
{
    changeID();
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
    // ui->onlineModeStackedWidget->setCurrentIndex(1);
    if(stage==resting)
    {
        setStage(findingRoom);
        QTimer::singleShot(100,[this](){
            if(stage==findingRoom)
            {
                // 请求超时,新建房间
                host=true;
                setStage(waitingStart);
                ui->onlineModeDebugLabel->setText("已创建房间 身份:房主 ID:"+ID);
                IDList.clear();
                IDList.append(ID);
            }
        });
        sendMessage(questRoom);
    }
    if(stage==waitingStart)
        sendMessage(noticeGameStart);
}


void GameWidget::on_onlineModeQuitGameButton_clicked()
{
    if(stage==playingGame)
    {
        if(host)
        {
            host=false;
            sendMessage(noticeGameEnd);
        }
        else
        {
            sendMessage(noticeGameQuit);
        }
    }
}


void GameWidget::on_onlineModeCheckButton_clicked()
{
    if(host&&ui->onlineModeCheckButton->text()=="加载题目")
    {
        getRandomNums();
        sendMessage(noticeNewQuestion);
    }
    else
    {
        Expression exp;
        exp.setExpression(ui->onlineModeTextEdit->toPlainText());
        exp.calculate();
        Frac res=exp.res;
        bool accept=!exp.err;
        bool used[4]={false,false,false,false};
        for(int i=0;i<4;i++)
            for(int j=0;j<4;j++)
                if(!used[j]&&exp.usedNums[i]==randomNums[j])
                {
                    used[j]=true;
                    break;
                }
        for(int j=0;j<4;j++)
            if(!used[j])
                accept=false;

        ui->onlineModeTextEdit->append(QString::number(res.num)+"/"+QString::number(res.den));

        if(accept==true&&res==Frac(24))
        {
            sendMessage(noticeCorrect);
        }
    }
}


void GameWidget::on_onlineModeSaveButton_clicked()
{

}


void GameWidget::on_onlineModeExitButton_clicked()
{
    IDList.clear();
    pointList.clear();
    setStage(resting);
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




