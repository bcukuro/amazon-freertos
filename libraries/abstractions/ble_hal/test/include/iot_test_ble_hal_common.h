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
 * @file iot_test_ble_hal_common.h
 * @brief
 */

#ifndef _IOT_TEST_BLE_HAL_COMMON_H
#define _IOT_TEST_BLE_HAL_COMMON_H

#include "iot_config.h"
/* C standard library includes. */
#include <stddef.h>
#include <string.h>
#include "iot_linear_containers.h"
#include "platform/iot_threads.h"
#include "types/iot_platform_types.h"
#include "bt_hal_manager_adapter_ble.h"
#include "bt_hal_manager.h"
#include "bt_hal_gatt_server.h"
#include "bt_hal_gatt_types.h"
/* Test framework includes. */
#include "unity_fixture.h"
#include "unity.h"

/**
 * @brief Connection parameters.
 */
typedef struct
{
    uint32_t minInterval; /**< Minimum connection interval. */
    uint32_t maxInterval; /**< Maximum connection interval. */
    uint32_t latency;     /**< Slave latency. */
    uint32_t timeout;     /**< Connection timeout. */
} IotBleConnectionParam_t;

#define bletestsAPP_UUID                 { 0x11, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
#define bletestsSERVER_UUID              { 0x22, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
#define bletestsFREERTOS_SVC_UUID        { 0x5A, 0xDB, 0x32, 0xF9, 0x79, 0xE6, 0xB5, 0x83, 0xFB, 0x4E, 0xAF, 0x48, 0x68, 0x11, 0x7F, 0x8A }

#define NO_HANDLE                        -1

#define MAX_EVENT                        20

#define bletestsDEFAULT_CHAR_VALUE       "hello"
#define bletestsSTRINGYFIED_UUID_SIZE    36 /* like "8a7f1168-48af-4efb-83b5-e679f9320002" */
#define bletestsFULL_PERMISSIONS         ( eBTPermRead | eBTPermWrite )
#define bletestsNB_INCLUDEDSERVICES      1

#define bletestsCCCD            \
    {                           \
        .uu.uu16 = 0x2902,      \
        .ucType = eBTuuidType16 \
    }

#define bletestsFREERTOS_SVC_A_UUID                                                                                     \
    {                                                                                                                   \
        .uu.uu128 = { 0x00, 0x00, 0x1B, 0xE1, 0x14, 0xC6, 0x83, 0xAA, 0x9A, 0x4F, 0x9F, 0x4B, 0x87, 0xA1, 0x13, 0x31 }, \
        .ucType = eBTuuidType128                                                                                        \
    }

#define bletestsFREERTOS_CHAR_DUMMY_UUID                                                                                \
    {                                                                                                                   \
        .uu.uu128 = { 0x01, 0x00, 0x1B, 0xE1, 0x14, 0xC6, 0x83, 0xAA, 0x9A, 0x4F, 0x9F, 0x4B, 0x87, 0xA1, 0x13, 0x31 }, \
        .ucType = eBTuuidType128                                                                                        \
    }

#define bletestsFREERTOS_SVC_B_UUID                                                                               \
    {                                                                                                             \
        .uu.uu128 = { 1, 0, 0x32, 0xF9, 0x79, 0xE6, 0xB5, 0x83, 0xFB, 0x4E, 0xAF, 0x48, 0x68, 0x11, 0x7F, 0x8A }, \
        .ucType = eBTuuidType128                                                                                  \
    }

#define bletestsFREERTOS_CHAR_A_UUID                                                                              \
    {                                                                                                             \
        .uu.uu128 = { 2, 0, 0x32, 0xF9, 0x79, 0xE6, 0xB5, 0x83, 0xFB, 0x4E, 0xAF, 0x48, 0x68, 0x11, 0x7F, 0x8A }, \
        .ucType = eBTuuidType128                                                                                  \
    }

#define bletestsFREERTOS_CHAR_B_UUID                                                                              \
    {                                                                                                             \
        .uu.uu128 = { 3, 0, 0x32, 0xF9, 0x79, 0xE6, 0xB5, 0x83, 0xFB, 0x4E, 0xAF, 0x48, 0x68, 0x11, 0x7F, 0x8A }, \
        .ucType = eBTuuidType128                                                                                  \
    }

#define bletestsFREERTOS_CHAR_C_UUID                                                                              \
    {                                                                                                             \
        .uu.uu128 = { 4, 0, 0x32, 0xF9, 0x79, 0xE6, 0xB5, 0x83, 0xFB, 0x4E, 0xAF, 0x48, 0x68, 0x11, 0x7F, 0x8A }, \
        .ucType = eBTuuidType128                                                                                  \
    }

