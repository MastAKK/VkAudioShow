#ifndef PROGRAMCONFIGWINDOW_H
#define PROGRAMCONFIGWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>

class ProgramConfigWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ProgramConfigWindow(QWidget *parent = 0);
    ~ProgramConfigWindow();

private:
    void readConfig();

    QString m_font;
    double  m_opacity;

    QLineEdit*      m_fontEdit;
    QSpinBox*       m_opacitySpinBox;
    QPushButton*    m_readyButton;

signals:
    void readyConfig();

private slots:
    void saveConfig();
};

#endif // PROGRAMCONFIGWINDOW_H
