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
APP_NAME='noi'
APP_DEST='bin/'
SOURCE_SUBFOLDERS = ['src', 'plg']
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
    COMPILER_FLAGS.extend(["-O3"])
env.Append( CCFLAGS=COMPILER_FLAGS )

# --- Search for files and folders ---
sources = []
for subfolder in SOURCE_SUBFOLDERS:
    sources.extend(GetFilesRecursive(subfolder, '*.cpp'))

sources.extend(GetFilesRecursive("./lib", '*.a'))

includes = []
for subfolder in INCLUDE_SUBFOLDERS:
    includes.extend(GetDirectoriesRecursive(subfolder))

# --- Buid it! ---
noi = env.Program(APP_DEST+APP_NAME, sources, LIBS=libs, CPPPATH=includes)
Depends(noi, 'SConstruct')
