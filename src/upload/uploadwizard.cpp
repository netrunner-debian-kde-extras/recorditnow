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
#include "uploadwizard.h"
#include "pluginpage.h"
#include "videopage.h"
#include "uploadpage.h"
#include "accountpage.h"
#include "termspage.h"

// KDE
#include <kicon.h>
#include <kiconloader.h>
#include <kmessagebox.h>

// Qt
#include <QtGui/QCloseEvent>


UploadWizard::UploadWizard(QWidget *parent)
    : QWizard(parent)
{

    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(i18n("Upload Wizard"));
    addPage(new PluginPage(this));
    addPage(new AccountPage(this));
    addPage(new VideoPage(this));
    addPage(new TermsPage(this));
    addPage(new UploadPage(this));

    setOption(QWizard::DisabledBackButtonOnLastPage, true);
    setPixmap(QWizard::LogoPixmap, KIcon("recorditnow-upload-media").pixmap(KIconLoader::SizeMedium,
                                                                            KIconLoader::SizeMedium));
}


void UploadWizard::closeEvent(QCloseEvent *event)
{

    if (currentId() == 4) { // upload
        UploadPage *page = static_cast<UploadPage*>(currentPage());
        if (!page->isComplete()) {
            const int ret = KMessageBox::warningYesNo(this, i18n("Do you want to cancel the upload?"));
            if (ret == KMessageBox::No) {
                event->ignore();
                return;

            }
        }
    }
    QWizard::closeEvent(event);

}



#include "uploadwizard.moc"
