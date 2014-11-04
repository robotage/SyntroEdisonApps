//
//  Copyright (c) 2014 richards-tech.
//
//  This file is part of SyntroNet
//
//  SyntroNet is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  SyntroNet is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SyntroNet.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef SYNTROEDISONGLOVE_H
#define SYNTROEDISONGLOVE_H

#include <QThread>
#include "SyntroGloveDefs.h"

#define PRODUCT_TYPE "SyntroEdisonGlove"

class GloveClient;
class IMUThread;

class SyntroEdisonGlove : public QThread
{
    Q_OBJECT

public:
    SyntroEdisonGlove(bool daemonMode, QObject *parent);

public slots:
    void aboutToQuit();

protected:
    void run();
    void timerEvent(QTimerEvent *event);

private:
    void showHelp();
    void showStatus();

    void runConsole();
    void runDaemon();

    void registerSigHandler();
    static void sigHandler(int sig);

    GloveClient *m_client;
    IMUThread *m_imuThread[IMU_ROLE_COUNT];
    int m_rateTimer;
    bool m_daemonMode;
    static volatile bool sigIntReceived;

};

#endif // SYNTROEDISONGLOVE_H

