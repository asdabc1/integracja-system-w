#ifndef REQUESTHANDLINGANDROUTING_H
#define REQUESTHANDLINGANDROUTING_H

#include "includes.h"
#include "helperFunctions.h"

extern boost::json::value data;

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;
namespace ssl = boost::asio::ssl;

http::response<http::string_body> handle_request(http::request<http::string_body> const &req);

#endif //REQUESTHANDLINGANDROUTING_H
