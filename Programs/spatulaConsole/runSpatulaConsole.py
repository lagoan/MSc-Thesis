#!/sw/bin/python

import sys
import glob
import re
import shutil
import os


path = sys.argv[1];
command = "/Users/kuervo/Workspace/fromSVN/spatulaConsole/spatula.app/Contents/MacOS/spatula " + path
print path

files = glob.glob(path + "/*.dae")



for currentFile in files:
	currentFile= re.sub("^.+/", "", currentFile)
	#currentFile = currentFile.replace(path + "/", "")
	currentCommand = command + " " + currentFile
	print currentCommand
	os.system(currentCommand)