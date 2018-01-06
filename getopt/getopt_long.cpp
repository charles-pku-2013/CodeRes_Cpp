#include <stdio.h>     /* for printf */
#include <stdlib.h>    /* for exit */
#include <getopt.h>


/*
 * 头文件中定义的全局变量
 * extern char *optarg; // 参数值字符串
 * extern int optind;
 * extern int optopt;
 * extern int opterr;
 * extern int optreset;
 */

/*
 * About flag & val
 * If flag is not NULL, then the integer pointed to by it will be set to the value in the val field (*flag = val).
 *      便于设置 with-xx without-xx  opt no-opt
 * If the flag field is NULL, then the val field will be returned.  Setting flag to NULL and setting val to the corresponding
 * short option will make this function act just like getopt(3).
 *
 * val 是 getopt_long() 的返回值
 */
/*
 * struct option {
 *    const char *name;         // 长参数名
 *    int         has_arg;      // no_argument(或者是0)，表示该参数后面不跟参数值
                                // required_argument(或者是1),表示该参数后面一定要跟个参数值
                                // optional_argument(或者是2),表示该参数后面可以跟，也可以不跟参数值
 *    int        *flag;         // 用来决定，getopt_long()的返回值到底是什么。如果flag是null，
 *                                  则函数会返回与该项option匹配的val值
 *    int         val;          // 和flag联合决定返回值
 * };
 */

int
main(int argc, char **argv)
{
    int c = 0;
    int digit_optind = 0;

    static struct option long_options[] = {
        {"add",     required_argument, 0,  0 },
        {"append",  no_argument,       0,  0 },
        {"delete",  required_argument, 0,  0 },
        {"verbose", no_argument,       0,  0 },
        {"create",  required_argument, 0, 'c'}, // 最后测'c' 是getopt_long() 的返回值，由switch case 'c' 处理
        {"file",    required_argument, 0,  0 },
        {0,         0,                 0,  0 }
    };

    while (1) {
        int this_option_optind = optind ? optind : 1;
        int option_index = 0; // 命中了 long_options中的哪一项
        c = getopt_long(argc, argv, "abc:d:012", long_options, &option_index);
        // c = getopt_long(argc, argv, "abd:012", long_options, &option_index); // 只能用 --create 但由 case 'c' 处理

        if (c == -1)
            break;

        switch (c) {
            case 0:
                // When a long option is processed, the call to getopt_long() will return 0
                // 在这里处理长参数如add verbose
                if (long_options[option_index].flag) // flag 绑定参数值,已处理
                    break;
                printf("option %s", long_options[option_index].name);
                if (optarg)
                    printf(" with arg %s", optarg);
                printf("\n");
                break;

            case '0':
            case '1':
            case '2':
                if (digit_optind != 0 && digit_optind != this_option_optind)
                    printf("digits occur in two different argv-elements.\n");
                digit_optind = this_option_optind;
                printf("option %c\n", c);
                break;

            case 'a':
                printf("option a\n");
                break;

            case 'b':
                printf("option b\n");
                break;

            case 'c':
                printf("option c with value '%s'\n", optarg);
                break;

            case 'd':
                printf("option d with value '%s'\n", optarg);
                break;

            case '?':
                break;

            default:
                printf("?? getopt returned character code 0%o ??\n", c);
        } // switch
    } // while 1

    // 打印剩余的未被解析的参数
    if (optind < argc) {
        printf("non-option ARGV-elements: ");
        while (optind < argc)
            printf("%s ", argv[optind++]);
        printf("\n");
    } // if

    exit(EXIT_SUCCESS);
}


static
void parse_args(int argc, char **argv)
{
    int bflag, ch, fd;
    int daggerset;

    /* options descriptor */
    static struct option longopts[] = {
        { "buffy",      no_argument,            NULL,           'b' },
        { "fluoride",   required_argument,      NULL,           'f' },
        { "daggerset",  no_argument,            &daggerset,     1 }, // getopt_long return 0
        { NULL,         0,                      NULL,           0 }
    };

    bflag = 0;
    while ((ch = getopt_long(argc, argv, "bf:", longopts, NULL)) != -1) {
        switch (ch) {
            case 'b':
                bflag = 1;
                break;
            case 'f':
                if ((fd = open(optarg, O_RDONLY, 0)) == -1)
                    err(1, "unable to open %s", optarg);
                break;
            case 0:
                if (daggerset) {
                    fprintf(stderr,"Buffy will use her dagger to "
                            "apply fluoride to dracula's teeth\n");
                } // if
                break;
            default:
                usage();
        } // switch
    } // while

    // 便于处理后续参数
    argc -= optind;
    argv += optind;
}

