#include <stdio.h>     /* for printf */
#include <stdlib.h>    /* for exit */
#include <getopt.h>

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
   int c;
   int digit_optind = 0;

   while (1) {
       int this_option_optind = optind ? optind : 1;
       int option_index = 0;
       static struct option long_options[] = {
           {"add",     required_argument, 0,  0 },
           {"append",  no_argument,       0,  0 },
           {"delete",  required_argument, 0,  0 },
           {"verbose", no_argument,       0,  0 },
           {"create",  required_argument, 0, 'c'},
           {"file",    required_argument, 0,  0 },
           {0,         0,                 0,  0 }
       };

       c = getopt_long(argc, argv, "abc:d:012",
                long_options, &option_index);
       if (c == -1)
           break;

       switch (c) {
       case 0:
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
       }
   } // while 1

   if (optind < argc) {
       printf("non-option ARGV-elements: ");
       while (optind < argc)
           printf("%s ", argv[optind++]);
       printf("\n");
   }

   exit(EXIT_SUCCESS);
}

