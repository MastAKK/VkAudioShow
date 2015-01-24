#include "programconfigwindow.h"

#include <QVBoxLayout>
#include <QFile>
#include <QDebug>

ProgramConfigWindow::ProgramConfigWindow(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout* lay = new QVBoxLayout;

    m_fontEdit = new QLineEdit;
    m_opacitySpinBox = new QSpinBox;
    m_opacitySpinBox->setMinimum(1);
    m_opacitySpinBox->setMaximum(100);
    m_opacitySpinBox->setSuffix("%");

    readConfig();

    m_fontEdit->setText(m_font);
    m_opacitySpinBox->setValue(m_opacity*100);

    m_readyButton = new QPushButton("Apply");

    connect(m_readyButton, SIGNAL(clicked()), this, SLOT(saveConfig()));

    lay->addWidget(m_fontEdit);
    lay->addWidget(m_opacitySpinBox);
    lay->addWidget(m_readyButton);
    this->setLayout(lay);
}

ProgramConfigWindow::~ProgramConfigWindow()
{
}

void ProgramConfigWindow::readConfig(){ //Чтение конфигурации из файла. При его отсутствии - создаём и читаем снова.
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
                    m_font = param;
                }
                else if(name == "opacity"){ //прозрачность
                    m_opacity = param.toDouble();
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

void ProgramConfigWindow::saveConfig(){
    m_opacity = (double)m_opacitySpinBox->value()/100;
    m_font = m_fontEdit->text();

    QFile file("config.txt");
    file.open(QIODevice::WriteOnly);
    file.write(QString("font=%1\n").arg(m_font).toStdString().c_str());
    file.write(QString("opacity=%1\n").arg(m_opacity).toStdString().c_str());
    file.close();
    emit readyConfig();
}
