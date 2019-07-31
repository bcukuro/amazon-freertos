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

#define TOTAL_NUMBER_STRESS_TEST 10

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
    RUN_TEST_CASE( Full_BLE_Stress_Test, BLE_Stack_Init )
    
    int loop;
    for ( loop = 1 ; loop < TOTAL_NUMBER_STRESS_TEST+1 ; loop++ )
    {
        TEST_NotifyNextTestStart( loop, TOTAL_NUMBER_STRESS_TEST );
        RUN_TEST_CASE( Full_BLE_Stress_Test, BLE_Setup );
        RUN_TEST_CASE( Full_BLE, BLE_Initialize_common_GAP );
        RUN_TEST_CASE( Full_BLE, BLE_Initialize_BLE_GAP );
        RUN_TEST_CASE( Full_BLE, BLE_Initialize_BLE_GATT );


        RUN_TEST_CASE( Full_BLE, BLE_CreateAttTable_CreateServices );

        RUN_TEST_CASE( Full_BLE, BLE_Advertising_SetProperties ); /*@TOTO, incomplete */
        RUN_TEST_CASE( Full_BLE, BLE_Connection_RemoveAllBonds );

        RUN_TEST_CASE( Full_BLE, BLE_Advertising_SetAvertisementData ); /*@TOTO, incomplete */
        RUN_TEST_CASE( Full_BLE, BLE_Advertising_StartAdvertisement );
        RUN_TEST_CASE( Full_BLE, BLE_Connection_SimpleConnection );
    /*RUN_TEST_CASE( Full_BLE, BLE_Connection_UpdateConnectionParamReq ); */

    /*RUN_TEST_CASE( Full_BLE, BLE_Connection_ChangeMTUsize ); */
        RUN_TEST_CASE( Full_BLE, BLE_Property_WriteCharacteristic );
        RUN_TEST_CASE( Full_BLE, BLE_Property_WriteDescriptor );
        RUN_TEST_CASE( Full_BLE, BLE_Property_ReadCharacteristic );
        RUN_TEST_CASE( Full_BLE, BLE_Property_ReadDescriptor );
        RUN_TEST_CASE( Full_BLE, BLE_Property_WriteNoResponse );
        RUN_TEST_CASE( Full_BLE, BLE_Property_Enable_Indication_Notification );
        RUN_TEST_CASE( Full_BLE, BLE_Property_Notification );
        RUN_TEST_CASE( Full_BLE, BLE_Property_Indication );
        RUN_TEST_CASE( Full_BLE, BLE_Property_Disable_Indication_Notification );

        RUN_TEST_CASE( Full_BLE, BLE_Connection_Mode1Level4 );
        RUN_TEST_CASE( Full_BLE, BLE_Connection_Mode1Level4_Property_WriteDescr );
        RUN_TEST_CASE( Full_BLE, BLE_Connection_Mode1Level4_Property_WriteChar );
        RUN_TEST_CASE( Full_BLE, BLE_Connection_Disconnect );
        RUN_TEST_CASE( Full_BLE, BLE_Connection_BondedReconnectAndPair );
        RUN_TEST_CASE( Full_BLE, BLE_Connection_Disconnect );

        RUN_TEST_CASE( Full_BLE, BLE_Connection_CheckBonding );
        RUN_TEST_CASE( Full_BLE, BLE_Connection_RemoveBonding );
        RUN_TEST_CASE( Full_BLE, BLE_Connection_Mode1Level2 );

        RUN_TEST_CASE( Full_BLE, BLE_DeInitialize );
        RUN_TEST_CASE( Full_BLE, BLE_Free );
    }
}

TEST( Full_BLE_Stress_Test, BLE_Stack_Init )
{
    BTStatus_t xStatus;

    xStatus = bleStackInit();

    if( xStatus != eBTStatusSuccess )
    {
        TEST_FAIL_MESSAGE( "Unable to initialize BLE stask.\n" );
    }
}

TEST( Full_BLE_Stress_Test, BLE_Setup )
{
    /* Create a queue, semaphore and mutexes for callbacks. */
    if( IotMutex_Create( &threadSafetyMutex, false ) != true )
    {
        TEST_FAIL_MESSAGE( "Could not create threadSafetyMutex.\n" );
    }

    if( IotSemaphore_Create( &eventSemaphore, 0, MAX_EVENT ) != true )
    {
        TEST_FAIL_MESSAGE( "Could not create eventSemaphore.\n" );
    }

    IotListDouble_Create( &eventQueueHead );
}