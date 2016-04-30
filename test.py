#!/usr/bin/python
import os
from subprocess import Popen, PIPE, call

retcode = os.system("python hello.py")
print("retcode is: %s" % retcode);

fouput = os.popen("python hello.py")
result = fouput.readlines()
print("result is: %s" % result);

finput = os.popen("python TestInput.py", "w")
finput.write("how are you\n")


f = call("python hello.py", shell=True)
print(f)

p1 = Popen("python hello.py", stdin = None, stdout = PIPE, shell=True)

p2 = Popen("python TestInput.py", stdin = p1.stdout, stdout = PIPE, shell=True)
print(p2.communicate()[0])
#other way
#print p2.stdout.readlines()