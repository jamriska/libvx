# Find module for DirectShow
# (c) Copyright 2007-2011 Hartmut Seichter
# - Test for DirectShow on Windows.
# Once loaded this will define
#   DIRECTSHOW_FOUND        - system has DirectShow
#   DIRECTSHOW_INCLUDE_DIRS - include directory for DirectShow
#   DIRECTSHOW_LIBRARIES    - libraries you need to link to


set(FINDDIRECTSHOW_DEBUG ON)  

if(MINGW)

	get_filename_component(MINGW_ROOT ${CMAKE_MAKE_PROGRAM} PATH)
	
	# message(STATUS "MinGW ${MINGW_ROOT}")

	find_path(_QEDIT_H_INCLUDE_DIR qedit.h
		${MINGW_ROOT}/../include
		${MINGW_ROOT}/../../../include
		${MINGW_ROOT}/../x86_64-w64-mingw32/include
		DOC "What is the path where the file dshow.h can be found"
		)
		
	find_path(_DSHOW_H_INCLUDE_DIR dshow.h
		${MINGW_ROOT}/../include
		${MINGW_ROOT}/../../../include
		${MINGW_ROOT}/../x86_64-w64-mingw32/include
		DOC "What is the path where the file dshow.h can be found"
		)
	
	find_path(_DDRAW_H_INCLUDE_DIR ddraw.h
		${MINGW_ROOT}/../include
		${MINGW_ROOT}/../../../include
		${MINGW_ROOT}/../x86_64-w64-mingw32/include
		DOC "What is the path where the file dshow.h can be found"
		)

	find_library(DIRECTSHOW_strmiids_LIBRARY strmiids
		${MINGW_ROOT}/../lib
		${MINGW_ROOT}/../../../lib
		${MINGW_ROOT}/../x86_64-w64-mingw32/lib
		NO_DEFAULT_PATH
	  )
	find_library(DIRECTSHOW_quartz_LIBRARY quartz
		${MINGW_ROOT}/../lib
		${MINGW_ROOT}/../../../lib
		${MINGW_ROOT}/../x86_64-w64-mingw32/lib
		DOC "Where can the DirectShow quartz library be found"
		NO_DEFAULT_PATH
	  )
endif()

if(MSVC)

	get_filename_component( MS_PLATFORMSDK_ROOT  
		"[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Microsoft SDKs\\Windows;CurrentInstallFolder]" ABSOLUTE  
		)
		
    find_path(_DSHOW_H_INCLUDE_DIR dshow.h
	  "${MS_PLATFORMSDK_ROOT}/Include"
      DOC "What is the path where the file dshow.h can be found"
      #NO_DEFAULT_PATH
	)
	
	find_path(_QEDIT_H_INCLUDE_DIR qedit.h
	  "${MS_PLATFORMSDK_ROOT}/Include"
	  ${DSHOW_HELPER_DIR}/Include
      DOC "What is the path where the file dshow.h can be found"
      #NO_DEFAULT_PATH
	)
	
	find_library(DIRECTSHOW_strmiids_LIBRARY_64 strmiids
		"${MS_PLATFORMSDK_ROOT}/Lib/x64"
		DOC "Where can the DirectShow strmiids library be found"
		NO_DEFAULT_PATH
		)

	find_library(DIRECTSHOW_quartz_LIBRARY_64 quartz
		"${MS_PLATFORMSDK_ROOT}/Lib/x64"
		DOC "Where can the DirectShow quartz library be found"
		NO_DEFAULT_PATH
		)
	
	find_library(DIRECTSHOW_strmiids_LIBRARY_32 strmiids
		"${MS_PLATFORMSDK_ROOT}/Lib"
		DOC "Where can the DirectShow strmiids library be found"
		NO_DEFAULT_PATH
		)

	find_library(DIRECTSHOW_quartz_LIBRARY_32 quartz
		"${MS_PLATFORMSDK_ROOT}/Lib"
		DOC "Where can the DirectShow quartz library be found"
		NO_DEFAULT_PATH
		)
	
	if(CMAKE_CL_64)
		set(_QUARTZ_LIBRARY ${DIRECTSHOW_quartz_LIBRARY_64})
		set(_STRMIIDS_LIBRARY ${DIRECTSHOW_strmiids_LIBRARY_64})
	else()
		set(_QUARTZ_LIBRARY ${DIRECTSHOW_quartz_LIBRARY_32})
		set(_STRMIIDS_LIBRARY ${DIRECTSHOW_strmiids_LIBRARY_32})
	endif()
	
	if(FINDDIRECTSHOW_DEBUG)
		message(STATUS "${_DSHOW_H_INCLUDE_DIR}")
		message(STATUS "${MS_PLATFORMSDK_ROOT}")
	endif()

endif()

set(DIRECTSHOW_INCLUDE_DIRS ${_QEDIT_H_INCLUDE_DIR})
set(DIRECTSHOW_LIBRARIES ${_QUARTZ_LIBRARY};${_STRMIIDS_LIBRARY})


#---------------------------------------------------------------------
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
  DIRECTSHOW
  DEFAULT_MSG
  DIRECTSHOW_INCLUDE_DIRS
  DIRECTSHOW_LIBRARIES
  )

if(FINDDIRECTSHOW_DEBUG)
	message(STATUS "DS Found '${DIRECTSHOW_FOUND}'")
	message(STATUS "DS Libs '${DIRECTSHOW_LIBRARIES}'")
	message(STATUS "DS Include '${DIRECTSHOW_INCLUDE_DIRS}'")
endif()
