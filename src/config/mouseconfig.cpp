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
#include "mouseconfig.h"
#include "mousebutton.h"
#include "devicesearchdialog.h"

// KDE
#include <kicon.h>
#include <kcolorbutton.h>
#include <kmessagebox.h>
#include <kwindowsystem.h>

// Qt
#include <QtGui/QTreeWidget>
#include <QtGui/QToolButton>
#include <QtGui/QButtonGroup>


MouseConfig::MouseConfig(KConfig *cfg, QWidget *parent)
    : RecordItNow::ConfigPage(cfg, parent)
{

    setupUi(this);

    addButton->setIcon(KIcon("list-add"));
    keyMonButton->setIcon(KIcon("system-search"));

    connect(addButton, SIGNAL(clicked()), this, SLOT(addClicked()));
    connect(keyMonButton, SIGNAL(clicked()), this, SLOT(showKeyMonDialog()));

    buttonCombo->addItem(MouseButton::getName(MouseButton::LeftButton));
    buttonCombo->addItem(MouseButton::getName(MouseButton::RightButton));
    buttonCombo->addItem(MouseButton::getName(MouseButton::MiddleButton));
    buttonCombo->addItem(MouseButton::getName(MouseButton::WheelUp));
    buttonCombo->addItem(MouseButton::getName(MouseButton::WheelDown));
    buttonCombo->addItem(MouseButton::getName(MouseButton::SpecialButton1));
    buttonCombo->addItem(MouseButton::getName(MouseButton::SpecialButton2));

    treeWidget->header()->setResizeMode(QHeaderView::ResizeToContents);
    cursorWidget->switchToPreviewMode();

    connect(this, SIGNAL(configChanged()), this, SLOT(buttonsChanged()));
    connect(kcfg_cursorWidgetSize, SIGNAL(valueChanged(int)), this, SLOT(buttonsChanged()));
    connect(kcfg_led, SIGNAL(toggled(bool)), this, SLOT(buttonsChanged()));
    connect(kcfg_cursorOpacity, SIGNAL(valueChanged(int)), this, SLOT(buttonsChanged()));
    connect(kcfg_circle, SIGNAL(toggled(bool)), this, SLOT(modeChanged()));
    connect(buttonCombo, SIGNAL(currentIndexChanged(QString)), this, SLOT(currentButtonChanged()));
    connect(treeWidget, SIGNAL(itemChanged(QTreeWidgetItem*,int)), this, SLOT(currentButtonChanged()));

    m_visibleGroup = new QButtonGroup(this);
    m_visibleGroup->addButton(kcfg_mouseWidgetInvisible);
    m_visibleGroup->addButton(kcfg_mouseWidgetAlwaysVisible);
    m_visibleGroup->setExclusive(true);

    if (!KWindowSystem::compositingActive()) {
        kcfg_circle->setText(i18n("Square"));
    }

    buttonsChanged();
    currentButtonChanged();

}


MouseConfig::~MouseConfig()
{

    delete m_visibleGroup;

}


void MouseConfig::saveConfig()
{

    KConfigGroup cfgGroup(config(), "Mouse");

    int buttons = 0;
    for (int i = 0; i < treeWidget->topLevelItemCount(); i++) {
        QTreeWidgetItem *item = treeWidget->topLevelItem(i);
        const int button =  static_cast<MouseButton*>(treeWidget->itemWidget(item, 1))->getXButton();
        const QColor color = static_cast<KColorButton*>(treeWidget->itemWidget(item, 2))->color();

        cfgGroup.writeEntry(QString("Button %1 key").arg(QString::number(buttons)), button);
        cfgGroup.writeEntry(QString("Button %1 color").arg(QString::number(buttons)), color);

        buttons++;
    }
    cfgGroup.writeEntry("Buttons", buttons);

}


void MouseConfig::loadConfig()
{

    QHash<int, QColor> buttons = getButtons(config());
    QHashIterator<int, QColor> it(buttons);
    while (it.hasNext()) {
        it.next();

        QTreeWidgetItem *item = new QTreeWidgetItem();
        KColorButton *button = newButton();

        button->setColor(it.value());

        MouseButton *mouseButton = newMouseButton();
        mouseButton->setXButton(it.key());

        treeWidget->addTopLevelItem(item);
        treeWidget->setItemWidget(item, 0, newRemoveButton());
        treeWidget->setItemWidget(item, 1, mouseButton);
        treeWidget->setItemWidget(item, 2, button);

    }

    if (buttons.isEmpty()) {
        setDefaults();
    }
    currentButtonChanged();

}


