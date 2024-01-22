
# 2DOF Painter and Gamer Robot Arm (DrawnPlay Buddy)

## Overview

This project entails the development of a 2DOF Painter and Gamer robot arm. The graphical interface is built using the Qt6 Framework. The project is specifically designed to run in a Linux environment and has been developed and tested on Ubuntu 22.04.

[Project Video](https://youtu.be/x612WRot3eU?si=9mIUNRFD5gjYUwXO)

![Robot image](https://github.com/elw1s/2DOF-Painter-Robot-Arm/assets/66280976/745f400f-53d0-419a-b141-731052d60ea9)

## Materials and Resources

For all the necessary materials and additional resources related to the project, please visit [BrachioGraph by Daniele Procida](https://www.brachiograph.art/en/latest/).

## Deployment

### a. Desktop Application

The graphical interface has been developed using Qt 6.6.1 on Ubuntu 22.04. Currently, there is no .deb or AppImage creator available for the specified Qt version. A .deb project has been prepared using "cpack," but for it to run, Qt 6.6.1 needs to be installed on the computer.

[Download for Ubuntu 22.04](https://drive.google.com/file/d/17MyQS3QuPF5W_aw8DlFgdDgftv5PlcJc/view?usp=sharing)

The most suitable method to run the program is to download the Qt Creator program, then open the project by selecting "Open Project" and choosing the "CMakeLists.txt" file (located in the folder where the source code is). Afterward, the program can be run with either the Debug or Release versions.

[Download Qt](https://www.qt.io/download-qt-installer-oss?hsCtaTracking=99d9dd4f-5681-48d2-b096-470725510d34%7C074ddad0-fdef-4e53-8aa8-5e8a876d6ab4)

In order to utilize the "Open Gazebo" feature, it is essential to have the latest Docker version installed on your computer. To run Gazebo Virtualization, execute the following command inside the `brachiograph_ws/docker` directory. The execution time may vary around 10 minutes depending on the hardware of your computer.

```bash
  sudo ./build-and-run.sh
```

### b. Mobile Application

The file with the .apk extension can be installed and run on the Android system.

### c. Embedded System

Firstly, the program needs to be run on Raspberry Pi. Connect to Raspberry Pi using the VNC Viewer program. Then navigate to the `BrachioGraphWithServer` folder. Open the terminal and execute the following command.

To test the graphical user interface (GUI) without the need for hardware, run the `simulate_embedded` project.

```bash
  cmake .. && make && clear && sudo ./server
```

After running the program on the Raspberry Pi, you should hear the motors running. The servos will move to their initial positions and be calibrated. Then, you can close the VNC Viewer program.

## Usage

After the embedded system is running, open either the Desktop or Mobile application. For the Desktop application, connect to the embedded system by entering the IP and Port in the Settings menu. Then, use the desired features of the applications to make the robot draw or play games. Have fun!
