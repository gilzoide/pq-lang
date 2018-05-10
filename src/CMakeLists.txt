include_directories("${PROJECT_SOURCE_DIR}/include"
	${LIBJIT_INCLUDE_DIRS} ${JUDY_INCLUDE_DIRS} ${PEGA_TEXTO_INCLUDE_DIRS})

set(libsrc builtin.c context.c function.c memory_manager.c parser.c scope.c
	scope_queue.c symbol.c type.c value.c)
add_library(pq-lang SHARED ${libsrc})
install(TARGETS pq-lang LIBRARY DESTINATION lib)

add_executable(pq main.c ${libsrc})
target_link_libraries(pq ${LIBJIT_LIBRARIES} ${JUDY_LIBRARIES} ${PEGA_TEXTO_LIBRARIES})
install(PROGRAMS pq DESTINATION bin)
