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


#ifndef KEYBOARDCONFIG_H
#define KEYBOARDCONFIG_H


// own
#include "config/configpage.h"
#include "ui_keyboardconfig.h"
#include "../keyboard/keyboardkey.h"

// Qt
#include <QtGui/QWidget>


class KConfig;
class KeyboardConfig : public RecordItNow::ConfigPage, Ui::KeyboardConfig
{
    Q_OBJECT


public:
    explicit KeyboardConfig(KConfig *cfg, QWidget *parent = 0);

    static QList<KeyboardKey> readConfig(KConfig *cfg);

    void saveConfig();
    void loadConfig();
    void setDefaults();


private slots:
    void add();
    void remove();
    void up();
    void down();
    void itemSelectionChanged();
    void wizardFinished(const int &key, const QString &icon, const QString &text);
    void showSearchDialog();
    void searchDialogFinished(const QString &uuid);
    void textChanged(const QString &text);


};


#endif // KEYBOARDCONFIG_H
