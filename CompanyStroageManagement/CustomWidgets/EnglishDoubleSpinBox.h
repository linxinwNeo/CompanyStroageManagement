#ifndef ENGLISHDOUBLESPINBOX_H
#define ENGLISHDOUBLESPINBOX_H

#include <QDoubleSpinBox>
#include "GlobalVars.h"


class EnglishDoubleSpinBox : public QDoubleSpinBox {
    Q_OBJECT

public:
    EnglishDoubleSpinBox(QWidget *parent = nullptr) : QDoubleSpinBox(parent) {

    }

protected:
    QString textFromValue(double value) const override {
        extern QLocale locale;
        return locale.toString(value, 'f', 2);
    }

    double valueFromText(const QString &text) const override {
        extern QLocale locale;
        return locale.toDouble(text);
    }
};


#endif // ENGLISHDOUBLESPINBOX_H
