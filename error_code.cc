/*
https://en.cppreference.com/w/cpp/error/errc.html

name                               equivalent posix error
address_family_not_supported       EAFNOSUPPORT
address_in_use                     EADDRINUSE
address_not_available              EADDRNOTAVAIL
already_connected                  EISCONN
argument_list_too_long             E2BIG
argument_out_of_domain             EDOM
bad_address                        EFAULT
bad_file_descriptor                EBADF
bad_message                        EBADMSG
broken_pipe                        EPIPE
connection_aborted                 ECONNABORTED
connection_already_in_progress     EALREADY
connection_refused                 ECONNREFUSED
connection_reset                   ECONNRESET
cross_device_link                  EXDEV
destination_address_required       EDESTADDRREQ
device_or_resource_busy            EBUSY
directory_not_empty                ENOTEMPTY
executable_format_error            ENOEXEC
file_exists                        EEXIST
file_too_large                     EFBIG
filename_too_long                  ENAMETOOLONG
function_not_supported             ENOSYS
host_unreachable                   EHOSTUNREACH
identifier_removed                 EIDRM
illegal_byte_sequence              EILSEQ
inappropriate_io_control_operation ENOTTY
interrupted                        EINTR
invalid_argument                   EINVAL
invalid_seek                       ESPIPE
io_error                           EIO
is_a_directory                     EISDIR
message_size                       EMSGSIZE
network_down                       ENETDOWN
network_reset                      ENETRESET
network_unreachable                ENETUNREACH
no_buffer_space                    ENOBUFS
no_child_process                   ECHILD
no_link                            ENOLINK
no_lock_available                  ENOLCK
no_message_available (deprecated)  ENODATA
no_message                         ENOMSG
no_protocol_option                 ENOPROTOOPT
no_space_on_device                 ENOSPC
no_stream_resources (deprecated)   ENOSR
no_such_device_or_address          ENXIO
no_such_device                     ENODEV
no_such_file_or_directory          ENOENT
no_such_process                    ESRCH
not_a_directory                    ENOTDIR
not_a_socket                       ENOTSOCK
not_a_stream (deprecated)          ENOSTR
not_connected                      ENOTCONN
not_enough_memory                  ENOMEM
not_supported                      ENOTSUP
operation_canceled                 ECANCELED
operation_in_progress              EINPROGRESS
operation_not_permitted            EPERM
operation_not_supported            EOPNOTSUPP
operation_would_block              EWOULDBLOCK
owner_dead                         EOWNERDEAD
permission_denied                  EACCES
protocol_error                     EPROTO
protocol_not_supported             EPROTONOSUPPORT
read_only_file_system              EROFS
resource_deadlock_would_occur      EDEADLK
resource_unavailable_try_again     EAGAIN
result_out_of_range                ERANGE
state_not_recoverable              ENOTRECOVERABLE
stream_timeout (deprecated)        ETIME
text_file_busy                     ETXTBSY
timed_out                          ETIMEDOUT
too_many_files_open_in_system      ENFILE
too_many_files_open                EMFILE
too_many_links                     EMLINK
too_many_symbolic_link_levels      ELOOP
value_too_large                    EOVERFLOW
wrong_protocol_type                EPROTOTYPE
 */

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <system_error>
#include <thread>

void print_error(const std::string& details, std::error_code error_code) {
    std::string value_name;
    if (error_code == std::errc::invalid_argument) value_name = "std::errc::invalid_argument";
    if (error_code == std::errc::no_such_file_or_directory) value_name = "std::errc::no_such_file_or_directory";
    if (error_code == std::errc::is_a_directory) value_name = "std::errc::is_a_directory";
    if (error_code == std::errc::permission_denied) value_name = "std::errc::permission_denied";

    std::cout << details << ":\n  " << std::quoted(error_code.message()) << " (" << value_name << ")\n\n";
}

void print_errno(const std::string& details, int errno_value = errno) {
    print_error(details, std::make_error_code(std::errc(errno_value)));
}

int main() {
    std::cout << "detaching a not-a-thread...\n";
    try {
        std::thread().detach();
    } catch (const std::system_error& e) {
        print_error("error detaching empty thread", e.code());
    }

    std::cout << "opening nonexistent file...\n";
    std::ifstream nofile{"nonexistent-file"};
    if (!nofile.is_open()) print_errno("error opening nonexistent file for reading");

    std::cout << "reading from directory as a file...\n";
    std::filesystem::create_directory("dir");
    std::ifstream         dir_stream{"dir"};
    [[maybe_unused]] char c = dir_stream.get();
    if (!dir_stream.good()) print_errno("error reading data from directory");

    std::cout << "open read-only file for writing...\n";
    std::fstream{"readonly-file", std::ios::out};
    std::filesystem::permissions("readonly-file", std::filesystem::perms::owner_read);
    std::fstream write_readonly("readonly-file", std::ios::out);
    if (!write_readonly.is_open()) print_errno("error opening read-only file for writing");
}
