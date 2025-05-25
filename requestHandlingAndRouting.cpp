#include "requestHandlingAndRouting.h"

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;
namespace ssl = boost::asio::ssl;

http::response<http::string_body> handle_request(http::request<http::string_body> const &req) {
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

    if (req.method() == http::verb::get && req.target() == "/index") {
        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::server, "Beast");
        res.set(http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());

        res.body() = R"(<!DOCTYPE html>
<html lang="pl">
  <head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <title>Integracja systemów</title>
  </head>
  <body>
    <main>
        <h1>Integracja systemów</h1>
        <a href="overview">overview</a>
        <a href="graph">graph</a>
    </main>
  </body>
</html>)";
        res.prepare_payload();
        return res;
    }

    return http::response<http::string_body>{http::status::bad_request, req.version()};
}