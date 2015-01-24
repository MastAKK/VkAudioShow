#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
class QWebView;
class QLineEdit;

class LogInWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LogInWidget(QWidget *parent = 0);
    ~LogInWidget();

private:
    QWebView* view;
    QLineEdit* line;

    bool m_loggedIn;

private slots:
    void slot_loadFinished(bool b);
};

#endif // LOGINWIDGET_H
