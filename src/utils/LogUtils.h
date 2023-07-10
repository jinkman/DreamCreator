#ifndef LOGUTILS_H
#define LOGUTILS_H

#include <QDebug> //注意，头文件一定要添加

#define DMLOG qDebug() << "[" << __FILE__ << ":" << __LINE__ << ":" << __func__ << __DATE__ << __TIME__ << "]"

#endif // LOGUTILS_H