/*
 * Copyright (C) 2017 The Android Open Source Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "android.hardware.biometrics.fingerprint@2.1-service.xiaomi_sdm660"

#include <android/log.h>
#include <hidl/HidlTransportSupport.h>

#include "BiometricsFingerprint.h"

// libhwbinder:
using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;

// Generated HIDL files
using android::hardware::biometrics::fingerprint::V2_1::IBiometricsFingerprint;
using android::hardware::biometrics::fingerprint::V2_1::implementation::BiometricsFingerprint;

int main() {
    android::sp<IBiometricsFingerprint> service = BiometricsFingerprint::getInstance();

    if (service == nullptr) {
        ALOGE("Instance of BiometricsFingerprint is null");
        return 1;
    }

    configureRpcThreadpool(1, true /*callerWillJoin*/);

    android::status_t status = service->registerAsService();
    if (status != android::OK) {
        ALOGE("Cannot register BiometricsFingerprint service");
        return 1;
    }

    joinRpcThreadpool();

    return 0; // should never get here
}
