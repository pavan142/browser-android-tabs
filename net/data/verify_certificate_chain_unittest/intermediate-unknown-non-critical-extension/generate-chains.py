#!/usr/bin/python
# Copyright (c) 2015 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

"""Certificate chain where the intermediate contains an unknown non-critical
extension."""

import sys
sys.path += ['..']

import common

# Self-signed root certificate.
root = common.create_self_signed_root_certificate('Root')
intermediate = common.create_intermediate_certificate('Intermediate', root)

# Intermediate that has an unknown non-critical extension.
intermediate.get_extensions().add_property('1.2.3.4', 'DER:01:02:03:04')

# Target certificate.
target = common.create_end_entity_certificate('Target', intermediate)

chain = [target, intermediate, root]
common.write_chain(__doc__, chain, 'chain.pem')
