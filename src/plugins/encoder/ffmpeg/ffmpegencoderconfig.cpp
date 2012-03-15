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
#include "ffmpegencoderconfig.h"
#include <recorditnow_ffmpeg.h>
#include "formatdialog.h"

// KDE
#include <kpluginfactory.h>
#include <kdialog.h>
#include <kconfiggroup.h>
#include <kdebug.h>


K_PLUGIN_FACTORY(ConfigFactory, registerPlugin<FfmpegEncoderConfig>();)
K_EXPORT_PLUGIN(ConfigFactory("recorditnow_ffmpeg_config"))
FfmpegEncoderConfig::FfmpegEncoderConfig(QWidget *parent, const QVariantList &args)
    : KCModule(ConfigFactory::componentData(), parent, args)
{

    setupUi(this);

    addButton->setIcon(KIcon("list-add"));
    editButton->setIcon(KIcon("document-edit"));
    removeButton->setIcon(KIcon("list-remove"));

    connect(formatCombo, SIGNAL(currentIndexChanged(QString)), this, SLOT(formatChanged(QString)));
    connect(addButton, SIGNAL(clicked()), this, SLOT(addClicked()));
    connect(editButton, SIGNAL(clicked()), this, SLOT(editClicked()));
    connect(removeButton, SIGNAL(clicked()), this, SLOT(removeClicked()));

    addConfig(Settings::self(), this);

}


FfmpegEncoderConfig::~FfmpegEncoderConfig()
{


}


void FfmpegEncoderConfig::defaults()
{

    m_format.clear();

    m_format[i18n("flv")] = "-i %1 -sameq -xerror %2.flv";
    m_format[i18n("avi")] = "-i %1 -sameq -xerror %2.avi";
    m_format[i18n("wmv")] = "-i %1 -sameq -xerror %2.wmv";
    m_format[i18n("mpeg")] = "-i %1 -sameq -xerror %2.mpeg";
    m_format[i18n("mkv")] = "-i %1 -sameq -xerror %2.mkv";

    formatListChanged();

}


void FfmpegEncoderConfig::load()
{

    m_format.clear();

    Settings::self()->readConfig();
    KConfigGroup cfg(Settings::self()->config(), "ffmpeg");
    const QStringList list = cfg.readEntry("List", QStringList());

    foreach (const QString &format, list) {
        m_format[format] = cfg.readEntry(format, QString());
    }

    formatListChanged();

    formatCombo->setCurrentItem(Settings::self()->format(), false);

}


void FfmpegEncoderConfig::save()
{

    const QString format = formatCombo->currentText();
    Settings::self()->setFormat(format);
    Settings::self()->setCommand(m_format[format]);

    KConfigGroup cfg(Settings::self()->config(), "ffmpeg");

    QHashIterator<QString, QString> it(m_format);
    while (it.hasNext()) {
        it.next();
        cfg.writeEntry(it.key(), it.value());
    }
    cfg.writeEntry("List", m_format.keys());

    Settings::self()->writeConfig();

}


void FfmpegEncoderConfig::addClicked()
{

    FormatDialog *dialog = new FormatDialog(QString(), QString(), this);
    connect(dialog, SIGNAL(addFinished(QString,QString)), this, SLOT(addFinished(QString, QString)));
    dialog->show();

}


void FfmpegEncoderConfig::editClicked()
{

    const QString format = formatCombo->currentText();
    if (!format.isEmpty()) {
        FormatDialog *dialog = new FormatDialog(format, m_format[format], this);
        connect(dialog, SIGNAL(editFinished(QString,QString, QString)), this,
                SLOT(editFinished(QString, QString, QString)));
        dialog->show();
    }

}


void FfmpegEncoderConfig::removeClicked()
{

    m_format.remove(formatCombo->currentText());
    formatListChanged();
    changed();

}


void FfmpegEncoderConfig::formatChanged(const QString &newFormat)
{

    if (!newFormat.isEmpty() && m_format.contains(newFormat)) {
        previewEdit->setText(m_format[newFormat]);
    }
    changed();

}


void FfmpegEncoderConfig::formatListChanged()
{

    const QString format = formatCombo->currentText();
    formatCombo->clear();

    if (m_format.isEmpty()) {
        defaults();
    } else {
        formatCombo->addItems(m_format.keys());
    }

    formatCombo->setCurrentItem(format, false);

}


void FfmpegEncoderConfig::editFinished(const QString &oldName, const QString &newName,
                                       const QString &command)
{

    m_format.remove(oldName);
    m_format[newName] = command;
    formatListChanged();
    changed();

}


void FfmpegEncoderConfig::addFinished(const QString &format, const QString &command)
{

    m_format[format] = command;
    formatListChanged();
    changed();

}


#include "ffmpegencoderconfig.moc"
