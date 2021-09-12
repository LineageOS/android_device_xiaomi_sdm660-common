/*
 * Copyright (C) 2018 The Android Open Source Project
 * Copyright (C) 2020 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

// Author := dev_harsh1998, Isaac Chen

#define LOG_TAG "android.hardware.light@2.0-impl.xiaomi_sdm660"
/* #define LOG_NDEBUG 0 */

#include "Light.h"

#include <android-base/file.h>
#include <android-base/logging.h>
#include <unistd.h>

namespace {

#define PPCAT_NX(A, B) A/B
#define PPCAT(A, B) PPCAT_NX(A, B)
#define STRINGIFY_INNER(x) #x
#define STRINGIFY(x) STRINGIFY_INNER(x)

#define LEDS(x) PPCAT(/sys/class/leds, x)
#define LCD_ATTR(x) STRINGIFY(PPCAT(LEDS(lcd-backlight), x))
#define WHITE_ATTR(x) STRINGIFY(PPCAT(LEDS(white), x))
#define BUTTON_ATTR(x) STRINGIFY(PPCAT(LEDS(button-backlight), x))
#define BUTTON1_ATTR(x) STRINGIFY(PPCAT(LEDS(button-backlight1), x))

using ::android::base::ReadFileToString;
using ::android::base::WriteStringToFile;

// Default max brightness
constexpr auto kDefaultMaxLedBrightness = 255;
constexpr auto kDefaultMaxScreenBrightness = 4095;

// Each step will stay on for 50ms by default.
constexpr auto kRampStepDuration = 50;

// Each value represents a duty percent (0 - 100) for the led pwm.
constexpr std::array kBrightnessRamp = {0, 12, 25, 37, 50, 72, 85, 100};

// Write value to path and close file.
bool WriteToFile(const std::string& path, uint32_t content) {
    return WriteStringToFile(std::to_string(content), path);
}

bool WriteToFile(const std::string& path, const std::string& content) {
    return WriteStringToFile(content, path);
}

uint32_t RgbaToBrightness(uint32_t color) {
    // Extract brightness from AARRGGBB.
    uint32_t alpha = (color >> 24) & 0xFF;

    // Retrieve each of the RGB colors
    uint32_t red = (color >> 16) & 0xFF;
    uint32_t green = (color >> 8) & 0xFF;
    uint32_t blue = color & 0xFF;

    // Scale RGB colors if a brightness has been applied by the user
    if (alpha != 0xFF) {
        red = red * alpha / 0xFF;
        green = green * alpha / 0xFF;
        blue = blue * alpha / 0xFF;
    }

    return (77 * red + 150 * green + 29 * blue) >> 8;
}

inline uint32_t RgbaToBrightness(uint32_t color, uint32_t max_brightness) {
    return RgbaToBrightness(color) * max_brightness / 0xFF;
}

/*
 * Scale each value of the brightness ramp according to the
 * brightness of the color.
 */
std::string GetScaledDutyPcts(uint32_t brightness) {
    std::stringstream ramp;

    for (size_t i = 0; i < kBrightnessRamp.size(); i++) {
        if (i > 0) ramp << ",";
        ramp << kBrightnessRamp[i] * brightness / 0xFF;
    }

    return ramp.str();
}

inline bool IsLit(uint32_t color) {
    return color & 0x00ffffff;
}

}  // anonymous namespace

