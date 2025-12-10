#!/bin/bash

cd /home/yatabay/Desktop/scop
wget https://github.com/nigels-com/glew/releases/download/glew-2.2.0/glew-2.2.0.tgz
tar -xzf glew-2.2.0.tgz

# Nur die Source für statisches Linking
cp glew-2.2.0/src/glew.c .
cp -r glew-2.2.0/include/GL .