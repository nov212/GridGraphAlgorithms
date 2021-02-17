cmake_minimum_required(VERSION 3.2)

# имя проекта
project(VTK_PROJECT)

find_package(VTK COMPONENTS
	LSDyna
    vtkCommon
    vtkDICOMParser
    vtkexpat
    vtkFiltering
    vtkGraphics
    vtkhdf5
    vtkhdf5_hl
    vtkIO
    vtkjpeg
    vtkmetaio
    vtkNetCDF
    vtkNetCDF_cxx
    vtkpng
    vtksys
    vtktiff
    vtkverdict
    vtkzlib
    QUIET
)

if (NOT VTK_FOUND)
    message("VTK not found")
    return()
endif ()
message(STATUS "VTK_VERSION: ${VTK_VERSION}")

# Экспортируем необходимые файлы
foreach ( _lib ${VTK_LIBRARIES} )
	# Экспорт
	file ( INSTALL ${${_lib}_LIBRARY_Debug}   DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug   )
	file ( INSTALL ${${_lib}_LIBRARY_Release} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release )
	# Инсталляция
	install ( FILES ${${_lib}_LIBRARY_Release} DESTINATION bin CONFIGURATIONS Release   )
	install ( FILES ${${_lib}_LIBRARY_Debug} DESTINATION bin CONFIGURATIONS Debug   )
endforeach ()

include(SourceFiles.cmake)

link_directories( ${VTK_LIBRARY_DIR} )
include_directories( ${VTK_INCLUDE_DIR} ${VTK_INCLUDE_DIRS} )

add_executable(vtkApp ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(vtkApp PRIVATE ${VTK_LIBRARIES})
#message(INFO "VTK lib dir: ${VTK_LIBRARY_DIR}")
