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

#include "SyntroEdisonNav.h"
#include <QApplication>

#include "SyntroUtils.h"
#include "RTIMULibDefs.h"

int runConsoleApp(int argc, char **);


int main(int argc, char *argv[])
{
    qRegisterMetaType <RTIMU_DATA>("RTIMU_DATA");
    return runConsoleApp(argc, argv);
}

int runConsoleApp(int argc, char **argv)
{
    QCoreApplication a(argc, argv);

    bool daemonMode = SyntroUtils::checkDaemonModeFlag(argc, argv);

    SyntroUtils::loadStandardSettings(PRODUCT_TYPE, a.arguments());

    SyntroEdisonNav console(daemonMode, &a);

    return a.exec();
}

