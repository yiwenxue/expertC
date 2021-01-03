#!/bin/bash

project=$1
echo ${project}

metadata=${HOME}/program/expertC/project/project_template.tar.xz
mkdir -p ${project}
tar -xvf ${metadata} -C ${project}
echo "Done!"
