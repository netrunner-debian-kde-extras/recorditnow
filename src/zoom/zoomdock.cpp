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
#include "zoomdock.h"

// QT
#include <QtCore/QTimer>

// KDE
#include <klocalizedstring.h>
#include <kicon.h>


ZoomDock::ZoomDock(QWidget *parent)
    : QDockWidget(parent), m_started(false)
{

    setupUi(this);

    QWidget *title = new QWidget(this);
    ui_title.setupUi(title);

    connect(ui_title.startButton, SIGNAL(clicked()), this, SLOT(startStopZoom()));
    connect(ui_title.zoomInButton, SIGNAL(clicked()), this, SLOT(zoomIn()));
    connect(ui_title.zoomOutButton, SIGNAL(clicked()), this, SLOT(zoomOut()));
    connect(zoomView, SIGNAL(factorChanged(int)), this, SLOT(factorChanged(int)));

    ui_title.startButton->setIcon(KIcon("system-run"));
    ui_title.zoomInButton->setIcon(KIcon("zoom-in"));
    ui_title.zoomOutButton->setIcon(KIcon("zoom-out"));

    setTitleBarWidget(title);
    hideLabel();

    m_zoomTimer = new QTimer(this);
    m_zoomTimer->setSingleShot(true);
    connect(m_zoomTimer, SIGNAL(timeout()), this, SLOT(hideLabel()));

}


ZoomDock::~ZoomDock()
{

    delete m_zoomTimer;

}


void ZoomDock::setFactor(const qreal &factor)
{

    zoomView->setFactor(factor);

}


void ZoomDock::setQuality(const ZoomView::Quality &quality)
{

    zoomView->setQuality(quality);

}


void ZoomDock::setFPS(const int &fps)
{

    zoomView->setFPS(fps);

}


void ZoomDock::startStopZoom()
{

    if (m_started) {
        m_started = false;
        zoomView->stop();
    } else {
        m_started = true;
        zoomView->start();
    }

}


void ZoomDock::zoomIn()
{

    zoomView->setFactor(zoomView->factor()+1);

}


void ZoomDock::zoomOut()
{

    zoomView->setFactor(zoomView->factor()-1);

}


void ZoomDock::factorChanged(const int &factor)
{

    if (m_zoomTimer->isActive()) {
        m_zoomTimer->stop();
    }
    zoomLabel->setText(i18n("Zoom Factor: 1:%1", factor));
    zoomFrame->show();

    m_zoomTimer->start(1500);

}


void ZoomDock::hideLabel()
{

    zoomFrame->hide();

}


#include "zoomdock.moc"
