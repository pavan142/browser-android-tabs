// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "components/cryptauth/ble/ble_advertisement_generator.h"

#include <vector>

#include "base/memory/ptr_util.h"
#include "components/cryptauth/local_device_data_provider.h"
#include "components/cryptauth/remote_beacon_seed_fetcher.h"
#include "components/proximity_auth/logging/logging.h"

namespace cryptauth {

// static
BleAdvertisementGenerator* BleAdvertisementGenerator::instance_ = nullptr;

// static
std::unique_ptr<DataWithTimestamp>
BleAdvertisementGenerator::GenerateBleAdvertisement(
    const RemoteDevice& remote_device,
    LocalDeviceDataProvider* local_device_data_provider,
    RemoteBeaconSeedFetcher* remote_beacon_seed_fetcher) {
  if (!instance_)
    instance_ = new BleAdvertisementGenerator();

  return instance_->GenerateBleAdvertisementInternal(
      remote_device, local_device_data_provider, remote_beacon_seed_fetcher);
}

// static
void BleAdvertisementGenerator::SetInstanceForTesting(
    BleAdvertisementGenerator* test_generator) {
  instance_ = test_generator;
}

BleAdvertisementGenerator::BleAdvertisementGenerator()
    : eid_generator_(base::MakeUnique<cryptauth::ForegroundEidGenerator>()) {}

BleAdvertisementGenerator::~BleAdvertisementGenerator() {}

std::unique_ptr<DataWithTimestamp>
BleAdvertisementGenerator::GenerateBleAdvertisementInternal(
    const RemoteDevice& remote_device,
    LocalDeviceDataProvider* local_device_data_provider,
    RemoteBeaconSeedFetcher* remote_beacon_seed_fetcher) {
  std::string local_device_public_key;
  if (!local_device_data_provider->GetLocalDeviceData(&local_device_public_key,
                                                      nullptr)) {
    PA_LOG(WARNING) << "Error fetching the local device's public key. Cannot "
                    << "advertise without the public key.";
    return nullptr;
  }

  if (local_device_public_key.empty()) {
    PA_LOG(WARNING) << "Local device's public key is empty. Cannot advertise "
                    << "with an invalid key.";
    return nullptr;
  }

  std::vector<BeaconSeed> remote_beacon_seeds;
  if (!remote_beacon_seed_fetcher->FetchSeedsForDevice(remote_device,
                                                       &remote_beacon_seeds)) {
    PA_LOG(WARNING) << "Error fetching beacon seeds for device with ID "
                    << remote_device.GetTruncatedDeviceIdForLogs() << ". "
                    << "Cannot advertise without seeds.";
    return nullptr;
  }

  if (remote_beacon_seeds.empty()) {
    PA_LOG(WARNING) << "No synced seeds exist for device with ID "
                    << remote_device.GetTruncatedDeviceIdForLogs() << ". "
                    << "Cannot advertise without seeds.";
    return nullptr;
  }

  std::unique_ptr<DataWithTimestamp> service_data =
      eid_generator_->GenerateAdvertisement(local_device_public_key,
                                            remote_beacon_seeds);
  if (!service_data) {
    PA_LOG(WARNING) << "Error generating advertisement for device with ID "
                    << remote_device.GetTruncatedDeviceIdForLogs() << ". "
                    << "Cannot advertise.";
    return nullptr;
  }

  return service_data;
}

void BleAdvertisementGenerator::SetEidGeneratorForTesting(
    std::unique_ptr<ForegroundEidGenerator> test_eid_generator) {
  eid_generator_ = std::move(test_eid_generator);
}

}  // namespace cryptauth
