/***************************************************************************
 *   Copyright (C) 2010 by Kai Dombrowe <just89@gmx.de>                    *
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

#ifndef PLUGINCONFIG_H
#define PLUGINCONFIG_H


// own
#include "configpage.h"
#include "ui_pluginconfig.h"


class RecordItNowPluginManager;
class PluginConfig : public RecordItNow::ConfigPage, Ui::PluginConfig
{
    Q_OBJECT


public:
    explicit PluginConfig(RecordItNowPluginManager *manager, KConfig *cfg, QWidget *parent = 0);


public slots:
    void saveConfig();
    void setDefaults();
    void loadConfig();


private:
    RecordItNowPluginManager *m_manager;


private slots:
    void pluginSettingsChanged(const bool &changed);


};


#endif // PLUGINCONFIG_H
