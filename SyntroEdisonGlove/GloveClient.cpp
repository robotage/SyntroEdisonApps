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

#include "SyntroLib.h"
#include "GloveClient.h"
#include "SyntroGloveDefs.h"

#include <qdebug.h>

GloveClient::GloveClient(QObject *)
    : Endpoint(GLOVECLIENT_BACKGROUND_INTERVAL, "GloveClient")
{
    m_servicePort = -1;
    clearGotData();
}

GloveClient::~GloveClient()
{
}


void GloveClient::appClientInit()
{
    newStream();
}

void GloveClient::appClientExit()
{

}

void GloveClient::appClientBackground()
{
    QMutexLocker locker(&m_lock);
    SYNTRO_GLOVEDATA data;
    RTIMU_DATA localData;
    int recordCount;
    int totalLength;

    //  Note: all the m_imuData queues have the same number of entries at all times

    if (m_imuData[IMU_ROLE_PALM].empty())
        return;

    if (m_servicePort == -1)
        return;

    if (!clientIsServiceActive(m_servicePort) || !clientClearToSend(m_servicePort))
        return;                                             // can't send for network reasons

    recordCount = m_imuData[IMU_ROLE_PALM].count();
    totalLength = sizeof(SYNTRO_GLOVEDATA) * recordCount;

    SYNTRO_EHEAD *multiCast = clientBuildMessage(m_servicePort, sizeof(SYNTRO_RECORD_HEADER) + totalLength);
    SYNTRO_RECORD_HEADER *head = (SYNTRO_RECORD_HEADER *)(multiCast + 1);
    SyntroUtils::convertIntToUC2(SYNTRO_RECORD_TYPE_GLOVE, head->type);
    SyntroUtils::convertIntToUC2(0, head->subType);
    SyntroUtils::convertIntToUC2(sizeof(SYNTRO_RECORD_HEADER), head->headerLength);
    SyntroUtils::convertInt64ToUC8(SyntroClock(), head->timestamp);

    for (int record = 0; record < recordCount; record++) {
        localData = m_imuData[IMU_ROLE_PALM].dequeue();
        SyntroUtils::convertInt64ToUC8(localData.timestamp, data.timestamp);
        data.fusionQPosePalm[0] = localData.fusionQPose.scalar();
        data.fusionQPosePalm[1] = localData.fusionQPose.x();
        data.fusionQPosePalm[2] = localData.fusionQPose.y();
        data.fusionQPosePalm[3] = localData.fusionQPose.z();

        localData = m_imuData[IMU_ROLE_THUMB].dequeue();
        SyntroUtils::convertInt64ToUC8(localData.timestamp, data.timestamp);
        data.fusionQPoseThumb[0] = localData.fusionQPose.scalar();
        data.fusionQPoseThumb[1] = localData.fusionQPose.x();
        data.fusionQPoseThumb[2] = localData.fusionQPose.y();
        data.fusionQPoseThumb[3] = localData.fusionQPose.z();

        localData = m_imuData[IMU_ROLE_FINGER].dequeue();
        SyntroUtils::convertInt64ToUC8(localData.timestamp, data.timestamp);
        data.fusionQPoseFinger[0] = localData.fusionQPose.scalar();
        data.fusionQPoseFinger[1] = localData.fusionQPose.x();
        data.fusionQPoseFinger[2] = localData.fusionQPose.y();
        data.fusionQPoseFinger[3] = localData.fusionQPose.z();

        memcpy(((SYNTRO_GLOVEDATA *)(head + 1)) + record, &data, sizeof(SYNTRO_GLOVEDATA));
    }
    clientSendMessage(m_servicePort, multiCast, sizeof(SYNTRO_RECORD_HEADER) + totalLength, SYNTROLINK_MEDPRI);
}

void GloveClient::newIMUData(int imuRole, const RTIMU_DATA& data)
{
    QMutexLocker locker(&m_lock);

    if ((m_servicePort == -1) || !clientIsServiceActive(m_servicePort)) {
        // can't send for network reasons so dump queue
        for (int i = 0; i < IMU_ROLE_COUNT; i++)
            m_imuData[i].clear();
        clearGotData();
        return;
    }

    m_tempData[imuRole] = data;
    m_gotData[imuRole] = true;

    if (gotAllData()) {
        clearGotData();
        for (int i = 0; i < IMU_ROLE_COUNT; i++) {
            if (m_imuData[i].count() > 5) {
                // stop queue getting stupidly big
                m_imuData[i].clear();
            }
            m_imuData[i].enqueue(m_tempData[i]);

         }
    }
}


void GloveClient::newStream()
{
    // remove the old streams

    if (m_servicePort != -1)
        clientRemoveService(m_servicePort);

    m_servicePort = clientAddService(SYNTRO_STREAMNAME_GLOVE, SERVICETYPE_MULTICAST, true);
}

bool GloveClient::gotAllData()
{
    bool ret = true;

    for (int imuRole = 0; imuRole < IMU_ROLE_COUNT; imuRole++)
        ret &= m_gotData[imuRole];
    return ret;
}

void GloveClient::clearGotData()
{
    for (int imuRole = 0; imuRole < IMU_ROLE_COUNT; imuRole++)
        m_gotData[imuRole] = false;
}
