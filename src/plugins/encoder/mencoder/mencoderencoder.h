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

#ifndef MENCODERENCODER_H
#define MENCODERENCODER_H

// own
#include "abstractencoder.h"

// Qt
#include <QtCore/QVariantList>


class KProcess;
class MencoderEncoder : public AbstractEncoder
{
    Q_OBJECT


public:
    explicit MencoderEncoder(QObject *parent = 0, const QVariantList &args = QVariantList());
    ~MencoderEncoder();

    void encode(const Data &d);
    void pause();
    void stop();


private:
    enum State {
        WorkDir = 0,
        RemoveOutputFile = 1,
        RemoveTmpFile = 2
    };

    KProcess *m_mencoder;
    QString m_outputFile;
    QString m_tmpFile;
    bool m_paused;
    bool m_stopped;
    QString m_currentId;
    State m_state;
    QString m_command;
    int m_status;
    AbstractEncoder::Data m_data;

    void prepare();
    void startMencoder();
    void finish();


private slots:
    void newMencoderOutput();
    void mencoderFinished(const int &ret);


protected:
    void jobFinished(const QString &id, const QString &errorString);


};


#endif // MENCODERENCODER_H
