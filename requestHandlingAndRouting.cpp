#include "requestHandlingAndRouting.h"

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;
namespace ssl = boost::asio::ssl;

http::response<http::string_body> handle_request(http::request<http::string_body> const &req) {
    // Endpoint /dane (bez zmian)
    if (req.method() == http::verb::get && req.target().starts_with("/dane")) {
        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::server, "Beast");
        res.set(http::field::content_type, "application/json");
        res.keep_alive(req.keep_alive());
        res.body() = boost::json::serialize(data);
        res.prepare_payload();
        return res;
    }

    if (req.method() == http::verb::delete_ && req.target().starts_with("/usun")) {
        std::string id_to_remove = extract_query_param(req.target(), "id");

        if (id_to_remove.empty()) {
            http::response<http::string_body> res{http::status::bad_request, req.version()};
            res.set(http::field::content_type, "text/plain");
            res.body() = "Brak parametru 'id' w URL";
            res.prepare_payload();
            return res;
        }

        removeObjectById(data, id_to_remove);

        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::server, "Beast");
        res.set(http::field::content_type, "text/plain");
        res.keep_alive(req.keep_alive());
        res.body() = "Usunieto obiekt o ID: " + id_to_remove;
        res.prepare_payload();
        return res;
    }

    if (req.method() == http::verb::get && req.target() == "/test") {
        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::server, "Beast");
        res.set(http::field::content_type, "text/plain");
        res.keep_alive(req.keep_alive());
        res.body() = "testowa odpowiedz";
        res.prepare_payload();
        return res;
    }

    return http::response<http::string_body>{http::status::bad_request, req.version()};
}