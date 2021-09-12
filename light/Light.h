/*
 * Copyright (C) 2017-2020 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <android/hardware/light/2.0/ILight.h>

#include <unordered_map>

namespace android {
namespace hardware {
namespace light {
namespace V2_0 {
namespace implementation {

using ::android::hardware::Return;
using ::android::hardware::light::V2_0::ILight;
using ::android::hardware::light::V2_0::LightState;
using ::android::hardware::light::V2_0::Status;
using ::android::hardware::light::V2_0::Type;

class Light : public ILight {
  public:
    Light();

    Return<Status> setLight(Type type, const LightState& state) override;
    Return<void> getSupportedTypes(getSupportedTypes_cb _hidl_cb) override;

  private:
    void setLightBacklight(Type type, const LightState& state);
    void setLightButtons(Type type, const LightState& state);
    void setLightNotification(Type type, const LightState& state);
    void applyNotificationState(const LightState& state);

    uint32_t max_button_brightness_;
    uint32_t max_led_brightness_;
    uint32_t max_screen_brightness_;

    std::unordered_map<Type, std::function<void(Type type, const LightState&)>> lights_{
            {Type::BACKLIGHT, [this](auto&&... args) { setLightBacklight(args...); }},
            {Type::BATTERY, [this](auto&&... args) { setLightNotification(args...); }},
            {Type::NOTIFICATIONS, [this](auto&&... args) { setLightNotification(args...); }}};

    // Keep sorted in the order of importance.
    std::array<std::pair<Type, LightState>, 2> notif_states_ = {{
            {Type::NOTIFICATIONS, {}},
            {Type::BATTERY, {}},
    }};

    std::vector<std::string> buttons_;
};

}  // namespace implementation
}  // namespace V2_0
}  // namespace light
}  // namespace hardware
}  // namespace android
