#!/bin/sh

#Simple File system Navigation
pwd
cd /home
pwd
cd cg
pwd
find /home/cg -type f -name "*.c"
cd /home
pwd

#Simple File Manipulation
pwd
cd /home
pwd
mkdir mycode
cd mycode
#copy all code
cd ..
mkdir mycode2
cp -a /mycode /mycode2
mv mycode deadcode
rm -r deadcode
