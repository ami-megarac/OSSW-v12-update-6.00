##################################################################################
#
# INTEL CONFIDENTIAL
#
# Copyright 2021 Intel Corporation.
#
# This software and the related documents are Intel copyrighted materials, and
# your use of them is governed by the express license under which they were
# provided to you ("License"). Unless the License provides otherwise, you may
# not use, modify, copy, publish, distribute, disclose or transmit this
# software or the related documents without Intel's prior written permission.
#
# This software and the related documents are provided as is, with no express
# or implied warranties, other than those that are expressly stated in
# the License.
#
##################################################################################

*** Settings ***
Documentation       General crashdump tests
Library             ../libs/crawler.py
Library             Process
Library             OperatingSystem
Library             String
Library             Collections
Test setup          Is Binary Present
Test Timeout        1 minute

*** Variables ***
${icx_crashdump}         tests/general_resources/ICX_crashdump_1.json
${triage_info}           tests/general_resources/BAFI_ICX_crashdump_1.json

*** Test Cases ***
Version Display Test
    [Documentation]                     This is a test of current version display

    ${binary_path} =    DetermineSystem
    ${result} =         Run         ${binary_path} -v
    Should Contain      ${result}   BAFI version 1.7

Triage Error Information Display Test
    [Documentation]                     This is a test of triage info display

    ${binary_path} =    DetermineSystem
    ${result} =         Run         ${binary_path} -t ${icx_crashdump}
    ${triage_info} =    Get File    ${triage_info}  encoding=UTF-8
    Should Be Equal     ${result}   ${triage_info}

No Crashdump Provided Test
    [Documentation]                     This is a test of using flags without crashdump_name

    ${binary_path} =    DetermineSystem
    ${result} =         Run         ${binary_path} -t -h
    Should Contain      ${result}   File '-h' not found.


*** Keywords ***
Run Command
    [Arguments]         ${crashdump_name}
    Log To Console      \n
    ${result} =         Run    ${binary_path} ${crashdump_name}
    [Return]            ${result}