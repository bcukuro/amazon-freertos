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
 * @file aws_test_ble_integration.c
 * @brief Tests for ble.
 */


#include <time.h>
#include <unistd.h>


#include "iot_test_ble_hal_integration.h"

extern BTUuid_t g_xServerUUID;
extern BTUuid_t g_xAppUUID;

extern BTGattAdvertismentParams_t g_xAdvertisementConfigA;

extern BTGattAdvertismentParams_t g_xAdvertisementConfigB;

extern BTCallbacks_t g_xBTManagerCb;

extern BTBleAdapterCallbacks_t g_xBTBleAdapterCb;


extern BTGattServerCallbacks_t g_xBTGattServerCb;

extern BTGattServerInterface_t * g_pxGattServerInterface;
extern BTBleAdapter_t * g_pxBTLeAdapterInterface;
extern BTInterface_t * g_pxBTInterface;
extern uint8_t g_ucBLEAdapterIf;
extern uint8_t g_ucBLEServerIf;
extern uint16_t g_usBLEConnId;
extern BTBdaddr_t g_xAddressConnectedDevice;

extern BTService_t g_xSrvcB;
extern uint16_t usHandlesBufferB[ bletestATTR_SRVCB_NUMBER ];


TEST_GROUP( Full_BLE_Integration_Test );

TEST_SETUP( Full_BLE_Integration_Test )
{
}

/*-----------------------------------------------------------*/

TEST_TEAR_DOWN( Full_BLE_Integration_Test )
{
}

/*-----------------------------------------------------------*/

TEST_GROUP_RUNNER( Full_BLE_Integration_Test )
{
    RUN_TEST_CASE( Full_BLE_Integration_Test, BLE_Init_Enable_Twice );
    RUN_TEST_CASE( Full_BLE_Integration_Test, BLE_Enable_Disable_BT_Module );

    RUN_TEST_CASE( Full_BLE_Integration_Test, BLE_Advertise_Without_Properties );
    /* RUN_TEST_CASE( Full_BLE_Integration_Test, BLE_Advertise_Before_Set_Data ); */
    RUN_TEST_CASE( Full_BLE_Integration_Test, BLE_Advertise_Interval_Consistent_After_BT_Reset );

    RUN_TEST_CASE( Full_BLE_Integration_Test, BLE_Write_Notification_Size_Greater_Than_MTU_3 );

    RUN_TEST_CASE( Full_BLE_Integration_Test, BLE_Integration_Teardown );
    RUN_TEST_CASE( Full_BLE, BLE_Free );
}


TEST( Full_BLE_Integration_Test, BLE_Advertise_Without_Properties )
{
    prvBLEGAPInit();
    prvBLEGATTInit();
    prvSetAdvData();
    prvStartAdvertisement();
    /* sleep( ADV_RECV_TIMEOUT ); */
    prvWaitConnection( true );
}

TEST( Full_BLE_Integration_Test, BLE_Advertise_Before_Set_Data )
{
    prvStartAdvertisement();
    prvSetAdvData();
    prvStopAdvertisement();
}

TEST( Full_BLE_Integration_Test, BLE_Enable_Disable_BT_Module )
{
    BTStatus_t xStatus = eBTStatusSuccess;
    BLETESTInitDeinitCallback_t xInitDeinitCb;
    clock_t returnTime, cbRecvTime;

    /* disable */
    xStatus = g_pxBTInterface->pxDisable( 0 );
    TEST_ASSERT_EQUAL( eBTStatusSuccess, xStatus );

    xStatus = prvWaitEventFromQueue( eBLEHALEventEnableDisableCb, NO_HANDLE, ( void * ) &xInitDeinitCb, sizeof( BLETESTInitDeinitCallback_t ), BLE_TESTS_WAIT );
    TEST_ASSERT_EQUAL( eBTStatusSuccess, xStatus );
    TEST_ASSERT_EQUAL( eBTstateOff, xInitDeinitCb.xBLEState );

    /* enable */
    xStatus = g_pxBTInterface->pxEnable( 0 );
    returnTime = clock();
    TEST_ASSERT_EQUAL( eBTStatusSuccess, xStatus );

    xStatus = prvWaitEventFromQueue( eBLEHALEventEnableDisableCb, NO_HANDLE, ( void * ) &xInitDeinitCb, sizeof( BLETESTInitDeinitCallback_t ), BLE_TESTS_WAIT );
    cbRecvTime = clock();
    TEST_ASSERT_EQUAL( eBTStatusSuccess, xStatus );
    TEST_ASSERT_EQUAL( eBTstateOn, xInitDeinitCb.xBLEState );
    TEST_ASSERT_LESS_THAN( CLOCKS_PER_SEC * 5, ( cbRecvTime - returnTime ) * 2 );
}


