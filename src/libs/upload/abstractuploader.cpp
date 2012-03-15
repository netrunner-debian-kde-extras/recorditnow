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
#include "abstractuploader.h"

// KDE
#include <kwallet.h>
#include <kdebug.h>


AbstractUploader::AbstractUploader(QObject *parent, const QVariantList &args)
    : RecordItNowPlugin(parent)
{

    Q_UNUSED(args);
    m_wId = -1;
    m_wallet = 0;

}


AbstractUploader::~AbstractUploader()
{

    if (m_wallet) {
        delete m_wallet;
    }

}


void AbstractUploader::getPassword(const QString &account)
{

    Q_ASSERT(m_wId != WId(-1));
    m_getPasswords.append(account);
    m_walletWait = Read;
    getWallet();

}


void AbstractUploader::setPassword(const QString &account, const QString &password)
{

    Q_ASSERT(m_wId != WId(-1));
    m_setPasswords[account] = password;
    m_walletWait = Write;
    getWallet();

}


void AbstractUploader::writeWallet(bool success)
{

    kDebug() << "success:" << success;
    if (success && enterWalletFolder("RecordItNow-"+m_folder)) {
        QHashIterator<QString, QString> it(m_setPasswords);
        while (it.hasNext()) {
            it.next();
            if (m_wallet->writePassword(it.key(), it.value()) == 0) {
                kDebug() << "successfully put password in wallet";
            }
        }
    }

    m_setPasswords.clear();
    m_walletWait = None;
    delete m_wallet;
    m_wallet = 0;

    if (!m_getPasswords.isEmpty()) {
        m_walletWait = Read;
        getWallet();
    }

}


void AbstractUploader::readWallet(bool success)
{

    kDebug() << "success:" << success;
    if (success && enterWalletFolder("RecordItNow-"+m_folder)) {
        foreach (const QString &username, m_getPasswords) {
            QString password;
            if (m_wallet->readPassword(username, password) == 0) {
                kDebug() << "successfully retrieved password from wallet";
                emit gotPassword(username, password);
            }
        }
    }

    m_getPasswords.clear();
    m_walletWait = None;
    delete m_wallet;
    m_wallet = 0;

    if (!m_setPasswords.isEmpty()) {
        m_walletWait = Write;
        getWallet();
    }

}


void AbstractUploader::getWallet()
{

    if (m_wallet) {
        delete m_wallet;
    }

    kDebug() << "opening wallet";
    m_wallet = KWallet::Wallet::openWallet(KWallet::Wallet::NetworkWallet(), m_wId,
                                           KWallet::Wallet::Asynchronous);

    if (m_walletWait == Write) {
        connect(m_wallet, SIGNAL(walletOpened(bool)), SLOT(writeWallet(bool)));
    } else {
        connect(m_wallet, SIGNAL(walletOpened(bool)), SLOT(readWallet(bool)));
    }

}


bool AbstractUploader::enterWalletFolder(const QString &folder)
{

    m_wallet->createFolder(folder);
    if (!m_wallet->setFolder(folder)) {
        kDebug() << "failed to open folder" << folder;
        return false;
    }
    kDebug() << "wallet now on folder" << folder;
    return true;

}


void AbstractUploader::setId(const WId &i, const QString &folder)
{

    m_wId = i;
    m_folder = folder;

}


void AbstractUploader::newPassword(const QString &account, const QString &password)
{

    setPassword(account, password);

}


#include "abstractuploader.moc"


