#include <stdio.h>
#include <stdlib.h>
#include <uv.h>

#include "generated/rpc.pb.h"
#include "generated/hello.pb.h"

#include "common.h"

#define PORT 8000

uv_loop_t* loop;
struct sockaddr_in addr;

int main(int argc, char* argv[])
{
    loop = uv_default_loop();

    uv_tcp_t server;
    uv_tcp_init(loop, &server);

    uv_ip4_addr("0.0.0.0", PORT, &addr);
    uv_tcp_bind(&server, (const struct sockaddr*)&addr, 0);
    int err = uv_listen((uv_stream_t*)&server, 100, [] (uv_stream_t* server, int status)
    {
        if (status < 0)
        {
            fprintf(stderr, "connection error %s\n", uv_strerror(status));
            return;
        }

        uv_tcp_t* client = (uv_tcp_t*) malloc(sizeof(uv_tcp_t));
        uv_tcp_init(loop, client);

        if (uv_accept(server, (uv_stream_t*)client) == 0)
        {
            uv_read_start(
                (uv_stream_t*)client,
                [] (uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
                {
                    buf->base = (char*) malloc(suggested_size);
                    buf->len = suggested_size;
                },
                [] (uv_stream_t* client, ssize_t nread, const uv_buf_t* buf)
                {
                    if (nread > 0)
                    {
                        // printf("%s\n", buf->base);

                        // Person person;
                        // person.ParseFromArray(buf->base, buf->len);

                        RpcRequest request;
                        request.ParseFromArray(buf->base, buf->len);

                        if (request.payload().Is<Person>())
                        {
                            Person person;
                            request.payload().UnpackTo(&person);
                            printf("%s\n", person.DebugString().c_str());
                        }
                        else if (request.payload().Is<FubarWithData>())
                        {
                            FubarWithData fubar;
                            request.payload().UnpackTo(&fubar);

                            std::string data = fubar.data();

                            fubar_data_t fb;
                            memcpy(&fb, data.c_str(), sizeof(fb));
                            fubar_data_print(fb);
                        }
                        else
                        {
                            printf("Don't know the payload\n");
                            printf("%s\n", request.DebugString().c_str());
                        }

                        return;
                    }

                    if (nread < 0)
                    {
                        if (nread != UV_EOF)
                        {
                            fprintf(stderr, "read error %s\n", uv_err_name(nread));
                        }

                        uv_close((uv_handle_t*)client, [] (uv_handle_t* handle)
                        {
                            free(handle);
                        });
                    }

                    free(buf->base);
                }
            );
        }
        else
        {
            uv_close((uv_handle_t*)client, [] (uv_handle_t* handle)
            {
                free(handle);
            });
        }
    });

    if (err)
    {
        fprintf(stderr, "Failed to listen %s\n", uv_strerror(err));
    }

    int ret = uv_run(loop, UV_RUN_DEFAULT);
    google::protobuf::ShutdownProtobufLibrary();
    return ret;
}
