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

#ifndef UPLOADPAGE_H
#define UPLOADPAGE_H


// own
#include "ui_uploadpage.h"

// JoschyCore
#include <joschycore/video.h>
#include <joschycore/plugin.h>
#include <joschycore/abstractprovider.h>

// Qt
#include <QtGui/QWizardPage>
#include <QtCore/QPointer>


class UploadPage : public QWizardPage, public Ui::UploadPage
{
Q_OBJECT


public:
    explicit UploadPage(QWidget *parent = 0);
    ~UploadPage();

    bool isComplete() const;
    void initializePage();
    void cleanupPage();


private slots:
    void authenticated(const QString &id);
    void error(const QString &id, const Joschy::Plugin::ErrorType &error, const QString &errorString);
    void uploadFinsihed(const QString &id, const Joschy::Video &video);
    void uploadProgress(const QString &id, qlonglong speed, qlonglong bytesSent, qlonglong bytesTotal);


private:
    QPointer<Joschy::AbstractProvider> m_provider;
    QString m_currentId;


};


#endif // UPLOADPAGE_H
