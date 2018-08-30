#!/bin/bash

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
setupArg=$1


# Fields: option_name config_file cpu_arch descriptions
generic_env_options=(
    "arm_android_32"        "arm_android_clang.sh"  "32"    " (use clang)"
    "arm_android_64"        "arm_android_clang.sh"  "64"    " (use clang)"
)
mac_env_options=(
    "ios_simulator"         "ios_simulator.sh"      "64"    " (64-bits)"
    "ios_device"            "ios_device.sh"         "64"    " (64-bits)"
    "mac_devtools_64"       "host_devtools.sh"      "64"    ""
    "mac_devtools_32"       "host_devtools.sh"      "32"    ""
)
linux_env_options=(
    "linux_64"              "host.sh"               "64"    ""
    "linux_32"              "host.sh"               "32"    ""
    "linux_rasp"            "raspberry.sh"          "32"    ""
    "linux_devtools_64"     "host_devtools.sh"      "64"    ""
    "linux_devtools_32"     "host_devtools.sh"      "32"    ""
)
id_option_name=0
id_config_file=1
id_cpu_arch=2
id_descriptions=3
number_of_fields=4

unavailable_options=()
available_options=()
if [[ $OSTYPE == *darwin* ]]; then
    # mac
    os_type="Mac"
    for i in "${linux_env_options[@]}";     do unavailable_options+=( "$i" ); done
    for i in "${mac_env_options[@]}";       do available_options+=( "$i" ); done
    for i in "${generic_env_options[@]}";   do available_options+=( "$i" ); done
elif [[ $OSTYPE == *linux* ]]; then
    # linux
    os_type="Linux"
    for i in "${mac_env_options[@]}";       do unavailable_options+=( "$i" ); done
    for i in "${linux_env_options[@]}";     do available_options+=( "$i" ); done
    for i in "${generic_env_options[@]}";   do available_options+=( "$i" ); done
else
    echo "Not supported OSTYPE($OSTYPE)."
    return
fi
unset generic_env_options
unset mac_env_options
unset linux_env_options

