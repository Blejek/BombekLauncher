#include "BombekLauncher.hpp"

#include <filesystem>

using namespace bbl;

class MainLayer : public Layer {
public:
    MainLayer(App& app);
    virtual ~MainLayer();

    virtual constexpr str name() const { return "MainLayer"; }

    virtual void start();
    virtual void update(time::ns ts);
    virtual void stop();

    virtual void onEvent(EventHandler::EventType& e);

private:
    void handleCommands(InputLayer::InputEvent& ev);

private:
    App& m_app;
};

MainLayer::MainLayer(App& app) :m_app(app) {
}

MainLayer::~MainLayer() {
}

void MainLayer::start() {
}

void MainLayer::update(time::ns ts) {
}

void MainLayer::stop() {
}

void MainLayer::onEvent(EventHandler::EventType& e) {
    EventHandler::handle_if<InputLayer::InputEvent>(e, &MainLayer::handleCommands, this);
}

void MainLayer::handleCommands(InputLayer::InputEvent& ev) {
    if (ev.input.size() > 0) {
        if (ev.input[0] == "close") {
            App::CloseEvent close{"app closed", 0};
            m_app.evHandler().dispatch(close);
        }
    }
}






int main(int argc, char* argv[]) {
    
    // TODO: Check for BombekLauncher folder existance
    // if yes, store data here
    // else store data in some %appdata%

    std::filesystem::path launcherDir{ "./BombekLauncher" };

    auto configPath = launcherDir / "config.json";
    auto instancesPath = launcherDir / "instances.json";

    auto logPath = launcherDir / "log.txt";

    Logger::init(logPath.string());

    App app;
    app.pushLayer(newBox<InputLayer>(app));
    app.pushLayer(newBox<MainLayer>(app));
    app.run();

    return 0;
}

