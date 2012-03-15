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
#include "ffmpegrecorderconfig.h"
#include <recorditnow_ffmpegrecorder.h>

// KDE
#include <kpluginfactory.h>
#include <kaboutdata.h>
#include <kdebug.h>


K_PLUGIN_FACTORY(ConfigFactory, registerPlugin<FfmpegRecorderConfig>();)
K_EXPORT_PLUGIN(ConfigFactory("recorditnow_recordmydesktop_config"))
FfmpegRecorderConfig::FfmpegRecorderConfig(QWidget *parent, const QVariantList &args)
    : KCModule(ConfigFactory::componentData(), parent, args)
{

    setupUi(this);
    addConfig(Settings::self(), this);

    connect(formatCombo, SIGNAL(currentIndexChanged(QString)), this, SLOT(formatChanged(QString)));

}


FfmpegRecorderConfig::~FfmpegRecorderConfig()
{


}


void FfmpegRecorderConfig::defaults()
{

    KCModule::defaults();
    formatCombo->setCurrentItem("mp4");

}


void FfmpegRecorderConfig::load()
{

    KCModule::load();
    Settings::self()->readConfig();
    formatCombo->setCurrentItem(Settings::format());

}


void FfmpegRecorderConfig::save()
{

    KCModule::save();
    Settings::self()->setFormat(formatCombo->currentText());
    Settings::self()->writeConfig();

}


void FfmpegRecorderConfig::formatChanged(const QString &format)
{

    unmanagedWidgetChangeState(Settings::format() != format);

}



#include "ffmpegrecorderconfig.moc"
