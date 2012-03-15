/*
    Copyright (C) 2010  Kai Dombrowe <just89@gmx.de>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef PLUGININFO_H
#define PLUGININFO_H


// own
#include "joschy_global.h"

// Qt
#include <QtCore/QString>
#include <QtCore/QHash>
#include <QtCore/QCoreApplication>
#include <QtCore/QExplicitlySharedDataPointer>


namespace Joschy {


class PluginInfoPrivate;


/**
 * @class PluginInfo joschycore/plugininfo.h
 *
 * @short Information about a plugin.
 *
 * This holds all the information about a plugin there is.
 **/
class JOSCHY_EXPORT PluginInfo
{


public:
    /**
     * Plugin types
     **/
    enum PluginType {
        InvalidType = -1,         /**< A invalid plugin */
        NetworkLayerType = 0,     /**< A network layer plugin */
        ProviderType = 1          /**< A provider plugin */
    };

    /**
     * Read plugin info from pluginFile\n
     * The file should be of the following form:
     * \verbatim
      [Joschy::Plugin]
      Name=QNetworkLayer
      Description=Qt based layer
      Icon=myIcon.png
      Version=0.1
      Type=0
      Library=joschyqnetworklayer
      \endverbatim
     **/
    PluginInfo(const QString &pluginFile);
    /**
     * Creates a PluginInfo object that shares the data with copy.
     **/
    PluginInfo(const Joschy::PluginInfo &copy);
    /**
     * Creates an invalid plugin.
     **/
    PluginInfo();
    ~PluginInfo();

    Joschy::PluginInfo &operator=(const Joschy::PluginInfo &rhs);
    bool operator==(const Joschy::PluginInfo &rhs) const;
    bool operator!=(const Joschy::PluginInfo &rhs) const;
    bool operator<(const Joschy::PluginInfo &rhs) const;
    bool operator>(const Joschy::PluginInfo &rhs) const;


    /**
     * @return Returns the path to the library
     **/
    QString library() const;
    /**
     * @return Returns the plugin name
     **/
    QString name() const;
    /**
     * @return Returns the plugin version
     **/
    QString version() const;
    /**
     * @return Returns the plugin description
     **/
    QString description() const;
    /**
     * @return Returns the plugin type
     **/
    Joschy::PluginInfo::PluginType type() const;
    /**
     * @return Returns whether the object is valid.
     **/
    bool isValid() const;
    /**
     * @return Returns the icon
     **/
    QString icon() const;


    /**
     * @internal
     **/
    static QStringList pluginDirs();


private:
    QExplicitlySharedDataPointer<PluginInfoPrivate> d;


};


} // namespace Joschy


#endif // PLUGININFO_H
