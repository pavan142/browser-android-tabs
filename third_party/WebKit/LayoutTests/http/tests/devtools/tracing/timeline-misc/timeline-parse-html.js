// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

(async function() {
  TestRunner.addResult(`Tests the Timeline API instrumentation of ParseHTML\n`);
  await TestRunner.loadModule('performance_test_runner');
  await TestRunner.showPanel('timeline');
  await TestRunner.evaluateInPagePromise(`
      function performActions()
      {
          var element = document.createElement("div");
          element.innerHTML = "Test data";
          document.body.appendChild(element);
      }

      if (!window.testRunner)
          setTimeout(performActions, 2000);
  `);

  PerformanceTestRunner.performActionsAndPrint('performActions()', 'ParseHTML');
})();
