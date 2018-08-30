
export XDK_COMPILER=gnu
export XDK_TARGET_PLATFORM=linux
export XDK_TARGET_CPU=arm
if [[ ! "$_ELASTOS64" == "" ]]; then
    export XDK_TARGET_CPU_ARCH=64
else
    export XDK_TARGET_CPU_ARCH=32
fi

export RPI_TOOLCHAIN_HOME="/home/wanli/workspace/RaspberryPi/tools"
export RPI_TOOLCHAIN_TOPDIR="$RPI_TOOLCHAIN_HOME/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64"
export RPI_TOOLCHAIN_REFIX="$RPI_TOOLCHAIN_TOPDIR/bin/arm-linux-gnueabihf-"
export CMAKE_SYSROOT="$RPI_TOOLCHAIN_TOPDIR/arm-linux-gnueabihf/libc"

export CMAKE_ARGS="-Ofast -mthumb -marm -march=armv7-a -mfloat-abi=hard -mfpu=vfpv3-d16 -Wall --sysroot=${CMAKE_SYSROOT}"
