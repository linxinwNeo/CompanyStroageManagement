#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

#include <QString>

inline bool is_float(const QString text);
inline bool is_int(const QString text);

bool is_int(const QString text){
    for (QChar ch : text) {
        int v = ch.toLatin1(); // ASCII Val converted
        if (!(v >= 48 && v <= 57)) {
            return false;
        }
    }
    return true;
}

bool is_float(const QString text){
    for (QChar ch : text) {
        int v = ch.toLatin1(); // ASCII Val converted
        if (!(v >= 48 && v <= 57) && v != 46) {
            return false;
        }
    }
    return true;
}

#endif // HELPER_FUNCTIONS_H
