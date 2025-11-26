#include <flow-ipc/session/shm/classic/client_session.hpp>
#include <flow-ipc/session/shm/classic/server_session.hpp>
#include <string_view>

using namespace ipc::session;
using namespace std::string_view_literals;

// Define unique IDs for the user and group
constexpr auto USER_ID_A = 1000;
constexpr auto USER_ID_B = 1000; // Same for this example
constexpr auto GROUP_ID = 1000;

// Describe the two communicating applications
const App APP_A{
    "app_a"sv,                     // Unique name
    "/bin/app_a.exec"sv,           // Executable path for security checks
    USER_ID_A,
    GROUP_ID
};
const App APP_B{
    "app_b"sv,
    "/bin/app_b.exec"sv,
    USER_ID_B,
    GROUP_ID
};

// Define roles: App A is the server, App B is the client
const Client_app APP_B_AS_CLI{ APP_B };
const Server_app APP_A_AS_SRV{ APP_A, { APP_B_AS_CLI } };

// server
#include <iostream>
#include <flow-ipc/transport/struc/channel.hpp>

void run_server() {
    using Server_session = ipc::session::shm::classic::Server_session<...>; // Template args omitted for brevity

    // ... setup logging and error handling ...
    flow::log::Logger logger; // Example logger
    flow::Error_code err_code;

    Server_session session_srv{
        logger,
        APP_A_AS_SRV,
        // ... other parameters like number of expected channels ...
    };

    // Synchronously wait for a client to connect and establish channels
    // The specifics of `async_accept` are detailed in the Flow-IPC manual
    // this is a simplified view of the logic:
    /*
    session_srv.async_accept(
        &session, ..., &my_init_channels, ..., &their_init_channels, ...,
        [&](const flow::Error_code& err_code) {
            if (!err_code) {
                // Connection successful, use my_init_channels[0] for communication
                ipc::transport::struc::Channel channel = std::move(my_init_channels[0]);
                // ... logic to handle incoming messages on the channel ...
            }
        }
    );
    */
}

// client
#include <iostream>

void run_client() {
    using Client_session = ipc::session::shm::classic::Client_session<...>; // Template args omitted for brevity

    // ... setup logging and error handling ...
    flow::log::Logger logger; // Example logger
    flow::Error_code err_code;

    Client_session session{
        logger,
        APP_B_AS_CLI,
        APP_A_AS_SRV // The server it wants to connect to
        // ... other parameters ...
    };

    // Synchronously connect (non-blocking) and obtain channels
    // The specifics of `sync_connect` are detailed in the Flow-IPC manual
    /*
    session.sync_connect(
        ..., &my_init_channels, ..., &their_init_channels, &err_code
    );

    if (!err_code) {
        // Connection successful, use my_init_channels[0] to send messages
        ipc::transport::struc::Channel channel = std::move(my_init_channels[0]);
        // ... logic to send Cap'n Proto messages or raw data blobs ...
    }
    */
}
