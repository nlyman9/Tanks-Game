if [ ! -d "build" ]; then
 mkdir build
fi

if [ "$1" = "-c" ]; then
  cd build
  rm -rf *
  cmake ../src
  make
else
  cd build
  cmake ../src
  make
fi