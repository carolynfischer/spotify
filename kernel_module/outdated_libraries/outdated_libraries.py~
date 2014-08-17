#!/usr/bin/env python
import fileinput
import os
import psutil

# used python3.4 and psutil 2.1.1

libdirs = [libdirs.strip('\n') for libdirs in open("/etc/ld.so.conf", "r")]
print(libdirs)
pid = 1
for p in psutil.process_iter():
	print(p)

