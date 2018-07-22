add_library(utility 
	utility.cpp
	hashbase.h
	hashset.h
	hashtable.h
	polygonalmesh.cpp
	polygonalmesh.h
	polygonalmeshio.cpp
	glutil.cpp
	glutil.h
	lattice.h
	meshlattice.cpp
	meshlattice.h)
target_include_directories(utility PUBLIC .)
target_link_libraries(utility ysclass geblkernel)
