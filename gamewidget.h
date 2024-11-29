#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QUdpSocket>
#include <QHostInfo>
#include <QtNetwork>
#include <QNetworkInterface>
#include <QFileDialog>
#include <QRandomGenerator>

#include "solver.h"
#include "expression.h"
#include "frac.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class GameWidget;
}
QT_END_NAMESPACE

class QUdpSocket;
enum MessageType{test};

class GameWidget : public QWidget
{
    Q_OBJECT

public:
    GameWidget(QWidget *parent = nullptr);
    ~GameWidget();

private slots:
    void hearMessage();

    void on_solveModeFileButton_clicked();

    void on_solveModeSolveButton_clicked();

    void on_modeSelectTabWidget_tabBarClicked(int index);

    void on_offlineModeGiveupButton_clicked();

    void on_offlineModeCheckButton_clicked();

    void on_onlineModeIDEdit_textChanged();

    void on_onlineModePortSelect_currentIndexChanged(int index);

    void on_onlineModeStartButton_clicked();

    void on_onlineModeQuitButton_clicked();

    void on_onlineModeCheckButton_clicked();

    void on_onlineModeSaveButton_clicked();

    void on_onlineModeExitButton_clicked();

    void on_modeSelectTabWidget_currentChanged(int index);

private:
    Ui::GameWidget *ui;
    QUdpSocket *udpSocket;
    Solver *solver;
    qint16 port;
    int randomNums[4];
    int offlinePoints;
    void sendMessage(MessageType type);
    void getRandomNums();
    void offlinePointShowcasing();
    void offlineNumShowcaseing();

    QString ID;
};
#endif // GAMEWIDGET_H
