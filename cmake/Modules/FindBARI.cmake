# FindBARI.cmake
# find the bari matrix library
# Sets the following variables
#   BARI_INCLUDE_DIR - directory of bari.h
#   BARI_LIBRARY_DIR - directory of libbari.a
#   BARI_LIBRARY     - library to link against

set(BARI_INCLUDE_DIR /usr/local/include)
set(BARI_LIBRARY_DIR /usr/local/lib64)
find_library(BARI_LIBRARY "bari" HINTS ${BARI_LIBRARY_DIR})
