import os
import os.path

def exists_dir(name):
    if os.path.exists("sample"): # compatible: isfile, isdir
        print "there exists sample!" 
    else:
        print "there doesn't exist sample!" 

if os.path.exists("sample") == False: 
    os.mkdir("sample")
exists_dir("sample")
os.rmdir("sample")
exists_dir("sample")

os.renames("test1", "test2")
if os.path.exists("test1"):
    print "there is test1"
if os.path.exists("test2"):
    print "there is test2"
 
os.renames("test2", "test1")
if os.path.exists("test1"):
    print "there is test1"
if os.path.exists("test2"):
    print "there is test2"
