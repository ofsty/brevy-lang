#!/bin/bash
set -e
if [[ $EUID -ne 0 ]]; then
   echo "This script must be run with sudo or as root" 
   exit 1
fi
SRC_DIR="src"
EXECUTABLE_NAME="brevy"
gcc -o "$EXECUTABLE_NAME" "$SRC_DIR/ec.c" "$SRC_DIR/main.c" "$SRC_DIR/vc.c" -Wall -Wextra -O2
mv "$EXECUTABLE_NAME" "/usr/bin/$EXECUTABLE_NAME"
chmod 755 "/usr/bin/$EXECUTABLE_NAME"
echo "Brevy has been successfully installed"
