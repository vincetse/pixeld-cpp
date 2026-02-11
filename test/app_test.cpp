#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../src/main.cpp"
#include "../src/assets.hpp"

// Helper to simulate a request and return the response
crow::response request_to(crow::SimpleApp& app, const std::string& url) {
    crow::request req;
    req.url = url;
    req.method = crow::HTTPMethod::Get; // Explicitly set method
    crow::response res;

    // Use the App's internal handle_full method if handle() is segfaulting
    // This ensures middlewares and full routing logic are engaged
    app.handle_full(req, res);

    return res;
}

TEST_CASE("pixeld++ Server Routes", "[api]") {
    crow::SimpleApp app;
    setup_routes(app);
    app.validate();

    SECTION("GET / (Root)") {
        auto res = request_to(app, "/");
        REQUIRE(res.code == 200);
        REQUIRE(res.body == "");
    }

    SECTION("GET /js (JavaScript)") {
        auto res = request_to(app, "/js");
        REQUIRE(res.code == 200);
        REQUIRE(res.get_header_value("Content-Type") == "text/javascript");
        REQUIRE(res.body == "// nop");
    }

    SECTION("GET /html (HTML)") {
        auto res = request_to(app, "/html");
        REQUIRE(res.code == 200);
        REQUIRE(res.get_header_value("Content-Type") == "text/html");
        REQUIRE(res.body == "<html></html>");
    }

    SECTION("GET /image (Embedded GIF)") {
        auto res = request_to(app, "/image");
        REQUIRE(res.code == 200);
        REQUIRE(res.get_header_value("Content-Type") == "image/gif");

        // Verify size and contents match assets.hpp
        REQUIRE(res.body.size() == assets::GIF_1x1.size());
        std::string expected_data(assets::get_gif_sv());
        REQUIRE(res.body == expected_data);
    }
}
