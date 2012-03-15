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

#ifndef ADDACCOUNTDIALOG_H
#define ADDACCOUNTDIALOG_H


// KDE
#include <kdialog.h>
#include <kdemacros.h>


namespace Ui {
    class Account;
}

class KConfigSkeleton;
class AbstractUploader;
class KDE_EXPORT AddAccountDialog : public KDialog
{
    Q_OBJECT


public:
    AddAccountDialog(QWidget *parent, KConfigSkeleton *config, const QString &icon,
                     const QString &group, AbstractUploader *uploader = 0,
                     const QString &account = QString());
    ~AddAccountDialog();

    static void removeAccount(const QString &account, KConfigSkeleton *settings, const QString &group);
    static QStringList getAccounts(KConfigSkeleton *settings, const QString &group);
    static bool hasPassword(const QString &account, KConfigSkeleton *settings, const QString &group);


private:
    QString m_account;
    KConfigSkeleton *m_settings;
    Ui::Account *m_ui;
    QString m_group;


private slots:
    void dialogFinished(const int &code);
    void gotPassword(const QString &account, const QString &password);


signals:
    void accountsChanged(const QStringList &accounts);
    void newPassword(const QString &account, const QString &password);


};


#endif // ADDACCOUNTDIALOG_H
