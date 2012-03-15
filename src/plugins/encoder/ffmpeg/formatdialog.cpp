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
#include "formatdialog.h"



FormatDialog::FormatDialog(const QString &format, const QString &command, QWidget *parent)
    : KDialog(parent), m_oldFormat(format)
{

    setAttribute(Qt::WA_DeleteOnClose, true);

    QWidget *widget = new QWidget(this);
    setupUi(widget);
    setMainWidget(widget);

    connect(this, SIGNAL(finished(int)), this, SLOT(dialogFinished(int)));

    if (!m_oldFormat.isEmpty()) {
        setWindowTitle(i18nc("%1 = format", "Edit %1", m_oldFormat));
        formatEdit->setText(format);
        commandEdit->setText(command);
    } else {
        setWindowTitle(i18n("Format"));
    }

}


FormatDialog::~FormatDialog()
{



}


void FormatDialog::dialogFinished(const int &ret)
{

    if (ret == KDialog::Accepted) {
        if (!m_oldFormat.isEmpty()) {
            emit editFinished(m_oldFormat, formatEdit->text(), commandEdit->text());
        } else {
            emit addFinished(formatEdit->text(), commandEdit->text());
        }
    }

}


#include "formatdialog.moc"

