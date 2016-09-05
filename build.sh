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

if [ "$1" = "" ]; then
  build_type="Debug"
  echo "Building 'debug' mode..."
elif [ "$1" = "--release" ]; then
  build_type="Release"
  echo "Building 'release' mode..."
else 
  exit 1
fi

# Configure and make 
cmake -DCMAKE_BUILD_TYPE=$build_type ..
make
if [ $? -ne "0" ]; then
  exit 1
fi

echo "Build completed"

