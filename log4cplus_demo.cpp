/*
 * c++ -o /tmp/test log4cplus_demo.cpp -llog4cplus -std=c++11 -g
 */
#include <log4cplus/logger.h> 
#include <log4cplus/fileappender.h> 
#include <log4cplus/consoleappender.h> 
#include <log4cplus/layout.h> 
#include <log4cplus/loggingmacros.h>

using namespace log4cplus; 
using namespace log4cplus::helpers; 

int main(int argc, char **argv) 
{ 
    // 定义一个控制台的Appender 
    SharedAppenderPtr pConsoleAppender(new ConsoleAppender()); 

    // 定义Logger 
    Logger pTestLogger = Logger::getInstance("LoggerName");    

    // 将需要关联Logger的Appender添加到Logger上 
    pTestLogger.addAppender(pConsoleAppender); 

    // 输出日志信息 
    LOG4CPLUS_WARN(pTestLogger, "This is a <Warn> log message..."); 
    
    return 0; 
}

/*
 * int main()
 * {
 *     // Initialization and deinitialization.
 *     log4cplus::Initializer initializer;
 * 
 *     log4cplus::BasicConfigurator config;
 *     config.configure();
 * 
 *     log4cplus::Logger logger = log4cplus::Logger::getInstance(
 *         LOG4CPLUS_TEXT("main"));
 *     LOG4CPLUS_WARN(logger, LOG4CPLUS_TEXT("Hello, World!"));
 * 
 *     return 0;
 * }
 */
