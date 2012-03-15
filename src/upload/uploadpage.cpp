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
#include "uploadpage.h"

// JoschyCore
#include <joschycore/manager.h>
#include <joschycore/abstractprovider.h>

// KDE
#include <kglobal.h>
#include <klocale.h>
#include <kdebug.h>


UploadPage::UploadPage(QWidget *parent)
    : QWizardPage(parent), m_provider(0)
{

    setupUi(this);

}


UploadPage::~UploadPage()
{

    if (m_provider) {
        Joschy::Manager::self()->unloadProvider(m_provider);
    }

}


bool UploadPage::isComplete() const
{

    return m_currentId == "fin";

}


void UploadPage::initializePage()
{

    Joschy::AbstractProvider *plugin = Joschy::Manager::self()->createProvider(field("Provider").toString(),
                                                                               "QNetworkLayer");

    if (!plugin) {
        logBrowser->append(i18n("Error: Plugin load failed!"));
        return;
    }

    connect(plugin, SIGNAL(error(QString,Joschy::Plugin::ErrorType,QString)), this,
            SLOT(error(QString,Joschy::Plugin::ErrorType,QString)));
    connect(plugin, SIGNAL(authenticated(QString)), this, SLOT(authenticated(QString)));
    connect(plugin, SIGNAL(uploadFinished(QString,Joschy::Video)), this,
            SLOT(uploadFinsihed(QString,Joschy::Video)));
    connect(plugin, SIGNAL(uploadProgress(QString,qlonglong,qlonglong,qlonglong)), this,
            SLOT(uploadProgress(QString,qlonglong,qlonglong,qlonglong)));

    m_provider = plugin;

    const QString account = field("Account").toString();
    const QString password = field("Password").toString();

    const Joschy::ActionReply reply = m_provider->authenticate(account, password);
    logBrowser->append(i18n("Authentication started..."));

    if (reply.error()) {
        logBrowser->append(i18nc("1 = error code", "Error %1: Authentication failed!", reply.errorType()));
        if (!reply.errorString().isEmpty()) {
            logBrowser->append(i18n("Error message: %1", reply.errorString()));
        }
        return;
    }
    m_currentId = reply.id();

}


void UploadPage::cleanupPage()
{

    if (m_provider) {
        Joschy::Manager::self()->unloadProvider(m_provider);
        m_provider = 0;
    }
    progressBar->setValue(0);

}



void UploadPage::authenticated(const QString &id)
{

    if (id != m_currentId || !m_provider) {
        return;
    }
    logBrowser->append(i18n("Authentication finished..."));

    Joschy::Video video;
    video.setUrl(field("Video").toString());
    video.setTitle(field("Title").toString());
    video.setDescription(field("Description").toString());
    video.setCategory(field("Category").toString());
    video.setKeywords(field("Tags").toStringList());

    const Joschy::ActionReply reply = m_provider->upload(field("Account").toString(), &video);
    logBrowser->append(i18n("Upload started..."));

    if (reply.error()) {
        logBrowser->append(i18nc("1 = error code", "Error %1: Upload failed!", reply.errorType()));
        if (!reply.errorString().isEmpty()) {
            logBrowser->append(i18n("Error message: %1", reply.errorString()));
        }
        return;
    }
    m_currentId = reply.id();

}


void UploadPage::error(const QString &id, const Joschy::Plugin::ErrorType &error, const QString &errorString)
{

    if (id != m_currentId) {
        return;
    }

    logBrowser->append(i18nc("1 = error code, %2 = error message", "Error %1: %2", error, errorString));
    if (!errorString.isEmpty()) {
        logBrowser->append(i18n("Error message: %1", errorString));
    }

}


void UploadPage::uploadFinsihed(const QString &id, const Joschy::Video &video)
{

    if (id != m_currentId) {
        return;
    }
    logBrowser->append(i18nc("%1 = video url", "Upload finished: <a href='%1'>%1</a>",
                             video.url().toString()));


    m_currentId = "fin";
    emit completeChanged();

}


void UploadPage::uploadProgress(const QString &id, qlonglong speed, qlonglong bytesSent, qlonglong bytesTotal)
{

    if (id != m_currentId) {
        return;
    }

    speedLabel->setText(KGlobal::locale()->formatByteSize(speed)+"/s");
    totalLabel->setText(KGlobal::locale()->formatByteSize(bytesTotal));
    sentLabel->setText(KGlobal::locale()->formatByteSize(bytesSent));

    progressBar->setMaximum(bytesTotal);
    progressBar->setValue(bytesSent);

}


#include "uploadpage.moc"

