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
Documentation       ICX crashdumps content tests
Library             ../libs/crawler.py
Library             Process
Library             OperatingSystem
Library             String
Library             Collections
Test setup          Is Binary Present
Test Timeout        1 minute

*** Variables ***
${icx_crashdump1}        tests/ICX/ICX_crashdump_1.json
${icx_crashdump1_bafi}   tests/ICX/BAFI_ICX_crashdump_1.json
${icx_crashdump2}        tests/ICX/ICX_crashdump_2.json
${icx_crashdump2_bafi}   tests/ICX/BAFI_ICX_crashdump_2.json
${icx_crashdump3}        tests/ICX/ICX_crashdump_3.json
${icx_crashdump3_bafi}   tests/ICX/BAFI_ICX_crashdump_3.json

*** Test Cases ***
Icx Test One
    [Documentation]                     This is a test comparing generated crashdump with BAFI_ICX_crashdump_1.json

    ${result} =                         Run Command                                              ${icx_crashdump1}
    ${json} =                           evaluate                                                 json.loads('''${result}''')     json
    Dictionary Should Contain Key       ${json['summary']}                                       MCA
    Dictionary Should Contain Key       ${json['summary']}                                       TOR
    Dictionary Should Contain Key       ${json['summary']}                                       output_summary
    Dictionary Should Contain Key       ${json['summary']['output_summary']}                     socket0
    Dictionary Should Contain Key       ${json['summary']['output_summary']}                     socket1
    Dictionary Should Contain Key       ${json['summary']['output_summary']['socket0'][0]}       MCE
    Dictionary Should Contain Key       ${json['summary']['output_summary']['socket0'][1]}       PCIe_AER_Uncorrectable_errors
    Dictionary Should Contain Key       ${json['summary']['output_summary']['socket0'][2]}       PCIe_AER_Correctable_errors
    Dictionary Should Contain Key       ${json['summary']['output_summary']['socket0'][3]}       TSC
    ${json_differences} =               Check Differences                       ${result}        ${icx_crashdump1_bafi}
    Should Be Empty                     ${json_differences}
    Run Keyword If                      '${json_differences}' != '''[]'''       Log To Console   ${json_differences}

Icx Test Two
    [Documentation]                     This is a test comparing generated crashdump with BAFI_ICX_crashdump_2.json

    ${result} =                         Run Command                                              ${icx_crashdump2}
    ${json} =                           evaluate                                                 json.loads('''${result}''')     json
    Dictionary Should Contain Key       ${json['summary']}                                       MCA
    Dictionary Should Contain Key       ${json['summary']}                                       TOR
    Dictionary Should Contain Key       ${json['summary']}                                       output_summary
    Dictionary Should Contain Key       ${json['summary']['output_summary']}                     socket0
    Dictionary Should Contain Key       ${json['summary']['output_summary']}                     socket1
    Dictionary Should Contain Key       ${json['summary']['output_summary']['socket0'][0]}       MCE
    Dictionary Should Contain Key       ${json['summary']['output_summary']['socket0'][1]}       TSC
    Dictionary Should Contain Key       ${json['summary']['output_summary']['socket0'][2]}       Errors_Per_Socket
    Dictionary Should Contain Key       ${json['summary']['output_summary']['socket0'][3]}       First_MCERR_FALSE
    Dictionary Should Contain Key       ${json['summary']['output_summary']['socket0'][4]}       First_MCERR
    ${json_differences} =               Check Differences                       ${result}        ${icx_crashdump2_bafi}
    Should Be Empty                     ${json_differences}
    Run Keyword If                      '${json_differences}' != '''[]'''       Log To Console   ${json_differences}

Icx Test Three
    [Documentation]                     This is a test comparing generated crashdump with BAFI_ICX_crashdump_3.json

    ${binary_path} =                    Determine System
    ${result} =                         Run Command                                              ${icx_crashdump3}
    ${json} =                           evaluate                                                 json.loads('''${result}''')     json
    Dictionary Should Contain Key       ${json['summary']}                                       MCA
    Dictionary Should Contain Key       ${json['summary']}                                       TOR
    Dictionary Should Contain Key       ${json['summary']}                                       output_summary
    Dictionary Should Contain Key       ${json['summary']['output_summary']}                     socket0
    Dictionary Should Contain Key       ${json['summary']['output_summary']}                     socket1
    Dictionary Should Contain Key       ${json['summary']['output_summary']['socket0'][0]}       MCE
    Dictionary Should Contain Key       ${json['summary']['output_summary']['socket0'][1]}       TSC
    Dictionary Should Contain Key       ${json['summary']['output_summary']['socket0'][2]}       Errors_Per_Socket
    Dictionary Should Contain Key       ${json['summary']['output_summary']['socket0'][3]}       First_MCERR_FALSE
    ${json_differences} =               Check Differences                       ${result}        ${icx_crashdump3_bafi}
    Should Be Empty                     ${json_differences}
    Run Keyword If                      '${json_differences}' != '''[]'''       Log To Console   ${json_differences}


*** Keywords ***
Run Command
    [Arguments]         ${crashdump_name}
    Log To Console      \n
    ${binary_path} =         Determine System
    ${result} =         Run    ${binary_path} -p default_device_map.json ${crashdump_name}
    [Return]            ${result}