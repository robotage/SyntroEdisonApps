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

#ifndef GLOVECLIENT_H
#define GLOVECLIENT_H

#include <qmutex.h>
#include <QMutex>
#include <QQueue>

#include "SyntroLib.h"

#include "SyntroGloveDefs.h"
#include "RTIMULib.h"

#define GLOVECLIENT_BACKGROUND_INTERVAL    (SYNTRO_CLOCKS_PER_SEC / 100)

class GloveClient : public Endpoint
{
    Q_OBJECT

public:
    GloveClient(QObject *parent);
    virtual ~GloveClient();

public slots:
    void newIMUData(int imuRole, const RTIMU_DATA& data);

    void newStream();

protected:
    void appClientInit();
    void appClientExit();
    void appClientBackground();

private:
    bool gotAllData();
    void clearGotData();
    QMutex m_lock;

    RTIMU_DATA m_tempData[IMU_ROLE_COUNT];
    bool m_gotData[IMU_ROLE_COUNT];

    QQueue<RTIMU_DATA> m_imuData[IMU_ROLE_COUNT];

    int m_servicePort;
};

#endif // GLOVECLIENT_H

