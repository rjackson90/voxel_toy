# This file provides various test cases for adding scripts to the script system

from . import Script

class Clock(Script):
    def __init__(self):
        self.count = 0
        Script.__init__(self)

    def tick(self):
        # This script is a one-shot wonder
        if self.count > 1:
            return

        # Print a message
        if self.count == 0:
            print "Tick"
        else:
            print "Tock"
        self.count += 1

