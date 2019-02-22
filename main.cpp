#include <iostream>

#include "src/anet.h"
#include "src/gujia.h"
#include "src/gujia_impl.h"

int main() {
    using namespace gujia;

    struct Acceptor {
    };
    struct Processor {
        char buf[1024];
    };
    struct Resource {
        int type;
        union {
            Acceptor acceptor;
            Processor processor;
        };

        explicit Resource(int t) : type(t) {}
    };

    int el_fd = EventLoop<>::Open();
    assert(el_fd >= 0);
    EventLoop<Resource> el(el_fd);

    int fd = anetTcpServer(nullptr, 8000, "0.0.0.0", 0);
    assert(fd >= 0);
    int r = el.Acquire(fd, std::make_unique<Resource>(0));
    assert(r == 0);
    r = el.AddEvent(fd, kReadable);
    assert(r == 0);

    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;

    while (true) {
        r = el.Poll(&tv);
        if (r < 0) {
            break;
        }

        const auto & events = el.GetEvents();
        for (int i = 0; i < r; ++i) {
            const auto & event = events[i];
            int efd = EventLoop<>::GetEventFD(event);
            auto & resource = el.GetResource(efd);

            if (resource->type == 0) { // acceptor
                char client_ip[128];
                int client_port, client_fd;
                client_fd = anetTcpAccept(nullptr, efd, client_ip, 128, &client_port);
                assert(client_fd >= 0);
                printf("Accepted %s:%d\n", client_ip, client_port);
                anetNonBlock(nullptr, client_fd);
                el.Acquire(client_fd, std::make_unique<Resource>(1));
                el.AddEvent(client_fd, kReadable);
            } else if (resource->type == 1) { // processor
                ssize_t nread = read(efd, resource->processor.buf, 1024);
                if (nread > 0) {
                    write(efd, resource->processor.buf, static_cast<size_t>(nread));
                }
                el.Release(efd);
            }
        }
    }

    std::cout << "Done." << std::endl;
    return 0;
}