void MouseConfig::setDefaults()
{

    treeWidget->clear();

    MouseButton *mouseButton1 = newMouseButton();
    MouseButton *mouseButton3 = newMouseButton();
    MouseButton *mouseButton4 = newMouseButton();
    MouseButton *mouseButton5 = newMouseButton();
    MouseButton *mouseButton8 = newMouseButton();
    MouseButton *mouseButton9 = newMouseButton();

    mouseButton1->setButton(MouseButton::LeftButton);
    mouseButton3->setButton(MouseButton::RightButton);
    mouseButton4->setButton(MouseButton::WheelUp);
    mouseButton5->setButton(MouseButton::WheelDown);
    mouseButton8->setButton(MouseButton::SpecialButton1);
    mouseButton9->setButton(MouseButton::SpecialButton2);

    KColorButton *button1 = newButton();
    KColorButton *button3 = newButton();
    KColorButton *button4 = newButton();
    KColorButton *button5 = newButton();
    KColorButton *button8 = newButton();
    KColorButton *button9 = newButton();

    button1->setColor(Qt::red);
    button3->setColor(Qt::yellow);
    button4->setColor(Qt::darkBlue);
    button5->setColor(Qt::blue);
    button8->setColor(Qt::magenta);
    button9->setColor(Qt::darkMagenta);

    QTreeWidgetItem *item1 = new QTreeWidgetItem();
    QTreeWidgetItem *item3 = new QTreeWidgetItem();
    QTreeWidgetItem *item4 = new QTreeWidgetItem();
    QTreeWidgetItem *item5 = new QTreeWidgetItem();
    QTreeWidgetItem *item8 = new QTreeWidgetItem();
    QTreeWidgetItem *item9 = new QTreeWidgetItem();

    // left click
    treeWidget->addTopLevelItem(item1);
    treeWidget->setItemWidget(item1, 0, newRemoveButton());
    treeWidget->setItemWidget(item1, 1, mouseButton1);
    treeWidget->setItemWidget(item1, 2, button1);

    // right click
    treeWidget->addTopLevelItem(item3);
    treeWidget->setItemWidget(item3, 0, newRemoveButton());
    treeWidget->setItemWidget(item3, 1, mouseButton3);
    treeWidget->setItemWidget(item3, 2, button3);

    // mouse wheel
    treeWidget->addTopLevelItem(item4);
    treeWidget->setItemWidget(item4, 0, newRemoveButton());
    treeWidget->setItemWidget(item4, 1, mouseButton4);
    treeWidget->setItemWidget(item4, 2, button4);
    treeWidget->addTopLevelItem(item5);
    treeWidget->setItemWidget(item5, 0, newRemoveButton());
    treeWidget->setItemWidget(item5, 1, mouseButton5);
    treeWidget->setItemWidget(item5, 2, button5);

    // special 1
    treeWidget->addTopLevelItem(item8);
    treeWidget->setItemWidget(item8, 0, newRemoveButton());
    treeWidget->setItemWidget(item8, 1, mouseButton8);
    treeWidget->setItemWidget(item8, 2, button8);

    // special 2
    treeWidget->addTopLevelItem(item9);
    treeWidget->setItemWidget(item9, 0, newRemoveButton());
    treeWidget->setItemWidget(item9, 1, mouseButton9);
    treeWidget->setItemWidget(item9, 2, button9);

}


QHash<int, QColor> MouseConfig::getButtons(KConfig *cfg)
{

    QHash<int, QColor> buttons;

    KConfigGroup cfgGroup(cfg, "Mouse");

    int keys = cfgGroup.readEntry("Buttons", 0);
    for (int i = 0; i < keys; i++) {
        const QString button = QString::number(i);
        const int key = cfgGroup.readEntry(QString("Button %1 key").arg(button), 0);
        const QColor color = cfgGroup.readEntry(QString("Button %1 color").arg(button), QColor());

        buttons[key] = color;
    }

    return buttons;

}


KColorButton *MouseConfig::newButton()
{

    KColorButton *button = new KColorButton(this);
    connect(button, SIGNAL(changed(QColor)), this, SIGNAL(configChanged()));
    return button;

}


QToolButton *MouseConfig::newRemoveButton()
{

    QToolButton *remove = new QToolButton(this);
    remove->setMaximumWidth(KIconLoader::SizeMedium);
    remove->setIcon(KIcon("list-remove"));
    connect(remove, SIGNAL(clicked()), this, SLOT(removeClicked()));
    return remove;

}


