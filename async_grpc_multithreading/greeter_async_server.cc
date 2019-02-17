/*
 * brew install --with-plugins grpc
 * brew install pkg-config
 * protoc -I. --cpp_out=. helloworld.proto
 * protoc -I. --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` helloworld.proto
 * c++ -o /tmp/server greeter_async_server.cc helloworld.grpc.pb.cc helloworld.pb.cc caculator.grpc.pb.cc caculator.pb.cc  `pkg-config --cflags protobuf grpc` `pkg-config --libs protobuf grpc++ grpc` -lgrpc++_reflection -std=c++11 -O3 -Wall -g
 */

#include <memory>
#include <iostream>
#include <string>
#include <thread>
#include <cstdio>
#include <boost/format.hpp>

#include <grpcpp/grpcpp.h>
#include <grpc/support/log.h>

#include "helloworld.grpc.pb.h"
#include "caculator.grpc.pb.h"

using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerCompletionQueue;
using grpc::Status;
using helloworld::HelloRequest;
using helloworld::HelloReply;
using helloworld::Greeter;

using namespace std;

// Class encompasing the state and logic needed to serve a request.
class HelloWorldCallData {
public:
    // Take in the "service" instance (in this case representing an asynchronous
    // server) and the completion queue "cq" used for asynchronous communication
    // with the gRPC runtime.
    HelloWorldCallData(Greeter::AsyncService* service, ServerCompletionQueue* cq)
            : service_(service), cq_(cq), responder_(&ctx_), status_(CREATE) {
        // Invoke the serving logic right away.
        cout << "HelloWorldCallData created " << boost::format("%lx") % (void*)this << endl;
        Proceed();
    }

    ~HelloWorldCallData() {
        cout << "HelloWorldCallData destroyed " << boost::format("%lx") % (void*)this << endl;
    }

    void Proceed() {
        if (status_ == CREATE) {
            cout << "HelloWorldCallData proceed CREATE " << boost::format("%lx") % (void*)this << endl;
            // Make this instance progress to the PROCESS state.
            status_ = PROCESS;

            // As part of the initial CREATE state, we *request* that the system
            // start processing SayHello requests. In this request, "this" acts are
            // the tag uniquely identifying the request (so that different HelloWorldCallData
            // instances can serve different requests concurrently), in this case
            // the memory address of this HelloWorldCallData instance.
            // 把request，contex等信息跟cq关联起来，当请求到达的时候cq->next会返回，返回的tag的值就是这个this
            service_->RequestSayHello(&ctx_, &request_, &responder_, cq_, cq_, this);
        } else if (status_ == PROCESS) {
            cout << "HelloWorldCallData proceed PROCESS " << boost::format("%lx") % (void*)this << endl;
            // Spawn a new HelloWorldCallData instance to serve new clients while we process
            // the one for this HelloWorldCallData. The instance will deallocate itself as
            // part of its FINISH state.
            new HelloWorldCallData(service_, cq_);

            // The actual processing.
            std::string prefix("Hello ");
            reply_.set_message(prefix + request_.name());

            // And we are done! Let the gRPC runtime know we've finished, using the
            // memory address of this instance as the uniquely identifying tag for
            // the event.
            status_ = FINISH;
            cout << "Press enter to run responder_.Finish()" << endl;
            ::getchar();
            responder_.Finish(reply_, Status::OK, this);
        } else {
            cout << "HelloWorldCallData proceed FINISH " << boost::format("%lx") % (void*)this << endl;
            GPR_ASSERT(status_ == FINISH);
            // Once in the FINISH state, deallocate ourselves (HelloWorldCallData).
            delete this;
        }
    }

private:
    // The means of communication with the gRPC runtime for an asynchronous
    // server.
    Greeter::AsyncService* service_;
    // The producer-consumer queue where for asynchronous server notifications.
    ServerCompletionQueue* cq_;
    // Context for the rpc, allowing to tweak aspects of it such as the use
    // of compression, authentication, as well as to send metadata back to the
    // client.
    ServerContext ctx_;

    // What we get from the client.
    HelloRequest request_;
    // What we send back to the client.
    HelloReply reply_;

    // The means to get back to the client.
    ServerAsyncResponseWriter<HelloReply> responder_;

    // Let's implement a tiny state machine with the following states.
    enum CallStatus { CREATE, PROCESS, FINISH };
    CallStatus status_;  // The current serving state.
};


class CaculatorCallData {
public:
    using CaculatorService = caculator::Caculator::AsyncService;
    using Request = caculator::Request;
    using Response = caculator::Response;
public:
    CaculatorCallData(CaculatorService* service, ServerCompletionQueue* cq)
            : service_(service), cq_(cq), responder_(&ctx_), status_(CREATE) {
        // Invoke the serving logic right away.
        cout << "CaculatorCallData created " << boost::format("%lx") % (void*)this << endl;
        Proceed();
    }

