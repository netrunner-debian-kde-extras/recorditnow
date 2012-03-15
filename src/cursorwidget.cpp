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
#include "cursorwidget.h"
#include "config/mousebutton.h"
#include "keymonmanager.h"

// KDE
#include <kdebug.h>
#include <kapplication.h>
#include <kwindowsystem.h>
#include <kxerrorhandler.h>
#include <klocalizedstring.h>

// Qt
#include <QtGui/QCursor>
#include <QtCore/QTimer>
#include <QtGui/QPainter>
#include <QtCore/QEvent>
#include <QtCore/QDateTime>
#include <QtCore/QThread>
#include <QtGui/QDesktopWidget>
#include <QtGui/QPaintEvent>

// X11
#include <X11/Xlib.h>
#include <X11/Xcursor/Xcursor.h>
#include <X11/extensions/shape.h>


#define RESET_TIME 600
CursorWidget::CursorWidget(QWidget *parent)
    : QWidget(parent, Qt::X11BypassWindowManagerHint|Qt::FramelessWindowHint|Qt::Tool)
{

    setAttribute(Qt::WA_TranslucentBackground);
    setAutoFillBackground(false);

    // _UGLY_ HACK: Prevents KWin to draw a shadow
    setAttribute(Qt::WA_X11NetWmWindowTypeDock);

    m_grab = false;
    m_mode = LEDMode;
    m_opacity = 0.4;
    m_show = true;

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updatePos()));

    m_resetTimer = new QTimer(this);
    connect(m_resetTimer, SIGNAL(timeout()), this, SLOT(resetColor()));
    m_resetTimer->setSingleShot(true);

    setContentsMargins(2, 2, 2, 2);

    setSize(QSize(40, 40));

    m_normalColor = Qt::black;
    m_currentColor = m_normalColor;

    connect(KeyMonManager::self(), SIGNAL(keyEvent(KeyMon::Event)), this,
            SLOT(buttonPressed(KeyMon::Event)));

}


CursorWidget::~CursorWidget()
{

    delete m_timer;
    delete m_resetTimer;

}


void CursorWidget::setSize(const QSize &size)
{

    setMinimumSize(size);
    setMaximumSize(size);

}


void CursorWidget::setButtons(const QHash<int, QColor> &buttons)
{

    m_buttons = buttons;

}


void CursorWidget::setDevice(const QString &deviceName)
{

    m_deviceName = deviceName;

}


void CursorWidget::setMode(const CursorWidget::WidgetMode &mode)
{

    m_mode = mode;

    if (m_mode == CircleMode) {
        // click-through
        int junk;
        if (XQueryExtension(x11Info().display(), "SHAPE", &junk, &junk, &junk)) {
            Pixmap mask = XCreatePixmap(x11Info().display(), winId(), 1, 1, 1);
            XShapeCombineMask(x11Info().display(), winId(), ShapeInput, 0, 0, mask, ShapeSet);
            XFreePixmap(x11Info().display(), mask);
        } else {
            kWarning() << "SHAPE extension is _NOT_ present!";
            m_mode = LEDMode;
        }
    }

    updateMask();
    update();

}


void CursorWidget::setOpacity(const qreal &opacity)
{

    if (m_opacity == opacity) {
        return;
    }

    const double max = 1.0;
    const double onePercent = max/100;
    const double value = opacity*onePercent;

    m_opacity = value;
    update();

}


void CursorWidget::setShowAlways(const bool &show)
{

    m_show = show;

}


void CursorWidget::switchToPreviewMode()
{

    setWindowFlags(windowFlags()&~(Qt::X11BypassWindowManagerHint|Qt::FramelessWindowHint|Qt::Tool));
    show(); // necessary to apply window flags
    m_timer->disconnect(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(previewColors()));
    m_timer->start(2000);

}


void CursorWidget::start()
{

    m_timer->start(1000/50);

    if (m_show) {
        show();
    }

}


void CursorWidget::stop()
{

    hide();
    deleteLater();

}


