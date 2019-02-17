/*
 * brew install --with-plugins grpc
 * brew install pkg-config
 * protoc -I. --cpp_out=. helloworld.proto
 * protoc -I. --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` helloworld.proto
 * c++ -o /tmp/caculator_client caculator_client.cc caculator.grpc.pb.cc caculator.pb.cc `pkg-config --cflags protobuf grpc` `pkg-config --libs protobuf grpc++ grpc` -lgrpc++_reflection -std=c++11 -O3 -Wall -g
 */

#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "caculator.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using Request = caculator::Request;
using Response = caculator::Response;
using Caculator = caculator::Caculator;

using namespace std;


class CaculatorClient {
public:
    CaculatorClient(std::shared_ptr<Channel> channel)
        : stub_(Caculator::NewStub(channel)) {}

    // Assembles the client's payload, sends it and presents the response back
    // from the server.
    int caculate(const std::string &opname, int op1, int op2) {
        Request req;
        Response res;

        req.set_opname(opname);
        req.set_op1(op1);
        req.set_op2(op2);

        ClientContext context;

        Status status = stub_->caculate(&context, req, &res);

        // Act upon its status.
        if (status.ok()) {
            return res.result();
        } else {
            std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
            return 0;
        }
    }

private:
    std::unique_ptr<Caculator::Stub> stub_;
};

int main(int argc, char** argv) {
    // Instantiate the client. It requires a channel, out of which the actual RPCs
    // are created. This channel models a connection to an endpoint (in this case,
    // localhost at port 50051). We indicate that the channel isn't authenticated
    // (use of InsecureChannelCredentials()).
    CaculatorClient caculator(grpc::CreateChannel(
                "localhost:8000", grpc::InsecureChannelCredentials()));
    cout << caculator.caculate("add", 3, 5) << endl;

    return 0;
}
