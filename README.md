# openqcam-feed-ij3c

OpenQCam is a smart IoT camera project initiated by Quanta, a Fortune-500 IT design and manufacturing company based in Taiwan.

The open platform contains open hardware and software on top of MT7620/MT7688, with contemporary H.264/VP8 encoding camera support and flexible camera module options.

We are ready to help. [Drop us a mail](mailto:CM_sales@quantatw.com) to get more information.

## Build Steps

### Prerequests

Hardware

	IJ3C platform
	Ethernet cable
	UART cable

Software

	Ubuntu 12.04 or later
	7GB disk space

Tools

	$ sudo apt-get update
	$ sudo apt-get install git-core libssl-dev libncurses5-dev unzip atftpd
		
### Configure tftp daemon

tftp is used to download firmwares to device over ethernet.

	Edit /etc/default/atftpd
		USE_INETD=false
		OPTIONS="--port 69 --daemon --tftpd-timeout 300 --retry-timeout 5 --mcast-addr 192.168.0.0-255 --mcast-ttl 1 --maxthread 100 --verbose=5 /tftproot"
	$ sudo mkdir /tftpboot
	$ sudo chmod -R 777 /tftproot
	$ sudo /etc/init.d/atftpd restart

### Build uboot

	$ git clone https://github.com/OpenQCam/uboot-ij3x -b mt7688
	$ cd uboot-ij3x
	$ make
	$ cp uboot.bin /tftproot/

### Build openwrt

	$ git clone https://github.com/OpenQCam/openwrt-ij3x -b mt7688
	$ cd openwrt-ij3x
	$ cp feeds.conf.default feeds.conf
	$ echo src-git linkit https://github.com/MediaTek-Labs/linkit-smart-7688-feed.git >> feeds.conf
	$ echo src-git openqcam https://github.com/OpenQCam/openqcam-feed-ij3c.git >> feeds.conf
	$ ./scripts/feeds update -a
	$ ./scripts/feeds install -a
	$ make menuconfig
		Select the options as below:
			Target System: `Ralink RT288x/RT3xxx`
			Subtarget: `MT7688 based boards`
			Target Profile: `LinkIt7688`	
		Save and exit (**use the deafult config file name without changing it**)
		Note: If `LinkIt7688` is not shown in Target Profile list, remove tmp/ and make mennuconfig again.
	$ make
	$ cp bin/ramips/openwrt-ramips-mt7688-LinkIt7688-squashfs-sysupgrade.bin /tftproot/sysupgrade.bin

### Download images by UART

Connect device and host by UART and ethnernet cable

	The UART pin order on IJ3C is VCC/GND/TX/RX.
	Directly connect IJ3C ethernet port to the host ethernet port.

Configure UART on host

	$ sudo chmod 777 /dev/ttyUSB0
	$ gtkterm -s 57600 

Download uboot

	Open IJ3C console via UART.
	Reboot IJ3C and press 9 immediately.
	Press Y to confirm update uboot image.
	Enter device IP, press enter for default value [10.10.10.123].
	Enter host IP, press enter for default value [10.10.10.3].
	Enter filename, type uboot.bin and press enter.
	Downloading, device reboot automatically after download completed.

	Note: if tftp is not started, set host ip by following command.
	$ sudo ifconfig eth0 10.10.10.3

Download openwrt image

	Open IJ3C console via UART.
	Reboot ij3c and press 2 immediately.
	Press Y to confirm update openwrt image.
	Enter device IP, press enter for default value [10.10.10.123].
	Enter host IP, press enter for default value [10.10.10.3].
	Enter filename, type sysupgrade.bin and press enter.
	Downloading, device reboot automatically after download completed.

	Note: if tftp is not started, set host ip by following command.
	$ sudo ifconfig eth0 10.10.10.3

## Usage

qic1832_sdk contains the header file and library to control QIC1832 encoding camera.

qic1832_sdk_util contains the sample code for how to use QIC1832 encoding camera.

	$ example_yuv
	$ example_avc
	$ example_vp8
	$ avc_simulcast
	$ vp8_simulcast

qcam

	Broadcast video stream.
	$ qcam /etc/qcam/streamer.lua

	Receive and playback video stream.
	$ vlc rtsp://192.168.1.1/v2/video/avmuxstream
	$ vlc http://192.168.1.1:8888/v2/video/mjpg/mjpgstream

