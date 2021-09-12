/*
 * Copyright 2016 The Android Open Source Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "android.hardware.ir@1.0-service"

#include <android/hardware/ir/1.0/IConsumerIr.h>
#include <hidl/LegacySupport.h>
#include "ConsumerIr.h"

using android::hardware::ir::V1_0::IConsumerIr;
using android::hardware::ir::V1_0::implementation::ConsumerIr;
using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;

int main() {
    android::sp<ConsumerIr> service = new ConsumerIr();

    if (!service->isOk()) {
        ALOGE("Instance of ConsumerIr failed to init");
        return 1;
    }

    configureRpcThreadpool(1, true /*callerWillJoin*/);

    android::status_t status = service->registerAsService();
    if (status != android::OK) {
        ALOGE("Cannot register ConsumerIr service");
        return 1;
    }

    joinRpcThreadpool();

    return 0; // should never get here
}