    ~CaculatorCallData() {
        cout << "CaculatorCallData destroyed " << boost::format("%lx") % (void*)this << endl;
    }

    void Proceed() {
        if (status_ == CREATE) {
            cout << "CaculatorCallData proceed CREATE " << boost::format("%lx") % (void*)this << endl;
            status_ = PROCESS;

            service_->Requestcaculate(&ctx_, &request_, &responder_, cq_, cq_, this);
        } else if (status_ == PROCESS) {
            cout << "CaculatorCallData proceed PROCESS " << boost::format("%lx") % (void*)this << endl;
            new CaculatorCallData(service_, cq_);

            int op1 = request_.op1();
            int op2 = request_.op2();
            reply_.set_result(op1 + op2);
            // The actual processing.
            // std::string prefix("Hello ");
            // reply_.set_message(prefix + request_.name());

            status_ = FINISH;
            responder_.Finish(reply_, Status::OK, this);
        } else {
            cout << "CaculatorCallData proceed FINISH " << boost::format("%lx") % (void*)this << endl;
            GPR_ASSERT(status_ == FINISH);
            // Once in the FINISH state, deallocate ourselves (CaculatorCallData).
            delete this;
        }
    }

private:
    CaculatorService* service_;
    ServerCompletionQueue* cq_;
    ServerContext ctx_;
    Request request_;
    Response reply_;
    ServerAsyncResponseWriter<Response> responder_;

    enum CallStatus { CREATE, PROCESS, FINISH };
    CallStatus status_;
};


class ServerImpl final {
public:
    using CaculatorService = caculator::Caculator::AsyncService;
public:
    ~ServerImpl() {
        server_->Shutdown();
        // Always shutdown the completion queue after the server.
        hello_world_cq_->Shutdown();
        caculator_cq_->Shutdown();
    }

    // There is no shutdown handling in this code.
    void Run() {
        std::string server_address("0.0.0.0:8000");

        ServerBuilder builder;
        // Listen on the given address without any authentication mechanism.
        builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
        // Register "hello_world_service_" as the instance through which we'll communicate with
        // clients. In this case it corresponds to an *asynchronous* service.
        builder.RegisterService(&hello_world_service_);
        builder.RegisterService(&caculator_service_);
        // Get hold of the completion queue used for the asynchronous communication
        // with the gRPC runtime.
        hello_world_cq_ = builder.AddCompletionQueue();
        caculator_cq_ = builder.AddCompletionQueue();
        // Finally assemble the server.
        server_ = builder.BuildAndStart();
        std::cout << "Server listening on " << server_address << std::endl;

        // Proceed to the server's main loop.
        std::thread thr1(std::bind(&ServerImpl::HelloWorldHandleRpcs, this));
        std::thread thr2(std::bind(&ServerImpl::CaculatorHandleRpcs, this));
        thr1.join();
        thr2.join();
        // HelloWorldHandleRpcs();
    }

private:
    // This can be run in multiple threads if needed.
    void HelloWorldHandleRpcs() {
        // Spawn a new HelloWorldCallData instance to serve new clients.
        new HelloWorldCallData(&hello_world_service_, hello_world_cq_.get());
        void* tag;  // uniquely identifies a request.
        bool ok;
        while (true) {
            // Block waiting to read the next event from the completion queue. The
            // event is uniquely identified by its tag, which in this case is the
            // memory address of a HelloWorldCallData instance.
            // The return value of Next should always be checked. This return value
            // tells us whether there is any kind of event or hello_world_cq_ is shutting down.
            GPR_ASSERT(hello_world_cq_->Next(&tag, &ok));
            GPR_ASSERT(ok);
            cout << "Processing next event " << boost::format("%lx") % tag << endl;
            static_cast<HelloWorldCallData*>(tag)->Proceed();
        }
    }

    void CaculatorHandleRpcs() {
        new CaculatorCallData(&caculator_service_, caculator_cq_.get());
        void* tag;  // uniquely identifies a request.
        bool ok;
        while (true) {
            GPR_ASSERT(caculator_cq_->Next(&tag, &ok));
            GPR_ASSERT(ok);
            cout << "Processing next event " << boost::format("%lx") % tag << endl;
            static_cast<CaculatorCallData*>(tag)->Proceed();
        }
    }

    std::unique_ptr<ServerCompletionQueue> hello_world_cq_;
    std::unique_ptr<ServerCompletionQueue> caculator_cq_;
    Greeter::AsyncService hello_world_service_;
    CaculatorService caculator_service_;
    std::unique_ptr<Server> server_;
};

int main(int argc, char** argv) {
    ServerImpl server;
    server.Run();

    return 0;
}
