# Clean old builds
cd build 2> /dev/null
if [ $? -eq "0" ]; then
  echo "Cleaning..."
  rm -rf *
else
  # Create build
  mkdir build
  cd build
fi

# Create 'debug' and 'release' build folders
mkdir debug release

# Configure and make 'debug' build
cd debug
echo "Building 'debug'..."
cmake -DCMAKE_BUILD_TYPE=Debug ../..
make
if [ $? -ne "0" ]; then
  exit 1
fi
cd ..

# Configure and make 'release' build
cd release
echo "Building 'release'..."
cmake -DCMAKE_BUILD_TYPE=Release ../..
make
if [ $? -ne "0" ]; then
  exit 1
fi
cd ..
echo "Build completed"
