#!/bin/sh

# Python package name
PROJECT_NAME="dsdana"

# ANL module list
MODULE_LIST="ReadTTree"

# Options:
#   -f     Force to overwrite existing files.
#   -a     Output this user name as author.
OPTIONS="-f -a"

# -------------------- #

mkproj="./make_anlpy_project.sh"

if ! type $mkproj > /dev/null 2>&1
then
    echo $mkproj "is not found."
    exit	
fi

$mkproj $OPTIONS -p $PROJECT_NAME $MODULE_LIST
echo "Skeleton code of ANL project '$PROJECT_NAME' is generated."
echo
echo "Type the following command to try :"
echo "    pip3 install" $PROJECT_NAME/
echo 
echo "Python script to run is here :"
echo "    ./$PROJECT_NAME/run/run.py"

