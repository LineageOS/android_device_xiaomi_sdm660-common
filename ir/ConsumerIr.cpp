/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "ConsumerIrService.xiaomi_sdm660"

#include <android-base/logging.h>

#include <hardware/hardware.h>
#include <hardware/consumerir.h>

#include "ConsumerIr.h"

namespace android {
namespace hardware {
namespace ir {
namespace V1_0 {
namespace implementation {

typedef struct ir_device {
    const std::string name;
    const std::string device_path;
} ir_device_t;

const static ir_device_t devices[] = {
    {"lirc", "/dev/lirc0"},
    {"spi", "/dev/spidev7.1"},
};

ConsumerIr::ConsumerIr() : mDevice(nullptr) {
    const hw_module_t *hw_module;
    int ret;

    for (auto& [name, device_path] : devices) {
        hw_module = NULL;
        ret = 0;

        if (access(device_path.c_str(), F_OK) == -1)
            continue;

        ret = hw_get_module_by_class(CONSUMERIR_HARDWARE_MODULE_ID, name.c_str(), &hw_module);
        if (ret != 0) {
            LOG(ERROR) << "hw_get_module " CONSUMERIR_HARDWARE_MODULE_ID " (class "
                       << name << ") failed: " << ret;
            continue;
        }
        ret = hw_module->methods->open(hw_module, CONSUMERIR_TRANSMITTER, (hw_device_t **) &mDevice);
        if (ret < 0) {
            LOG(ERROR) << "Can't open consumer IR transmitter (class " << name
                       << "), error: " << ret;
            mDevice = nullptr;
            continue;
        }
        break;
    }

    if (mDevice == nullptr) {
        LOG(FATAL) << "Could not find a working ConsumerIR HAL";
    }
}

ConsumerIr::~ConsumerIr() {
    if (mDevice == nullptr)
        return;

    mDevice->common.close((hw_device_t *) mDevice);
    mDevice = nullptr;
}

// Methods from ::android::hardware::consumerir::V1_0::IConsumerIr follow.
Return<bool> ConsumerIr::transmit(int32_t carrierFreq, const hidl_vec<int32_t>& pattern) {
    return mDevice->transmit(mDevice, carrierFreq, pattern.data(), pattern.size()) == 0;
}

Return<void> ConsumerIr::getCarrierFreqs(getCarrierFreqs_cb _hidl_cb) {
    int32_t len = mDevice->get_num_carrier_freqs(mDevice);
    if (len < 0) {
        _hidl_cb(false, {});
        return Void();
    }

    consumerir_freq_range_t *rangeAr = new consumerir_freq_range_t[len];
    bool success = (mDevice->get_carrier_freqs(mDevice, len, rangeAr) >= 0);
    if (!success) {
        _hidl_cb(false, {});
        return Void();
    }

    hidl_vec<ConsumerIrFreqRange> rangeVec;
    rangeVec.resize(len);
    for (int32_t i = 0; i < len; i++) {
        rangeVec[i].min = static_cast<uint32_t>(rangeAr[i].min);
        rangeVec[i].max = static_cast<uint32_t>(rangeAr[i].max);
    }
    _hidl_cb(true, rangeVec);
    return Void();
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace ir
}  // namespace hardware
}  // namespace android
