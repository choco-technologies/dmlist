# =====================================================================
#               DMLIST Tests
# =====================================================================

# Create a simple test executable
add_executable(test_simple test_simple.c dmod_stubs.c)
target_link_libraries(test_simple PRIVATE dmlist dmod_system)

# Add test
add_test(NAME test_simple COMMAND test_simple)
