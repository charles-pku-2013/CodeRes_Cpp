#include <glog/logging.h>


/*
 * link with -lglog
 *
 * env var opts:
 * GLOG_logtostderr=1 ./your_application
 * stderrthreshold
 * log_dir
 *
 * INFO, WARNING, ERROR, and FATAL. 
 * Logging a FATAL message terminates the program (after the message is logged). 
 */


int main(int argc, char* argv[])
{
	// Initialize Google's logging library.
	google::InitGoogleLogging(argv[0]);

	// ...
    int num_cookies = 25;
	LOG(INFO) << "Found " << num_cookies << " cookies";

    return 0;
}