TEST( Full_BLE_Integration_Test, BLE_Init_Enable_Twice )
{
    prvBLESetUp();
    /* init -> enable -> deinit -> init -> enable */
    prvGAPInitEnableTwice();
}

TEST( Full_BLE_Integration_Test, BLE_Advertise_Interval_Consistent_After_BT_Reset )
{
    prvWaitConnection( false );

    BTStatus_t xStatus = eBTStatusSuccess;
    BLETESTInitDeinitCallback_t xInitDeinitCb;

    /* Get BT interface */
    g_pxBTInterface = ( BTInterface_t * ) BTGetBluetoothInterface();
    TEST_ASSERT_NOT_EQUAL( NULL, g_pxBTInterface );

    xStatus = g_pxBTInterface->pxBtManagerCleanup();
    TEST_ASSERT_EQUAL( eBTStatusSuccess, xStatus );
    xStatus = g_pxBTInterface->pxBtManagerInit( &g_xBTManagerCb );
    TEST_ASSERT_EQUAL( eBTStatusSuccess, xStatus );

    xStatus = g_pxBTInterface->pxEnable( 0 );
    TEST_ASSERT_EQUAL( eBTStatusSuccess, xStatus );

    xStatus = prvWaitEventFromQueue( eBLEHALEventEnableDisableCb, NO_HANDLE, ( void * ) &xInitDeinitCb, sizeof( BLETESTInitDeinitCallback_t ), BLE_TESTS_WAIT );
    TEST_ASSERT_EQUAL( eBTStatusSuccess, xStatus );
    TEST_ASSERT_EQUAL( eBTstateOn, xInitDeinitCb.xBLEState );

    prvBLEGAPInit();
    prvBLEGATTInit();
    prvCreateAndStartServiceB();
    prvSetAdvProperty();
    prvSetAdvData();
    prvStartAdvertisement();

    prvWaitConnection( true );
    prvGetResult( bletestATTR_SRVCB_CHAR_D,
                  false,
                  0 );
}

TEST( Full_BLE_Integration_Test, BLE_Write_Notification_Size_Greater_Than_MTU_3 )
{
    BTStatus_t xStatus, xfStatus;
    BLETESTindicateCallback_t xIndicateEvent;
    uint8_t ucLargeBuffer[ bletestsMTU_SIZE1 + 2 ];

    char bletests_MTU_2_CHAR_VALUE[ bletestsMTU_SIZE1 + 2 ];

    for( int i = 0; i < bletestsMTU_SIZE1 + 1; i++ )
    {
        bletests_MTU_2_CHAR_VALUE[ i ] = 'a';
    }

    bletests_MTU_2_CHAR_VALUE[ bletestsMTU_SIZE1 + 1 ] = '\0';

    checkNotificationIndication( bletestATTR_SRVCB_CCCD_E, true );
    memcpy( ucLargeBuffer, bletests_MTU_2_CHAR_VALUE, bletestsMTU_SIZE1 + 1 );

    xStatus = g_pxGattServerInterface->pxSendIndication( g_ucBLEServerIf,
                                                         usHandlesBufferB[ bletestATTR_SRVCB_CHAR_E ],
                                                         g_usBLEConnId,
                                                         bletestsMTU_SIZE1 + 1,
                                                         ucLargeBuffer,
                                                         false );
    if( xStatus != eBTStatusSuccess )
    {
        /* Notify RPI failure here. Expect to receive failure message. */
        memcpy( ucLargeBuffer, bletestsFAIL_CHAR_VALUE, sizeof( bletestsFAIL_CHAR_VALUE ) - 1 );
        xfStatus = g_pxGattServerInterface->pxSendIndication( g_ucBLEServerIf,
                                                              usHandlesBufferB[ bletestATTR_SRVCB_CHAR_E ],
                                                              g_usBLEConnId,
                                                              sizeof( bletestsFAIL_CHAR_VALUE ) - 1,
                                                              ucLargeBuffer,
                                                              false );
        TEST_ASSERT_EQUAL( eBTStatusSuccess, xfStatus );
    }

    TEST_ASSERT_NOT_EQUAL( eBTStatusSuccess, xStatus );
}

