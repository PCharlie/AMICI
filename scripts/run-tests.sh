#!/bin/bash

cd ./SuiteSparse/SuiteSparse_config

make library

cd ./../AMD

make library

cd ./../BTF

make library

cd ./../CAMD

make library

cd ./../COLAMD

make library

cd ./../KLU

make library


mkdir ./../../sundials/build/
cd ./../../sundials/build/

cmake -DCMAKE_INSTALL_PREFIX="$./../../build/" \
-DBUILD_ARKODE=OFF \
-DBUILD_CVODE=OFF \
-DBUILD_IDA=OFF \
-DBUILD_KINSOL=OFF \
-DBUILD_SHARED_LIBS=ON \
-DBUILD_STATIC_LIBS=OFF \
-DEXAMPLES_ENABLE=OFF \
-DEXAMPLES_INSTALL=OFF \
-DKLU_ENABLE=ON \
-DKLU_LIBRARY_DIR="$./../../SuiteSparse/lib" \
-DKLU_INCLUDE_DIR="$./../../SuiteSparse/include" \
.. 

make 
make install

cd ../../

cmake -DCMAKE_BUILD_TYPE=Debug CMakeLists.txt
make

if [[ "$OSTYPE" == "darwin"* ]]; then
	install_name_tool -change libsundials_cvodes.2.dylib $(pwd)/sundials/build/lib/libsundials_cvodes.2.dylib model_dirac
	install_name_tool -change libsundials_nvecserial.0.dylib $(pwd)/sundials/build/lib/libsundials_nvecserial.0.dylib model_dirac
fi


#cmake -f ./../models/model_dirac/CMakeLists.txt