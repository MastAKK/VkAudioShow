#ifndef RUNNINGLINE_H
#define RUNNINGLINE_H

#include <QLabel>

class RunningLine : public QLabel
{
    Q_OBJECT
public:
    explicit RunningLine(const QString &text, int speed, QWidget *parent = 0);
    ~RunningLine();
    void setMovingText(const QString& text);
    QString getMovingText(){ return m_textOrig; }
    void stopMoving(){ moving = false; }
    void startMoving(){ moving = true; }

private:
    QString m_text;
    int position;
    int timerID;
    QString m_textOrig;
    bool moving;
protected:
    virtual void timerEvent(QTimerEvent *);
};

#endif // RUNNINGLINE_H
