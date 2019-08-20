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

extern BTService_t g_xSrvcA;
extern BTService_t g_xSrvcB;
extern BTInterface_t * g_pxBTInterface;
extern uint8_t g_ucBLEServerIf;
extern BTCallbacks_t g_xBTManagerCb;
extern BTGattServerInterface_t * g_pxGattServerInterface;
extern BTBleAdapter_t * g_pxBTLeAdapterInterface;
extern uint8_t g_ucBLEAdapterIf;

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

    for (uint32_t init_loop = 0; init_loop < INIT_DEINIT_NUMBER_STRESS_TEST; init_loop++)
    {
        RUN_TEST_CASE( Full_BLE_Stress_Test, BLE_Stack_Init );

        for (uint32_t enable_loop = 0; enable_loop < ENABLE_DISABLE_NUMBER_STRESS_TEST; enable_loop++)
        {
            RUN_TEST_CASE( Full_BLE_Stress_Test, BLE_Stack_Enable );

            RUN_TEST_CASE( Full_BLE, BLE_Initialize_BLE_GAP );
            RUN_TEST_CASE( Full_BLE, BLE_Initialize_BLE_GATT );

            RUN_TEST_CASE( Full_BLE_Stress_Test, BLE_Service_Create );
            RUN_TEST_CASE( Full_BLE_Stress_Test, BLE_Service_Restart );
            RUN_TEST_CASE( Full_BLE_Stress_Test, BLE_Service_Delete );

            RUN_TEST_CASE( Full_BLE, BLE_Advertising_SetProperties ); /*@TOTO, incomplete */
            RUN_TEST_CASE( Full_BLE, BLE_Connection_RemoveAllBonds );
            RUN_TEST_CASE( Full_BLE, BLE_Advertising_SetAvertisementData ); /*@TOTO, incomplete */

            RUN_TEST_CASE( Full_BLE_Stress_Test, BLE_Connection_ReConnect );

            RUN_TEST_CASE( Full_BLE_Stress_Test, BLE_Teardown );

            RUN_TEST_CASE( Full_BLE_Stress_Test, BLE_Stack_Disable );
        }

        RUN_TEST_CASE( Full_BLE_Stress_Test, BLE_Stack_Deinit );
    }

    RUN_TEST_CASE( Full_BLE, BLE_Free );
}

TEST( Full_BLE_Stress_Test, BLE_Stack_Init )
{
    prvBLEManagerInit();
}

TEST( Full_BLE_Stress_Test, BLE_Stack_Enable )
{  
    prvBLEEnable(true);
}

TEST( Full_BLE_Stress_Test, BLE_Stack_Disable )
{
    prvBLEEnable(false);
}

TEST( Full_BLE_Stress_Test, BLE_Stack_Deinit )
{
    BTStatus_t xStatus = eBTStatusSuccess;
    xStatus = g_pxBTInterface->pxBtManagerCleanup();
    TEST_ASSERT_EQUAL( eBTStatusSuccess, xStatus );
}

TEST( Full_BLE_Stress_Test, BLE_Service_Delete )
{
    prvDeleteService( &g_xSrvcA );
    prvDeleteService( &g_xSrvcB );
}

TEST( Full_BLE_Stress_Test, BLE_Teardown )
{
    BTStatus_t xStatus = eBTStatusSuccess;
    BLETESTInitDeinitCallback_t xInitDeinitCb;

    xStatus = g_pxGattServerInterface->pxUnregisterServer( g_ucBLEServerIf );
    TEST_ASSERT_EQUAL( eBTStatusSuccess, xStatus );

    xStatus = prvWaitEventFromQueue( eBLEHALEventRegisterUnregisterGattServerCb, NO_HANDLE, ( void * ) &xInitDeinitCb, sizeof( BLETESTInitDeinitCallback_t ), BLE_TESTS_WAIT );
    TEST_ASSERT_EQUAL( eBTStatusSuccess, xStatus );
    TEST_ASSERT_EQUAL( eBTStatusSuccess, xInitDeinitCb.xStatus );


    xStatus = g_pxBTLeAdapterInterface->pxUnregisterBleApp( g_ucBLEAdapterIf );
    TEST_ASSERT_EQUAL( eBTStatusSuccess, xStatus );
}

