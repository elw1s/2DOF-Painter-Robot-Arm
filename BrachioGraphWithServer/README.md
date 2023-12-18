
# Brachio Graph C++ Implementation

https://github.com/evildmp/BrachioGraph




## Installation

Install opencv for c++ through https://www.geeksforgeeks.org/how-to-install-opencv-in-c-on-linux/

Then install
```bash
  sudo apt update
  sudo apt install nlohmann-json3-dev
  sudo apt install xtensor-dev
  sudo apt-get install libopencv-dev
  wget https://github.com/joan2937/pigpio/archive/master.zip
  unzip master.zip
  cd pigpio-master
  make
  sudo make install
```

Download required projects
```bash
  cd BrachioGraph
  git clone --recurse https://github.com/zurutech/pillow-resize.git
  cd pillow-resize
  git submodule update --init --recursive
  mkdir build && cd build
  // Go to BrachioGraph/pillow-resize/libs
  rmdir CLI11
  rmdir magic_enum
  git clone https://github.com/CLIUtils/CLI11.git
  cd CLI11
  mkdir build & cd build
  cmake ..
  make
  // Go to BrachioGraph/pillow-resize/libs
  git clone https://github.com/Neargye/magic_enum.git
  cd magic_enum
  mkdir build & cd build
  cmake ..
  make
  // Go to BrachioGraph/pillow-resize/build
  cmake ..
  make
```


Run
```bash
  mkdir build & cd build
  cmake ..
  make
  ./brachioGraph
```
