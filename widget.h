#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSystemTrayIcon>
#include <QPushButton>

class RunningLine;
class QNetworkReply;
class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QString token, QString uid, QWidget *parent = 0);
    ~Widget();

private:
    QString m_token;
    QString m_uid;
    QString downloadUrl;
    QString m_artist;
    QString m_title;

    QPushButton* m_settingsBut;

    RunningLine* m_curAudio;

    QNetworkAccessManager *m_manager;

    QSystemTrayIcon *m_trayIcon;

    void createTrayIcon();

    int m_nMouseClick_X_Coordinate;
    int m_nMouseClick_Y_Coordinate;

    void readConfig();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);



private slots:
    void doDownload();
    void checkAudio();

    void configProg();
    void programConfigReady();

    void replyReady     (QNetworkReply *);
    void replyFinished  (QNetworkReply *);
    void iconActivated  (QSystemTrayIcon::ActivationReason);
};

#endif // WIDGET_H
