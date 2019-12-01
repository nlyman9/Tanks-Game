if [ ! -d "build" ]; then
 mkdir build
fi

if [ "$1" = "-c" ]; then
  cd build
  rm -rf *
  cmake ../src
  make
elif [ "$1" = "-v" ]; then
  cd build
  cmake ../src -DVERBOSE=ON
  make
else
   cd build
  cmake ../src -DVERBOSE=OFF
  make
fi