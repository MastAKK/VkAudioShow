#include "loginwidget.h"
#include <QApplication>
#include <QtGlobal>
#include <QFile>

#if QT_VERSION > 0x050000
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg){
    QFile* logfile;
    switch(type){
    case QtDebugMsg:
        logfile = new QFile("./logs/debug.log");
        logfile->open(QIODevice::Append);
        logfile->write(QString(QString("[")+context.file+":"+context.line+"]: "+msg).toStdString().c_str());
        logfile->close();
        delete logfile;
        break;
    case QtWarningMsg:
        logfile = new QFile("./logs/warning.log");
        logfile->open(QIODevice::Append);
        logfile->write(QString(QString("[")+context.file+":"+context.line+"]: "+msg).toStdString().c_str());
        logfile->close();
        delete logfile;
        break;
    case QtCriticalMsg:
        logfile = new QFile("./logs/critical.log");
        logfile->open(QIODevice::Append);
        logfile->write(QString(QString("[")+context.file+":"+context.line+"]: "+msg).toStdString().c_str());
        logfile->close();
        delete logfile;
        break;
    case QtFatalMsg:
        logfile = new QFile("./logs/fatal.log");
        logfile->open(QIODevice::Append);
        logfile->write(QString(QString("[")+context.file+":"+context.line+"]: "+msg).toStdString().c_str());
        logfile->close();
        delete logfile;
        abort();
    }
}
#endif

int main(int argc, char *argv[])
{
    //qInstallMessageHandler(myMessageOutput); //install : set the callback
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);
    LogInWidget w;
    w.show();

    return a.exec();
}
