import requests
import sys
from client import client
			
def execute(argv, number, method) :
	if (len(argv) == number):
		if (number == 3):
			(result, desc) = method(argv[2])
		else:
			(result, desc) = method(argv[2], argv[3])
		if (not result):
			print "Error occured" + str(desc)
	else:
		print "Wrong amount of arguments"
cl = client()
cmd = sys.argv[1];
if (cmd == "ls"):
	execute(sys.argv, 3, cl.ls)
elif (cmd == "rm"):
	execute(sys.argv, 3, cl.rm)
elif (cmd == "mv"):
	execute(sys.argv, 4, cl.mv)
elif (cmd == "cp"):
	execute(sys.argv, 4, cl.cp)
elif (cmd == "upload"):
	execute(sys.argv, 4, cl.upload)
elif (cmd == "download"):
	execute(sys.argv, 4, cl.download)
else:
	print "Wrong command"
