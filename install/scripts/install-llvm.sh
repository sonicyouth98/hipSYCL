#!/bin/bash
HIPSYCL_PKG_LLVM_VERSION_MAJOR=${HIPSYCL_PKG_LLVM_VERSION_MAJOR:-9}
HIPSYCL_PKG_LLVM_VERSION_MINOR=${HIPSYCL_PKG_LLVM_VERSION_MINOR:-0}
HIPSYCL_PKG_LLVM_VERSION_PATCH=${HIPSYCL_PKG_LLVM_VERSION_PATCH:-1}
HIPSYCL_PKG_LLVM_REPO_BRANCH=${HIPSYCL_PKG_LLVM_REPO_BRANCH:-release/${HIPSYCL_PKG_LLVM_VERSION_MAJOR}.x}

HIPSYCL_PKG_LLVM_VERSION=${HIPSYCL_PKG_LLVM_VERSION_MAJOR}.${HIPSYCL_PKG_LLVM_VERSION_MINOR}.${HIPSYCL_PKG_LLVM_VERSION_PATCH}


HIPSYCL_PKG_LLVM_REPO_BRANCH=${HIPSYCL_PKG_LLVM_REPO_BRANCH:-release/9.x}
export INSTALL_PREFIX=${INSTALL_PREFIX:-/opt/hipSYCL}

set -e
BUILD_DIR=$HOME/git/llvm-vanilla
rm -rf $BUILD_DIR

echo "Cloning LLVM $HIPSYCL_PKG_LLVM_REPO_BRANCH"
git clone -b $HIPSYCL_PKG_LLVM_REPO_BRANCH https://github.com/llvm/llvm-project $BUILD_DIR

case $HIPSYCL_PKG_LLVM_VERSION in
	9.0.1)
		echo "Applying patch on $HIPSYCL_PKG_LLVM_VERSION"
		sed -i 's/CHECK_SIZE_AND_OFFSET(ipc_perm, mode);//g' $BUILD_DIR/compiler-rt/lib/sanitizer_common/sanitizer_platform_limits_posix.cc
		;;
esac


export CC=${HIPSYCL_BASE_CC:-clang}
export CXX=${HIPSYCL_BASE_CXX:-clang++}
export BUILD_TYPE=Release
export LLVM_INSTALL_PREFIX=$INSTALL_PREFIX/llvm
export TARGETS_TO_BUILD="AMDGPU;NVPTX;X86"
export NUMTHREADS=`nproc`

CMAKE_OPTIONS="-DLLVM_ENABLE_PROJECTS=clang;compiler-rt;lld;openmp -DOPENMP_ENABLE_LIBOMPTARGET=OFF -DCMAKE_C_COMPILER=$CC -DCMAKE_CXX_COMPILER=$CXX -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_INSTALL_PREFIX=$LLVM_INSTALL_PREFIX -DLLVM_ENABLE_ASSERTIONS=OFF -DLLVM_TARGETS_TO_BUILD=$TARGETS_TO_BUILD -DCLANG_ANALYZER_ENABLE_Z3_SOLVER=0 -DLLVM_INCLUDE_BENCHMARKS=0 -DCMAKE_INSTALL_RPATH_USE_LINK_PATH=ON -DCMAKE_INSTALL_RPATH=$INSTALL_PREFIX/lib -DLLVM_ENABLE_OCAMLDOC=OFF -DLLVM_ENABLE_BINDINGS=OFF -DLLVM_TEMPORARILY_ALLOW_OLD_TOOLCHAIN=ON -DLLVM_ENABLE_DUMP=ON"

mkdir -p $BUILD_DIR/build
cd $BUILD_DIR/build
cmake $CMAKE_OPTIONS $BUILD_DIR/llvm
make -j $NUMTHREADS
make install
cp -p $BUILD_DIR/build/bin/llvm-lit   $LLVM_INSTALL_PREFIX/bin/llvm-lit
cp -p $BUILD_DIR/build/bin/FileCheck  $LLVM_INSTALL_PREFIX/bin/FileCheck
cp -p $BUILD_DIR/build/bin/count      $LLVM_INSTALL_PREFIX/bin/count
cp -p $BUILD_DIR/build/bin/not        $LLVM_INSTALL_PREFIX/bin/not
cp -p $BUILD_DIR/build/bin/yaml-bench $LLVM_INSTALL_PREFIX/yaml-bench