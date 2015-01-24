#include "widget.h"
#include "runningline.h"

#include "utils.h"

#include "programconfigwindow.h"

#include <QDebug>
#include <QApplication>
#include <QGridLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QMenu>
#include <QFile>

#include <QTimer>

#if QT_VERSION > 0x050000
#include <QJsonObject>
#include <QJsonDocument>
#else
#include <QJson/Parser>
#endif

Widget::Widget(QString token, QString uid, QWidget *parent) :
    QWidget(parent), m_token(token), m_uid(uid), m_artist("none")
{
    this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::Tool | Qt::FramelessWindowHint);
    this->setFixedSize(300, 70);

    createTrayIcon();
    connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    QGridLayout* lay = new QGridLayout;

    m_curAudio = new RunningLine(QString("").toUtf8(), 5);
    m_curAudio->setMovingText(QString::fromUtf8("Получение текущего аудио..."));
    m_curAudio->setSizePolicy( QSizePolicy::Ignored,QSizePolicy::Ignored);
    m_curAudio->setScaledContents(true);
    m_curAudio->setAlignment(Qt::AlignLeft);
    lay->addWidget(m_curAudio, 1, 0, 2, 4);
/*
    QPushButton* downloadBut = new QPushButton(QIcon("./download.png"), "");
    downloadBut->setFixedSize(25, 20);
    lay->addWidget(downloadBut, 4, 0);
    connect(downloadBut, SIGNAL(clicked()), this, SLOT(doDownload()));
*/
    setLayout(lay);

    QTimer* timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(checkAudio()));
    timer->start(1500);

    m_settingsBut = new QPushButton(this);
    m_settingsBut->resize(20, 20);
    m_settingsBut->move(this->width()-m_settingsBut->width(), 0);

    m_settingsBut->setIcon(QIcon("settings.png"));

    connect(m_settingsBut, SIGNAL(clicked()), this, SLOT(configProg()));

    readConfig();
}

Widget::~Widget(){
    qDebug() << "Destroyed";
    exit(0);
}

void Widget::readConfig(){ //Чтение конфигурации из файла. При его отсутствии - создаём и читаем снова.
    QFile file("config.txt");
    if(file.open(QIODevice::ReadOnly)){
        qDebug() << "Reading config";
        while(!file.atEnd()){
            QString line = file.readLine();
            if(line.isEmpty())
                continue;

            line = line.split('#').at(0);
            if(!line.isEmpty() && line.split('=').size() > 1){
                QString name = line.split('=').at(0);
                QString param = line.split('=').at(1);
                if(name == "font"){ //шрифт
                    m_curAudio->setFont(QFont(param));
                }
                else if(name == "opacity"){ //прозрачность
                    setWindowOpacity(param.toFloat());
                    qDebug() << name << "=" << param;
                } else{
                    qDebug() << "Unknown config parameter: " << name;
                }
            }
        }
        file.close();
    } else{ //Если файла нет - мы его создадим!
        qDebug() << "Can\'t open config file";
        file.close();
        file.open(QIODevice::WriteOnly);
        file.write("font=Segoe UI\n");
        file.write("opacity=0.8\n");
        file.close();
        readConfig();
    }
}

void Widget::iconActivated(QSystemTrayIcon::ActivationReason reason){
    switch(reason){
    case QSystemTrayIcon::Trigger:
        break;
    case QSystemTrayIcon::Context:
        qApp->quit();
        break;
    case QSystemTrayIcon::DoubleClick:
        qApp->quit();
        break;
    default:
        ;
    }
    qDebug() << "QSystemTrayIcon: Activated";
}