void CursorWidget::updatePos()
{

    QRect geo = geometry();

    switch (m_mode) {
    case LEDMode: {
            const int size = XcursorGetDefaultSize(x11Info().display());
            geo.moveTopLeft(QCursor::pos()+QPoint(size, size));

            const QRect desktop = kapp->desktop()->screenGeometry(x11Info().appScreen());
            if (!desktop.contains(geo)) {
                if (geo.x()+geo.width() > desktop.width()) {
                    geo.moveRight(desktop.right());
                }
                if (geo.y()+geo.height() > desktop.height()) {
                    geo.moveBottom(desktop.bottom());
                }

                if (geo.contains(QCursor::pos())) {
                    geo.moveBottomRight(QCursor::pos()-QPoint(size/2, size/2));
                }
            }
            break;
        }
    case CircleMode: geo.moveCenter(QCursor::pos()); break;
    }

    setGeometry(geo);
    raise();

}


void CursorWidget::resetColor()
{

    m_currentColor = m_normalColor;

    if (m_show) {
        update();
    } else {
        hide();
    }

}


void CursorWidget::buttonPressed(const KeyMon::Event &event)
{

    if (!event.mouseEvent) {
        return;
    }

    if (m_resetTimer->isActive()) {
        m_resetTimer->stop();
    }

    if (!isVisible()) {
        show();
    }

    switch (event.key) {
    case KeyMon::Event::WheelUp:
    case KeyMon::Event::WheelDown: {
            m_currentColor = m_buttons[event.keyToXButton(event.key)];
            m_resetTimer->start(RESET_TIME);
            break;
        }
    default: {
            if (event.pressed) {
                m_currentColor = m_buttons[event.keyToXButton(event.key)];
            } else {
                m_resetTimer->start(RESET_TIME);
            }
            break;
        }
    }
    update();

}



void CursorWidget::previewColors()
{

    QHash<int, QColor>::const_iterator it = m_buttons.find(m_buttons.key(m_currentColor));
    if (it == m_buttons.constEnd()) {
        it = m_buttons.constBegin();
    } else {
        it++;
    }
    m_currentColor = it.value();
    if (!m_currentColor.isValid()) {
        m_currentColor = m_normalColor;
    }
    update();

}


void CursorWidget::paintEvent(QPaintEvent *event)
{

    QPainter painter(this);
    painter.setClipRegion(event->region());

    switch (m_mode) {
    case LEDMode: paintLED(&painter); break;
    case CircleMode: paintCircle(&painter); break;
    }

}


void CursorWidget::paintLED(QPainter *painter)
{

    painter->setRenderHint(QPainter::Antialiasing);
    const bool compositing = KWindowSystem::compositingActive();


    if (!compositing) {
        QBrush brush;
        brush.setColor(Qt::gray);
        brush.setStyle(Qt::SolidPattern);
        painter->setBrush(brush);
        painter->drawRect(rect());
    }

    // base
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(m_currentColor);
    painter->setBrush(brush);
    painter->drawEllipse(contentsRect());

    // spot
    QRadialGradient grad(contentsRect().center(), size().height()/2);
    grad.setColorAt(0, Qt::white);
    grad.setColorAt(1, Qt::transparent);
    grad.setFocalPoint(contentsRect().center()-QPoint(size().height()/4, size().height()/4));
    painter->setBrush(QBrush(grad));
    painter->drawEllipse(contentsRect());

    // border
    if (compositing) {
        QPen pen;
        pen.setWidth(2);
        pen.setColor(Qt::black);
        painter->setPen(pen);
        painter->drawEllipse(contentsRect());
    }

}


void CursorWidget::paintCircle(QPainter *painter)
{

    painter->setRenderHints(QPainter::Antialiasing|QPainter::HighQualityAntialiasing);

    if (KWindowSystem::compositingActive()) {
        QRadialGradient grad(contentsRect().center(), size().height());
        grad.setColorAt(0, m_currentColor);
        grad.setColorAt(1, Qt::transparent);
        painter->setBrush(QBrush(grad));
        painter->setOpacity(m_opacity);
        painter->drawEllipse(contentsRect());
    } else {
        painter->fillRect(rect(), m_currentColor);
    }

}


void CursorWidget::resizeEvent(QResizeEvent *event)
{

    QWidget::resizeEvent(event);
    updateMask();

}


void CursorWidget::updateMask()
{

    switch (m_mode) {
    case CircleMode: {
            if (!KWindowSystem::compositingActive()) {
                QRect maskRect = rect();
                maskRect.setHeight(maskRect.height()-10);
                maskRect.setWidth(maskRect.width()-10);
                maskRect.moveCenter(rect().center());
                setMask(QRegion(maskRect).xored(QRegion(rect())));
            } else {
                setMask(QRegion());
            }
            break;
        }
    case LEDMode: setMask(QRegion()); break;
    }

}


#include "cursorwidget.moc"

