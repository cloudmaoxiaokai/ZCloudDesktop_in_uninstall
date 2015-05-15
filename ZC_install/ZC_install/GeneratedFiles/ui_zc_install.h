/********************************************************************************
** Form generated from reading UI file 'zc_install.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ZC_INSTALL_H
#define UI_ZC_INSTALL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_ZC_installClass
{
public:

    void setupUi(QDialog *ZC_installClass)
    {
        if (ZC_installClass->objectName().isEmpty())
            ZC_installClass->setObjectName(QStringLiteral("ZC_installClass"));
        ZC_installClass->resize(600, 400);

        retranslateUi(ZC_installClass);

        QMetaObject::connectSlotsByName(ZC_installClass);
    } // setupUi

    void retranslateUi(QDialog *ZC_installClass)
    {
        ZC_installClass->setWindowTitle(QApplication::translate("ZC_installClass", "ZC_install", 0));
    } // retranslateUi

};

namespace Ui {
    class ZC_installClass: public Ui_ZC_installClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ZC_INSTALL_H
