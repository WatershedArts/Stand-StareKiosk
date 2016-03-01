Hardware
===

##Raspberry Pi Setup
This application is built for Raspberry Pi 2 running Raspbian Jessie.

#####Setup guide
We customised the Pi so that it would work with openFrameworks and some custom scripts that would be run along side the application itself.

Download the standard Jessie Image for RPi and write it to an SD Card. Once thats done boot up the Pi.

Update everything.

````
sudo -i 
apt-get update
apt-get upgrade
````

To speed up the development time, we built the application using a cross compiler and copied the compiled app over the network to the RPi. 

However, it is useful to install and build oF for the Pi so that there is a back up and all of the libraries are the same.

* [Compiling openFrameworks](http://openframeworks.cc/setup/raspberrypi/)
* [Cross Compiler for openFrameworks and RPi2] (https://github.com/twobitcircus/rpi-build-and-boot)

Now we need to install a couple of extra packages for some custom scripts that will aid the application.

````
apt-get install imagemagick inotify-tools git

cd 

// Move into the package folder
cd /usr/src

git clone git://git.videolan.org/x264

cd x264

./configure --arch=armel --target-os=linux --enable-gpl --enable-libx264 --enable-nonfree

make -j4

make install 
````

Next we are going to download and install FFMPEG.

````
cd ..
 
git clone git://source.ffmpeg.org/ffmpeg.git

cd ffmpeg

./configure --arch=armel --target-os=linux --enable-gpl --enable-libx264 --enable-nonfree

make -j4

make install
````

Thanks to [Jeffrey Thompson] (http://www.jeffreythompson.org/blog/2014/11/13/installing-ffmpeg-for-raspberry-pi/) for his guide on ffmpeg.

With these installed we can move on the Pi Config.

````
sudo raspi-config
Option 8 Overclock - Set to Medium
Option 9 Advanced Options - A3 256Mb *
Option 9 Advanced Options - A6 Enable
Option 9 Advanced Options - A8 Enable 
````

Then reboot.

##Wiring

Here is the schematic for the project.
sudo apt-get install pulseaudio

And the wiring table.

| Component     |Pin| Raspberry Pi Pin |
|---------------|---|------------------|
| RFID Breakout |Vcc|      3.3v        |   
|               |GND|      GND         |   
|               |D0 | GPIO 15 UART0_RXD|   
|               |+/-|      GND         |   
|               |Tag in Range |     GPIO 16      |   

##RFID
Rather than being connected via USB, RFID unit is connected directly to the GPIO pins. This allows us to detect whether a card has been placed or removed from the reader. See the wiring diagram.

The RFID Unit is usually located on serial port <code>"/dev/ttyAMA0"</code>.

To make the RFID Reader work with the RPi. We altered the ofSerial class slightly.

Line 360 sets the serial communication options. 

We simply added the following options.

````
options.c_cflag &= ~HUPCL;
options.c_iflag &= ~BRKINT;
options.c_iflag |= IGNPAR;
options.c_iflag &= ~ICRNL;
options.c_oflag &= ~OPOST;
options.c_oflag &= ~ONLCR;
options.c_lflag &= ~ISIG;
options.c_lflag &= ~ICANON;
options.c_lflag &= ~ECHO;

````


##Donations
TBD

##Projector Control 
The projector is controller by the applcation itself using our ofxCasioProjectorController addon.

It uses a USB to Serial DB9 cable to communicate from the RPi to the projector.

The projector is usually located on serial port <code>"/dev/ttyUSB0"</code>.

You can disable the projector controller by changing the useprojectorcontroller flag in the config file.