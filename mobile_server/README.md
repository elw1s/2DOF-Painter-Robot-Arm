
# mobile server

Projeyi Ubuntu'da çalıştırın. Ubuntunun "tmp" klasöründe (/tmp/cse396/) klasörünü oluşturun.
Projeyi cloneladıktan sonra aşağıdaki dosya yolunu değiştirin. 

system("python3 /home/arda/Desktop/CSE396/mobile_server/linedraw.py"); (mainserver.cpp 173. satır)


## Installation
  
```bash
  sudo apt-get -y install rapidjson-dev
```

Run
```bash
  cd tmp
  mkdir cse396
  cd "PROJECT FOLDER OF MOBILE_SERVER"
  mkdir build && cd build
  cmake ..
  make
  ./server
```
