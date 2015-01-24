#include "runningline.h"

RunningLine::RunningLine(const QString& text, int speed, QWidget *parent) :
    QLabel(parent), m_text(text), position(0), moving(1)
{
    setMovingText(text);
    timerID = startTimer((int)1000/speed);
}

RunningLine::~RunningLine(){
    killTimer(timerID);
}

void RunningLine::timerEvent(QTimerEvent *event){
    if(moving){
        int length;
        length = m_text.length();
        if(++position >= length)
            position = 0;

        setText(m_text.right(position)+m_text.left(length-position));
    }
}

void RunningLine::setMovingText(const QString &text){
    m_text = text+"          ";
    m_textOrig = text;
    setText(m_text);
    QFontMetrics fontM = fontMetrics();
    setFixedSize(fontM.width(text), fontM.height());
}
