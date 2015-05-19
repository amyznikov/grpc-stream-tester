// server.cc
#include <memory>
#include <string>
#include <thread>
#include <chrono>
#include <grpc/grpc.h>
#include <grpc++/channel_arguments.h>
#include <grpc++/channel_interface.h>
#include <grpc++/create_channel.h>
#include <grpc++/server.h>
#include <grpc++/server_builder.h>
#include <grpc++/server_credentials.h>
#include <grpc++/stream.h>
#include <grpc++/status.h>
#include "test.pb.h"
#include "test.grpc.pb.h"


using namespace grpc;
using namespace tester;
using namespace std;


class TestService
  : public StreamTester::Service
{
  unique_ptr<Server> server_;

public:

  TestService(const string & address) {
    ServerBuilder builder;
    builder.AddListeningPort(address, InsecureServerCredentials());
    builder.RegisterService(this);
    server_ = builder.BuildAndStart();
  }

  void run() {
    server_->Wait();
  }

private:
  Status GetStream(ServerContext * context, const StreamRequest * request, ServerWriter<TestEvent> * writer)
  {
    (void)(context); // unused
    (void)(request); // unused

    TestEvent e;

    for ( int i = 0; ; ++i ) {

      e.set_event_name( string("event #") + to_string(i) );
      
      fprintf(stderr,"sending '%s'\n", e.event_name().c_str() );

      if ( !writer->Write(e) ) {
        fprintf(stderr,"Write() fails: finish loop\n");
        break;
      }
      
      this_thread::sleep_for(chrono::seconds(1));
    }

    return Status::OK;
  }
};


void run_test()
{
  TestService service("localhost:7000");
  service.run();
}

int main()
{
  grpc_init();
  run_test();
  grpc_shutdown();

  return 0;
}
