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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// own
#include "ui_toolbarwidget.h"
#include "libs/recorder/abstractrecorder.h"
#include "libs/encoder/abstractencoder.h"

// KDE
#include <kxmlguiwindow.h>
#include <kstatusnotifieritem.h>

// Qt
#include <QtCore/QPointer>


class ZoomDock;
class KeyboardDock;
class TimelineDock;
class CursorWidget;
class KSqueezedTextLabel;
class KAction;
class RecorderManager;
class EncoderManager;
class RecordItNowPluginManager;
class Frame;
class MainWindow : public KXmlGuiWindow, public Ui::ToolBarWidget
{
    Q_OBJECT


public:
    enum State {
        Idle = 0,
        Timer = 1,
        TimerPaused = 2,
        Recording = 3,
        Paused = 4,
        Encode = 5,
        Config = 7
    };
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void startWithArgs(const QString &backend, const QString &file, const int &time,
                       const QRect &geometry);


private:
    QWidget *m_grabber;
    Frame *m_frame;
    KStatusNotifierItem *m_tray;
    QTimer *m_timer;
    AbstractRecorder::Data m_recordData;
    State m_state;
    RecordItNowPluginManager *m_pluginManager;
    RecorderManager *m_recorderManager;
    EncoderManager *m_encoderManager;
    KSqueezedTextLabel *m_statusLabel;
    QPointer<CursorWidget> m_cursor;
    TimelineDock *m_timelineDock;
    KeyboardDock *m_keyboardDock;
    ZoomDock *m_zoomDock;

    KAction *getAction(const QString &name);

    inline void setupActions();
    inline void initRecorder(AbstractRecorder::Data *d);
    inline void initEncoder(AbstractEncoder::Data *d);
    inline void setupTray();
    void setTrayOverlay(const QString &name);
    void setState(const State &newState);
    inline MainWindow::State state() const;
    inline void updateRecorderCombo();


private slots:
    void startRecord();
    void pauseRecord();
    void stopRecord();
    void recordTriggered();
    void recordWindow();
    void recordCurrentWindow();
    void triggerFrame(const bool &checked);
    void recordFullScreen();
    void pluginStatus(const QString &text);
    void recorderFinished(const QString &error, const bool &isVideo);
    void encoderFinished(const QString &error);
    void configure();
    void configDialogFinished();
    void startTimer();
    void tick();
    void lcdUp();
    void lcdDown();
    void trayActivated(const bool &active, const QPoint &pos);
    void backendChanged(const QString &newBackend);
    void aboutToQuit();
    void pluginsChanged();
    void upload();
    void playFile(const bool &force = true);
    void removeFile();
    void outputFileChanged(const QString &newFile);
    void recorderStateChanged(const AbstractRecorder::State &newState);
    void linkActivated(const QString &link);
    void zoomIn();
    void zoomOut();
    void setupDocks();
    void timeLineFinsihed();
    void resolutionActionTriggered();
    void frameSizesChanged(const QList< QPair<QString, QSize> > &sizes);
    void initRecordWidgets(const bool &start);
    void initKeyMon(const bool &start);
    void errorNotification(const QString &error);


protected:
    bool eventFilter(QObject *watched, QEvent *event);
    void hideEvent(QHideEvent *event);
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);
    void saveNewToolbarConfig();


};


#endif // MAINWINDOW_H
