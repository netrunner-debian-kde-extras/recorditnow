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
#include "pluginpage.h"

// JoschyCore
#include <joschycore/manager.h>

// KDE
#include <kicon.h>
#include <kiconloader.h>
#include <kdebug.h>

// Qt
#include <QtGui/QListWidgetItem>


Q_DECLARE_METATYPE(Joschy::PluginInfo);
PluginPage::PluginPage(QWidget *parent)
    : QWizardPage(parent)
{

    setupUi(this);

    connect(providerList, SIGNAL(currentRowChanged(int)), this, SLOT(currentRowChanged(int)));

    registerField("Provider*", nameLine);

}


Joschy::PluginInfo PluginPage::plugin() const
{

    QListWidgetItem *item = providerList->item(providerList->currentRow());
    Q_ASSERT(item);

    return item->data(Qt::UserRole+1).value<Joschy::PluginInfo>();

}



void PluginPage::initializePage()
{

    providerList->clear();
    foreach (const Joschy::PluginInfo &info, Joschy::Manager::self()->availableProvider()) {
        QListWidgetItem *item = new QListWidgetItem();
        item->setText(info.name());
        item->setIcon(KIcon(info.icon()));
        item->setData(Qt::UserRole+1, QVariant::fromValue(info));

        providerList->addItem(item);
    }
    providerList->setCurrentRow(0);
    currentRowChanged(0);

}


void PluginPage::currentRowChanged(int currentRow)
{

    QListWidgetItem *item = providerList->item(currentRow);
    if (!item) {
        return;
    }

    const Joschy::PluginInfo info = item->data(Qt::UserRole+1).value<Joschy::PluginInfo>();
    nameLine->setText(info.name());
    descriptionLine->setText(info.description());
    versionLine->setText(info.version());

    setField("Provider", info.name());

}


#include "pluginpage.moc"








