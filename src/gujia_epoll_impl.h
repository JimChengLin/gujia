#pragma once
#ifndef GUJIA_GUJIA_EPOLL_IMPL_H
#define GUJIA_GUJIA_EPOLL_IMPL_H

#include "gujia.h"

#if defined(GUJIA_HAS_EPOLL)
namespace gujia {
    template<size_t SIZE>
    int EventLoop<SIZE>::
    AddEvent(int fd, int mask) {
    }

    template<size_t SIZE>
    int EventLoop<SIZE>::
    DelEvent(int fd, int mask) {
    }

    template<size_t SIZE>
    int EventLoop<SIZE>::
    Poll(const struct timeval * tv) {
    }

    template<size_t SIZE>
    int EventLoop<SIZE>::
    GetEventFD(const Event & e) {
    }

    template<size_t SIZE>
    bool EventLoop<SIZE>::
    IsReadableEvent(const Event & e) {
    }

    template<size_t SIZE>
    bool EventLoop<SIZE>::
    IsWritableEvent(const Event & e) {
    }

    template<size_t SIZE>
    int EventLoop<SIZE>::
    Open() {
    }
}
#endif

#endif //GUJIA_GUJIA_EPOLL_IMPL_H