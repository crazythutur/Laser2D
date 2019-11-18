config=release
#confi=debug

rm -r $config

mkdir $config

cd $config

qmake ../src/laser2D.pro -spec linux-g++ CONFIG+=$config CONFIG+=qml_debug

make
cd ..
