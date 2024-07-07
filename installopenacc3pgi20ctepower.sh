#!/bin/bash

./bootstrap
./configure --prefix=/home/bsc41/bsc41636/extrae-funcional-instalacion --with-mpi=/gpfs/apps/POWER9/OPENMPI/4.0.1/GCC-8.3.0/ --with-unwind=/gpfs/apps/POWER/LIBUNWIND/1.2 --with-papi=/gpfs/apps/POWER9/PAPI/5.6.0 --without-dyninst --with-openacc=/gpfs/apps/POWER9/PGI/2020-201/linuxpower/20.1/
make -j
make install
