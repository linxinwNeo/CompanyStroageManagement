#ifndef SUBMITENTRY_H
#define SUBMITENTRY_H

#include <QDialog>

namespace Ui {
class submitEntry;
}

class submitEntry : public QDialog
{
    Q_OBJECT

public:
    explicit submitEntry(QWidget *parent = nullptr);
    ~submitEntry();

private:
    Ui::submitEntry *ui;
};

#endif // SUBMITENTRY_H
