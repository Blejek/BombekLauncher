#pragma once

#include "core.hpp"

#include <chrono>

namespace bbl {
namespace time
{

using clock = std::chrono::high_resolution_clock;

template<class rep, class period = std::ratio<1>>
using duration = std::chrono::duration<rep, period>;

template< class to, class rep, class period >
constexpr to durationCast(const std::chrono::duration<rep, period>& d) {
    return std::chrono::duration_cast<to>(d);
}

using ns = duration<clock::rep, std::nano>;
using us = duration<clock::rep, std::micro>;
using ms = duration<clock::rep, std::milli>;
using s = duration<clock::rep>;
using min= duration<clock::rep, std::ratio<60>>;
using hour = duration<clock::rep, std::ratio<3600>>;
using day = duration<clock::rep, std::ratio<86400>>;
using week = duration<clock::rep, std::ratio<604800>>;
using month = duration<clock::rep, std::ratio<2629746>>;
using year = duration<clock::rep, std::ratio<31556952>>;

template<typename T>
concept durationType = requires {
    duration<typename T::rep, typename T::period>{};
};

class Timer {
public:
    using clock = clock;
    using timePoint = clock::time_point;
    using defaultDuration = duration<i64, clock::period>;

public:
    Timer():m_start(clock::now()), m_prev(m_start), m_next(m_start) {}
    constexpr Timer(const Timer&) = default;
    constexpr Timer(Timer&&) = default;
    constexpr ~Timer() = default;

    constexpr Timer& operator=(const Timer&) = default;
    constexpr Timer& operator=(Timer&&) = default;

    template<durationType D>
    D ts() const {
        return durationCast<D>(m_next - m_prev);
    }

    void tick() {
        m_prev = m_next;
        m_next = clock::now();
    }

    template<durationType D>
    D getTick() const {
        return durationCast<D>(clock::now() - m_next);
    }

    template<durationType D>
    D get() const {
        return durationCast<D>(clock::now() - m_start);
    }

    operator defaultDuration () const {
        return get<defaultDuration>();
    }

private:
    timePoint m_start;
    timePoint m_prev;
    timePoint m_next;
};

}
}