TEST( Full_BLE_Integration_Test, BLE_Integration_Teardown )
{
    BTStatus_t xStatus = eBTStatusSuccess;
    BLETESTInitDeinitCallback_t xInitDeinitCb;

    prvWaitConnection( false );
    prvStopAndDeleteService( &g_xSrvcB );
    xStatus = g_pxGattServerInterface->pxUnregisterServer( g_ucBLEServerIf );
    TEST_ASSERT_EQUAL( eBTStatusSuccess, xStatus );

    xStatus = prvWaitEventFromQueue( eBLEHALEventRegisterUnregisterGattServerCb, NO_HANDLE, ( void * ) &xInitDeinitCb, sizeof( BLETESTInitDeinitCallback_t ), BLE_TESTS_WAIT );
    TEST_ASSERT_EQUAL( eBTStatusSuccess, xStatus );
    TEST_ASSERT_EQUAL( eBTStatusSuccess, xInitDeinitCb.xStatus );


    xStatus = g_pxBTLeAdapterInterface->pxUnregisterBleApp( g_ucBLEAdapterIf );
    TEST_ASSERT_EQUAL( eBTStatusSuccess, xStatus );

    xStatus = g_pxBTInterface->pxDisable( 0 );
    TEST_ASSERT_EQUAL( eBTStatusSuccess, xStatus );

    xStatus = prvWaitEventFromQueue( eBLEHALEventEnableDisableCb, NO_HANDLE, ( void * ) &xInitDeinitCb, sizeof( BLETESTInitDeinitCallback_t ), BLE_TESTS_WAIT );
    TEST_ASSERT_EQUAL( eBTStatusSuccess, xStatus );
    TEST_ASSERT_EQUAL( eBTstateOff, xInitDeinitCb.xBLEState );


    xStatus = g_pxBTInterface->pxBtManagerCleanup();
    TEST_ASSERT_EQUAL( eBTStatusSuccess, xStatus );
}


void prvGetResult( bletestAttSrvB_t xAttribute,
                   bool IsPrep,
                   uint16_t usOffset )
{
    BLETESTwriteAttrCallback_t xWriteEvent;
    BLETESTconfirmCallback_t xConfirmEvent;
    BTGattResponse_t xGattResponse;
    BTStatus_t xStatus;

    xStatus = prvWaitEventFromQueue( eBLEHALEventWriteAttrCb, usHandlesBufferB[ xAttribute ], ( void * ) &xWriteEvent, sizeof( BLETESTwriteAttrCallback_t ), BLE_TESTS_WAIT );
    TEST_ASSERT_EQUAL( eBTStatusSuccess, xStatus );
    TEST_ASSERT_EQUAL( IsPrep, xWriteEvent.bIsPrep );
    TEST_ASSERT_EQUAL( 49, xWriteEvent.ucValue[ 0 ] );
    TEST_ASSERT_EQUAL( g_usBLEConnId, xWriteEvent.usConnId );
    TEST_ASSERT_EQUAL( 0, memcmp( &xWriteEvent.xBda, &g_xAddressConnectedDevice, sizeof( BTBdaddr_t ) ) );
    TEST_ASSERT_EQUAL( usOffset, xWriteEvent.usOffset );
}

void prvCreateAndStartServiceB()
{
    BTStatus_t xStatus;

    xStatus = g_pxGattServerInterface->pxAddServiceBlob( g_ucBLEServerIf, &g_xSrvcB );
    if( xStatus == eBTStatusUnsupported )
    {
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

        prvStartService( &g_xSrvcB );
    }
    TEST_ASSERT_EQUAL( eBTStatusSuccess, xStatus );
}


void prvGAPInitEnableTwice()
{
    BTStatus_t xStatus = eBTStatusSuccess;
    BLETESTInitDeinitCallback_t xInitDeinitCb;

    /* Get BT interface */
    g_pxBTInterface = ( BTInterface_t * ) BTGetBluetoothInterface();
    TEST_ASSERT_NOT_EQUAL( NULL, g_pxBTInterface );

    uint32_t loop;

    for( loop = 0; loop < 2; loop++ )
    {
        /* Initialize callbacks */
        xStatus = g_pxBTInterface->pxBtManagerInit( &g_xBTManagerCb );
        TEST_ASSERT_EQUAL( eBTStatusSuccess, xStatus );

        if( loop == 1 )
        {
            TEST_ASSERT_EQUAL( eBTstateOn, xInitDeinitCb.xBLEState );
        }

        /* Enable RADIO and wait for callback. */
        xStatus = g_pxBTInterface->pxEnable( 0 );
        TEST_ASSERT_EQUAL( eBTStatusSuccess, xStatus );

        xStatus = prvWaitEventFromQueue( eBLEHALEventEnableDisableCb, NO_HANDLE, ( void * ) &xInitDeinitCb, sizeof( BLETESTInitDeinitCallback_t ), BLE_TESTS_WAIT );
        TEST_ASSERT_EQUAL( eBTStatusSuccess, xStatus );
        TEST_ASSERT_EQUAL( eBTstateOn, xInitDeinitCb.xBLEState );

        /*First time Deinit*/
        if( loop == 0 )
        {
            xStatus = g_pxBTInterface->pxBtManagerCleanup();
            TEST_ASSERT_EQUAL( eBTStatusSuccess, xStatus );
        }
    }
}


