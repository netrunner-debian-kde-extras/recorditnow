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

#ifndef MOUSECONFIG_H
#define MOUSECONFIG_H


// own
#include "config/configpage.h"
#include "ui_mouseconfig.h"
#include "mousebutton.h"

// Qt
#include <QtGui/QWidget>


class QButtonGroup;
class QToolButton;
class KColorButton;
class MouseConfig : public RecordItNow::ConfigPage, Ui::MouseConfig
{
    Q_OBJECT


public:
    explicit MouseConfig(KConfig *cfg, QWidget *parent = 0);
    ~MouseConfig();

    void saveConfig();
    void loadConfig();
    void setDefaults();


    static QHash<int, QColor> getButtons(KConfig *cfg);


private:
    KColorButton *newButton();
    QToolButton *newRemoveButton();
    MouseButton *newMouseButton();
    QButtonGroup *m_visibleGroup;

    bool contains(const MouseButton::Button &button, QWidget *exclude = 0) const;


private slots:
    void addClicked();
    void removeClicked();
    void updateColumnSize();
    void buttonChanged(const MouseButton::Button &oldButton, const MouseButton::Button &newButton);
    void showKeyMonDialog();
    void buttonsChanged();
    void modeChanged();
    void currentButtonChanged();


};


#endif // MOUSECONFIG_H
