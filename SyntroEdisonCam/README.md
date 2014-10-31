# SyntroEdisonCam

SyntroEdisonCam is an Intel Edison MJPEG/PCM streaming application for use with the SyntroNet system.

Check out www.richards-tech.com and search for SyntroEdisonCam to find more instructions and advice.

### Configuration

SyntroEdisonCam writes a configuration file to the ~/Settings/Syntro directory, 
so if you run it from your home directory like this

    SyntroEdisonCam

Then you will get a file called ~/Settings/Syntro/SyntroEdisonCam.ini like this created

    [General]
    appName=edison
    appType=SyntroEdisonCam
    controlRevert=0
    encryptLink=false
    heartbeatInterval=2
    heartbeatTimeout=3
    localControl=true
    localControlPriority=0
    logHeartbeatInterval=5
    logHeartbeatTimeout=3
    runtimeAdapter=
    runtimePath=/home/root/Settings/Syntro/SyntroEdisonCam.ini

    [AudioGroup]
    AudioChannels=2
    AudioEnable=true
    AudioInputCard=1
    AudioInputDevice=0
    AudioSampleRate=8000

    [CameraGroup]
    Device=0
    Format=MJPG
    FrameRate=10
    Height=480
    Width=640

    [Logging]
    diskLog=true
    logKeep=1
    logLevel=info
    maxDiskSize=1000
    netLog=true

    [MotionGroup]
    MotionDeltaInterval=0
    MotionIntervalsToSkip=0
    MotionMinDelta=400
    MotionMinNoise=40
    MotionPostroll=2000
    MotionPreroll=4000
    MotionTilesToSkip=0

    [StreamGroup]
    GenerateLowRate=false
    HighRateMaxInterval=10000
    HighRateMinInterval=100
    HighRateNullInterval=2000
    LowRateHalfRes=false
    LowRateMaxInterval=60000
    LowRateMinInterval=6000
    LowRateNullInterval=8000

    [controlNames]
    1\controlName=
    2\controlName=
    3\controlName=
    size=3

The appName line can be changed to give the resulting media stream a unique name. By default, it is the hostname of the Edison.

In the [CameraGroup], the frame rate along with frame height and width can be set. If there is more than one camera, the Device filed can be used to select an alternate.

In the [AudioGroup], set AudioEnable=false to disable the audio portion of the stream. AudioChannels can be set to 1 or 2 but 2 is more generally supported at the receiving end. AudioSampleRate can be anything supported by the apps at each end but sticking to either 8000 or 48000 is recommended for widest compatibility.

In the [MotionGroup], the MotionDeltaInterval field can be set non-zero to enable motion checking and steady state suppression. This value can be set to something like 200 for normal operation (it is the checking interval in mS).

In the [StreamGroup] are the settings to control the rates at which frames are sent on the network. All times are in mS. For example, a HighRateMinInterval of 100 means 10 frames per second. This will be true even if the camera frame rate is set higher. So this field needs to be changed to allow, say, 30 fps to be achieved. A value of 30 would be appropriate in that case.

The software can generate a low rate stream as well with different frame rates and optionally different frame size. Set GenerateLowRate=true to enable this and LowRateHalfRes=true to send half size frames. Other fields can be left unchanged.
