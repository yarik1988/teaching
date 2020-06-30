#include <iostream>
#include "tcc/libtcc.h"
typedef float (*test_fun) (int);

int main() {
    test_fun sqr = NULL;
    TCCState *S = tcc_new();
    tcc_add_include_path(S, "../tcc/win32/include");
    tcc_add_library_path(S,"../libVS");
    tcc_set_output_type(S, TCC_OUTPUT_MEMORY);
    tcc_compile_string(S, "#include<math.h> float func(int x) { return sin(x); }");
    tcc_relocate(S, TCC_RELOCATE_AUTO);
    sqr = (test_fun)tcc_get_symbol(S, "func");
    printf("result: %f", sqr(2));
    tcc_delete(S);
    return 0;
}