namespace android {
namespace hardware {
namespace light {
namespace V2_0 {
namespace implementation {

Light::Light() {
    std::string buf;

    if (ReadFileToString(LCD_ATTR(max_brightness), &buf)) {
        max_screen_brightness_ = std::stoi(buf);
    } else {
        max_screen_brightness_ = kDefaultMaxScreenBrightness;
        LOG(ERROR) << "Failed to read max screen brightness, fallback to "
                   << kDefaultMaxScreenBrightness;
    }

    if (ReadFileToString(WHITE_ATTR(max_brightness), &buf)) {
        max_led_brightness_ = std::stoi(buf);
    } else {
        max_led_brightness_ = kDefaultMaxLedBrightness;
        LOG(ERROR) << "Failed to read max LED brightness, fallback to " << kDefaultMaxLedBrightness;
    }

    if (!access(BUTTON_ATTR(brightness), W_OK)) {
        lights_.emplace(std::make_pair(Type::BUTTONS,
                                       [this](auto&&... args) { setLightButtons(args...); }));
        buttons_.emplace_back(BUTTON_ATTR(brightness));

        if (!access(BUTTON1_ATTR(brightness), W_OK)) {
            buttons_.emplace_back(BUTTON1_ATTR(brightness));
        }

        if (ReadFileToString(BUTTON_ATTR(max_brightness), &buf)) {
            max_button_brightness_ = std::stoi(buf);
        } else {
            max_button_brightness_ = kDefaultMaxLedBrightness;
            LOG(ERROR) << "Failed to read max button brightness, fallback to "
                       << kDefaultMaxLedBrightness;
        }
    }
}

Return<Status> Light::setLight(Type type, const LightState& state) {
    auto it = lights_.find(type);

    if (it == lights_.end()) {
        return Status::LIGHT_NOT_SUPPORTED;
    }

    it->second(type, state);

    return Status::SUCCESS;
}

Return<void> Light::getSupportedTypes(getSupportedTypes_cb _hidl_cb) {
    std::vector<Type> types;

    for (auto&& light : lights_) types.emplace_back(light.first);

    _hidl_cb(types);

    return Void();
}

void Light::setLightBacklight(Type /*type*/, const LightState& state) {
    uint32_t brightness = RgbaToBrightness(state.color, max_screen_brightness_);
    WriteToFile(LCD_ATTR(brightness), brightness);
}

void Light::setLightButtons(Type /*type*/, const LightState& state) {
    uint32_t brightness = RgbaToBrightness(state.color, max_button_brightness_);
    for (auto&& button : buttons_) {
        WriteToFile(button, brightness);
    }
}

void Light::setLightNotification(Type type, const LightState& state) {
    bool found = false;
    for (auto&& [cur_type, cur_state] : notif_states_) {
        if (cur_type == type) {
            cur_state = state;
        }

        // Fallback to battery light
        if (!found && (cur_type == Type::BATTERY || IsLit(cur_state.color))) {
            found = true;
            LOG(DEBUG) << __func__ << ": type=" << toString(cur_type);
            applyNotificationState(cur_state);
        }
    }
}

void Light::applyNotificationState(const LightState& state) {
    uint32_t white_brightness = RgbaToBrightness(state.color, max_led_brightness_);

    // Turn off the leds (initially)
    WriteToFile(WHITE_ATTR(blink), 0);

    if (state.flashMode == Flash::TIMED && state.flashOnMs > 0 && state.flashOffMs > 0) {
        /*
         * If the flashOnMs duration is not long enough to fit ramping up
         * and down at the default step duration, step duration is modified
         * to fit.
         */
        int32_t step_duration = kRampStepDuration;
        int32_t pause_hi = state.flashOnMs - (step_duration * kBrightnessRamp.size() * 2);
        if (pause_hi < 0) {
            step_duration = state.flashOnMs / (kBrightnessRamp.size() * 2);
            pause_hi = 0;
        }

        LOG(DEBUG) << __func__ << ": color=" << std::hex << state.color << std::dec
                   << " onMs=" << state.flashOnMs << " offMs=" << state.flashOffMs;

        // White
        WriteToFile(WHITE_ATTR(start_idx), 0);
        WriteToFile(WHITE_ATTR(duty_pcts), GetScaledDutyPcts(white_brightness));
        WriteToFile(WHITE_ATTR(pause_lo), static_cast<uint32_t>(state.flashOffMs));
        WriteToFile(WHITE_ATTR(pause_hi), static_cast<uint32_t>(pause_hi));
        WriteToFile(WHITE_ATTR(ramp_step_ms), static_cast<uint32_t>(step_duration));
        WriteToFile(WHITE_ATTR(blink), 1);
    } else {
        WriteToFile(WHITE_ATTR(brightness), white_brightness);
    }
}

}  // namespace implementation
}  // namespace V2_0
}  // namespace light
}  // namespace hardware
}  // namespace android
