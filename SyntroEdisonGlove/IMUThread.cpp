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

#include "IMUThread.h"
#include "RTIMUSettings.h"
#include "RTFusion.h"
#include "RTIMU.h"

#include "SyntroGloveDefs.h"

QMutex IMUThread::imuLock;

IMUThread::IMUThread(int imuRole) : SyntroThread("IMUThread", "IMUThread")
{
    QString settingsFile;

    switch (imuRole) {
    case IMU_ROLE_PALM:
        settingsFile = "Palm";
        break;

    case IMU_ROLE_THUMB:
        settingsFile = "Thumb";
        break;

    case IMU_ROLE_FINGER:
        settingsFile = "Finger";
        break;
    }

    m_settings = new RTIMUSettings(qPrintable(settingsFile));
    m_imuRole = imuRole;
}

IMUThread::~IMUThread()
{

}


void IMUThread::initThread()
{
    m_imu = NULL;
    m_timer = -1;
    newIMU();
}

void IMUThread::finishThread()
{
    killTimer(m_timer);

    if (m_imu != NULL)
        delete m_imu;

    m_imu = NULL;

    delete m_settings;
}

void IMUThread::newIMU()
{
    QMutexLocker lock(&imuLock);

    if (m_imu != NULL) {
        delete m_imu;
        m_imu = NULL;
    }

    m_imu = RTIMU::createIMU(m_settings);

    if (m_imu == NULL)
        return;

    //  set up IMU

    m_imu->IMUInit();

    m_timer = startTimer(m_imu->IMUGetPollInterval());
}


void IMUThread::timerEvent(QTimerEvent * /* event */)
{
    if (m_imu == NULL)
        return;

    if (m_imu->IMUType() == RTIMU_TYPE_NULL)
        return;

    QMutexLocker lock(&imuLock);
    while (m_imu->IMURead()) {
        emit newIMUData(m_imuRole, m_imu->getIMUData());
    }
}


