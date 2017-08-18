/*
 * c++ -o /tmp/test demo.cpp -lboost_program_options -lglog -std=c++11 -g
 * 
 * Test:
 * /tmp/test -agoogle.com -p1575 --online -b -t100 abc def
 */
#include <iostream>
#include <string>
#include <sstream>
#include <functional>
#include <climits>
#include <stdexcept>
#include <boost/program_options.hpp>
#include <glog/logging.h>


#define THROW_RUNTIME_ERROR(args) \
    do { \
        std::ostringstream __err_stream; \
        __err_stream << args << std::flush; \
        throw std::runtime_error( __err_stream.str() ); \
    } while (0)

#define THROW_RUNTIME_ERROR_IF(cond, args) \
    do { \
        if (cond) THROW_RUNTIME_ERROR(args); \
    } while (0)


#define MAJOR_VER   1
#define MINOR_VER   0


namespace po = boost::program_options;

enum RunMode {STANDALONE, ONLINE};

static std::string      g_strApiSvr;
static uint16_t         g_nPort = 0;
static uint32_t         g_nTrees = 0;
static uint32_t         g_nWorkThreads = 0;
static RunMode          g_eRunMode = STANDALONE;
static bool             g_bBackground = false;
static std::string      g_strInputFile, g_strOutputFile;


template<typename T>
void check_range(const std::string &name, const T &val, const T &min, const T &max)
{
    DLOG(INFO) << "check_range for " << name;
    THROW_RUNTIME_ERROR_IF(val < min || val > max,
        "Invalid value for arg " << name << ", should be in range [" << min << "," << max << "]");
}


static
void parse_args(int argc, char **argv)
{
    using namespace std;

    ostringstream oss;
    oss << "Usage: " << argv[0] << " [options] target" << flush;
    string strUsage = oss.str();

    // 🔴🔴 -指定的短参数只能是单个字母
    po::options_description desc(strUsage);
    desc.add_options()
        ("help,h", "Print help message")
        ("version,v", "Print version info")
        ("apisvr,a", po::value<string>(&g_strApiSvr)->required(), "ApiServer addr")
        ("port,p", po::value<uint16_t>(&g_nPort)->required()->
                        notifier([&](uint16_t port){
                            if (g_eRunMode == ONLINE)
                                check_range<uint16_t>("port", port, 1024, 65535);
                        }), "Server port of this program")
        ("ntrees,t", po::value<uint32_t>(&g_nTrees)->default_value(10)->
                notifier(std::bind(&check_range<uint32_t>, "ntrees", placeholders::_1, 1, UINT_MAX)),
                    "Number of trees")
        ("online", po::value<bool>()->implicit_value(true)->
                notifier([&](bool flag){
                    if (flag) g_eRunMode = ONLINE;
                }), "RunMode of the program")
        ("nworkers,w", po::value<uint32_t>(&g_nWorkThreads)->default_value(16)->
                    notifier([&](uint32_t val){
                        if (g_eRunMode == ONLINE)
                            check_range<uint32_t>("nworkers", val, 1, 100000);
                    }), "Number of work threads")
        ("background,b", po::value<bool>(&g_bBackground)->default_value(false)->implicit_value(true),
                 "Run as a background service")
        // 🍑🍑position options 要先定义然后声明为positional
        ("input-file", po::value<string>(&g_strInputFile), "Input file")
        ("output-file", po::value<string>(&g_strOutputFile), "Output file");

    po::positional_options_description      pDesc;
    pDesc.add("input-file", 1).add("output-file", 1);

    po::variables_map vm;
    // po::store(po::parse_command_line(argc, argv, desc), vm);
    po::store(po::command_line_parser(argc, argv).
            options(desc).positional(pDesc).run(), vm);

    // ⭐⭐这两个参数单独处理，因为正常情况下有些参数必须提供 required
    if (vm.count("help")) {
        cout << desc << endl;
        exit(0);
    } else if (vm.count("version")) {
        cout << MAJOR_VER << "." << MINOR_VER << endl;
        exit(0);
    } // if

    po::notify(vm);
}


static
void print_args()
{
    LOG(INFO) << "RunMode = " << (g_eRunMode == STANDALONE ? "Standalone" : "Online");
    LOG(INFO) << "ApiSvr = " << g_strApiSvr;
    LOG(INFO) << "port = " << g_nPort;
    LOG(INFO) << "ntrees = " << g_nTrees;
    LOG(INFO) << "nWorkThreads = " << g_nWorkThreads;
    LOG(INFO) << "background = " << g_bBackground;
    LOG(INFO) << "InputFile = " << g_strInputFile;
    LOG(INFO) << "OutputFile = " << g_strOutputFile;
}


int main(int argc, char **argv)
try {
    using namespace std;

    parse_args(argc, argv);
    print_args();

    return 0;

} catch (const std::exception &ex) {
    std::cerr << "Exception caught by main: " << ex.what() << std::endl;
    return -1;
}



