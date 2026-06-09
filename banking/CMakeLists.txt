add_library(banking STATIC Account.cpp Transaction.cpp)
add_library(banking::banking ALIAS banking)

target_include_directories(banking PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

install(TARGETS banking EXPORT bankingConfig ARCHIVE DESTINATION lib)
install(FILES Account.h Transaction.h DESTINATION include)

# Export a CMake config package so that the library can be consumed via
# find_package(banking CONFIG) — the base of a reusable Hunter package.
install(EXPORT bankingConfig
    NAMESPACE banking::
    DESTINATION lib/cmake/banking
)