TEST( Full_BLE_Stress_Test, BLE_Service_Create )
{
    /* Create service A */
    prvCreateService( &g_xSrvcA );
    prvCreateCharacteristic( &g_xSrvcA, bletestATTR_SRVCA_CHAR_A );

    /* Create service B */
    prvCreateService( &g_xSrvcB );
    prvCreateCharacteristic( &g_xSrvcB, bletestATTR_SRVCB_CHAR_A );
    prvCreateCharacteristic( &g_xSrvcB, bletestATTR_SRVCB_CHAR_B );
    prvCreateCharacteristic( &g_xSrvcB, bletestATTR_SRVCB_CHAR_C );
    prvCreateCharacteristic( &g_xSrvcB, bletestATTR_SRVCB_CHAR_D );
    prvCreateCharacteristic( &g_xSrvcB, bletestATTR_SRVCB_CHAR_E );
    prvCreateCharacteristicDescriptor( &g_xSrvcB, bletestATTR_SRVCB_CCCD_E );
    prvCreateCharacteristic( &g_xSrvcB, bletestATTR_SRVCB_CHAR_F );
    prvCreateCharacteristicDescriptor( &g_xSrvcB, bletestATTR_SRVCB_CCCD_F );
    prvCreateCharacteristicDescriptor( &g_xSrvcB, bletestATTR_SRVCB_CHARF_DESCR_A );
    prvCreateCharacteristicDescriptor( &g_xSrvcB, bletestATTR_SRVCB_CHARF_DESCR_B );
    prvCreateCharacteristicDescriptor( &g_xSrvcB, bletestATTR_SRVCB_CHARF_DESCR_C );
    prvCreateCharacteristicDescriptor( &g_xSrvcB, bletestATTR_SRVCB_CHARF_DESCR_D );

}

TEST( Full_BLE_Stress_Test, BLE_Service_Restart )
{
    prvRestartService( &g_xSrvcA );
    prvRestartService( &g_xSrvcB );
}

TEST( Full_BLE_Stress_Test, BLE_Connection_ReConnect )
{
    uint32_t loop;

    for( loop = 0; loop < RECONNECT_NUMBER_STRESS_TEST; loop++ )
    {
        prvStartAdvertisement();
        prvWaitConnection( true );
        prvWaitConnection( false );
    }
}

void prvRestartService( BTService_t * xRefSrvc )
{
    uint16_t loop;

    for( loop = 0; loop < RESTART_NUMBER_STRESS_TEST; loop++ )
    {
        BTStatus_t xStatus = eBTStatusSuccess;
        BLETESTServiceCallback_t xStopDeleteServiceCb;
        prvStartService( xRefSrvc );
        xStatus = g_pxGattServerInterface->pxStopService( g_ucBLEServerIf, xRefSrvc->pusHandlesBuffer[ 0 ] );
        TEST_ASSERT_EQUAL( eBTStatusSuccess, xStatus );

        xStatus = prvWaitEventFromQueue( eBLEHALEventServiceStoppedCb, xRefSrvc->pusHandlesBuffer[ 0 ], ( void * ) &xStopDeleteServiceCb, sizeof( BLETESTServiceCallback_t ), BLE_TESTS_WAIT );
        TEST_ASSERT_EQUAL( eBTStatusSuccess, xStatus );
        TEST_ASSERT_EQUAL( eBTStatusSuccess, xStopDeleteServiceCb.xStatus );
    }
}

void prvDeleteService( BTService_t * xRefSrvc )
{
    BTStatus_t xStatus = eBTStatusSuccess;
    BLETESTServiceCallback_t xStopDeleteServiceCb;

    xStatus = g_pxGattServerInterface->pxDeleteService( g_ucBLEServerIf, xRefSrvc->pusHandlesBuffer[ 0 ] );
    TEST_ASSERT_EQUAL( eBTStatusSuccess, xStatus );

    xStatus = prvWaitEventFromQueue( eBLEHALEventServiceDeletedCb, xRefSrvc->pusHandlesBuffer[ 0 ], ( void * ) &xStopDeleteServiceCb, sizeof( BLETESTServiceCallback_t ), BLE_TESTS_WAIT );
    TEST_ASSERT_EQUAL( eBTStatusSuccess, xStatus );
    TEST_ASSERT_EQUAL( eBTStatusSuccess, xStopDeleteServiceCb.xStatus );
}
