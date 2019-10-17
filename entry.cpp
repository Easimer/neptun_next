#include "neptun_next.h"

static void parse_cmdline(cmdargs_t* args, int argc, char** argv) {
    for(int i = 1; i < argc; i++) {
        if(argv[i][0] == '-') {
            switch(argv[i][1]) {
                case 's':
                    args->status_bar = true;
                    break;
            }
        }
    }
}


int main(int argc, char** argv) {
    cmdargs_t args;
    load_cfg();

    parse_cmdline(&args, argc, argv);

    if(args.status_bar) {
        status_bar(&args);
    } else {
        verbose_mode(&args);
    }

    unload_cfg();

    return 0;
}
