# SUR (Sunset under rain)

## What's that ?
SUR, Sunset Under Rain, is a satisfying beautiful screen of a sunset along a rainy weather. It can also be used as a clock. It has been made to run on a raspberry pi 3 with no desktop environment, displaying the animation on any screen linked by a HDMI cable.

## Prerequisites
* Raspberry pi 3 (with stuffs necessary to make it works)
* HDMI cable
* Any screen with an HDMI input

## Installation

### 1. Setup the raspberry pi
For that step you are going to find any information that you need on the [official website](https://www.raspberrypi.org/documentation/) of the raspberry pi foundation.
Once this is done, turn on your raspberry pi, log in, and let's go.

### 2. Install raylib
For this step, once again I am not going to explain you how to do it. Because there is already a [website](https://github.com/raysan5/raylib/wiki/Working-on-Raspberry-Pi) explaining it way better than I could.

### 3. Install this repository
If it is not done yet, install git on your raspberry pi with the following command : ```sudo apt install git```

Now go to /raylib/examples : ```cd ~/raylib/examples```

And clone the SUR repository :  ```git clone https://github.com/Thuirox/SUR.git```

Compile the code : ```make SUR/rain PLATFORM=PLATFORM_RPI```

### 4. Make the raspberry boot on SUR
Firstly, we update the file .bashrc to run the program when a console is launched.
```sudo nano ~/.bashrc```

At the end of the file add the following line : ```~/raylib/examples/SUR/rain```

And now, we need to make the system stop asking to log in when it boots. For this, we are going to run : ```sudo raspi-config```

Go to "3 Boot options" -> "B1 Desktop / CLI" -> "B2 Console Autologin".

Finish and reboot the raspberry pi.