#define bletestsFREERTOS_CHAR_D_UUID                                                                              \
    {                                                                                                             \
        .uu.uu128 = { 5, 0, 0x32, 0xF9, 0x79, 0xE6, 0xB5, 0x83, 0xFB, 0x4E, 0xAF, 0x48, 0x68, 0x11, 0x7F, 0x8A }, \
        .ucType = eBTuuidType128                                                                                  \
    }

#define bletestsFREERTOS_CHAR_E_UUID                                                                              \
    {                                                                                                             \
        .uu.uu128 = { 6, 0, 0x32, 0xF9, 0x79, 0xE6, 0xB5, 0x83, 0xFB, 0x4E, 0xAF, 0x48, 0x68, 0x11, 0x7F, 0x8A }, \
        .ucType = eBTuuidType128                                                                                  \
    }

#define bletestsFREERTOS_CHAR_F_UUID                                                                              \
    {                                                                                                             \
        .uu.uu128 = { 7, 0, 0x32, 0xF9, 0x79, 0xE6, 0xB5, 0x83, 0xFB, 0x4E, 0xAF, 0x48, 0x68, 0x11, 0x7F, 0x8A }, \
        .ucType = eBTuuidType128                                                                                  \
    }

#define bletestsFREERTOS_DESCR_A_UUID                                                                             \
    {                                                                                                             \
        .uu.uu128 = { 8, 0, 0x32, 0xF9, 0x79, 0xE6, 0xB5, 0x83, 0xFB, 0x4E, 0xAF, 0x48, 0x68, 0x11, 0x7F, 0x8A }, \
        .ucType = eBTuuidType128                                                                                  \
    }
#define bletestsFREERTOS_DESCR_B_UUID                                                                             \
    {                                                                                                             \
        .uu.uu128 = { 9, 0, 0x32, 0xF9, 0x79, 0xE6, 0xB5, 0x83, 0xFB, 0x4E, 0xAF, 0x48, 0x68, 0x11, 0x7F, 0x8A }, \
        .ucType = eBTuuidType128                                                                                  \
    }
#define bletestsFREERTOS_DESCR_C_UUID                                                                              \
    {                                                                                                              \
        .uu.uu128 = { 10, 0, 0x32, 0xF9, 0x79, 0xE6, 0xB5, 0x83, 0xFB, 0x4E, 0xAF, 0x48, 0x68, 0x11, 0x7F, 0x8A }, \
        .ucType = eBTuuidType128                                                                                   \
    }
#define bletestsFREERTOS_DESCR_D_UUID                                                                              \
    {                                                                                                              \
        .uu.uu128 = { 11, 0, 0x32, 0xF9, 0x79, 0xE6, 0xB5, 0x83, 0xFB, 0x4E, 0xAF, 0x48, 0x68, 0x11, 0x7F, 0x8A }, \
        .ucType = eBTuuidType128                                                                                   \
    }


#define bletestsDEVICE_NAME                   "TEST"
#define bletestsMAX_PROPERTY_SIZE             30

#define bletestsMTU_SIZE1                     200
#define bletestsMAX_ADVERTISEMENT_INTERVAL    64

#define bletestsMIN_CONNECTION_INTERVAL       32
#define bletestsMAX_CONNECTION_INTERVAL       64

#define QUEUE_LENGTH                          20
#define ITEM_SIZE                             sizeof( void * )

typedef enum
{
    eBLEHALEventServerRegisteredCb = 0,
    eBLEHALEventEnableDisableCb = 1,
    eBLEHALEventCharAddedCb = 2,
    eBLEHALEventServiceAddedCb = 3,
    eBLEHALEventServiceStartedCb = 4,
    eBLEHALEventServiceStoppedCb = 5,
    eBLEHALEventServiceDeletedCb = 6,
    eBLEHALEventCharDescrAddedCb = 7,
    eBLEHALEventIncludedServiceAdded = 8,
    eBLEHALEventRegisterBleAdapterCb = 9,
    eBLEHALEventAdapterPropertiesCb = 10,
    eBLEHALEventRegisterUnregisterGattServerCb = 11,
    eBLEHALEventPropertyCb = 12,
    eBLEHALEventSetAdvCb = 13,
    eBLEHALEventStartAdvCb = 14,
    eBLEHALEventConnectionCb = 15,
    eBLEHALEventConnectionUpdateCb = 16,
    eBLEHALEventReadAttrCb = 17,
    eBLEHALEventWriteAttrCb = 18,
    eBLEHALEventIndicateCb = 19,
    eBLEHALEventConfimCb = 20,
    eBLEHALEventSSPrequestCb = 21,
    eBLEHALEventSSPrequestConfirmationCb = 22,
    eBLEHALEventPairingStateChangedCb = 23,
    eBLEHALEventRequestExecWriteCb = 24,
    eBLEHALEventBondedCb = 25,
    eBLENbHALEvents,
} BLEHALEventsTypes_t;

