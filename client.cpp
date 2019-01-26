#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <uv.h>

#include "generated/rpc.pb.h"
#include "generated/hello.pb.h"

#include "common.h"

unsigned int sequenceId = 0;

int main(int argc, char* argv[])
{
    auto loop = uv_default_loop();
    auto socket = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
    uv_tcp_init(loop, socket);

    auto connect = (uv_connect_t*)malloc(sizeof(uv_connect_t));

    struct sockaddr_in dest;
    uv_ip4_addr("127.0.0.1", 8000, &dest);

    uv_tcp_connect(connect, socket, (const struct sockaddr*)&dest, [] (uv_connect_t* req, int status)
    {
        // Proto stuff /////////////////////

        /* Person person; */
        /* person.set_name("Emma"); */
        /* person.set_id(1337); */
        /* person.set_email("emma@icloud.com"); */

        /* Animal animal; */
        /* animal.set_name("Lion"); */
        /* animal.set_family("Cat"); */

        // size_t size = person.ByteSizeLong();
        // void* data = malloc(size);
        // bool success = person.SerializePartialToArray(data, size);

        fubar_data_t fb = create_fixed_fubar_data();
        FubarWithData fubar;
        fubar.set_id(1337);
        fubar.set_data(&fb, sizeof(fb));

        RpcRequest request;
        request.set_id(sequenceId++);
        auto payload = request.mutable_payload();
        // payload->PackFrom(person);
        // payload->PackFrom(animal);
        payload->PackFrom(fubar);

        size_t size = request.ByteSizeLong();
        void* data = malloc(size);
        bool success = request.SerializePartialToArray(data, size);

        if (!success)
        {
            printf("Request serialization failed\n");
        }

        ////////////////////////////////////

        /* const char* data = "hello from client\n"; */
        /* size_t datasize = strlen(data); */

        auto buf = uv_buf_init((char*)malloc(size), size);
        memcpy(buf.base, data, size);

        uv_write_t wreq;
        uv_write(&wreq, req->handle, &buf, 1, [] (uv_write_t* req, int status)
        {
            printf("write complete\n");
            printf("status %d", status);
        });
    });

    int ret = uv_run(loop, UV_RUN_DEFAULT);
    return ret;
}
