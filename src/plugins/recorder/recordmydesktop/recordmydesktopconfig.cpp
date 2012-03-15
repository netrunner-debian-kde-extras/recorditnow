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


// own
#include "recordmydesktopconfig.h"
#include <recorditnow_recordmydesktop.h>
#include "sounddevicedialog.h"

// KDE
#include <kpluginfactory.h>
#include <kaboutdata.h>


K_PLUGIN_FACTORY(ConfigFactory, registerPlugin<RecordMyDesktopConfig>();)
K_EXPORT_PLUGIN(ConfigFactory("recorditnow_recordmydesktop_config"))
RecordMyDesktopConfig::RecordMyDesktopConfig(QWidget *parent, const QVariantList &args)
    : KCModule(ConfigFactory::componentData(), parent, args)
{

    setupUi(this);
    deviceButton->setIcon(KIcon("audio-card"));
    connect(deviceButton, SIGNAL(clicked()), this, SLOT(showDeviceDialog()));

    addConfig(Settings::self(), this);

}


RecordMyDesktopConfig::~RecordMyDesktopConfig()
{


}


void RecordMyDesktopConfig::showDeviceDialog()
{

    SoundDeviceDialog *dialog = new SoundDeviceDialog(this);
    connect(dialog, SIGNAL(deviceSelected(QString)), this, SLOT(deviceDialogFinished(QString)));
    dialog->show();

}


void RecordMyDesktopConfig::deviceDialogFinished(const QString &id)
{

    if (!id.isEmpty()) {
        kcfg___device->setText(id);
    }

}


#include "recordmydesktopconfig.moc"
