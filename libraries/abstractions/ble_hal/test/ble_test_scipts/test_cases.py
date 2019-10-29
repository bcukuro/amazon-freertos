from startTests_afqp import main_afqp
from startTests_integration import *
from testClass import runTest

AFQP_UUID = "8a7f1168-48af-4efb-83b5-e679f9320100"
INTEGRATION_UUID = "8a7f1168-48af-4efb-83b5-e679f93202"

test_uuid = {
    AFQP_UUID :
    {
        "test_function" : main_afqp
    },
    INTEGRATION_UUID + "02" :
    {
        "test_function" : Advertise_Without_Properties
    },
    runTest.UUID_16to128("0203") :
    {
        "test_function" : Advertise_With_16bit_ServiceUUID
    },
    INTEGRATION_UUID + "04" :
    {
        "test_function" : Advertise_With_Manufacture_Data
    },
    INTEGRATION_UUID + "05" :
    {
        "test_function" : Advertise_Interval_Consistent_After_BT_Reset
    },
    INTEGRATION_UUID + "06" :
    {
        "test_function" : Write_Notification_Size_Greater_Than_MTU_3
    },
    INTEGRATION_UUID + "08" :
    {
        "test_function" : Send_Data_After_Disconnected
    }
}