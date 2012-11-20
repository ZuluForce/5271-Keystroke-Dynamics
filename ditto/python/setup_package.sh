#!/bin/bash

## Sets up this directory to be in the python path so that modules can be more easily referenced.
## If this isn't done, some of the scripts will not work. This setup script also assumes you are
## using bash with a .bashrc file

bash_config="$HOME/.bashrc"

PWD=`pwd`

CHECK_PP_SET=`echo $PYTHONPATH | grep $PWD`

if [[ CHECK_PP_SET != "" ]]; then
  echo "This directory ($PWD) is already in PYTHONPATH"
  exit 0
fi

echo "CHECK_PP = ${CHECK_PP_SET}"
echo "Adding $PWD to PYTHONPATH"

echo "export PYTHONPATH=$PYTHONPATH:$PWD" >> $bash_config
