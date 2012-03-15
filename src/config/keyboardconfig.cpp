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
#include "keyboardconfig.h"
#include "keyboardwizard.h"
#include "devicesearchdialog.h"

// KDE
#include <kicon.h>
#include <kmessagebox.h>
#include <kconfiggroup.h>

// Qt
#include <QtGui/QListWidget>


KeyboardConfig::KeyboardConfig(KConfig *cfg, QWidget *parent)
    : RecordItNow::ConfigPage(cfg, parent)
{

    setupUi(this);

    addButton->setIcon(KIcon("list-add"));
    removeButton->setIcon(KIcon("list-remove"));
    upButton->setIcon(KIcon("go-up"));
    downButton->setIcon(KIcon("go-down"));
    searchButton->setIcon(KIcon("system-search"));

    connect(addButton, SIGNAL(clicked()), this, SLOT(add()));
    connect(removeButton, SIGNAL(clicked()), this, SLOT(remove()));
    connect(upButton, SIGNAL(clicked()), this, SLOT(up()));
    connect(downButton, SIGNAL(clicked()), this, SLOT(down()));
    connect(listWidget, SIGNAL(itemSelectionChanged()), this, SLOT(itemSelectionChanged()));
    connect(searchButton, SIGNAL(clicked()), this, SLOT(showSearchDialog()));
    connect(kcfg_keyboardDevice, SIGNAL(textChanged(QString)), this, SLOT(textChanged(QString)));

    itemSelectionChanged();

}


QList<KeyboardKey> KeyboardConfig::readConfig(KConfig *cfg)
{

    KConfigGroup group(cfg, "Keyboard");
    const int count = group.readEntry("Keys", 0);
    QList<KeyboardKey> keyMap;

    for (int i = 0; i < count; i++) {
        const int key = group.readEntry(QString("Key %1 Code").arg(i), -1);
        const QString icon = group.readEntry(QString("Key %1 Icon").arg(i), QString());
        const QString text = group.readEntry(QString("Key %1 Text").arg(i), QString());

        keyMap.append(KeyboardKey(key, icon, text));
    }

    return keyMap;

}


void KeyboardConfig::loadConfig()
{

    QList<KeyboardKey> list(readConfig(config()));
    foreach (const KeyboardKey &key, list) {
        QListWidgetItem *item = new QListWidgetItem();
        item->setText(key.text());
        item->setIcon(KIcon(key.icon()));
        item->setData(Qt::UserRole+1, key.code());
        item->setData(Qt::UserRole+2, key.icon());

        listWidget->addItem(item);
    }

}


void KeyboardConfig::saveConfig()
{

    KConfigGroup group(config(), "Keyboard");
    int count = 0;
    for (int i = 0; i < listWidget->count(); i++) {
        QListWidgetItem *item = listWidget->item(i);
        group.writeEntry(QString("Key %1 Code").arg(i), item->data(Qt::UserRole+1).toInt());
        group.writeEntry(QString("Key %1 Icon").arg(i), item->data(Qt::UserRole+2).toString());
        group.writeEntry(QString("Key %1 Text").arg(i), item->text());
        count++;
    }
    group.writeEntry("Keys", count);

}


void KeyboardConfig::setDefaults()
{

    listWidget->clear();

}


void KeyboardConfig::add()
{

    KeyboardWizard *wizard = new KeyboardWizard(kcfg_keyboardDevice->text(), this);
    connect(wizard, SIGNAL(wizardFinished(int, QString, QString)), this,
            SLOT(wizardFinished(int, QString, QString)));

    wizard->show();

}


void KeyboardConfig::remove()
{

    QList<QListWidgetItem*> items = listWidget->selectedItems();
    if (items.isEmpty()) {
        return;
    }

    QListWidgetItem *item = items.first();

    listWidget->takeItem(listWidget->row(item));
    delete item;

    emit configChanged();

}


void KeyboardConfig::up()
{

    QList<QListWidgetItem*> items = listWidget->selectedItems();
    if (items.isEmpty()) {
        return;
    }

    QListWidgetItem *item = items.first();
    const int index = listWidget->row(item);

    listWidget->takeItem(index);
    listWidget->insertItem(index-1, item);

    listWidget->setCurrentItem(item);

    emit configChanged();

}


void KeyboardConfig::down()
{

    QList<QListWidgetItem*> items = listWidget->selectedItems();
    if (items.isEmpty()) {
        return;
    }

    QListWidgetItem *item = items.first();
    const int index = listWidget->row(item);

    listWidget->takeItem(index);
    listWidget->insertItem(index+1, item);

    listWidget->setCurrentItem(item);

    emit configChanged();

}


void KeyboardConfig::itemSelectionChanged()
{

    if(listWidget->selectedItems().isEmpty()) {
        upButton->setEnabled(false);
        downButton->setEnabled(false);
        removeButton->setEnabled(false);
    } else {
        const int index = listWidget->row(listWidget->selectedItems().first());
        upButton->setEnabled(index != 0);
        downButton->setEnabled(index != listWidget->count()-1);
        removeButton->setEnabled(true);
    }

}


void KeyboardConfig::wizardFinished(const int &key, const QString &icon, const QString &text)
{

    for (int i = 0; i < listWidget->count(); i++) {
        if (listWidget->item(i)->data(Qt::UserRole+1).toInt() == key) {
            KMessageBox::error(this, i18n("There is already a key with code '%1' in your list!", key));
            return;
        }

    }

    QListWidgetItem *item = new QListWidgetItem();
    item->setText(text);
    item->setIcon(KIcon(icon));
    item->setData(Qt::UserRole+1, key);
    item->setData(Qt::UserRole+2, icon);

    listWidget->addItem(item);

    emit configChanged();

}


void KeyboardConfig::showSearchDialog()
{

    DeviceSearchDialog *dialog = new DeviceSearchDialog(KeyMon::DeviceInfo::KeyboardType, this);
    connect(dialog, SIGNAL(deviceSelected(QString)), this, SLOT(searchDialogFinished(QString)));

    dialog->show();

}


void KeyboardConfig::searchDialogFinished(const QString &uuid)
{

    kcfg_keyboardDevice->setText(uuid);

}


void KeyboardConfig::textChanged(const QString &text)
{

    addButton->setDisabled(text.isEmpty());

}


#include "keyboardconfig.moc"

