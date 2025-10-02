#include <cstdio>
#include <cstring>
#include <iostream>

#include "libtcc.h"
typedef float (*test_fun) (float);

int main() {

    test_fun sqr = NULL;
    TCCState *S = tcc_new();
    tcc_add_include_path(S, (std::string(TINYCC_SOURCE_DIR)+"/win32/include").c_str());
    tcc_add_library_path(S,(std::string(TINYCC_SOURCE_DIR)+"/win32/lib").c_str());
    tcc_set_output_type(S, TCC_OUTPUT_MEMORY);
    tcc_compile_string(S, "#include<math.h> \n float func(float x) { return (float)(sin(x)); }");
    tcc_relocate(S);
    sqr = (test_fun)tcc_get_symbol(S, "func");
    printf("Result: %f", sqr(1));
    tcc_delete(S);
    return 0;
}
