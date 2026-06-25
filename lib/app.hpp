#pragma once

#include "core.hpp"
#include "layer.hpp"

namespace bbl {
class App {
public:
    struct CloseEvent {
        string cause;
        i32 code;
        constexpr static str name() { return "App::CloseEvent"; }
    };

public:
    App();
    ~App();

    void run();

    void pushLayer(box<Layer>&& layer);
    void onEvent(EventHandler::EventType& ev);

    EventHandler& evHandler();

private:
    void onClose(CloseEvent& ev);

private:
    bool m_running = false;
    EventHandler m_handler;
    vec<box<Layer>> m_layers;
};
}