

# Joystick to servo on RaspberryPi linux + STM8 Board
- Joytick code based on
https://www.kernel.org/doc/Documentation/input/joystick-api.txt
- C++ wrapper for joystick and serial port
- STM8 for servo control

## Platform
- PS4 Wireless controller : used with USB and wireless
- Raspberry pi 3 : integrated bluetooth module
- STM8S103F3 based Board, with 16bit PWM x7 channels (still not documented, priorisation on request).

## Setup:
sudo apt-get install joystick
##Usage
###Compilation
- first subdirectories are direct compile and makefile attempts, then scons is used for all others with SConstruct file

>scons

### run
>./joy

##connection with USB cable
- check evironment
>ls /dev/input
added "event2 js0"

>ls /dev/input/by-id/
added "Usb-Sony_Computer_Entertainment_Wireless_Controller-event-joystick" and " -joystick only"

##connection wireless
- Disconnect the USB cable of the PS4 controller from the RaspberryPi
>sudo bluetoothctl

- very important the help, and general understanding of the bluetooth features, Pairing, Trusting, reconnecting,...

>[bluetooth]# help

>[bluetooth]# agent on

>[bluetooth]# default-agent

>[bluetooth]# scan on

starts a scan for available devices, at this step, you should :
- Switch off the PS4 console and unplug its cable so that it does not use the controlled you try to pair with the Pi
- Kepp pressing the PS4 controller's "share" button, then the "Home PS" button until the light starts quickly flashing from time to time
- the should appear a line in the scan

>[NEW]Device 1C:66:6D:XX:XX:XX Wireless Controller

- copy and paste the mac of the Wireless Controller after the connect command (XX replaced with custom values)

>connect 1C:66:6D:XX:XX:XX

>Attempting to connect to,..., connected : yes, paired : yes, Connection successful

- you can then turn the scan off otherwise it might disturb the bluetooth communication

>[bluetooth]# scan off

- now the Joystick should appear again in the input list, the rest of the usage is identical to the USB connected mode
 
>ls /dev/input
> event2 js0

- To switch off the PS4 controller, a long press on the home button is needed (around 10s)
- Switching it on back again seams to reconnect at once, as the device is listed as trusted
