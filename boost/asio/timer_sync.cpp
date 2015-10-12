// compile with -lboost_system -lboost_thread
// Timer.1 - Using a timer synchronously

#include <iostream>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

int main()
{
	// All programs that use asio need to have at least one io_service object. This class provides access to I/O functionality.
	boost::asio::io_service io;

	// the call to deadline_timer::wait() will not return until the timer has expired, 5 seconds after it was created (i.e. not from when the wait starts).
	boost::asio::deadline_timer t(io, boost::posix_time::seconds(5));
	t.wait();
	
	// A deadline timer is always in one of two states: "expired" or "not expired". If the deadline_timer::wait() function is called on an expired timer, it will return immediately.

	std::cout << "Hello, world!" << std::endl;

	return 0;
}





