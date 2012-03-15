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
#include "keyboardkeyiconpage.h"

// KDE
#include <kstandarddirs.h>
#include <kdebug.h>
#include <kicon.h>
#include <kconfig.h>
#include <kconfiggroup.h>

// Qt
#include <QtCore/QDir>
#include <QtGui/QListWidgetItem>


KeyboardKeyIconPage::KeyboardKeyIconPage(QWidget *parent)
    : QWizardPage(parent)
{

    setupUi(this);
    searchLine->setListWidget(iconList);

    registerField("Icon*", iconEdit, "text", SIGNAL(textChanged(QString)));

    connect(iconList, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
            this, SLOT(currentItemChanged(QListWidgetItem*,QListWidgetItem*)));
    connect(themeCombo, SIGNAL(currentIndexChanged(QString)), this, SLOT(themeChanged(QString)));

    updateThemeCombo();

}


QStringList KeyboardKeyIconPage::themeDirs()
{

    QStringList dirs;
    dirs.append(KGlobal::dirs()->findDirs("appdata", "themes/keyboard"));
    return dirs;

}


void KeyboardKeyIconPage::currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{

    Q_UNUSED(previous);

    if (!current) {
        iconEdit->clear();
    } else {
        iconEdit->setText(m_iconDir+QDir::separator()+current->text());
    }

}


void KeyboardKeyIconPage::themeChanged(const QString &newTheme)
{

    listGroup->setTitle(newTheme);

    iconList->clear();
    foreach (const Theme &theme, m_themes) {
        if (theme.name == newTheme) {
            descriptionLabel->setText(theme.description);
            authorLabel->setText(theme.author);
            licenseLabel->setText(theme.license);

            QDir dir(theme.themeDir);
            foreach (const QString &file, dir.entryList(QStringList(), QDir::Files)) {
                if (file.endsWith(QLatin1String(".themerc"))) {
                    continue;
                }
                QListWidgetItem *item = new QListWidgetItem();
                item->setIcon(KIcon(dir.absoluteFilePath(file)));
                item->setText(file);

                iconList->addItem(item);
            }
            m_iconDir = theme.themeDir;
        }
    }

}


void KeyboardKeyIconPage::updateThemeCombo()
{

    themeCombo->clear();
    m_themes.clear();
    foreach (const QString &path, themeDirs()) {
        QDir dir(path);
        foreach (const QString &themePath, dir.entryList(QStringList(), QDir::Dirs|QDir::NoDotAndDotDot)) {
            QDir themeDir(dir.absoluteFilePath(themePath));
            const QStringList files = themeDir.entryList(QStringList() << "*.themerc", QDir::Files);
            if (files.isEmpty() || files.size() > 1) {
                continue;
            }
            KConfig config(themeDir.absoluteFilePath(files.first()));
            KConfigGroup group(&config, "RecordItNow::Keyboard::Theme");

            Theme theme;
            theme.name = group.readEntry("Name", i18n("No name"));
            theme.description = group.readEntry("Description", QString());
            theme.author = group.readEntry("Author", i18n("Unknown"));
            theme.license= group.readEntry("License", i18n("Unknwon"));
            theme.themeDir = themeDir.absolutePath();

            m_themes.append(theme);
            themeCombo->addItem(theme.name);
        }
    }

}


#include "keyboardkeyiconpage.moc"

