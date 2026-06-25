#include "inputLayer.hpp"

#include <iostream>

namespace bbl
{

namespace utils
{

vec<string> splitCommandString(str str) {
    vec<string> tokens;
    if (str.empty()) return tokens;

    const chr* ptr = str.data();
    size offset = 0;
    while (*ptr) {
        size substr_size = 0;
        while (*ptr == ' ') {
            ptr++;
            offset++;
        }

        substr_size = 0;
        if (*ptr == '"') {
            ptr++;
            offset++;
            while (*ptr && *ptr != '"') {
                ptr++;
                substr_size++;
            }
        }
        else {
            while (*ptr && *ptr != ' ') {
                ptr++;
                substr_size++;
            }
        }

        if (substr_size) tokens.emplace_back(str.substr(offset, substr_size));
        offset += substr_size;
    }

    return tokens;
}

}

InputLayer::InputLayer(App& app) :m_app(app) {
}

InputLayer::~InputLayer() {
}

void InputLayer::start() {
    m_wait.store(true);
}

void InputLayer::update(time::ns ts) {
}

void InputLayer::stop() {
    m_running.store(false);
    m_thread.join();
}

void InputLayer::onEvent(EventHandler::EventType& e) {
    EventHandler::handle_if<App::CloseEvent>(e, [&](App::CloseEvent& ev) { m_running.store(false); });
}

void InputLayer::handleInput() {
    while (m_running.load()) {
        while (!m_wait.load()) {
            std::this_thread::sleep_for(m_wait_time);
        }

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        string input;
        std::getline(std::cin, input);

        std::lock_guard<std::mutex> lock(m_mutex);
        InputEvent e{ std::move(utils::splitCommandString(input)) };
        m_app.evHandler().dispatch(e);
    }
}

}