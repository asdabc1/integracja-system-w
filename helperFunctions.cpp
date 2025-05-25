#include "helperFunctions.h"

std::string extract_query_param(const std::string& target, const std::string& param_name) {
    size_t query_start = target.find('?');
    if (query_start == std::string::npos) {
        return "";
    }

    std::string query = target.substr(query_start + 1);
    size_t param_start = query.find(param_name + "=");
    if (param_start == std::string::npos) {
        return "";
    }

    size_t value_start = param_start + param_name.size() + 1;
    size_t value_end = query.find('&', value_start);
    if (value_end == std::string::npos) {
        return query.substr(value_start);
    } else {
        return query.substr(value_start, value_end - value_start);
    }
}

void removeObjectById(boost::json::value& data, const std::string& targetId) {
    if (data.is_object() && data.as_object().contains("data")) {
        boost::json::array& dataArray = data.at("data").as_array();
        auto it = std::remove_if(dataArray.begin(), dataArray.end(),
            [&targetId](const boost::json::value& item) {
                return item.is_object() &&
                       item.as_object().contains("id") &&
                       item.at("id").as_string() == targetId;
            });
        dataArray.erase(it, dataArray.end());
    }
}

std::pair<double, double> fetchDollarAndEuro(std::string date) {
    boost::asio::io_context ioc;

    ssl::context ctx(ssl::context::sslv23_client);
    tcp::resolver resolver(ioc);
    boost::asio::ssl::stream<beast::tcp_stream> stream(ioc, ctx);

    if (!SSL_set_tlsext_host_name(stream.native_handle(), "api.nbp.pl")) {
        throw beast::system_error(
            beast::error_code(static_cast<int>(::ERR_get_error()), net::error::get_ssl_category()));
    }

    auto const results = resolver.resolve("api.nbp.pl", "443");

    beast::get_lowest_layer(stream).connect(results);

    stream.handshake(ssl::stream_base::client);

    std::string target = "/api/exchangerates/rates/A/USD/" + date + "/";

    http::request<http::string_body> req{http::verb::get, target , 11};
    req.set(http::field::host, "api.nbp.pl");
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    req.set(http::field::accept, "application/json");

    http::write(stream, req);

    beast::flat_buffer buffer;
    http::response<http::string_body> res;
    http::read(stream, buffer, res);

    if (res.result() != http::status::ok) {
        return {-1, -1};
    }

    boost::json::value dollar = boost::json::parse(res.body());

    target = "/api/exchangerates/rates/A/EUR/" + date + "/";

    req = http::request<http::string_body>(http::verb::get, target, 11);
    req.set(http::field::host, "api.nbp.pl");
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    req.set(http::field::accept, "application/json");

    http::write(stream, req);

    buffer = beast::flat_buffer();
    res = http::response<http::string_body>();
    http::read(stream, buffer, res);

    boost::json::value euro = boost::json::parse(res.body());

    beast::error_code ec;
    stream.shutdown(ec);
    if (ec == net::error::eof) {
        ec = {};
    }
    if (ec) {
        throw beast::system_error{ec};
    }

    double usd = dollar.as_object()["rates"].as_array()[0].as_object()["mid"].as_double();
    double eur = euro.as_object()["rates"].as_array()[0].as_object()["mid"].as_double();

    return {usd, eur};
}

std::string create_tables() {
    std::string result = R"(<!DOCTYPE html>
<html lang="pl">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0"/>
    <meta http-equiv="X-UA-Compatible" content="ie=edge"/>
    <title>Integracja systemów</title>
    <style>
      table {
        border-collapse: collapse;
        width: 50%;
        margin-bottom: 40px;
      }
      th, td {
        border: 1px solid #999;
        padding: 8px 12px;
        text-align: center;
      }
      h2 {
        margin-top: 40px;
      }
    </style>
  </head>
  <body>
    <main>
      <h1>Integracja systemów</h1>

      <h2>Tabela EUR</h2>
      <table>
        <tr>
          <th>EUR range</th>
          <th>mean value</th>
        </tr>
)";

    std::string element;

    double temp = 4.8;
    for (int i = 9; i >= 0; i--) {
        element = "<tr><td>" + std::to_string(temp) + " - " + std::to_string(temp + 0.09) + "</td><td>" + (std::to_string(usdVal[i]) == "-nan(ind)" ? "brak danych" : std::to_string(usdVal[i])) + "</td></tr>";
        result += element;
        temp -= 0.1;
    }

    result += R"(      </table>

      <h2>Tabela USD</h2>
      <table>
        <tr>
          <th>USD range</th>
          <th>mean value</th>
        </tr>)";

    temp = 4.9;
    for (int i = 19; i >= 0; i--) {
        element = "<tr><td>" + std::to_string(temp) + " - " + std::to_string(temp + 0.09) + "</td><td>" + (std::to_string(usdVal[i]) == "-nan(ind)" ? "brak danych" : std::to_string(usdVal[i])) + "</td></tr>";
        result += element;
        temp -= 0.1;
    }

    result += R"(      </table>
    </main>
  </body>
</html>)";

    return result;
}
