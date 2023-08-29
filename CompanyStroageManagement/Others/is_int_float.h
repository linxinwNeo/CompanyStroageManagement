#ifndef IS_INT_FLOAT_H
#define IS_INT_FLOAT_H

#include <QString>

inline bool is_float(const QString text);
inline bool is_int(const QString text);

inline bool is_int(const QString text){
    for (QChar ch : text) {
        int v = ch.toLatin1(); // ASCII Val converted
        if (!(v >= 48 && v <= 57)) {
            return false;
        }
    }
    return true;
}

inline bool is_float(const QString text){
    for (QChar ch : text) {
        int v = ch.toLatin1(); // ASCII Val converted
        if (!(v >= 48 && v <= 57) && v != 46) {
            return false;
        }
    }
    return true;
}

#endif // IS_INT_FLOAT_H
