// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

(async function() {
  TestRunner.addResult(`Tests that page reload with async stacks turned on does not crash. Bug 441223.\n`);
  await TestRunner.loadModule('sources_test_runner');
  await TestRunner.showPanel('sources');
  await TestRunner.loadHTML(`
      <p>
      Tests that page reload with async stacks turned on does not crash.
      <a href="https://code.google.com/p/chromium/issues/detail?id=441223">Bug 441223.</a>
      </p>
    `);
  await TestRunner.evaluateInPagePromise(`
      function testFunction()
      {
          setInterval(callback, 0);
          debugger;
      }

      function callback()
      {
          return window.__foo;
      }
  `);

  var maxAsyncCallStackDepth = 8;
  SourcesTestRunner.startDebuggerTest(step1, true);

  async function step1() {
    await TestRunner.DebuggerAgent.setAsyncCallStackDepth(maxAsyncCallStackDepth);
    SourcesTestRunner.runTestFunctionAndWaitUntilPaused(didPause);
  }

  function didPause() {
    TestRunner.addResult('Reloading the page...');
    TestRunner.reloadPage(afterReload);
  }

  function afterReload() {
    TestRunner.addResult('PASS: Reloaded successfully.');
    SourcesTestRunner.completeDebuggerTest();
  }
})();
