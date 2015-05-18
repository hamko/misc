# ODEの最新版を展開，その中に移動して，
sudo apt-get install automake libtool freeglut3-dev

./bootstrap
./configure --with-trimesh=opcode --enable-new-trimesh --enable-shared --enable-release --with-x --enable-double-precision --with-libccd

make -j4
sudo make install

sudo cp -r include/drawstuff /usr/local/include/
sudo cp drawstuff/src/.libs/libdrawstuff.* /usr/local/lib
sudo ldconfig
