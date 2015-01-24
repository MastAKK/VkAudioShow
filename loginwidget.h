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

private:
    QWebView* view;
    QLineEdit* line;

private slots:
    void slot_loadFinished(bool b);
};

#endif // LOGINWIDGET_H
