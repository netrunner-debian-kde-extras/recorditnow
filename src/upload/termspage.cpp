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
#include "termspage.h"



TermsPage::TermsPage(QWidget *parent)
    : QWizardPage(parent)
{

    setupUi(this);
    registerField("Terms*", termsCheck);

}


void TermsPage::initializePage()
{

    const QString button = buttonText(QWizard::NextButton).remove('&').remove('>').trimmed();
    const QString provider = field("Provider").toString();

    termsBrowser->setHtml(i18n("By clicking &apos;%1&apos;, you certify that you own all rights to "\
                               "the content or that you are authorized by the owner to make the "\
                               "content publicly available on %2, and that it otherwise "\
                               "complies with the %2 Terms of Service.", button, provider));

}


#include "termspage.moc"

