#include "requestHandlingAndRouting.h"

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;
namespace ssl = boost::asio::ssl;

http::response<http::string_body> handle_request(http::request<http::string_body> const &req) {
    if (req.target() != "/login" && req.target() != "/index") {
        std::string cookie;
        if (req.find(http::field::cookie) != req.end()) {
            cookie = std::string(req[http::field::cookie]);
        }

        std::string token = get_token_from_cookie(cookie);
        if (sessions.find(token) == sessions.end()) {
            http::response<http::string_body> res{http::status::unauthorized, req.version()};
            res.set(http::field::content_type, "text/plain");
            res.body() = "Brak dostepu - niezalogowany.";
            res.prepare_payload();
            return res;
        }
    }

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
        <form method="POST" action="/login">
            <label>Login: <input name="login" /></label><br/>
            <label>Hasło: <input name="password" type="password" /></label><br/>
            <button type="submit">Zaloguj</button>
        </form>
    </main>
  </body>
</html>)";
        res.prepare_payload();
        return res;
    }

    if (req.method() == http::verb::get && req.target() == "/overview") {
        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::server, "Beast");
        res.set(http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());

        res.body() = create_tables();

        res.prepare_payload();
        return res;
    }

    if (req.method() == http::verb::get && req.target() == "/graph") {
        {
        std::ofstream jsonFile("usd_data.json");
        jsonFile << "{\n  \"usd_ranges\": [\n";
        for (int i = 0; i < 20; ++i) {
            double start = 3.5 + 0.05 * i;
            double end = start + 0.049;
            jsonFile << "    \"" << std::fixed << std::setprecision(6) << start
                     << " - " << end << "\"";
            if (i < 19) jsonFile << ",\n";
        }
        jsonFile << "\n  ],\n  \"mean_values\": [\n";
        for (int i = 0; i < 20; ++i) {
            if (usdOcc[i] == 0)
                jsonFile << "    null";
            else
                jsonFile << "    " << std::fixed << std::setprecision(6) << usdVal[i] / usdOcc[i];
            if (i < 19) jsonFile << ",\n";
        }
        jsonFile << "\n  ]\n}";
    } {
            std::ofstream jsonFile("eur_data.json");
            jsonFile << "{\n  \"eur_ranges\": [\n";
            for (int i = 0; i < 10; ++i) {
                double start = 3.0 + 0.1 * i;
                double end = start + 0.09;
                jsonFile << "    \"" << std::fixed << std::setprecision(6) << start
                        << " - " << end << "\"";
                if (i < 9) jsonFile << ",\n";
            }
            jsonFile << "\n  ],\n  \"mean_values\": [\n";
            for (int i = 0; i < 10; ++i) {
                double mean = eurOcc[i] == 0 ? std::numeric_limits<double>::quiet_NaN() : eurVal[i] / eurOcc[i];
                if (std::isnan(mean) || std::isinf(mean)) {
                    jsonFile << "    null";
                } else {
                    jsonFile << "    " << std::fixed << std::setprecision(6) << mean;
                }
                if (i < 9) jsonFile << ",\n";
            }
            jsonFile << "\n  ]\n}";
            jsonFile.close();
        }

    int result = std::system("python3 wykresy.py usd_data.json eur_data.json");

    if (result != 0) {
        http::response<http::string_body> res{http::status::internal_server_error, req.version()};
        res.set(http::field::content_type, "text/plain");
        res.body() = "Błąd przy generowaniu wykresów.";
        res.prepare_payload();
        return res;
    }

    std::ifstream htmlFile("all_plot.html");
    std::stringstream buffer;
    buffer << htmlFile.rdbuf();

    http::response<http::string_body> res{http::status::ok, req.version()};
    res.set(http::field::server, "Beast");
    res.set(http::field::content_type, "text/html");
    res.keep_alive(req.keep_alive());
    res.body() = buffer.str();
    res.prepare_payload();
    return res;
    }

    if (req.method() == http::verb::post && req.target() == "/login") {
        std::string body = req.body();
        std::string username, password;

        size_t pos1 = body.find("login=");
        size_t pos2 = body.find("&password=");
        if (pos1 != std::string::npos && pos2 != std::string::npos) {
            username = body.substr(pos1 + 6, pos2 - (pos1 + 6));
            password = body.substr(pos2 + 10);
        }

        if (username == "admin" && password == "haslo123") {
            std::string token = generate_token();
            sessions[token] = username;

            http::response<http::string_body> res{http::status::see_other, req.version()};
            res.set(http::field::location, "/main");
            res.set(http::field::set_cookie, "token=" + token + "; HttpOnly; Path=/");
            res.prepare_payload();
            return res;
        } else {
            http::response<http::string_body> res{http::status::unauthorized, req.version()};
            res.set(http::field::content_type, "text/plain");
            res.body() = "Błędne dane logowania";
            res.prepare_payload();
            return res;
        }
    }

    if (req.method() == http::verb::get && req.target() == "/main") {
        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.set(http::field::server, "Beast");
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
        <a href="dane">JSON</a>
    </main>
  </body>
</html>)";
        res.prepare_payload();
        return res;
    }

    return http::response<http::string_body>{http::status::bad_request, req.version()};
}