#!/bin/bash
#
# Copyright (C) 2018 The LineageOS Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

set -e

DEVICE_COMMON=sdm660-common
VENDOR=xiaomi

INITIAL_COPYRIGHT_YEAR=2018

# Load extract_utils and do some sanity checks
MY_DIR="${BASH_SOURCE%/*}"
if [[ ! -d "$MY_DIR" ]]; then MY_DIR="$PWD"; fi

LINEAGE_ROOT="$MY_DIR"/../../..

HELPER="$LINEAGE_ROOT"/vendor/lineage/build/tools/extract_utils.sh
if [ ! -f "$HELPER" ]; then
    echo "Unable to find helper script at $HELPER"
    exit 1
fi
. "$HELPER"

# Initialize the common helper
setup_vendor "$DEVICE_COMMON" "$VENDOR" "$LINEAGE_ROOT" true

# Copyright headers and guards
write_headers "jasmine_sprout jason lavender twolip wayne whyred"

write_makefiles "$MY_DIR"/proprietary-files.txt true

printf "\n%s\n" "ifeq (\$(BOARD_HAVE_QCOM_FM),true)" >> "$PRODUCTMK"
write_makefiles "$MY_DIR"/proprietary-files-fm.txt true
echo "endif" >> "$PRODUCTMK"

# Finish
write_footers

if [ -s "$MY_DIR"/../$DEVICE_SPECIFIED_COMMON/proprietary-files.txt ]; then
    DEVICE_COMMON=$DEVICE_SPECIFIED_COMMON

    # Reinitialize the helper for device specified common
    INITIAL_COPYRIGHT_YEAR="$DEVICE_BRINGUP_YEAR"
    setup_vendor "$DEVICE_SPECIFIED_COMMON" "$VENDOR" "$LINEAGE_ROOT" true

    # Copyright headers and guards
    write_headers "$DEVICE_SPECIFIED_COMMON_DEVICE"

    # The standard device specified common blobs
    write_makefiles "$MY_DIR"/../$DEVICE_SPECIFIED_COMMON/proprietary-files.txt true

    # We are done!
    write_footers

    DEVICE_COMMON=sdm660-common
fi

if [ -s "$MY_DIR"/../$DEVICE/proprietary-files.txt ]; then
    # Reinitialize the helper for device
    INITIAL_COPYRIGHT_YEAR="$DEVICE_BRINGUP_YEAR"
    setup_vendor "$DEVICE" "$VENDOR" "$LINEAGE_ROOT" false

    # Copyright headers and guards
    write_headers

    # The standard device blobs
    write_makefiles "$MY_DIR"/../$DEVICE/proprietary-files.txt true

    # We are done!
    write_footers
fi
