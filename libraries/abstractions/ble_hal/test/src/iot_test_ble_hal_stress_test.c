/*
 * Amazon FreeRTOS BLE HAL V1.0.0
 * Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://aws.amazon.com/freertos
 * http://www.FreeRTOS.org
 */

/**
 * @file aws_test_ble_stress_test.c
 * @brief Tests for ble.
 */

#include "iot_test_ble_hal_stress_test.h"

extern BTService_t _xSrvcA;
extern BTService_t _xSrvcB;
extern BTInterface_t * _pxBTInterface;
extern BTCallbacks_t _xBTManagerCb;
extern BTBleAdapterCallbacks_t _xBTBleAdapterCb;
extern BTGattServerCallbacks_t _xBTGattServerCb;

#define bletestsFREERTOS_SVC_UUID_128_STRESS   { 0x00, 0x03, 0x32, 0xF9, 0x79, 0xE6, 0xB5, 0x83, 0xFB, 0x4E, 0xAF, 0x48, 0x68, 0x11, 0x7F, 0x8A }
#define bletestsFREERTOS_SVC_UUID_32_STRESS    0x8A7F0300
#define bletestsFREERTOS_SVC_UUID_16_STRESS    0x0300

uint8_t prv_test_number;

static BTUuid_t Test_Service_UUID_setup(BTuuidType_t type)
{
    uint8_t serviceUUID_128[ bt128BIT_UUID_LEN ] = bletestsFREERTOS_SVC_UUID_128_STRESS;
    BTUuid_t xServiceUuid_Integration =
    {
        .ucType = type
    };

    switch( type )
    {
        case eBTuuidType16:
            xServiceUuid_Integration.uu.uu16 = bletestsFREERTOS_SVC_UUID_16_STRESS + (uint16_t) prv_test_number;
            break;

        case eBTuuidType32:
            xServiceUuid_Integration.uu.uu32 = bletestsFREERTOS_SVC_UUID_32_STRESS + (uint32_t) prv_test_number;
            break;

        case eBTuuidType128:
            memcpy( xServiceUuid_Integration.uu.uu128, serviceUUID_128, sizeof( serviceUUID_128 ) );
            xServiceUuid_Integration.uu.uu128[0] += prv_test_number;
            break;
    }
    return xServiceUuid_Integration;
}

#define RUN_TEST_CASE_STRESS(group, name, repetation) \
    { void TEST_##group##_##name##_run(void);\
    for (uint16_t i = 0; i < (uint16_t) repetation; i++) \
      TEST_##group##_##name##_run(); }

static void GAP_common_teardown()
{
    BTStatus_t xStatus = eBTStatusSuccess;

    /* Disable */
    IotTestBleHal_BLEEnable( false );

    /* Deinit */
    xStatus = _pxBTInterface->pxBtManagerCleanup();
    TEST_ASSERT_EQUAL( eBTStatusSuccess, xStatus );
}
/*-----------------------------------------------------------*/

static void GAP_common_setup()
{
    BTStatus_t xStatus = eBTStatusSuccess;

    /* Get BT interface */
    _pxBTInterface = ( BTInterface_t * ) BTGetBluetoothInterface();
    TEST_ASSERT_NOT_EQUAL( NULL, _pxBTInterface );

    /* First time init */
    xStatus = _pxBTInterface->pxBtManagerInit( &_xBTManagerCb );
    TEST_ASSERT_EQUAL( eBTStatusSuccess, xStatus );

    /* Enable */
    IotTestBleHal_BLEEnable( true );
}

static void GATT_teardown()
{
    IotTestBleHal_BTUnregister();
    GAP_common_teardown();
}
/*-----------------------------------------------------------*/

static void GATT_setup()
{
    GAP_common_setup();
    IotTestBleHal_BLEGAPInit( &_xBTBleAdapterCb, true );
    IotTestBleHal_BLEGATTInit( &_xBTGattServerCb, true );
}

/*-----------------------------------------------------------*/

TEST_GROUP( Full_BLE_Stress_Test );

/*-----------------------------------------------------------*/

TEST_SETUP( Full_BLE_Stress_Test )
{
}

/*-----------------------------------------------------------*/

TEST_TEAR_DOWN( Full_BLE_Stress_Test )
{
}

/*-----------------------------------------------------------*/

TEST_GROUP_RUNNER( Full_BLE_Stress_Test )
{
    /* Initializations that need to be done once for all the tests. */
    RUN_TEST_CASE( Full_BLE, BLE_Setup );
    RUN_TEST_CASE_STRESS( Full_BLE_Stress_Test, BLE_Manager_Enable_Disable, 10 );
    RUN_TEST_CASE_STRESS( Full_BLE_Integration_Test_Advertisement, BLE_Advertise_With_16bit_ServiceUUID, 10 );
    RUN_TEST_GROUP( Full_BLE );
    RUN_TEST_CASE( Full_BLE, BLE_Setup );
    RUN_TEST_CASE_STRESS( Full_BLE_Integration_Test_Advertisement, BLE_Advertise_With_16bit_ServiceUUID, 10 );
    // RUN_TEST_CASE( Full_BLE_Stress_Test, BLE_WriteCharacteristicDecriptor );

    RUN_TEST_CASE( Full_BLE, BLE_Free );
}

TEST( Full_BLE_Stress_Test, BLE_Manager_Enable_Disable )
{
    GAP_common_setup();
    GAP_common_teardown();
}

// TEST( Full_BLE_Stress_Test, BLE_WriteCharacteristicDecriptor )
// {
//     GAP_common_setup();
//     GAP_common_teardown();
// }