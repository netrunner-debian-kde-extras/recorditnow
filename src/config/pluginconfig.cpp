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


// own
#include "pluginconfig.h"
#include "recorditnowpluginmanager.h"

// KDE
#include <klocalizedstring.h>


PluginConfig::PluginConfig(RecordItNowPluginManager *manager, KConfig *cfg, QWidget *parent)
    : RecordItNow::ConfigPage(cfg, parent), m_manager(manager)
{

    setupUi(this);
    connect(pluginSelector, SIGNAL(changed(bool)), this, SLOT(pluginSettingsChanged(bool)));

}


void PluginConfig::saveConfig()
{

    pluginSelector->updatePluginsState();
    pluginSelector->save();

}


void PluginConfig::setDefaults()
{


}


void PluginConfig::loadConfig()
{

    pluginSelector->addPlugins(m_manager->getRecorderList(),
                               KPluginSelector::ReadConfigFile,
                               i18n("Record Plugins"), "Recorder");
    pluginSelector->addPlugins(m_manager->getEncoderList(),
                               KPluginSelector::ReadConfigFile,
                               i18n("Encode Plugins"), "Encoder");

}



void PluginConfig::pluginSettingsChanged(const bool &changed)
{

    emit configChanged();
    if (!changed) {
        return;
    }
    pluginSelector->updatePluginsState();

}


#include "pluginconfig.moc"
