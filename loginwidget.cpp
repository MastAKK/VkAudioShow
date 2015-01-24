#include "loginwidget.h"
#include "widget.h"
#include "utils.h"

#include <QWebView>
#include <QVBoxLayout>
#include <QDebug>
#include <QLineEdit>

LogInWidget::LogInWidget(QWidget *parent) :
    QWidget(parent)
{
    view = new QWebView;
    view->setUrl(QUrl("https://oauth.vk.com/authorize?client_id=4437740&scope=audio,status&display=page&v=5.27&response_type=token"));


    line = new QLineEdit;
    line->setText(view->url().toString());
    line->setEnabled(false);


    QVBoxLayout* lay = new QVBoxLayout;
    //lay->addWidget(line);
    lay->addWidget(view);
    setLayout(lay);

    connect(view, SIGNAL(loadFinished(bool)), this, SLOT(slot_loadFinished(bool)));

}

void LogInWidget::slot_loadFinished(bool b){

    qDebug() << "Loaded";
    if(!view->url().toString().isNull())
        line->setText(view->url().toString());

    if(view->url().toString().contains("access_token=")){
        disconnect(view, SIGNAL(loadFinished(bool)), this, SLOT(slot_loadFinished(bool)));

        QString token = view->url().fragment().remove("access_token=").split('&').at(0);
        QString uid = view->url().fragment().remove("user_id=").split('&').at(view->url().fragment().remove("user_id=").split('&').size()-1);

        utils::justCreateNewWidget(new Widget(token, uid, this), this);
    }
}
