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
#include "keyboardwizard.h"
#include "keyboardkeypage.h"
#include "keyboardkeyiconpage.h"

// KDE
#include <kicon.h>
#include <kiconloader.h>


KeyboardWizard::KeyboardWizard(const QString &device, QWidget *parent)
    : QWizard(parent)
{

    setAttribute(Qt::WA_DeleteOnClose);
    setPixmap(QWizard::LogoPixmap, KIcon("input-keyboard").pixmap(KIconLoader::SizeMedium,
                                                                  KIconLoader::SizeMedium));

    addPage(new KeyboardKeyPage(device, this));
    addPage(new KeyboardKeyIconPage(this));

    connect(this, SIGNAL(finished(int)), this, SLOT(wizardFinished(int)));

}


void KeyboardWizard::wizardFinished(int result)
{

    if (result == QDialog::Accepted) {
        emit wizardFinished(field("KeyCode").toInt(), field("Icon").toString(),
                            field("Text").toString());
    }

}


#include "keyboardwizard.moc"
