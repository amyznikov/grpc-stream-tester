// client.cc

#include <memory>
#include <string>

#include <grpc/grpc.h>
#include <grpc++/channel_arguments.h>
#include <grpc++/channel_interface.h>
#include <grpc++/create_channel.h>
#include <grpc++/client_context.h>
#include <grpc++/stream.h>
#include <grpc++/status.h>
#include "test.pb.h"
#include "test.grpc.pb.h"


using namespace grpc;
using namespace tester;
using namespace std;


static void run_test()
{
  TestEvent e;
  ClientContext context;

  unique_ptr<StreamTester::Stub> tester(
      StreamTester::NewStub(CreateChannel("localhost:7000", InsecureCredentials(), ChannelArguments())));

  unique_ptr<ClientReader<TestEvent>> rd =
        tester->GetStream(&context, StreamRequest());


  printf("begin read\n");
  
  while ( rd->Read(&e) ) {
    printf("event: %s\n", e.event_name().c_str());
  }
  
  printf("end read\n");

  rd->Finish();
}



int main()
{
  grpc_init();

  run_test();

  grpc_shutdown();
  return 0;
}
