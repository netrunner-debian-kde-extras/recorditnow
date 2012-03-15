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
#include "addaccountdialog.h"
#include "abstractuploader.h"
#include "ui_addaccount.h"

// KDE
#include <klocalizedstring.h>
#include <kicon.h>
#include <kconfigskeleton.h>


AddAccountDialog::AddAccountDialog(QWidget *parent, KConfigSkeleton *config, const QString &icon,
                                   const QString &group,
                                   AbstractUploader *uploader,
                                   const QString &account)
                                       : KDialog(parent),
                                       m_settings(config),
                                       m_ui(new Ui::Account),
                                       m_group(group)
{

    setWindowIcon(KIcon(icon));
    setAttribute(Qt::WA_DeleteOnClose, true);

    QWidget *widget = new QWidget(this);
    m_ui->setupUi(widget);
    setMainWidget(widget);

    if (uploader && !account.isEmpty()) {
        setWindowTitle(i18nc("%1 = account name", "Edit %1", account));
        m_ui->accountEdit->setText(account);
        m_ui->saveCheck->setChecked(hasPassword(account, m_settings, m_group));
        m_account = account;
        if (m_ui->saveCheck->isChecked()) {
            connect(uploader, SIGNAL(gotPassword(QString,QString)), this,
                    SLOT(gotPassword(QString,QString)));
            uploader->getPassword(account);
        }
    } else {
        setWindowTitle(i18n("Add a new Account"));
    }

    connect(this, SIGNAL(finished(int)), this, SLOT(dialogFinished(int)));

}


AddAccountDialog::~AddAccountDialog()
{

    delete m_ui;

}


void AddAccountDialog::removeAccount(const QString &account, KConfigSkeleton *settings,
                                     const QString &group)
{

    KConfig *cfg = settings->config();
    KConfigGroup cfgGroup(cfg, group);
    QStringList accounts = cfgGroup.readEntry("Accounts", QStringList());
    accounts.removeAll(account);

    cfgGroup.deleteEntry(account);
    cfgGroup.writeEntry("Accounts", accounts);

}


QStringList AddAccountDialog::getAccounts(KConfigSkeleton *settings, const QString &group)
{

    KConfig *cfg = settings->config();
    KConfigGroup cfgGroup(cfg, group);
    return cfgGroup.readEntry("Accounts", QStringList());

}


bool AddAccountDialog::hasPassword(const QString &account, KConfigSkeleton *settings,
                                   const QString &group)
{

    KConfig *cfg = settings->config();
    KConfigGroup cfgGroup(cfg, group);
    return cfgGroup.readEntry(account, false);

}


void AddAccountDialog::dialogFinished(const int &code)
{

    if (code == KDialog::Accepted) {
        const QString account = m_ui->accountEdit->text();
        const QString password = m_ui->passwordEdit->text();
        const bool savePassword = m_ui->saveCheck->isChecked();

        KConfig *cfg = m_settings->config();
        KConfigGroup group(cfg, m_group);
        QStringList accounts = group.readEntry("Accounts", QStringList());

        if (!m_account.isEmpty()) {
            accounts.removeAll(m_account);
            removeAccount(m_account, m_settings, m_group);
        }

        if (!accounts.contains(account)) {
            accounts.append(account);
        }

        group.writeEntry(account, savePassword);
        group.writeEntry("Accounts", accounts);

        if (savePassword) {
            emit newPassword(account, password);
        }

        emit accountsChanged(accounts);
    }

}


void AddAccountDialog::gotPassword(const QString &account, const QString &password)
{

    if (m_ui->accountEdit->text() == account) {
        m_ui->passwordEdit->setText(password);
    }

}


#include "addaccountdialog.moc"


