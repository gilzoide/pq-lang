include_directories("${PROJECT_SOURCE_DIR}/include"
	${LLVM_INCLUDE_DIRS} ${JUDY_INCLUDE_DIRS} ${PEGA_TEXTO_INCLUDE_DIRS})

# file(GLOB src *.c)
add_library(pq-lang SHARED context.c scope.c parser.c)
install(TARGETS pq-lang LIBRARY DESTINATION lib)

add_executable(pq main.c)
target_link_libraries(pq pq-lang ${LLVM_LIBRARIES} ${JUDY_LIBRARIES} ${PEGA_TEXTO_LIBRARIES})
install(PROGRAMS pq DESTINATION bin)
