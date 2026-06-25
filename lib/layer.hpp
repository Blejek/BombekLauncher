#pragma once

#include "core.hpp"
#include "time.hpp"
#include "event.hpp"

namespace bbl
{

class Layer {
public:
	virtual ~Layer() = default;

	virtual constexpr str name() const = 0;

	virtual void start() = 0;
	virtual void update(time::ns ts) = 0;
	virtual void stop() = 0;

	virtual void onEvent(EventHandler::EventType& e) = 0;
};

}