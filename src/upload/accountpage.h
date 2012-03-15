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

#ifndef ACCOUNTPAGE_H
#define ACCOUNTPAGE_H


// own
#include "ui_accountpage.h"

// Qt
#include <QtGui/QWizardPage>


namespace KWallet {
    class Wallet;
};


class AccountPage : public QWizardPage, public Ui::AccountPage
{
Q_OBJECT


public:
    explicit AccountPage(QWidget *parent = 0);
    ~AccountPage();

    void initializePage();


private:
    enum WalletWait {
        None = 0,
        Read = 1,
        Write = 2
    };

    WalletWait m_walletWait;
    KWallet::Wallet *m_wallet;
    QString m_folder;

    void getWallet();
    bool enterWalletFolder(const QString &folder);
    bool validatePage();


private slots:
    void readWallet(bool success);
    void writeWallet(bool success);
    void setPassword();
    void getPassword();


};


#endif // ACCOUNTPAGE_H
