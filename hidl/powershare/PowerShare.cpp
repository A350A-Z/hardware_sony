/*
 * Copyright (C) 2024 XperiaLabs Project
 * Copyright (C) 2022 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * Conversion Note (2026-08-28):
 * This implementation has been migrated from HIDL (vendor.lineage.powershare@1.0)
 * to AIDL (aidl/vendor/lineage/powershare) for Android 14+ compatibility.
 *
 * Key changes:
 * - Namespace: vendor::lineage::powershare::V1_0::implementation
 *              -> aidl::vendor::lineage::powershare
 * - Method signatures: Return<T> -> ndk::ScopedAStatus with output pointers
 * - Data types: uint32_t -> int32_t for minBattery
 * - Removed unused include <android-base/strings.h>
 * - Adjusted LOG_TAG to reflect new service naming
 */


#define LOG_TAG "vendor.lineage.powershare-service.sony"

#include <android-base/logging.h>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <fstream>

#include <powershare/sony/PowerShare.h>

namespace aidl {
namespace vendor {
namespace lineage {
namespace powershare {

static bool set(const std::string& path, const std::string& value) {
    int fd = open(path.c_str(), O_WRONLY);
    if (fd < 0) {
        LOG(ERROR) << "open failed: " << errno;
        return false;
    }

    ssize_t ret = write(fd, value.c_str(), value.size());
    if (ret < 0) {
        int write_errno = errno;
        close(fd);
        LOG(ERROR) << "write failed: " << write_errno;
        return false;
    }

    if (static_cast<size_t>(ret) != value.size()) {
        close(fd);
        LOG(ERROR) << "short write: " << ret << "/" << value.size();
        return false;
    }

    if (close(fd) < 0) {
        int close_errno = errno;
        LOG(WARNING) << "close returned error: " << close_errno;
        return false;
    }

    return true;
}

template <typename T>
static T get(const std::string& path, const T& def) {
    std::ifstream file(path);
    T result;

    file >> result;
    return file.fail() ? def : result;
}

ndk::ScopedAStatus PowerShare::isEnabled(bool* _aidl_return) {
    const auto value = get<std::string>(WIRELESS_TX_ENABLE_PATH, "0");

    *_aidl_return = !(value == "disable" || value == "0");

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus PowerShare::setEnabled(bool enable) {
    set(WIRELESS_TX_ENABLE_PATH, enable ? "1" : "0");

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus PowerShare::getMinBattery(int32_t* _aidl_return) {
    *_aidl_return = 0;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus PowerShare::setMinBattery(int32_t /*minBattery*/) {
    return ndk::ScopedAStatus::ok();
}

}  // namespace powershare
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