void prvBLEGAPInit()
{
    BTStatus_t xStatus = eBTStatusSuccess;
    BLETESTInitDeinitCallback_t xInitDeinitCb;

    g_pxBTLeAdapterInterface = ( BTBleAdapter_t * ) g_pxBTInterface->pxGetLeAdapter();
    TEST_ASSERT_NOT_EQUAL( NULL, g_pxBTLeAdapterInterface );

    xStatus = g_pxBTLeAdapterInterface->pxBleAdapterInit( &g_xBTBleAdapterCb );
    TEST_ASSERT_EQUAL( eBTStatusSuccess, xStatus );

    xStatus = g_pxBTLeAdapterInterface->pxRegisterBleApp( &g_xAppUUID );
    TEST_ASSERT_EQUAL( eBTStatusSuccess, xStatus );

    xStatus = prvWaitEventFromQueue( eBLEHALEventRegisterBleAdapterCb, NO_HANDLE, ( void * ) &xInitDeinitCb, sizeof( BLETESTInitDeinitCallback_t ), BLE_TESTS_WAIT );
    TEST_ASSERT_EQUAL( eBTStatusSuccess, xInitDeinitCb.xStatus );
}

void prvBLEGATTInit()
{
    BTStatus_t xStatus = eBTStatusSuccess;
    BLETESTInitDeinitCallback_t xInitDeinitCb;

    g_pxGattServerInterface = ( BTGattServerInterface_t * ) g_pxBTLeAdapterInterface->ppvGetGattServerInterface();
    TEST_ASSERT_NOT_EQUAL( NULL, g_pxGattServerInterface );

    g_pxGattServerInterface->pxGattServerInit( &g_xBTGattServerCb );
    TEST_ASSERT_EQUAL( eBTStatusSuccess, xStatus );

    xStatus = g_pxGattServerInterface->pxRegisterServer( &g_xServerUUID );
    TEST_ASSERT_EQUAL( eBTStatusSuccess, xStatus );

    xStatus = prvWaitEventFromQueue( eBLEHALEventRegisterUnregisterGattServerCb, NO_HANDLE, ( void * ) &xInitDeinitCb, sizeof( BLETESTInitDeinitCallback_t ), BLE_TESTS_WAIT );
    TEST_ASSERT_EQUAL( eBTStatusSuccess, xStatus );
    TEST_ASSERT_EQUAL( eBTStatusSuccess, xInitDeinitCb.xStatus );
}


void prvSetAdvData( void )
{
    uint16_t usServiceDataLen;
    char * pcServiceData;
    BTUuid_t xServiceUuid =
    {
        .ucType   = eBTuuidType128,
        .uu.uu128 = bletestsFREERTOS_SVC_UUID
    };

    size_t xNbServices;

    usServiceDataLen = 0;
    pcServiceData = NULL;
    xNbServices = 1;

    prvSetAdvertisement( &g_xAdvertisementConfigA,
                         usServiceDataLen,
                         pcServiceData,
                         &xServiceUuid,
                         xNbServices );


    prvSetAdvertisement( &g_xAdvertisementConfigB,
                         usServiceDataLen,
                         pcServiceData,
                         NULL,
                         0 );
}

void prvStopAdvertisement( void )
{
    BTStatus_t xStatus = g_pxBTLeAdapterInterface->pxStopAdv( g_ucBLEAdapterIf );

    TEST_ASSERT_EQUAL( eBTStatusSuccess, xStatus );
}

void prvSetAdvProperty( void )
{
    BTProperty_t pxProperty;
    uint16_t usMTUsize = bletestsMTU_SIZE1;

    pxProperty.xType = eBTpropertyBdname;
    pxProperty.xLen = strlen( bletestsDEVICE_NAME );
    pxProperty.pvVal = ( void * ) bletestsDEVICE_NAME;

    /* Set the name */
    prvSetGetProperty( &pxProperty, true );

    /* Get the name to check it is set */
/*@TODO prvSetGetProperty(&pxProperty, false); */

    pxProperty.xType = eBTpropertyLocalMTUSize;
    pxProperty.xLen = sizeof( usMTUsize );
    pxProperty.pvVal = &usMTUsize;

    /* Set the MTU size */
    prvSetGetProperty( &pxProperty, true );

    /* Get the MTU size to check it is set */
    /*@TODOprvSetGetProperty(&pxProperty, false); */
}
