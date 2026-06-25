#pragma once

#include "core.hpp"

#include <functional>

namespace bbl
{

template<typename T>
concept Event = requires(T t) {
	{ T::name() } -> std::same_as<str>;
};

class EventHandler {
public:
	struct EventType {
		void* ptr;
		size id;
	};
	using mainFn = std::function<void(EventType&)>;

public:
	EventHandler(mainFn fn) :m_fn(fn) {
	}

	template<Event E>
	void dispatch(E& e) {
		EventType ev{ reinterpret_cast<void*>(&e), utils::fnv1a(E::name()) };
		m_fn(ev);
	}

	template<Event E, typename F>
	static void handle_if(EventType& e, F fn, auto&&... args) {
		if (e.id == utils::fnv1a(E::name())) {
			std::invoke(fn, std::forward<decltype(args)>(args)..., *static_cast<E*>(e.ptr));
		}
	}

private:
	mainFn m_fn;
};

}