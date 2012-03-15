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


#ifndef ABSTRACTUPLOADER_H
#define ABSTRACTUPLOADER_H


// own
#include "recorditnowplugin.h"

// KDE
#include <kdemacros.h>

// Qt
#include <QtCore/QVariantList>
#include <QtGui/QAction>


namespace KWallet {
    class Wallet;
};

class KDE_EXPORT AbstractUploader : public RecordItNowPlugin
{
    Q_OBJECT


public:
    explicit AbstractUploader(QObject *parent = 0, const QVariantList &args = QVariantList());
    ~AbstractUploader();

    virtual void show(const QString &file, QWidget *parent) = 0;
    virtual void cancel() = 0;

    void getPassword(const QString &account);
    void setPassword(const QString &account, const QString &password);

private:
    enum WalletWait { None=0, Read, Write };

    WId m_wId;
    WalletWait m_walletWait;
    KWallet::Wallet *m_wallet;
    QStringList m_getPasswords;
    QHash<QString, QString> m_setPasswords;
    QString m_folder;

    void getWallet();
    bool enterWalletFolder(const QString &folder);


private slots:
    void readWallet(bool success);
    void writeWallet(bool success);
    void newPassword(const QString &account, const QString &password);


protected:
    void setId(const WId &i, const QString &folder);


signals:
    void gotPassword(const QString &account, const QString &password);
    void finished(const QString &error);
    void status(const QString &status);


};


#endif // ABSTRACTUPLOADER_H
