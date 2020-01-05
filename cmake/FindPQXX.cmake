#Author: Krzysztof Aleksander Pyrkosz

find_package(PostgreSQL REQUIRED)

include(FindPackageHandleStandardArgs)
include(SelectLibraryConfigurations)

find_library(PQXX_LIBRARY_RELEASE NAMES pqxx libpqxx)
find_library(PQXX_LIBRARY_DEBUG NAMES pqxxd libpqxxd)
find_path(PQXX_INCLUDE_DIRS NAMES pqxx/pqxx)

select_library_configurations(PQXX)
find_package_handle_standard_args(PQXX REQUIRED_VARS PQXX_LIBRARY PQXX_INCLUDE_DIRS)

if(NOT TARGET PQXX::PQXX)
	add_library(PQXX::PQXX UNKNOWN IMPORTED)
	target_link_libraries(PQXX::PQXX INTERFACE PostgreSQL::PostgreSQL)
	set_target_properties(PQXX::PQXX PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${PQXX_INCLUDE_DIRS}")
	if(PQXX_LIBRARY_RELEASE)
		set_property(TARGET PQXX::PQXX APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
        set_target_properties(PQXX::PQXX PROPERTIES IMPORTED_LOCATION_RELEASE "${PQXX_LIBRARY_RELEASE}")
    endif()
	if(PQXX_LIBRARY_DEBUG)
		set_property(TARGET PQXX::PQXX APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
        set_target_properties(PQXX::PQXX PROPERTIES PQXX_LIBRARY_DEBUG "${PQXX_LIBRARY_DEBUG}")
    endif()
	if(NOT PQXX_LIBRARY_RELEASE AND NOT PQXX_LIBRARY_DEBUG)
		set_property(TARGET PQXX::PQXX APPEND PROPERTY IMPORTED_LOCATION "${PQXX_LIBRARY}")
	endif()
endif()
