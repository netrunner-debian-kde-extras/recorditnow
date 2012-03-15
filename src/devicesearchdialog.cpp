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
#include "devicesearchdialog.h"
#include "keymonmanager.h"

// KDE
#include <klocalizedstring.h>
#include <kmessagebox.h>

// Qt
#include <QtGui/QTreeWidgetItem>


DeviceSearchDialog::DeviceSearchDialog(const KeyMon::DeviceInfo::DeviceType &type, QWidget *parent)
    : KDialog(parent)
{

    QWidget *main = new QWidget(this);
    setupUi(main);
    setMainWidget(main);

    setAttribute(Qt::WA_DeleteOnClose);
    resize(500, 300);

    switch (type) {
    case KeyMon::DeviceInfo::MouseType:
    case KeyMon::DeviceInfo::KeyboardType: {
            foreach (const KeyMon::DeviceInfo &info, KeyMonManager::self()->getInputDeviceList()) {
                if (info.type != type) {
                    continue;
                }
                QTreeWidgetItem *item = new QTreeWidgetItem;
                if (!info.icon.isEmpty()) {
                    item->setIcon(0, KIcon(info.icon));
                }
                item->setText(0, info.name);
                item->setText(1, info.file);
                item->setText(2, info.uuid);
                treeWidget->addTopLevelItem(item);
            }
        }
    default: break;
    }

    switch (type) {
    case KeyMon::DeviceInfo::MouseType: setWindowTitle(i18n("Mouse"));  break;
    case KeyMon::DeviceInfo::KeyboardType: setWindowTitle(i18n("Keyboard")); break;
    default: break;
    }

    connect(this, SIGNAL(finished(int)), this, SLOT(dialogFinished(int)));
    treeWidget->header()->setResizeMode(QHeaderView::ResizeToContents);


    if (treeWidget->topLevelItemCount() == 0) {
        KMessageBox::information(this, i18n("No devices found."));
        reject();
    }

}


void DeviceSearchDialog::dialogFinished(const int &ret)
{

    if (ret == KDialog::Accepted) {
        QList<QTreeWidgetItem*> items = treeWidget->selectedItems();
        if (items.isEmpty()) {
            return;
        }
        emit deviceSelected(items.first()->text(2));
    }

}


#include "devicesearchdialog.moc"
