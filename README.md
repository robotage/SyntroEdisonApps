# SyntroEdisonApps

SyntroEdisonApps contains all the SyntroNet apps for Intel Edison in one repo.

Check out www.richards-tech.com for more details.

## Release history

### October 31 2014 - 1.0.0

Intial release.


### Pre-requisites and fetch

The Intel Edison must have the Qt runtime installed and the matching SDK must be installed on the development machine.
See http://wp.me/p4qcHg-bE for details on how to create the correct environment.

In addition, SyntroEdisonCam requires that video and audio capture be enabled in the kernel.
See http://wp.me/p4qcHg-bL on how to do this.

The software requires that the SyntroCore and SyntroApps repos be available (although they do not need to be built on the development machine).

For example, from the home directory:

    mkdir SyntroNet
    cd SyntroNet
    git clone git://github.com/richards-tech/SyntroCore
    git clone git://github.com/richards-tech/SyntroApps
    git clone git://github.com/richards-tech/SyntroEdisonApps
    
This ensures that they are at the same level in the directory hierarchy.


### Build

The apps must be built using QtCreator in the correct environment. So, open a terminal window and run:

    source /opt/poky-edison/1.6/environment-setup-core2-32-poky-linux
    qtcreator
    
This assumes that QtCreator has been configured as described in Step 5 of http://wp.me/p4qcHg-bE. Open the .pro files as usual and build. If all is well, it should be possible to debug and run the apps on the target Intel Edison.
    
