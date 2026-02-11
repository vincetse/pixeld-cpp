#include "crow_all.h"
#include "assets.hpp"
#include <string>
#include <cstdlib>

// Define the routes in a testable function
void setup_routes(crow::SimpleApp& app) {
    // Route: /
    CROW_ROUTE(app, "/")([]() {
        return "";
    });

    // Route: /js
    CROW_ROUTE(app, "/js")([](const crow::request&, crow::response& res) {
        res.set_header("Content-Type", "text/javascript");
        res.write("// nop");
        res.end();
    });

    // Route: /html
    CROW_ROUTE(app, "/html")([](const crow::request&, crow::response& res) {
        res.set_header("Content-Type", "text/html");
        res.write("<html></html>");
        res.end();
    });

    // Route: /image
    CROW_ROUTE(app, "/image")([](const crow::request&, crow::response& res) {
        res.set_header("Content-Type", "image/gif");
        // We convert the string_view to a string for Crow's response body
        res.write(std::string(assets::get_gif_sv()));
        res.end();
    });
}

// Only compile main() if we aren't running unit tests
#ifndef UNIT_TEST
int main() {
    crow::SimpleApp app;

    setup_routes(app);

    // Pull PORT from environment (standard for Docker/Cloud) or default to 5000
    const char* port_env = std::getenv("PORT");
    int port = port_env ? std::stoi(port_env) : 5000;

    // Run the server
    app.port(port)
       .multithreaded() // Required for production performance
       .run();

    return 0;
}
#endif
