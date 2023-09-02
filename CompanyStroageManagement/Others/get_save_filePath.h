#ifndef GET_SAVE_FILEPATH_H
#define GET_SAVE_FILEPATH_H

#include <QFileDialog>
#include <QStandardPaths>
#include <QString>

inline QString get_save_filePath(const QString default_fname, const QString window_Title,
                                 const QString filter)
{
    QFileDialog saveFileDialog;
    saveFileDialog.setFileMode(QFileDialog::AnyFile);
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    saveFileDialog.setDirectory(desktopPath);
    saveFileDialog.setWindowTitle(window_Title);
    saveFileDialog.setNameFilter(filter);
    saveFileDialog.selectFile(default_fname);

    QString filename;
    if (saveFileDialog.exec()) {
        // Do something with the selected file
        filename = saveFileDialog.selectedFiles().first();
    }

    return filename;
}

#endif // GET_SAVE_FILEPATH_H
