// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "gpu/command_buffer/service/image_factory.h"

#include "ui/gl/gl_image.h"

namespace gpu {

ImageFactory::ImageFactory() {
}

ImageFactory::~ImageFactory() {
}

scoped_refptr<gl::GLImage> ImageFactory::CreateAnonymousImage(
    const gfx::Size& size,
    gfx::BufferFormat format,
    gfx::BufferUsage usage,
    unsigned internalformat) {
  NOTREACHED();
  return nullptr;
}

unsigned ImageFactory::RequiredTextureType() {
  NOTIMPLEMENTED();
  return 0;
}

bool ImageFactory::SupportsFormatRGB() {
  return true;
}

}  // namespace gpu
