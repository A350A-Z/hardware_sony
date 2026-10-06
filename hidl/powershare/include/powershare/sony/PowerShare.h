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
 * Conversion Note(2026.8.28):
 * This implementation has been converted from HIDL (V1_0) to AIDL (NDK).
 * Key differences:
 * - All methods now return ndk::ScopedAStatus and use pointers for output.
 * - Parameter types changed: uint32_t -> int32_t.
 */
#pragma once

#include <aidl/vendor/lineage/powershare/BnPowerShare.h>

namespace aidl {
namespace vendor {
namespace lineage {
namespace powershare {

class PowerShare : public BnPowerShare {
  public:
    ndk::ScopedAStatus isEnabled(bool* _aidl_return) override;
    ndk::ScopedAStatus setEnabled(bool enable) override;
    ndk::ScopedAStatus getMinBattery(int32_t* _aidl_return) override;
    ndk::ScopedAStatus setMinBattery(int32_t minBattery) override;
};

}  // namespace powershare
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