typedef struct
{
    IotLink_t eventList;
    BLEHALEventsTypes_t xEventTypes;
    int32_t lHandle;
} BLEHALEventsInternals_t;

typedef struct
{
    BLEHALEventsInternals_t xEvent;
    BTStatus_t xStatus;
    BTState_t xBLEState;
} BLETESTInitDeinitCallback_t;

typedef struct
{
    BLEHALEventsInternals_t xEvent;
    BTStatus_t xStatus;
    uint16_t usAttrHandle;
    uint16_t usSrvHandle;
    BTUuid_t xUUID;
} BLETESTAttrCallback_t;

typedef struct
{
    BLEHALEventsInternals_t xEvent;
    uint16_t usAttrHandle;
    BTStatus_t xStatus;
    BTGattSrvcId_t xSrvcId;
} BLETESTServiceCallback_t;

typedef struct
{
    BLEHALEventsInternals_t xEvent;
    BTStatus_t xStatus;
    uint32_t ulNumProperties;
    BTProperty_t xProperties;
} BLETESTSetGetPropertyCallback_t;

typedef struct
{
    BLEHALEventsInternals_t xEvent;
    BTStatus_t xStatus;
    bool bStart;
} BLETESTAdvParamCallback_t;

typedef struct
{
    BLEHALEventsInternals_t xEvent;
    BTStatus_t xStatus;
    IotBleConnectionParam_t xConnParam;
    BTBdaddr_t xBda;
} BLETESTUpdateConnectionParamReqCallback_t;

typedef struct
{
    BLEHALEventsInternals_t xEvent;
    uint16_t usConnId;
    uint32_t ulTransId;
    BTBdaddr_t xBda;
    uint16_t usAttrHandle;
    uint16_t usOffset;
} BLETESTreadAttrCallback_t;

typedef struct
{
    BLEHALEventsInternals_t xEvent;
    uint16_t usConnId;
    uint32_t ulTransId;
    BTBdaddr_t xBda;
    uint16_t usAttrHandle;
    uint16_t usOffset;
    size_t xLength;
    bool bNeedRsp;
    bool bIsPrep;
    uint8_t ucValue[ bletestsSTRINGYFIED_UUID_SIZE ];
} BLETESTwriteAttrCallback_t;


typedef struct
{
    BLEHALEventsInternals_t xEvent;
    uint16_t usConnId;
    BTStatus_t xStatus;
} BLETESTindicateCallback_t;

typedef struct
{
    BLEHALEventsInternals_t xEvent;
    BTStatus_t xStatus;
    uint16_t usAttrHandle;
} BLETESTconfirmCallback_t;

typedef struct
{
    BLEHALEventsInternals_t xEvent;
    BTBdaddr_t xRemoteBdAddr;
    BTBdname_t xRemoteBdName;
    uint32_t ulCod;
    BTSspVariant_t xPairingVariant;
    uint32_t ulPassKey;
} BLETESTsspRequestCallback_t;

typedef struct
{
    BLEHALEventsInternals_t xEvent;
    BTStatus_t xStatus;
    BTBdaddr_t xRemoteBdAddr;
    BTSecurityLevel_t xSecurityLevel;
    BTAuthFailureReason_t xReason;
} BLETESTPairingStateChangedCallback_t;

typedef struct
{
    BLEHALEventsInternals_t xEvent;
    uint16_t usConnId;
    uint32_t ulTransId;
    BTBdaddr_t xBda;
    bool bExecWrite;
} BLETESTRequestExecWriteCallback_t;

typedef struct
{
    BLEHALEventsInternals_t xEvent;
    BTStatus_t xStatus;
    uint16_t usConnId;
    uint8_t ucServerIf;
    BTBdaddr_t pxBda;
    bool bConnected;
} BLETESTConnectionCallback_t;

typedef struct
{
    BLEHALEventsInternals_t xEvent;
    BTStatus_t xStatus;
    BTBdaddr_t xRemoteBdAddr;
    bool bIsBonded;
} BLETESTBondedCallback_t;

IotListDouble_t eventQueueHead;
IotMutex_t threadSafetyMutex;
IotSemaphore_t eventSemaphore;

BTInterface_t * pxBTInterface;
BTBleAdapter_t * pxBTLeAdapterInterface;
static BTUuid_t xServerUUID =
{
    .ucType   = eBTuuidType128,
    .uu.uu128 = bletestsSERVER_UUID
};
static BTUuid_t xAppUUID =
{
    .ucType   = eBTuuidType128,
    .uu.uu128 = bletestsAPP_UUID
};
uint8_t ucBLEAdapterIf;
uint8_t ucBLEServerIf;
BTBdaddr_t xAddressConnectedDevice;
BTGattServerInterface_t * pxGattServerInterface;
uint16_t usBLEConnId;

BTStatus_t bleStackInit( void );

#endif /* _IOT_TEST_BLE_HAL_COMMON_H */