#include <iostream>

#include "src/anet.h"
#include "src/gujia.h"
#include "src/gujia_impl.h"

int main() {
    using namespace gujia;

    int el_fd = EventLoop<>::Open();
    assert(el_fd >= 0);
    EventLoop el(el_fd);

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

        Resource(int t) : type(t) {}
    };
    ResourceManager<Resource> manager;

    int fd = anetTcpServer(nullptr, 8000, "0.0.0.0", 0);
    assert(fd >= 0);
    auto resource = std::make_unique<Resource>(0);
    int r = manager.Acquire(fd, std::move(resource));
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
            auto & resource = manager.GetResource(efd);

            if (resource->type == 0) { // acceptor
                char client_ip[128];
                int client_port, client_fd;
                client_fd = anetTcpAccept(nullptr, efd, client_ip, 128, &client_port);
                printf("Accepted %s:%d\n", client_ip, client_port);
                manager.Acquire(client_fd, std::make_unique<Resource>(1));
                el.AddEvent(client_fd, kReadable);
                anetNonBlock(nullptr, client_fd);
            } else if (resource->type == 1) { // processor
                ssize_t nread = read(efd, resource->processor.buf, 1024);
                if (nread > 0) {
                    write(efd, resource->processor.buf, nread);
                }
                el.DelEvent(efd, kReadable | kWritable);
                manager.Release(efd);
            }
        }
    }

    std::cout << "Done." << std::endl;
    return 0;
}