include_directories("${PROJECT_SOURCE_DIR}/include"
	${LIBJIT_INCLUDE_DIRS}
	${JUDY_INCLUDE_DIRS}
	${PEGA_TEXTO_INCLUDE_DIRS}
	${REPLXX_INCLUDE_DIRS})

file(GLOB libsrc "*.c" "core/*.c")
add_library(pq-lang SHARED ${libsrc})
install(TARGETS pq-lang LIBRARY DESTINATION lib)

add_executable(pq "bin/main.c")
target_link_libraries(pq
	pq-lang
	${LIBJIT_LIBRARIES}
	${JUDY_LIBRARIES}
	${PEGA_TEXTO_LIBRARIES}
	${REPLXX_LIBRARIES})
install(PROGRAMS pq DESTINATION bin)
