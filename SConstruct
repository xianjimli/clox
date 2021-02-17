import os
import platform


APP_TOOLS=''
APP_LIBS=[]
APP_LINKFLAGS=''
OS_NAME = platform.system();

APP_ROOT=os.getcwd()
APP_BIN_DIR=os.path.join(APP_ROOT, 'bin')
APP_LIB_DIR=os.path.join(APP_ROOT, 'lib')
APP_SRC_DIR=os.path.join(APP_ROOT, 'src')
APP_3RD_DIR=os.path.join(APP_ROOT, '3rd')
GTEST_ROOT=os.path.join(APP_ROOT, '3rd/gtest/googletest')

APP_CPPPATH=[APP_3RD_DIR, APP_SRC_DIR]
APP_LIBPATH=[APP_LIB_DIR, APP_BIN_DIR]

if OS_NAME == 'Linux':
  APP_CCFLAGS=' -g -Wall '
  APP_LIBS=['pthread', 'm']
elif OS_NAME == 'Darwin':
  APP_CCFLAGS=' -g -Wall '
  APP_LIBS=['pthread', 'm']
elif OS_NAME == 'Windows':
  APP_CCFLAGS=' /EHsc '

DefaultEnvironment(
  CCFLAGS = APP_CCFLAGS,
  LIBS = APP_LIBS,
  LIBPATH = APP_LIBPATH,
  CPPPATH = APP_CPPPATH, 
  LINKFLAGS = APP_LINKFLAGS
)
  
os.environ['BIN_DIR'] = APP_BIN_DIR
os.environ['LIB_DIR'] = APP_LIB_DIR
os.environ['SRC_DIR'] = APP_SRC_DIR
os.environ['3RD_DIR'] = APP_3RD_DIR
os.environ['GTEST_ROOT'] =GTEST_ROOT 

SConscript([
 'src/base/SConscript',
 'src/parser/SConscript',
 'src/interpreter/SConscript',
 'src/compiler/SConscript',
 'tests/SConscript',
 'src/main/SConscript'
])

