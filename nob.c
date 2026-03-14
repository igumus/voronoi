#define NOB_IMPLEMENTATION
#include "./thirdparty/nob.h"

#define BUILD_FOLDER "bin/"
#define SRC_FOLDER   "src/"

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);
    if (!nob_mkdir_if_not_exists(BUILD_FOLDER)) return 1;

    Nob_Cmd cmd = {0};

    nob_cc(&cmd);
    nob_cc_flags(&cmd);
    cmd_append(&cmd, "-ggdb");
    nob_cc_output(&cmd, BUILD_FOLDER "voronoi");
    nob_cc_inputs(&cmd, SRC_FOLDER "main.c");
    cmd_append(&cmd, "-I./raylib/include");
    cmd_append(&cmd, "-I./thirdparty");
    cmd_append(&cmd, "-L./raylib/lib/");
    cmd_append(&cmd, "-l:libraylib.a");
    cmd_append(&cmd, "-lm");
    if (!nob_cmd_run(&cmd)) return 1;

}
