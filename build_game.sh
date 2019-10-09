if [ "$1" = "-m" ]; then
  cd build
  cmake ../src -DMAC=1
  make
else
  cd build
  cmake ../src -DMAC=0
  make
fi