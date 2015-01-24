#ifndef UTILS_H
#define UTILS_H

#include <QWidget>
#include <QLayout>

namespace utils{
void createNewWidget(QWidget* newWidget, QWidget* oldWidget);
void justCreateNewWidget(QWidget* newWidget, QWidget* oldWidget);
void removeLayout(QLayout* layout);
void removeLayout(QWidget* widget);
void createNewWidgetOverOld(QWidget* newWidget, QWidget* oldWidget = 0);
}

#endif // UTILS_H
