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
#include "accountpage.h"
#include <recorditnow.h>

// KDE
#include <kdebug.h>
#include <kwallet.h>



AccountPage::AccountPage(QWidget *parent)
    : QWizardPage(parent), m_wallet(0)
{

    setupUi(this);

    registerField("Account*", accountEdit);
    registerField("Password*", passwordEdit);

}


AccountPage::~AccountPage()
{

    if (m_wallet) {
        delete m_wallet;
    }

}


void AccountPage::initializePage()
{

    wallCheck->setChecked(Settings::uploadPassword());
    if (wallCheck->isChecked()) {
        KConfigGroup cfg(Settings::self()->config(), "Upload");
        accountEdit->setText(cfg.readEntry("RecordItNow-"+field("Provider").toString(), QString()));
        getPassword();
    }

}



void AccountPage::writeWallet(bool success)
{

    kDebug() << "success:" << success;
    if (success && enterWalletFolder("RecordItNow-"+field("Provider").toString())) {
        if (m_wallet->writePassword(accountEdit->text(), passwordEdit->text()) == 0) {
            kDebug() << "successfully put password in wallet";
        }
    }

    m_walletWait = None;
    delete m_wallet;
    m_wallet = 0;

}


void AccountPage::readWallet(bool success)
{

    kDebug() << "success:" << success;
    if (success && enterWalletFolder("RecordItNow-"+field("Provider").toString())) {
        QString password;
        if (m_wallet->readPassword(accountEdit->text(), password) == 0) {
            kDebug() << "successfully retrieved password from wallet";
            passwordEdit->setText(password);
        }
    }

    m_walletWait = None;
    delete m_wallet;
    m_wallet = 0;

}


void AccountPage::getWallet()
{

    if (m_wallet) {
        delete m_wallet;
    }

    kDebug() << "opening wallet";
    m_wallet = KWallet::Wallet::openWallet(KWallet::Wallet::NetworkWallet(), winId(),
                                           KWallet::Wallet::Asynchronous);

    if (m_walletWait == Write) {
        connect(m_wallet, SIGNAL(walletOpened(bool)), SLOT(writeWallet(bool)));
    } else {
        connect(m_wallet, SIGNAL(walletOpened(bool)), SLOT(readWallet(bool)));
    }

}


bool AccountPage::enterWalletFolder(const QString &folder)
{

    if (!m_wallet) {
        return false;
    }

    m_wallet->createFolder(folder);
    if (!m_wallet->setFolder(folder)) {
        kDebug() << "failed to open folder" << folder;
        return false;
    }
    kDebug() << "wallet now on folder" << folder;
    return true;

}


bool AccountPage::validatePage()
{

    Settings::self()->setUploadPassword(wallCheck->isChecked());
    if (wallCheck->isChecked()) {
        KConfigGroup cfg(Settings::self()->config(), "Upload");
        cfg.writeEntry("RecordItNow-"+field("Provider").toString(), accountEdit->text());
        setPassword();
    }
    return true;

}


void AccountPage::getPassword()
{

    if (accountEdit->text().isEmpty()) {
        return;
    }
    m_walletWait = Read;
    getWallet();

}


void AccountPage::setPassword()
{

    if (accountEdit->text().isEmpty() || passwordEdit->text().isEmpty()) {
        return;
    }
    m_walletWait = Write;
    getWallet();

}


#include "accountpage.moc"
