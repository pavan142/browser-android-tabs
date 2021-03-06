// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "components/download/internal/service_config_impl.h"
#include "components/download/internal/config.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace download {

TEST(ServiceConfigImplTest, TestApi) {
  Configuration config;
  ServiceConfigImpl impl(&config);

  config.max_scheduled_downloads = 7;
  config.file_keep_alive_time = base::TimeDelta::FromSeconds(12);

  EXPECT_EQ(config.max_scheduled_downloads,
            impl.GetMaxScheduledDownloadsPerClient());
  EXPECT_EQ(config.file_keep_alive_time, impl.GetFileKeepAliveTime());
}

}  // namespace download
