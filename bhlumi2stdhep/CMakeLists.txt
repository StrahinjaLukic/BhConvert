add_executable (bhlumi2stdhep src/bhlumi2stdhep.cxx src/StdhepWriter.cxx )

target_include_directories ( bhlumi2stdhep PUBLIC include ../StdhepWriter/include/ ${STDHEP_INCLUDE_DIRS} ${LCIO_INCLUDE_DIRS} )
target_link_libraries ( bhlumi2stdhep PRIVATE ${STDHEP_LIBRARIES} -lgfortran ${LCIO_LIBRARIES} )

install( TARGETS bhlumi2stdhep 
         RUNTIME DESTINATION bin )

