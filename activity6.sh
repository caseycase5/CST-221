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

#Simple Text File Viewing
pwd
cd /home
pwd
ls
ls -a
ls -l
cd /mycode2
less main.sql
less -b main.sql
less -h main.sql
less -j main.sql

more main.sql
more -d main.sql
more -f main.sql
more -p main.sql

file /home/*
file /bin/*

#Misc. Commands
name="Casey"
age=25

echo $name
echo $age

echo "File Name: $0"
echo "First Parameter: $1"
echo "Second Parameter: $2"
echo "Quoted Values: $@"
echo "Quoted Values: $*"
echo "Total Number of Parameters: $#"

#Permissions


