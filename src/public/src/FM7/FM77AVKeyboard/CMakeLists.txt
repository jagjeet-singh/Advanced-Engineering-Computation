set(TARGET_NAME FM77AVKeyboard)
set(WIN_SUBSYSTEM WINDOWS)


if(MSVC)
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:${WIN_SUBSYSTEM},5.02 /MACHINE:x64")
	else()
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:${WIN_SUBSYSTEM},5.01 /MACHINE:X86")
	endif()
	set(COMPORT_SOURCE comport.cpp comport_win.cpp comport.h)
else()
	set(COMPORT_SOURCE comport.cpp comport_null.cpp comport.h)
endif()


add_library(cheapgui gui.cpp gui.h)
target_include_directories(cheapgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(cheapgui ysgl ysbitmapfont)


add_library(fm77avkey 
	fm77avkey.cpp 
	fm77avkey.h 
	fm77avkeymap.h 
	fm77avkeymap.cpp 
	fm77avrkana.h 
	fm77avrkana.cpp)
target_include_directories(fm77avkey PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(fm77avkey fssimplewindow)

add_executable(${TARGET_NAME} MACOSX_BUNDLE
	main.cpp 
	fm77avkeyboardemu.cpp 
	fm77avkeyboardemu.h 
	irtoy.cpp 
	irtoy.h 
	util.cpp 
	util.h 
	${COMPORT_SOURCE}
)


target_link_libraries(${TARGET_NAME} cheapgui fm77avkey fssimplewindow ysgl ysbitmapfont)
