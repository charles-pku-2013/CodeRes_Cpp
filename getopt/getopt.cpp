#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "LOG.h"

// 如果不希望getopt()打印出错信息，则只要将全域变量opterr设为0即可
/*
 * optstring中的指定的内容的意义（例如getopt(argc, argv, "ab:c:de::");）
 * 1.单个字符，表示选项，（如上例中的abcde各为一个选项）
 * 2.单个字符后接一个冒号：表示该选项后必须跟一个参数。参数紧跟在选项后或者以空格隔开。
 * 该参数的指针赋给optarg。（如上例中的b:c:）
 * 3 单个字符后跟两个冒号，表示该选项后可以跟一个参数，也可以不跟。如果跟一个参数，
 * 参数必须紧跟在选项后不能以空格隔开。该参数的指针赋给optarg。
 * (如上例中的e::，如果没有跟参数，则optarg = NULL)
 */


/*
 * getopt() 所设置的全局变量包括：
 * char *optarg——当前选项参数字串（如果有）。
 * int optind——argv的当前索引值。当getopt()在while循环中使用时，循环结束后，剩下的字串视为操作数，
 *          在argv[optind]至argv[argc-1]中可以找到。
 * int opterr——这个变量非零时，getopt()函数为“无效选项”和“缺少参数选项，并输出其错误信息。
 * int optopt——当发现无效选项字符之时，getopt()函数或返回'?'字符，或返回':'字符，
 *          并且optopt包含了所发现的无效选项字符。
 */


int
main(int argc, char *argv[])
{
    int flags, opt;
    int nsecs, tfnd;

    nsecs = 0;
    tfnd = 0;
    flags = 0;
    while ((opt = getopt(argc, argv, "nt:")) != -1) {
        DBG("getopt returned %c, optind = %d", opt, optind);
        switch (opt) {
            case 'n':
                flags = 1;
                break;
            case 't':
                nsecs = atoi(optarg);
                tfnd = 1;
                break;
            default: /* '?' */
                fprintf(stderr, "Usage: %s [-t nsecs] [-n] name\n",
                        argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    printf("flags=%d; tfnd=%d; optind=%d\n", flags, tfnd, optind);

    // optind 指期待的下一个，>=argc 表示没有后续参数了
    if (optind >= argc) {
        fprintf(stderr, "Expected argument after options\n");
        exit(EXIT_FAILURE);
    }

    printf("name argument = %s\n", argv[optind]);

    /* Other code omitted */

    exit(EXIT_SUCCESS);
}


/*
 * 如果不希望输出任何错误信息，或更希望输出自定义的错误信息。
 *          可以采用以下两种方法来更改getopt()函数的出错信息输出行为：
 * 在调用getopt()之前，将opterr设置为0，这样就可以在getopt()函数发现错误的时候强制它不输出任何消息。
 * 如果optstring参数的第一个字符是冒号，
 *          那么getopt()函数就会保持沉默，并根据错误情况返回不同字符，如下：
 * “无效选项” —— getopt()返回'?'，并且optopt包含了无效选项字符（这是正常的行为）。
 * “缺少选项参数” —— getopt()返回':'，如果optstring的第一个字符不是冒号，
 *          那么getopt()返回'?'，这会使得这种情况不能与无效选项的情况区分开。
 */

int main (int argc, char **argv)
{
    int oc;                     /*选项字符 */
    char ec;                             /*无效的选项字符*/
    char *b_opt_arg;            /*选项参数字串 */

    while((oc = getopt(argc, argv, ":ngl:")) != -1)
    {
        switch(oc)
        {
            case 'n':
                printf("My name is Lyong.\n");
                break;
             case 'g':
                printf("Her name is Xxiong.\n");
                break;
            case 'l':
                b_opt_arg = optarg;
                printf("Our love is %s\n", optarg);
                break;
            case '?':
                ec = (char)optopt;
                printf("无效的选项字符 \' %c \'!\n", ec);
                break;
            case ':':
                printf("缺少选项参数！\n");
                break;
        }
    }
    return 0;
}

/*
 * $ ./opt_parse_demo -a
 * 无效的选项字符 ' a '!
 * $ ./opt_parse_demo -l
 * 缺少选项参数！
 */
