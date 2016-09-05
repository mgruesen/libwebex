cd build/debug 2> /dev/null
if [ $? -ne "0" ]; then
  echo "Error: No build folder found"
  exit
else
  ctest .
fi
