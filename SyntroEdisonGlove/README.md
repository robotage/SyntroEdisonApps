# SyntroEdisonGlove

SyntroEdisonGlove is an Intel Edison SyntroNet app for streaming SyntroGlove data across a network. A separate app, SyntroGloveView can be used to display the data stream. The stream consists of the the 9-dof fused data from three IMUs on the smart glove.

Check out www.richards-tech.com and search for SyntroEdisonGlove to find more instructions and advice.

SyntroEdisonGlove uses the RTIMULib IMU library. Check out https://github.com/richards-tech/RTIMULib for more information on the operation of the library and configuration of the system via the RTIMULib.ini settings file.

Note that by default the SyntroEdisonGlove uses I2C1 to connect to the three IMU chips. All three chips must be at different I2C addresses. An alternative would be to use an I2C switch to allow three identical IMUs to be used but this would require some software changes in IMUThread.cpp to select the correct IMU before using it.

SyntroEdisonGlove assumes the placement of IMUs like this:

1. IMU on the back of the hand or in the palm - this is the Palm IMU.
2. IMU on the top of the thumb - this is the Thumb IMU.
3. IMU on the top of the middle finger - this is the Finger IMU.

Each IMU has its own settings file in the working directory. For example, Palm IMU is defined by Palm.ini. Run SyntroEdisonGlove once to generate default .ini files. Autodetection won't work properly as it cannot detect three separate IMUs so these will need to be configured by hand. Edit each of the three .ini files to set the IMU type and I2C address for the three different roles.

### Calibration

It is very important that at least min/max magnetometer calibration is performed otherwise very poor fusion results will be obtained. The RTIMULib repo contains a program called RTIMULibCal that can be used to generate calibration data. RTIMULibCal can be built natively on the Edison itself.

Since the Edison does not normally have git available, use wget to copy the RTIMULib repo:

    wget https://github.com/richards-tech/RTIMULib/archive/master.zip --no-check-certificate
    unzip master.zip
    
This will create the repo in RTIMULib-master. Navigate to the RTIMULibCal directory and built it:

    make
    make install
    
Note that although RTIMULibCal supports ellipsoid fit magnetometer calibration, octave isn't normally present on the Edison and so cannot be successfully completed. Min/max magnetometer and accelerometer calibration can be performed however. The program is self-documenting: read the instructions when a calibration mode is selected to see how to use it.

The simplest way to calibrate the glove is to first run SyntroEdisonGlove to generate three .ini files called Palm.ini, Thumb.ini and Finger.ini. For example, to calibrate the Palm IMU, go to where the three .ini files were generated and run:

    RTIMULibCal Palm
    
will calibrate the Palm IMU. Do this for each of the three IMUs in turn. When you start SyntroEdisonGlove again it should report the correct IMU types and also that calibration is in use.


