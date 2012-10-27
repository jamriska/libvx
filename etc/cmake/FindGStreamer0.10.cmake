#
# GStreamer CMake Module  
#
# (c) Copyrights 2010 Hartmut Seichter 
# 
# Unlike other GStreamer modules this one is independent of pkg-config
# 



#set(FIND_GSTREAMER_DEBUGGING On)

find_path(GST_libxml2_INCLUDE_DIR 
		libxml/parser.h
		PATHS
		/usr/include
		/usr/include/libxml2
		$ENV{OSSBUILD_GSTREAMER_SDK_DIR}/include/libxml2
		/opt/local/include/libxml2
	)

find_path(GST_gst_INCLUDE_DIR gst/gst.h
		/usr/include
		/usr/include/gstreamer-0.10
		$ENV{OSSBUILD_GSTREAMER_SDK_DIR}/include/gstreamer-0.10
		/opt/local/include/gstreamer-0.10
	)

find_path(GST_glib2_INCLUDE_DIR glib.h
		/usr/include
		/usr/include/glib-2.0
		$ENV{OSSBUILD_GSTREAMER_SDK_DIR}/include/glib-2.0
		/opt/local/include/glib-2.0
	)


find_path(GST_glib2_config_INCLUDE_DIR glibconfig.h
		/usr/include
		/usr/include/glib-2.0
		/usr/lib/glib-2.0/include
		$ENV{OSSBUILD_GSTREAMER_SDK_DIR}/include/glib-2.0
		/opt/local/include/glib-2.0
		/opt/local/lib/glib-2.0/include
	)

find_path(GST_pcre_INCLUDE_DIR pcre.h
		/usr/include
		/opt/local/include
	)

if(FIND_GSTREAMER_DEBUGGING)
	message(STATUS "Debug: '${GST_libxml2_INCLUDE_DIR}'")
	message(STATUS "Debug: '${GST_gst_INCLUDE_DIR}'")
	message(STATUS "Debug: '${GST_glib2_INCLUDE_DIR}'")
	message(STATUS "Debug: '${GST_glib2_config_INCLUDE_DIR}'")
endif()

find_library(GST_pcreposix_LIBRARY 
	NAMES libpcreposix.so pcreposix
	/usr/lib
	$ENV{OSSBUILD_GSTREAMER_SDK_DIR}/lib
	/opt/local/lib
	)

find_library(GST_pcre_LIBRARY pcre
	/usr/lib
	$ENV{OSSBUILD_GSTREAMER_SDK_DIR}/lib
	/opt/local/lib
	)

find_library(GST_libxml2_LIBRARY 
	NAMES libxml2.so xml2
	/usr/lib
	$ENV{OSSBUILD_GSTREAMER_SDK_DIR}/lib
	/opt/local/lib
	)

find_library(GST_gstbase_LIBRARY 
	NAMES libgstbase-0.10.so gstbase-0.10
	/usr/lib
	$ENV{OSSBUILD_GSTREAMER_SDK_DIR}/lib
	/opt/local/lib
	)

find_library(GST_gstreamer_LIBRARY 
	NAMES gstreamer-0.10.so gstreamer-0.10
	/usr/lib
	$ENV{OSSBUILD_GSTREAMER_SDK_DIR}/lib
	/opt/local/lib
	)

find_library(GST_gobject_LIBRARY gregex-2.0
	/usr/lib
	$ENV{OSSBUILD_GSTREAMER_SDK_DIR}/lib
	/opt/local/lib
	)

find_library(GST_gobject_LIBRARY gobject-2.0
		/usr/lib
		$ENV{OSSBUILD_GSTREAMER_SDK_DIR}/lib
	)

find_library(GST_gmodule_LIBRARY gmodule-2.0
	/usr/lib
	$ENV{OSSBUILD_GSTREAMER_SDK_DIR}/lib
	/opt/local/lib
	)

find_library(GST_gthread_LIBRARY gthread-2.0
	/usr/lib
	$ENV{OSSBUILD_GSTREAMER_SDK_DIR}/lib
	/opt/local/lib
	)

find_library(GST_glib_LIBRARY glib-2.0
	/usr/lib
	$ENV{OSSBUILD_GSTREAMER_SDK_DIR}/lib
	/opt/local/lib
	)

if(FIND_GSTREAMER_DEBUGGING)
	message(STATUS "Debug: '${GST_pcreposix_LIBRARY}'")
	message(STATUS "Debug: '${GST_libxml2_LIBRARY}'")
	message(STATUS "Debug: '${GST_gstbase_LIBRARY}'")
	message(STATUS "Debug: '${GST_gstreamer_LIBRARY}'")
	message(STATUS "Debug: '${GST_gobject_LIBRARY}'")
	message(STATUS "Debug: '${GST_gmodule_LIBRARY}'")
	message(STATUS "Debug: '${GST_gthread_LIBRARY}'")
	message(STATUS "Debug: '${GST_glib_LIBRARY}'")
endif()

set(GSTREAMER_FOUND FALSE)
set(GSTREAMER_INCLUDE_DIRS)
set(GSTREAMER_LIBRARIES)


mark_as_advanced(
	GST_libxml2_INCLUDE_DIR
	GST_libxml2_INCLUDE_DIR
	GST_gst_INCLUDE_DIR
	GST_glib2_INCLUDE_DIR
	GST_glib2_config_INCLUDE_DIR
	GST_pcre_INCLUDE_DIR
	GST_libxml2_LIBRARY
	GST_gstbase_LIBRARY
	GST_gstreamer_LIBRARY
	GST_gobject_LIBRARY
	GST_gmodule_LIBRARY
	GST_gthread_LIBRARY
	GST_glib_LIBRARY
	GST_pcre_LIBRARY
	GST_pcreposix_LIBRARY
	)

if(GST_gstreamer_LIBRARY AND GST_gst_INCLUDE_DIR)

	set(GSTREAMER_FOUND TRUE)

	set(GSTREAMER_INCLUDE_DIRS
		$ENV{OSSBUILD_GSTREAMER_SDK_DIR}/include
#		${GST_pcre_INCLUDE_DIR}
		${GST_libxml2_INCLUDE_DIR}
		${GST_gst_INCLUDE_DIR}
		${GST_glib2_INCLUDE_DIR}
		${GST_glib2_config_INCLUDE_DIR}
		
	)

	set(GSTREAMER_LIBRARIES
#		${GST_pcre_LIBRARY}
		${GST_libxml2_LIBRARY}
		${GST_gstbase_LIBRARY}
		${GST_gstreamer_LIBRARY}
		${GST_gobject_LIBRARY}
		${GST_gmodule_LIBRARY}
		${GST_gthread_LIBRARY}
		${GST_glib_LIBRARY}
	)

	set(GSTREAMER_CFLAGS)

endif()


#---------------------------------------------------------------------
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
  GSTREAMER
  DEFAULT_MSG
  GSTREAMER_INCLUDE_DIRS
  GSTREAMER_LIBRARIES
  )

if(FIND_GSTREAMER_DEBUGGING)
	message(STATUS "GStreamer Found '${GSTREAMER_FOUND}'")
	message(STATUS "GStreamer Libs '${GSTREAMER_LIBRARIES}'")
	message(STATUS "GStreamer Include '${GSTREAMER_INCLUDE_DIRS}'")
endif()





