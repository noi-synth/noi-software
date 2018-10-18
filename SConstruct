import os
import os.path
import fnmatch, re

# --- Helpers ---
def GetFilesRecursive(directory = '.', filterPattern = '*'):
    ret = []
    for dirpath, dirnames, filenames in os.walk(directory):
        for f in filenames:
            if (fnmatch.fnmatch(f, filterPattern)):
                ret.append(dirpath+'/'+f)  
    return ret;

def GetDirectoriesRecursive(directory = '.'):
    ret = []
    for dirpath, dirnames, filenames in os.walk(directory):
        ret.append(dirpath)
    return ret  

# ----------------------------------------------------------

# --- CONSTANTS AND SETTINGS ---
APP_NAME = 'noi'
APP_DEST = 'bin/'
TEST_DEST = 'bin/tests/'
SOURCE_SUBFOLDERS = ['src', 'plg']
TEST_SUBFOLDERS = ['tst']
INCLUDE_SUBFOLDERS = ['include', 'plg']
libs = ["rt", "m", "asound", "pthread", "ncurses", "jack"]
debug = True
COMPILER_FLAGS = ["-std=c++17", "-Wall", "-pedantic", "-Wextra"]
# Setup enviroment
env = Environment()

# Enable compiler color output
env['ENV']['TERM'] = os.environ['TERM']  

# --- Compiler flags ---

if (debug):
    COMPILER_FLAGS.extend(["-O0", "-g"])
else:
    COMPILER_FLAGS.extend(["-O4"])
env.Append( CCFLAGS=COMPILER_FLAGS )

# --- Search for files and folders ---
sources = []
for subfolder in SOURCE_SUBFOLDERS:
    sources.extend(GetFilesRecursive(subfolder, '*.cpp'))

sources.extend(GetFilesRecursive("./lib", '*.a'))
# Exclude main so it is not in test builds.
sources.remove("src/main.cpp")

includes = []
for subfolder in INCLUDE_SUBFOLDERS:
    includes.extend(GetDirectoriesRecursive(subfolder))

tests = []
for subfolder in TEST_SUBFOLDERS:
    tests.extend(GetFilesRecursive(subfolder, '*.cpp'))

# print ("\n\nSOURCES")
# print (sources)
# print ("\n\nINCLUDES")
# print ( includes)
# print ("\n\nTESTS")
# print (tests)
# print ("\n\n")

# --- Buid it! ---

# main app 
noi = env.Program(APP_DEST+APP_NAME, sources + ["src/main.cpp"], LIBS=libs, CPPPATH=includes)
Depends(noi, 'SConstruct')

# tests
for test in tests:
	tstSources = sources + [test]
	tstName = os.path.splitext(os.path.basename(test))[0]
	env.Program(TEST_DEST + tstName, tstSources, LIBS=libs, CPPPATH=includes)




