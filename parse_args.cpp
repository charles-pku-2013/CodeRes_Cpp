/*
 * parse_args.cpp
 */

static
void parse_args( int argc, char **argv )
{
    auto print_and_exit = [] {
        print_usage();
        exit(0);
    };

    if (argc < 4)
        print_and_exit();

    char    *parg = NULL;
    char    optc;
    int     i;

    if (strcmp(argv[1], "build") == 0) {
        g_eRunType = BUILD;
        for (i = 2; i < argc;) {
            parg = argv[i];
            if ( *parg++ != '-' )
                print_and_exit();
            optc = *parg;
            if (!optc)
                print_and_exit();
            if (optc == 'i') {
                if (++i >= argc)
                    print_and_exit();
                g_cstrInputData = argv[i];
            } else if (optc == 'o') {
                if (++i >= argc)
                    print_and_exit();
                g_cstrOutputData = argv[i];
            } else if (strcmp(parg, "min-count") == 0) {
                if (++i >= argc)
                    print_and_exit();
                if (sscanf(argv[i], "%u", &g_nMinCount) != 1)
                    print_and_exit();
            } else if (strcmp(parg, "max-vocab") == 0) {
                if (++i >= argc)
                    print_and_exit();
                if (sscanf(argv[i], "%u", &g_nMaxVocab) != 1)
                    print_and_exit();
            } else if (strcmp(parg, "window-size") == 0) {
                if (++i >= argc)
                    print_and_exit();
                if (sscanf(argv[i], "%u", &g_nWindowSize) != 1)
                    print_and_exit();
            } else if (strcmp(parg, "topk") == 0) {
                if (++i >= argc)
                    print_and_exit();
                if (sscanf(argv[i], "%u", &g_nTopK) != 1)
                    print_and_exit();
            } else if (strcmp(parg, "memory") == 0) {
                if (++i >= argc)
                    print_and_exit();
                if (sscanf(argv[i], "%f", &g_fMemorySize) != 1)
                    print_and_exit();
            } else {
                print_and_exit();
            } // if

            ++i;
        } // for

    } else if (strcmp(argv[1], "load") == 0) {
        g_eRunType = LOAD;
        for (i = 2; i < argc;) {
            parg = argv[i];
            if ( *parg++ != '-' )
                print_and_exit();
            optc = *parg;
            if (!optc)
                print_and_exit();
            if (optc == 'i') {
                if (++i >= argc)
                    print_and_exit();
                g_cstrInputData = argv[i];
            } else {
                print_and_exit();
            } // if

            ++i;
        } // for
    } else {
        print_and_exit();
    } // if
}

static
void check_args()
{
    using namespace std;

    auto err_exit = [] (const char *msg) {
        cerr << msg << endl;
        exit(-1);
    };

    if (g_eRunType == BUILD) {
        if (!g_cstrInputData)
            err_exit( "arg error: no input data file specified." );
        if (!g_nMinCount)
            err_exit( "arg error: -min-count must be specified." );
    } else if (g_eRunType == LOAD) {
        if (!g_cstrInputData)
            err_exit( "arg error: no input data file specified." );
    } // if
}


namespace Test {
    void print_args()
    {
        using namespace std;
        cerr << "g_nMinCount = " << g_nMinCount << endl;
        cerr << "g_nMaxVocab = " << g_nMaxVocab << endl;
        cerr << "g_nWindowSize = " << g_nWindowSize << endl;
        cerr << "g_fMemorySize = " << g_fMemorySize << endl;
        cerr << "g_cstrInputData = " << (g_cstrInputData ? g_cstrInputData : "NULL") << endl;
        cerr << "g_cstrOutputData = " << (g_cstrOutputData ? g_cstrOutputData : "NULL") << endl;
        cerr << "g_eRunType = " << (g_eRunType == BUILD ? "BUILD" : "LOAD") << endl;
    }
} // namespace Test

