#include <glog/logging.h>

using namespace std;

int main(int argc, char **argv) {
    google::InitGoogleLogging(argv[0]);
    google::LogToStderr();

    LOG(INFO) << "This is a test.";

    return 0;
}

