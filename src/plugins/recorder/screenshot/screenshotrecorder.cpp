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
#include "screenshotrecorder.h"
#include <recorditnow_screenshot.h>

// KDE
#include <klocalizedstring.h>
#include <kplugininfo.h>
#include <kdebug.h>

// Qt
#include <QtGui/QPixmap>
#include <QtGui/QX11Info>
#include <QtCore/QFile>
#include <QtGui/QPainter>
#include <QtCore/QDir>

// X
#ifdef XFIXES_FOUND
    #include <X11/extensions/Xfixes.h>
    #include <X11/Xlib.h>
#endif


K_PLUGIN_FACTORY(myFactory, registerPlugin<ScreenshotRecorder>();)
K_EXPORT_PLUGIN(myFactory("screenshot_recorder"))
ScreenshotRecorder::ScreenshotRecorder(QObject *parent, const QVariantList &args)
    : AbstractRecorder(parent, args)
{



}



ScreenshotRecorder::~ScreenshotRecorder()
{



}


bool ScreenshotRecorder::isVideoRecorder() const
{

    return false;

}


void ScreenshotRecorder::record(const AbstractRecorder::Data &d)
{

    setState(Record);

    emit status(i18n("Cheese!"));

    // reload cfg
    Settings::self()->readConfig();

    int x = d.geometry.x();
    int y = d.geometry.y();

    const int w = d.geometry.width();
    const int h = d.geometry.height();

    const int screen = QX11Info::appScreen();
    const qlonglong window = d.winId != -1 ? d.winId : QX11Info::appRootWindow(screen);

    QPixmap cheese = QPixmap::grabWindow(window, x, y, w, h); // screenshot
    QPainter painter(&cheese);

#ifdef XFIXES_FOUND
    // cursor
    if (Settings::drawCursor()) {

        if (d.winId != -1) { // set x + y
            XWindowAttributes attributes;
            Window junkwin;

            if (XGetWindowAttributes(QX11Info::display(), d.winId, &attributes)) {
                (void) XTranslateCoordinates(QX11Info::display(), d.winId, attributes.root,
                                             -attributes.border_width,
                                             -attributes.border_width,
                                             &x, &y, &junkwin);
            }

        }

        XFixesCursorImage *xcursor = XFixesGetCursorImage(QX11Info::display());
        unsigned char *pixels = (unsigned char*) malloc(xcursor->width*xcursor->height*4);
        for (int i = 0; i < xcursor->width*xcursor->height; i++) {
            unsigned long pix = xcursor->pixels[i];
            pixels[i * 4] = pix & 0xff;
            pixels[(i * 4) + 1] = (pix >> 8) & 0xff;
            pixels[(i * 4) + 2] = (pix >> 16) & 0xff;
            pixels[(i * 4) + 3] = (pix >> 24) & 0xff;
        }
        QImage qcursor(pixels, xcursor->width, xcursor->height, QImage::Format_ARGB32);
        painter.drawImage(xcursor->x-(x+xcursor->xhot), xcursor->y-(y+xcursor->yhot), qcursor);

        free(pixels);
        XFree(xcursor);
    }
#endif
    // branding
    if (Settings::branding() && QFile::exists(Settings::brandingFile().path())) {
        QImage branding(Settings::brandingFile().path());
        painter.setOpacity(Settings::brandingOpacity());

        QPoint pos(0, 0);
        switch (Settings::position()) {
        case 0: pos = cheese.rect().topLeft(); break;
        case 1: {
                pos = cheese.rect().topRight();
                pos.rx() -= branding.width();
                break;
            }
        case 2: {
                pos = cheese.rect().bottomLeft();
                pos.ry() -= branding.height();
                break;
            }
        case 3: {
                pos = cheese.rect().bottomRight();
                pos.rx() -= branding.width();
                pos.ry() -= branding.height();
                break;
            }
        case 4: {
                pos = cheese.rect().center();
                pos.rx() -= (branding.width()/2);
                pos.ry() = cheese.rect().top();
                break;
            }
        case 5: {
                pos = cheese.rect().center();
                pos.rx() -= (branding.width()/2);
                pos.ry() = (cheese.rect().bottom()-branding.height());
                break;
            }
        default: break;
        }
        painter.drawImage(pos, branding);
    }

    m_shot = cheese;
    m_outputFile = d.outputFile;

    QFile outFile(d.outputFile);
    if (outFile.exists()) {
        if (d.overwrite) {
            m_outputFile = d.outputFile;
            m_removeId = remove(d.outputFile);
            return;
        } else {
            outFile.setFileName(unique(outFile.fileName()));
            emit outputFileChanged(outFile.fileName());
            m_outputFile = outFile.fileName();
        }
    }
    save();

}


void ScreenshotRecorder::pause()
{



}


void ScreenshotRecorder::stop()
{



}


void ScreenshotRecorder::save()
{

    QFile file(m_outputFile);
    if (!file.open(QIODevice::WriteOnly)) {
        emit error(i18nc("%1 = error string", "Cannot open output file: %1", file.errorString()));
        return;
    }

    kDebug() << "format:" << Settings::format() << "quality:" << Settings::quality();
    if (!m_shot.save(&file, Settings::format().toUpper().toLatin1(), Settings::quality())) {
        file.close();
        emit error(i18n("Cannot save image."));
    } else {
        file.close();
        emit finished(AbstractRecorder::Normal);
    }

}


void ScreenshotRecorder::jobFinished(const QString &id, const QString &errorString)
{

    if (!errorString.isEmpty()) {
        emit error(errorString);
        return;
    }

    if (id == m_removeId) {
        save();
    }

}



#include "screenshotrecorder.moc"


