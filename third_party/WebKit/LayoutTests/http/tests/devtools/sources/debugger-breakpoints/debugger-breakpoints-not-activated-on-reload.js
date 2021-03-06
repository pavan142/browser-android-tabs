// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

(async function() {
  TestRunner.addResult(`Tests that breakpoints are not activated on page reload.Bug 41461\n`);
  await TestRunner.loadModule('sources_test_runner');
  await TestRunner.showPanel('sources');
  await TestRunner.loadHTML(`
      <p>
      Tests that breakpoints are not activated on page reload.<a href="https://bugs.webkit.org/show_bug.cgi?id=41461">Bug 41461</a>
      </p>
    `);
  await TestRunner.evaluateInPagePromise(`
      function testFunction()
      {
          return 0;
      }
  `);

  var testName = TestRunner.mainTarget.inspectedURL();
  testName = testName.substring(testName.lastIndexOf('/') + 1);

  SourcesTestRunner.startDebuggerTest(step1);

  function step1() {
    SourcesTestRunner.showScriptSource(testName, step2);
  }

  function step2(sourceFrame) {
    TestRunner.addResult('Main resource was shown.');
    SourcesTestRunner.setBreakpoint(sourceFrame, 8, '', true);
    UI.panels.sources._toggleBreakpointsActive();
    TestRunner.reloadPage(step3);
  }

  function step3() {
    TestRunner.addResult('Main resource was shown.');
    if (!Bindings.breakpointManager.breakpointsActive())
      TestRunner.addResult('Breakpoints are deactivated.');
    else
      TestRunner.addResult('Error: breakpoints are activated.');
    SourcesTestRunner.completeDebuggerTest();
  }
})();