#if QT_VERSION > 0x050000
void Widget::replyReady(QNetworkReply* reply){
    if(reply->url().toString().contains("status.get")){
        QByteArray replyStr = reply->readAll();
        QJsonObject json = QJsonDocument::fromJson(replyStr).object();
        QJsonValue val = json["response"].toObject()["audio"];
        if(val.isNull() && !m_artist.isEmpty()){
            m_artist = "";
            m_title = "";
            downloadUrl = "";
            m_curAudio->setMovingText(QString("В даный момент нету аудио. Проверьте ваш статус.").toUtf8());
        }
        else if(!val.isNull()){
            QString artist = val.toObject()["artist"].toString();
            QString title = val.toObject()["title"].toString();
            QString audiourl = val.toObject()["url"].toString();
            if(m_curAudio->getMovingText() != QString("%1 - %2").arg(artist, title)){
                m_artist = artist;
                m_title = title;
                downloadUrl = audiourl;

                m_curAudio->setMovingText(QString("%1 - %2").arg(artist, title));
                if(m_curAudio->width() <= width())
                    m_curAudio->stopMoving();
                else
                    m_curAudio->startMoving();
            }
        }
    }
}
#else
void Widget::replyReady(QNetworkReply *reply){
    if(reply->url().toString().contains("status.get")){
        QByteArray replyStr = reply->readAll();
        QJson::Parser parser;
        QVariantMap replyMap = parser.parse(replyStr).toMap();
        auto val = replyMap["response"].toMap()["audio"];
        if(val.isNull()  && !m_artist.isEmpty()){ //Логично, что если нету объекта аудио - в данный момент ничего не воспроизводится
            m_artist = "";
            m_title = "";
            downloadUrl = "";
            m_curAudio->setMovingText(QString::fromUtf8("В даный момент нету аудио. Проверьте ваш статус."));
        } else if(!replyMap["response"].toMap()["audio"].isNull()){
            QString artist = val.toMap()["artist"].toString().toUtf8();
            QString title = val.toMap()["title"].toString().toUtf8();
            QString audiourl = val.toMap()["url"].toString().toUtf8();
            if(m_curAudio->getMovingText() != QString("%1 - %2").arg(artist, title)){
                m_artist = artist;
                m_title = title;
                downloadUrl = audiourl;

                m_curAudio->setMovingText(QString("%1 - %2").arg(artist, title));
                if(m_curAudio->width() <= width())
                    m_curAudio->stopMoving();
                else
                    m_curAudio->startMoving();
            }
        }
    }
}
#endif

void Widget::checkAudio(){
    QNetworkAccessManager* mgr = new QNetworkAccessManager;
    mgr->get(QNetworkRequest(QUrl(QString("https://api.vk.com/method/status.get?user_id=%1&v=5.21&access_token=%2").arg(m_uid, m_token))));
    connect(mgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyReady(QNetworkReply*)));
}

void Widget::mousePressEvent(QMouseEvent *event) {
    m_nMouseClick_X_Coordinate = event->x();
    m_nMouseClick_Y_Coordinate = event->y();
}

void Widget::mouseMoveEvent(QMouseEvent *event){ //для движения окна путём нажатия и удержания
    move(event->globalX()-m_nMouseClick_X_Coordinate, event->globalY()-m_nMouseClick_Y_Coordinate);
}

void Widget::doDownload(){ //начать скачку текущего аудио
    if(!downloadUrl.isEmpty()){
        m_manager = new QNetworkAccessManager(this);
        connect(m_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
        m_manager->get(QNetworkRequest(QUrl(downloadUrl))); //get запрос к серверу с текущим аудио
    }
}

void Widget::replyFinished(QNetworkReply *reply){
    if(reply->error()){
        qDebug() << "ERROR!";
        qDebug() << reply->errorString();
    }
    else{
        QFile *file = new QFile(QString("./%1 - %2.mp3").arg(m_artist, m_title));
        if(file->open(QFile::WriteOnly)){
            file->write(reply->readAll()); //запись аудио в файл
            file->flush();
            file->close();
            qDebug() << file->fileName();
        }
        delete file;
    }

    reply->deleteLater();
}

void Widget::createTrayIcon(){
    m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setIcon(QIcon("./download.png"));
    m_trayIcon->show();
    m_trayIcon->showMessage("Hi!", "I'm very glad that you choose my program! For exit just right or double click on icon in system tray.");
}

void Widget::configProg(){
    ProgramConfigWindow* configWindow = new ProgramConfigWindow();
    connect(configWindow, SIGNAL(readyConfig()), this, SLOT(programConfigReady()));
    utils::justCreateNewWidget(configWindow, 0);
}

void Widget::programConfigReady(){
    readConfig();
}
