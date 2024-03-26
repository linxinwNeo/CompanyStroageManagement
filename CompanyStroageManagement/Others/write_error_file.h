#include <QFile>
#include <QTextStream>
#include <QString>
#include <QDateTime>

inline void write_error_file(const QString message)
{
    // Create a new file
    QFile file("ErrorMessage.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream out(&file);
    QDateTime dt = dt.currentDateTime();

    out << dt.toLocalTime().toString() << ": " << message;

    qDebug() << dt.toLocalTime().toString() << ": " << message;

    // optional, as QFile destructor will already do it:
    file.close();
}
