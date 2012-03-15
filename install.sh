#!/bin/sh


if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root" 1>&2
   exit 1
fi

# build recorditnow
buildDir="./build"
linguas="de;hu;pt_BR;cs;fr;tr"



if [ ! -d ${buildDir} ]
then mkdir build || exit
fi


cd ${buildDir} || exit
cmake -DLINGUAS=${linguas} -DCMAKE_INSTALL_PREFIX=`kde4-config --prefix` -DCMAKE_BUILD_TYPE=release ../ || exit
make || exit
make install || exit
kbuildsycoca4 || exit
