/***************************************************************************
 *   Copyright (C) 2009 by Kai Dombrowe <just89@gmx.de>                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#ifndef JOSCHY_GLOBAL_H
#define JOSCHY_GLOBAL_H

// Qt
#include <QtCore/qglobal.h>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QTime>


#if defined(JOSCHY_LIBRARY)
    #define JOSCHY_EXPORT Q_DECL_EXPORT
    #define GET_TIME() QTime::currentTime().toString("hh:mm:ss:zzz")
    #define GET_FILE(path) QString(path).remove(0, QString(path).lastIndexOf(QDir::separator())+1)
    #define DEBUG_STRING QString("%1: %2: line %3 -->").arg(GET_TIME()).arg(GET_FILE(__FILE__)).arg(__LINE__)
    #define JOSCHY_DEBUG() qDebug() << DEBUG_STRING << __PRETTY_FUNCTION__
#else
    #define JOSCHY_EXPORT Q_DECL_IMPORT
#endif



#define JOSCHY_SINGLETON(TYPE, NAME) \
typedef void (*JCleanUpFunction)(); \
class JCleanUpGlobalStatic \
{ \
public: \
    JCleanUpFunction func; \
    inline ~JCleanUpGlobalStatic() { func(); } \
}; \
static QBasicAtomicPointer<TYPE> _j_static_##NAME = Q_BASIC_ATOMIC_INITIALIZER(0); \
static bool _j_static_##NAME##_destroyed;                                          \
static struct NAME {                                                               \
    inline bool isDestroyed() const                                                \
    {                                                                              \
        return _j_static_##NAME##_destroyed;                                       \
    }                                                                              \
    inline bool exists() const                                                     \
    {                                                                              \
        return _j_static_##NAME != 0;                                              \
    }                                                                              \
    inline operator TYPE*()                                                        \
    {                                                                              \
        return operator->();                                                       \
    }                                                                              \
    inline TYPE *operator->()                                                      \
    {                                                                              \
        if (!_j_static_##NAME) {                                                   \
            if (isDestroyed()) {                                                   \
                qFatal("Fatal Error: Accessed global static '%s *%s()' after destruction. " \
                       "Defined at %s:%d", #TYPE, #NAME, __FILE__, __LINE__);               \
            }                                                                               \
            TYPE *x = new TYPE;                                                             \
            if (!_j_static_##NAME.testAndSetOrdered(0, x) && _j_static_##NAME != x ) {      \
                delete x;                                                                   \
            } else {                                                                        \
                static JCleanUpGlobalStatic cleanUpObject = { destroy };                    \
            }                                                                               \
        }                                                                                   \
        return _j_static_##NAME;                                                            \
    }                                                                                       \
    inline TYPE &operator*()                                                                \
    {                                                                                       \
        return *operator->();                                                               \
    }                                                                                       \
    static void destroy()                                                                   \
    {                                                                                       \
        _j_static_##NAME##_destroyed = true;                                                \
        TYPE *x = _j_static_##NAME;                                                         \
        _j_static_##NAME = 0;                                                               \
        delete x;                                                                           \
    }                                                                                       \
} NAME;


#endif // JOSCHY_GLOBAL_H
