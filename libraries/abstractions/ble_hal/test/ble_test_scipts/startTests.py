#
# Amazon FreeRTOS BLE HAL V2.0.0
# Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#
# http://aws.amazon.com/freertos
# http://www.FreeRTOS.org
#

import Queue
import sys
import os
import threading
import securityAgent
import bleAdapter
import time
from testClass import runTest
from bleAdapter import bleAdapter
from test_cases import test_uuid
import testutils


def main():
    scan_filter = dict()

    bleAdapter.init()
    agent = securityAgent.createSecurityAgent()

    while True:
        scan_filter.update({"UUIDs": list(test_uuid.keys())})
        bleAdapter.setDiscoveryFilter(scan_filter)

        # Discovery test
        bleAdapter.startDiscovery(runTest.discoveryEventCb)
        runTest.mainloop.run()
        isTestSuccessFull = True
        runTest.submitTestResult(isTestSuccessFull, runTest.advertisement)
        bleAdapter.stopDiscovery()
        
        test_uuid[runTest.TEST_CASE_UUID]["test_function"](scan_filter=scan_filter, bleAdapter=bleAdapter, agent=agent)

        securityAgent.removeSecurityAgent()

        time.sleep(2)
        runTest.printTestsSummary()

        os.system("rm -rf \"/var/lib/bluetooth/*\"")
        os.system("hciconfig hci0 reset")

        testutils.removeBondedDevices()

        agent = securityAgent.createSecurityAgent(agent=agent)