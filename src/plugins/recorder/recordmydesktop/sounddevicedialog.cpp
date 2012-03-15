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
#include "sounddevicedialog.h"
#include "sounddevice.h"

// KDE
#include <kicon.h>
#include <klocalizedstring.h>

// Qt
#include <QtGui/QTreeWidgetItem>


SoundDeviceDialog::SoundDeviceDialog(QWidget *parent)
    : KDialog(parent)
{

    setWindowTitle(i18n("Sound Device"));
    setAttribute(Qt::WA_DeleteOnClose);
    QWidget *widget = new QWidget(this);
    setupUi(widget);
    setMainWidget(widget);

    foreach (const SoundDevice &dev, SoundDevice::getDeviceList()) {
        QTreeWidgetItem *item = new QTreeWidgetItem;
        item->setText(0, dev.name());
        item->setIcon(0, KIcon(dev.icon()));
        item->setText(1, dev.key());

        treeWidget->addTopLevelItem(item);
    }
    treeWidget->header()->setResizeMode(QHeaderView::ResizeToContents);
    connect(this, SIGNAL(finished(int)), this, SLOT(dialogFinished(int)));

    resize(600, 300);

}



SoundDeviceDialog::~SoundDeviceDialog()
{




}


void SoundDeviceDialog::dialogFinished(const int &ret)
{

    if (ret == KDialog::Accepted) {
        QList<QTreeWidgetItem*> items = treeWidget->selectedItems();
        if (!items.isEmpty()) {
            emit deviceSelected(items[0]->text(1));
        }
    }

}


#include "sounddevicedialog.moc"

