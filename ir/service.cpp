/*
 * Copyright 2016 The Android Open Source Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "android.hardware.ir@1.0-service"

#include <android/hardware/ir/1.0/IConsumerIr.h>
#include <hidl/LegacySupport.h>

using android::hardware::ir::V1_0::IConsumerIr;
using android::hardware::defaultPassthroughServiceImplementation;

int main() {
    return defaultPassthroughServiceImplementation<IConsumerIr>();
}
