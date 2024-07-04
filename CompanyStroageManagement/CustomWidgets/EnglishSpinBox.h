#ifndef ENGLISHSPINBOX_H
#define ENGLISHSPINBOX_H

#include <QSpinBox>
#include "GlobalVars.h"


class EnglishSpinBox : public QSpinBox {
    Q_OBJECT

public:
    EnglishSpinBox(QWidget *parent = nullptr) : QSpinBox(parent) {

    }

protected:
    QString textFromValue(int value) const override {
        return GlobalVars::locale.toString(value);
    }

    int valueFromText(const QString &text) const override {
        return GlobalVars::locale.toInt(text);
    }
};

#endif // ENGLISHSPINBOX_H
