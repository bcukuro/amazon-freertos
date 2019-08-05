#!/usr/bin/python

from __future__ import absolute_import, print_function, unicode_literals

from optparse import OptionParser, make_option
import dbus
import time
import dbus.mainloop.glib
import bleAdapter
try:
  from gi.repository import GObject
except ImportError:
  import gobject as GObject
import testutils
import threading
import securityAgent

devices = {}

def backGroundEvents():
    try:
        mainloop = GObject.MainLoop()
        mainloop.run()

    except KeyboardInterrupt:
        mainloop.quit()
        print("Thread: KeyboardInterrupt")
    return
