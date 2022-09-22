#!/bin/bash -l
#SBATCH --export=ALL
#SBATCH --constraint=gpu
#SBATCH --partition=cscsci
#SBATCH --nodes=1
##SBATCH --ntasks-per-node=1
##SBATCH --cpus-per-task=1

set -o errexit
set -o nounset
set -o pipefail

module swap PrgEnv-cray PrgEnv-gnu
module load cdt/22.05
module load nvhpc-nompi/22.2
module load cray-hdf5-parallel/1.12.1.3
module list -t
CC --version
nvcc --version
# module unload cray-libsci_acc
export PATH=/project/c32/src/cmake-3.24.2-linux-x86_64/bin:$PATH

set -o xtrace  # do not set earlier to avoid noise from module
umask 0002  # make sure group members can access the data

mkdir -p "${SCRATCH}/${BUILD_TAG}.gnu"
chmod 0775 "${SCRATCH}/${BUILD_TAG}.gnu"
cd "${SCRATCH}/${BUILD_TAG}.gnu"
echo "# WORKSPACE=$WORKSPACE"
pwd
ls -la

# sed -i "s@GIT_REPOSITORY@SOURCE_DIR $VV/\n#@" ./cmake/setup_GTest.cmake \
# sed -i "s@GIT_REPOSITORY@SOURCE_DIR $VV/\n#@" ./domain/cmake/setup_GTest.cmake \
# sed -i "s@GIT_REPOSITORY@SOURCE_DIR $VV/\n#@" ./ryoanji/cmake/setup_GTest.cmake \
# 
# sed -i "s@GIT_TAG@#GIT_TAG @" ./cmake/setup_GTest.cmake \
# sed -i "s@GIT_TAG@#GIT_TAG @" ./domain/cmake/setup_GTest.cmake \
# sed -i "s@GIT_TAG@#GIT_TAG @" ./ryoanji/cmake/setup_GTest.cmake \

cmake -S "${WORKSPACE}" \
    -B build \
    -DCMAKE_CXX_COMPILER=CC \
    -DCMAKE_C_COMPILER=cc \
    -DBUILD_TESTING=OFF \
    -DBUILD_ANALYTICAL=OFF \
    -DGPU_DIRECT=OFF \
    -DCMAKE_CUDA_FLAGS='-arch=sm_60' \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_INSTALL_PREFIX=$PWD/local
    # -DSPH_EXA_WITH_H5PART=ON \

cmake --build build -t sphexa -j 12 |& tee -a "${STAGE_NAME}.out"
find $PWD/local -type f

# cmake --install build 

#    "${WORKSPACE}" |& tee -a "${STAGE_NAME}.out"

# make VERBOSE=1 -j |& tee -a "${STAGE_NAME}.out"
