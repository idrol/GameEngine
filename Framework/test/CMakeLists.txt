set_property(TARGET FrameworkTest PROPERTY CXX_STANDARD 17)
target_compile_definitions(FrameworkTest PUBLIC BUILD_CLIENT)

target_sources(FrameworkTest
        PRIVATE
            TestLevel.cpp
        )