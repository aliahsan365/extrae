#!/bin/bash

source @sub_PREFIXDIR@/etc/extrae.sh


export EXTRAE_CONFIG_FILE=extrae.xml
export LD_PRELOAD=${EXTRAE_HOME}/lib/liboacctrace.so

./pi