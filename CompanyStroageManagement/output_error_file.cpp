#include <QFile>
#include <QTextStream>
#include <QString>

inline void write_error_file(const QString message)
{
    // Create a new file
    QFile file("ErrorMessage.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream out(&file);
    out << message;

    // optional, as QFile destructor will already do it:
    file.close();
}
