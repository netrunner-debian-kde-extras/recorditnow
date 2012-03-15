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


#ifndef DEVICESEARCHDIALOG_H
#define DEVICESEARCHDIALOG_H


// own
#include "ui_devicesearchdialog.h"
#include "keymon/deviceinfo.h"

// KDE
#include <kdialog.h>


class DeviceSearchDialog : public KDialog, Ui::DeviceSearchDialog
{
    Q_OBJECT


public:
    explicit DeviceSearchDialog(const KeyMon::DeviceInfo::DeviceType &type, QWidget *parent = 0);


private:
    bool m_input;


private slots:
    void dialogFinished(const int &ret);


signals:
    void deviceSelected(const QString &file);



};


#endif // DEVICESEARCHDIALOG_H
