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
 * Migration Note (2026-08-28):
 * This service has been converted from HIDL (vendor.lineage.powershare@1.0)
 * to AIDL (aidl/vendor/lineage/powershare) for Android 14+ compatibility.
 *
 * Key changes:
 * - Removed HIDL dependencies: <hidl/HidlTransportSupport.h>, <binder/ProcessState.h>
 * - Replaced with NDK Binder: <android/binder_manager.h>, <android/binder_process.h>
 * - Thread pool: configureRpcThreadpool(1, true) -> ABinderProcess_setThreadPoolMaxThreadCount(1)
 * - Service registration: registerAsService() -> AServiceManager_addService()
 * - Service object: sp<IPowerShare> -> ndk::SharedRefBase::make<PowerShare>()
 * - Added explicit instance name: descriptor + "/default"
 * - Removed vndbinder driver init (now uses default binder)
 * - Simplified shutdown handling (no goto, return 1 on failure)
 */

#define LOG_TAG "vendor.lineage.powershare-service.sony"

#include <android-base/logging.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>

#include <powershare/sony/PowerShare.h>

using aidl::vendor::lineage::powershare::PowerShare;

int main() {
    ABinderProcess_setThreadPoolMaxThreadCount(1);

    auto ps = ndk::SharedRefBase::make<PowerShare>();

    const std::string instance =
            std::string() + PowerShare::descriptor + "/default";

    LOG(INFO) << "PowerShare HAL service is starting.";

    binder_status_t status =
            AServiceManager_addService(ps->asBinder().get(), instance.c_str());

    if (status != STATUS_OK) {
        LOG(ERROR) << "Could not register PowerShare HAL service: "
                   << status;
        return 1;
    }

    LOG(INFO) << "PowerShare HAL service is ready.";

    ABinderProcess_joinThreadPool();

    return 0;
}
