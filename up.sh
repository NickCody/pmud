sudo apt-get install gcc10
sudo apt-get install g++10
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-10 100
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-10 50
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-10 100
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-10 50
sudo update-alternatives --install /usr/bin/cpp cpp-bin /usr/bin/cpp-10 100
sudo update-alternatives --install /usr/bin/cpp cpp-bin /usr/bin/cpp-10 50
sudo update-alternatives --set g++ /usr/bin/g++-10
sudo update-alternatives --set gcc /usr/bin/gcc-10
sudo update-alternatives --set cpp-bin /usr/bin/cpp-10
