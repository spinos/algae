# render task queuing program

import socket, time, threading, sys, os, commands
from optparse import OptionParser
from time import gmtime, strftime

# set command line option
parser = OptionParser()
parser.add_option("-f", "--file", dest="filename", help="read ascii job script file")
args = ["-f", "foo.txt"]
(options, args) = parser.parse_args()

print 'read job script',options.filename

# Set the socket parameters
port = 52907

jobstack = []

f = open(options.filename,'r')
for line in f:
        jobstack.append(line[:len(line)-1])

print 'job count: ', len(jobstack)

nleft = len(jobstack)
njob = nleft

class NilProc(threading.Thread):
        def __init__(self, scmd):
                self.scmd = scmd
                threading.Thread.__init__(self)
        def run(self):
                print '_',self.scmd,'_'
		os.system(self.scmd)
                print 'end'
                
while nleft > 0:
	if threading.activeCount() > 1:
		time.sleep(3)
        else:
# start process here
		print 'job left: ', nleft
		print strftime("%a, %d %b %Y %H:%M:%S", gmtime())
		NilProc(jobstack[njob - nleft]).start()
		nleft = nleft - 1
		
		