MouseButton *MouseConfig::newMouseButton()
{

    MouseButton *button = new MouseButton(this);
    connect(button, SIGNAL(sizeChanged()), this, SLOT(updateColumnSize()));
    connect(button, SIGNAL(buttonChanged(MouseButton::Button,MouseButton::Button)), this,
            SLOT(buttonChanged(MouseButton::Button,MouseButton::Button)));
    return button;

}


bool MouseConfig::contains(const MouseButton::Button &button, QWidget *exclude) const
{

    for (int i = 0; i < treeWidget->topLevelItemCount(); i++) {
        QTreeWidgetItem *item = treeWidget->topLevelItem(i);
        const MouseButton *widget = static_cast<MouseButton*>(treeWidget->itemWidget(item, 1));

        if (exclude && exclude == widget) {
            continue;
        }

        if (widget->getMouseButton() == button) {
            return true;
        }
    }
    return false;

}


void MouseConfig::addClicked()
{

    MouseButton::Button mButton = MouseButton::getButtonFromName(buttonCombo->currentText());
    if (mButton == MouseButton::NoButton) {
        return;
    }

    if (contains(mButton)) {
        return;
    }

    MouseButton *button = newMouseButton();
    button->setButton(mButton);

    QTreeWidgetItem *item = new QTreeWidgetItem();

    treeWidget->addTopLevelItem(item);
    treeWidget->setItemWidget(item, 0, newRemoveButton());
    treeWidget->setItemWidget(item, 1, button);
    treeWidget->setItemWidget(item, 2, newButton());

    emit configChanged();

}


void MouseConfig::removeClicked()
{

    for (int i = 0; i < treeWidget->topLevelItemCount(); i++) {
        QTreeWidgetItem *item = treeWidget->topLevelItem(i);
        if (sender() == treeWidget->itemWidget(item, 0)) {
            treeWidget->invisibleRootItem()->removeChild(item);
        }
    }
    emit configChanged();

}


void MouseConfig::updateColumnSize()
{

    treeWidget->header()->resizeSections(QHeaderView::ResizeToContents);
    treeWidget->header()->setStretchLastSection(true);

}


void MouseConfig::buttonChanged(const MouseButton::Button &oldButton,
                                const MouseButton::Button &newButton)
{

    MouseButton *changed = static_cast<MouseButton*>(sender());
    if (contains(newButton, changed)) {
        KMessageBox::information(this, i18n("The button '%1' has already been defined",
                                            MouseButton::getName(newButton)));
        changed->setButton(oldButton);
    }
    emit configChanged();

}


void MouseConfig::showKeyMonDialog()
{

    DeviceSearchDialog *dialog = new DeviceSearchDialog(KeyMon::DeviceInfo::MouseType, this);
    connect(dialog, SIGNAL(deviceSelected(QString)), kcfg_keyMonDevice, SLOT(setText(QString)));

    dialog->show();

}


void MouseConfig::buttonsChanged()
{

    QHash<int, QColor> buttons;
    for (int i = 0; i < treeWidget->topLevelItemCount(); i++) {
        QTreeWidgetItem *item = treeWidget->topLevelItem(i);
        const int button =  static_cast<MouseButton*>(treeWidget->itemWidget(item, 1))->getXButton();
        const QColor color = static_cast<KColorButton*>(treeWidget->itemWidget(item, 2))->color();

        buttons[button] = color;
    }
    cursorWidget->setButtons(buttons);
    cursorWidget->setSize(QSize(kcfg_cursorWidgetSize->value(), kcfg_cursorWidgetSize->value()));
    cursorWidget->setMode(kcfg_led->isChecked() ? CursorWidget::LEDMode : CursorWidget::CircleMode);
    cursorWidget->setOpacity(kcfg_cursorOpacity->value());

    currentButtonChanged();

}


void MouseConfig::modeChanged()
{

    if (!KWindowSystem::compositingActive()) {
        kcfg_cursorOpacity->setDisabled(true);
        opacityLabel->setDisabled(true);
    }

}


void MouseConfig::currentButtonChanged()
{

    const MouseButton::Button button = MouseButton::getButtonFromName(buttonCombo->currentText());
    if (button == MouseButton::NoButton || contains(button)) {
        addButton->setEnabled(false);
    } else {
        addButton->setEnabled(true);
    }

}




#include "mouseconfig.moc"