unavailable_option_names=()
for i in $(seq 1 $((${#unavailable_options[@]}/$number_of_fields))); do
    unavailable_option_names+=(${unavailable_options[$(( ($i-1)*$number_of_fields+$id_option_name ))]})
done
unset unavailable_options

col_option_name=()
col_config_file=()
col_cpu_arch=()
col_descriptions=()
options=()
num_available_options=$((${#available_options[@]}/$number_of_fields))
for i in $(seq 1 $num_available_options); do
    col_option_name+=( "${available_options[$(( ($i-1)*$number_of_fields+$id_option_name ))]}" )
    col_config_file+=( "${available_options[$(( ($i-1)*$number_of_fields+$id_config_file ))]}" )
    col_cpu_arch+=( "${available_options[$(( ($i-1)*$number_of_fields+$id_cpu_arch ))]}" )
    col_descriptions+=( "${available_options[$(( ($i-1)*$number_of_fields+$id_descriptions ))]}" )

    options+=("${col_option_name[$(($i-1))]}${col_descriptions[$(($i-1))]}")
done
options+=("quit")
unset id_option_name
unset id_config_file
unset id_cpu_arch
unset id_descriptions
unset number_of_fields
unset available_options

chose_index=
for i in $(seq 1 $num_available_options); do
    if [[ "$setupArg" == "${col_option_name[$(($i-1))]}" || "$setupArg" == "$i" ]]; then
        chose_index=$(($i-1))
    fi
done

if [ "$chose_index" == "" ]; then
    echo "NOTE: These options are not available on $os_type."
    echo "    ${unavailable_option_names[@]}"
    echo
    PS3='Please enter your choice: '
    select opt in "${options[@]}"
    do
        if [[ "$REPLY" -eq "${#options[@]}" ]]; then
            # Chose "quit" option
            return
        elif [[ "$REPLY" -gt "0" && "$REPLY" -le "$num_available_options" ]]; then
            chose_index=$(($REPLY-1))
            break
        else
            echo "Invalid option"
        fi
    done
fi
unset PS3

if ! [[ "$chose_index" -ge "0" &&
        "$chose_index" -lt "$num_available_options" ]]; then
    return
fi

unset num_available_options
unset unavailable_option_names
unset os_type

echo "You have chose the \"${options[$chose_index]}\" build environment."

unset options

if [ "${col_cpu_arch[$chose_index]}" == "64" ]; then
    export _ELASTOS64=YES
else
    unset _ELASTOS64
fi
unset col_cpu_arch

chose_config_file=${col_config_file[$chose_index]}
unset col_config_file
unset chose_index


################################################################################
# Set DevKit path

if [ ! $OS_PATH ]; then
    export OS_PATH=$PATH:
else
    export PATH=$OS_PATH:
fi

if [ "$setupArg" == "sdk" ]; then
    export XDK_BUILD_ENV=sdk
    shift
else
    export XDK_BUILD_ENV=rdk
fi

export XDK_ROOT="$( cd "$( dirname "${BASH_SOURCE[0]}" )/.." && pwd )"

if [[ ! -f $XDK_ROOT/Setup/Config/$chose_config_file ]]; then
    echo "Error: $XDK_SETUP_PATH/Config/$chose_config_file is not exist."
    unset XDK_BUILD_ENV XDK_ROOT
    return
fi
################################################################################
# Clean WINDOWS path
source $XDK_ROOT/Setup/CleanEnv.sh

export XDK_SETUP_PATH=$XDK_ROOT/Setup
export XDK_BUILD_PATH=$XDK_ROOT/Build
export XDK_SOURCE_PATH=$XDK_ROOT/Sources
export XDK_TARGETS_PATH=$XDK_ROOT/Targets
export XDK_TEMP=$XDK_TARGETS_PATH/Temp

################################################################################
# Set user defined environment variables if the user config file exist
source $XDK_SETUP_PATH/Config/$chose_config_file
unset chose_config_file

# Set command and build-tool lookup path
export XDK_TOOLS=$XDK_BUILD_PATH/Tools
[[ $OSTYPE == *linux* ]] && export PATH=$XDK_TOOLS/$XDK_TARGET_CPU_ARCH/linux:$PATH
[[ $OSTYPE == *darwin* ]] && export PATH=$XDK_TOOLS/$XDK_TARGET_CPU_ARCH/mac:$PATH


#export XDK_COMMANDES=$XDK_ROOT/Commandes
export PATH=.:$XDK_TOOLS:$PATH:
#export PATH=.:$XDK_TOOLS:$XDK_COMMANDES:

# Set user defined environment variables if the user config file exists
if [ -f  $XDK_ROOT/Setup/UserDefined.sh ]; then
    source $XDK_ROOT/Setup/UserDefined.sh
fi

if [ -z $2 ]; then
    export XDK_TARGET_HARDWARE=generic
else
    export XDK_TARGET_HARDWARE=$2
fi

if [ ! -f $XDK_SETUP_PATH/Config/$XDK_TARGET_HARDWARE.sh ]; then
    echo
    echo "Error: $XDK_SETUP_PATH/Config/$XDK_TARGET_HARDWARE.sh doesn't exist."
else
    source $XDK_SETUP_PATH/Config/$XDK_TARGET_HARDWARE.sh

    source $XDK_TOOLS/env_tools.sh

    if [ -z "$XDK_VERSION" ]; then
        export XDK_VERSION=dbg
    fi

    # Finally add Ubuntu System to the path, so we could use system tools
    PATH_LIST=${PATH//\:/ }
    for line in $PATH_LIST; do
        #if [ ! `echo $line | grep \/android\/` ]; then
            if [ -z "$PATH_FILTEROUT_ANDROID" ]; then
                PATH_FILTEROUT_ANDROID="$line"
            else
                PATH_FILTEROUT_ANDROID="$PATH_FILTEROUT_ANDROID:$line"
            fi
        #fi
    done

    export PATH=$PATH_FILTEROUT_ANDROID
    export XDK_GENERAL_PATH=$PATH_FILTEROUT_ANDROID

    unset PATH_LIST
    unset PATH_FILTEROUT_ANDROID

    chv $XDK_VERSION

    cd $XDK_SOURCE_PATH

    #set title
    PS1_RESET='\[\e[0m\]' # Text Reset

    if [[ ! "$_ELASTOS64" == "" ]]; then
        PS1_COLOR='\[\e[0;32m\]' # Green
    else
        PS1_COLOR='\[\e[0;33m\]' # Yellow
    fi

    TITLE='$XDK_TARGET_PLATFORM-${XDK_TARGET_CPU}_$XDK_TARGET_CPU_ARCH.$XDK_COMPILER.'
    TITLE+='${XDK_TARGET_PRODUCT:+$XDK_TARGET_PRODUCT.}'
    TITLE+='$XDK_VERSION'

    export PS1="\[\e]0;$TITLE \w\a\]\n\${debian_chroot:+(\$debian_chroot) }${PS1_COLOR}\${XDK_TARGET_CPU}_\$XDK_TARGET_CPU_ARCH.\$XDK_VERSION \w${PS1_RESET}\n$ "

    unset PS1_RESET
    unset PS1_COLOR
    unset TITLE
    #clear
fi
