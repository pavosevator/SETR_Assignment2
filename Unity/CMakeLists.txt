# Set the source files that make up the library
set(Unity
 unity_internals.h unity.h unity.c 
)
# Set the library type (can be static or dynamic )
set(LIB_TYPE STATIC)

# Create the library
add_library (Unity ${LIB_TYPE} ${Unity})
