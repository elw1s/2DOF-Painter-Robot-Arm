
# BrachioGraph C++ Implementation with GUI

This project is a direct translation of BrachioGraph (Python) into C++. The project includes a graphical interface developed with Qt6. This interface allows control of the BrachioGraph robot.

## Structure

* #### BrachioGraph

It includes a direct translation of Python files. The Turtle feature is still under construction. It doesn't function virtually.

* #### BrachioGraphWithServer

This version is embedded within the BrachioGraph project and contains a TCPSocket that communicates with the GUI.

* #### GUI/scara_gui

It includes graphical interface files written with the Qt6 Framework.

* #### mobile_server and mobile

It contains files written for testing purposes to embed TCPSocket into the mobile application (Flutter + Dart).

* #### simulate_embedded

It includes project files simulating the embedded system to test the server functionality of the embedded system.
