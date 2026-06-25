#include "app.hpp"
#include "log.hpp"

namespace bbl {

using AppLog = LoggerBase<"App">;

App::App() 
    :m_handler([&](EventHandler::EventType& ev) { onEvent(ev); }) {
    AppLog::info("App constructed");
}

App::~App() {
}

void App::run() {
    time::Timer t{};

    AppLog::info("App is running");

    for (const auto& layer : m_layers) {
        layer->start();
    }

    m_running = true;

    while (m_running) {
        for (const auto& layer : m_layers) {
            layer->update(t.ts<time::ns>());
        }
        t.tick();
    }

    for (const auto& layer : m_layers) {
        layer->stop();
    }
}

void App::pushLayer(box<Layer>&& layer) {
    if(m_running)
        layer->start();
    m_layers.push_back(std::move(layer));
}

void App::onEvent(EventHandler::EventType& ev) {
    EventHandler::handle_if<CloseEvent>(ev, &App::onClose, this);

    for (const auto& layer : m_layers) {
        layer->onEvent(ev);
    }
}

EventHandler& App::evHandler() {
    return m_handler;
}

void App::onClose(CloseEvent& ev) {
    m_running = false;
    AppLog::info("App closed. \nCode: {}\n{}", ev.code, ev.cause);

}


}