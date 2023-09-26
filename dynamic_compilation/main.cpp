#include <stdio.h>
#include "tinycc/libtcc.h"
typedef float (*test_fun) (float);

int main() {
    test_fun sqr = NULL;
    TCCState *S = tcc_new();
    tcc_add_include_path(S, "../tinycc/win32/include");
    tcc_add_include_path(S, "../tinycc/include");
    tcc_add_library_path(S,"../tinycc/win32/lib");
    tcc_set_output_type(S, TCC_OUTPUT_MEMORY);
    tcc_compile_string(S, "#include<math.h> float func(float x) { return (float)(sin(x)); }");
    tcc_relocate(S, TCC_RELOCATE_AUTO);
    sqr = (test_fun)tcc_get_symbol(S, "func");
    printf("Result: %f", sqr(1));
    tcc_delete(S);
    return 0;
}
