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


// own
#include "plugininfo.h"

// Qt
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QRegExp>
#include <QtCore/QSharedData>


namespace Joschy {


class PluginInfoPrivate : public QSharedData {
public:
    PluginInfoPrivate(PluginInfo *info)
        : q(info)
    {}


    QHash<QString, QString> pluginData;


private:
    PluginInfo *q;


};


PluginInfo::PluginInfo(const QString &pluginFile)
    : d(new PluginInfoPrivate(this))
{

    JOSCHY_DEBUG() << "Plugin file:" << pluginFile;

    QFile file(pluginFile);
    if (!file.exists() || !file.open(QIODevice::ReadOnly)) {
        JOSCHY_DEBUG() << file.error() << file.errorString();
        return;
    }

    // check the first line
    const QString line = file.readLine(1024).trimmed();
    if (line != QLatin1String("[Joschy::Plugin]")) {
        JOSCHY_DEBUG() << "this is not a valid Joschy::Plugin file!";
        return;
    }

    // fill pluginData
    const QRegExp entryRX("^.*=");
    const QRegExp valueRX("=.*$");
    while (!file.atEnd()) {
        const QString line = file.readLine(1024).trimmed();

        entryRX.indexIn(line);
        valueRX.indexIn(line);

        const QString entry = entryRX.cap().remove(valueRX);
        const QString value = valueRX.cap().remove(entryRX);

        if (!entry.isEmpty() && !value.isEmpty()) {
            d->pluginData[entry] = value;
        }
    }
    d->pluginData["Self"] = pluginFile;

    // plugin dir
    QDir dir(d->pluginData["Self"]);
    dir.cdUp();

    d->pluginData["PluginDir"] = dir.absolutePath();

    // filter
    QStringList filters;
    filters.append(QString("lib")+d->pluginData["Library"]+".*");
    filters.append(d->pluginData["Library"]+".*");

    // look for our library
    const QStringList files = dir.entryList(filters, QDir::Files);
    foreach (const QString &file, files) {
        if (file == d->pluginData["Self"]) {
            continue;
        }
        d->pluginData["Library"] = dir.absoluteFilePath(file);
        break;
    }
    Q_ASSERT(QDir().exists(d->pluginData.value("Library")));

}


PluginInfo::PluginInfo(const Joschy::PluginInfo &copy)
    : d(copy.d)
{


}


PluginInfo::PluginInfo()
    : d(new PluginInfoPrivate(this))
{


}


PluginInfo::~PluginInfo()
{



}



Joschy::PluginInfo &PluginInfo::operator=(const Joschy::PluginInfo &rhs)
{

    d = rhs.d;
    return *this;

}


bool PluginInfo::operator==(const Joschy::PluginInfo &rhs) const
{

    return d == rhs.d;

}


bool PluginInfo::operator!=(const Joschy::PluginInfo &rhs) const
{

    return d != rhs.d;

}


bool PluginInfo::operator<(const Joschy::PluginInfo &rhs) const
{

    return d < rhs.d;

}


bool PluginInfo::operator>(const Joschy::PluginInfo &rhs) const
{

    return d > rhs.d;

}


QString PluginInfo::library() const
{

    return d->pluginData.value("Library");

}


QString PluginInfo::name() const
{

    return d->pluginData.value("Name");

}


QString PluginInfo::version() const
{

    return d->pluginData.value("Version");

}


QString PluginInfo::description() const
{

    return d->pluginData.value("Description");

}


Joschy::PluginInfo::PluginType PluginInfo::type() const
{

    return static_cast<PluginType>(d->pluginData.value("Type").toInt());

}


bool PluginInfo::isValid() const
{

    if (name().isEmpty() ||
        type() == PluginInfo::InvalidType ||
        !QFile(library()).exists()
        ) {
        return false;
    }
    return true;

}


QString PluginInfo::icon() const
{

    return d->pluginData.value("PluginDir")+QDir::separator()+d->pluginData.value("Icon");

}


QStringList PluginInfo::pluginDirs()
{

    QStringList dirs;
    dirs.append(JOSCHY_PLUGINDIR);
    dirs.append(QCoreApplication::applicationDirPath()+QDir::separator()+"joschy");

    foreach (const QString &path, QCoreApplication::libraryPaths()) {
        dirs.append(path+QDir::separator()+"joschy");
    }
    dirs.removeDuplicates();

    return dirs;

}



} // namespace Joschy


