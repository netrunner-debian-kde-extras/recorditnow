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


#ifndef KEYBOARDKEYICONPAGE_H
#define KEYBOARDKEYICONPAGE_H


// own
#include "ui_keyboardkeyiconpage.h"

// Qt
#include <QtGui/QWizardPage>


class QListWidgetItem ;
class KeyboardKeyIconPage: public QWizardPage, Ui::KeyboardKeyIconPage
{
    Q_OBJECT


public:
    explicit KeyboardKeyIconPage(QWidget *parent = 0);


private:
    struct Theme {
        QString name;
        QString description;
        QString author;
        QString license;
        QString themeDir;
    };

    QString m_iconDir;
    QList<Theme> m_themes;

    static QStringList themeDirs();


private slots:
    void currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void themeChanged(const QString &newTheme);
    void updateThemeCombo();


};


#endif // KEYBOARDKEYICONPAGE_H
