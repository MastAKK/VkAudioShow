#include "utils.h"


void utils::createNewWidget(QWidget *newWidget, QWidget *oldWidget){
    newWidget->setGeometry(oldWidget->geometry());

    if(oldWidget != 0)
        oldWidget->close();

    newWidget->show();
}

void utils::removeLayout(QLayout *layout){
    if(layout != 0){
        QLayoutItem* item;
        while((item = layout->takeAt(0)) != 0){
            layout->removeItem (item);
            item->widget()->hide();
            item->widget()->deleteLater();
        }
        delete layout;
    }
}

void utils::removeLayout(QWidget* widget){
    QLayout* layout = widget->layout();
    if(layout != 0){
        QLayoutItem* item;
        while((item = layout->takeAt(0)) != 0)
            layout->removeItem (item);
        delete layout;
    }
}

void utils::createNewWidgetOverOld(QWidget *oldWidget, QWidget *newWidget){
    removeLayout(oldWidget->layout());
    oldWidget->setLayout(newWidget->layout());
}

void utils::justCreateNewWidget(QWidget *newWidget, QWidget *oldWidget){
    if(oldWidget != 0)
        oldWidget->close();
    newWidget->show();
}
