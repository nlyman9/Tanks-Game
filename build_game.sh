if [ "$1" = "-m" ]; then
  cd build
  cmake ../source -DMAC=1
  make
else
  cd build
  cmake ../source -DMAC=0
  make
fi