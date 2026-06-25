#pragma once

#include "core.hpp"
#include "layer.hpp"
#include "app.hpp"

#include <thread>
#include <mutex>

namespace bbl
{

class InputLayer : public Layer {
public:
	struct InputEvent {
		vec<string> input;
		static constexpr str name() { return "InputLayer::InputEvent"; }
	};

public:
	InputLayer(App& app);
	virtual ~InputLayer();

	virtual constexpr str name() const { return "InputLayer"; }

	virtual void start();
	virtual void update(time::ns ts);
	virtual void stop();

	virtual void onEvent(EventHandler::EventType& e);


private:
	void handleInput();

private:
	App& m_app;

	std::thread m_thread{ &InputLayer::handleInput, this };
	std::mutex m_mutex;
	std::atomic<bool> m_running{ true };
	std::atomic<bool> m_wait{ true };
	const time::ms m_wait_time{ 1000 };
};

}