#!/usr/bin/python

import test_common
import startTests_integration as startTests

if __name__ == '__main__':

  dbus.mainloop.glib.DBusGMainLoop(set_as_default=True)

  testutils.removeBondedDevices()

  #startBackGroundEvents = threading.Thread(target=backGroundEvents)
  #startBackGroundEvents.start()
  startTests.main()
