add_executable (bhlumi2slcio src/bhlumi2slcio.cxx)

target_include_directories ( bhlumi2slcio PUBLIC include ${LCIO_INCLUDE_DIRS} ${LCIO_ROOT}/src/cpp/include ${LCIO_ROOT}/sio/include )
target_link_libraries ( bhlumi2slcio PRIVATE ${LCIO_LIBRARIES} )

install( TARGETS bhlumi2slcio 
         RUNTIME DESTINATION bin )

