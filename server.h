#ifndef SERVER_H
#define SERVER_H

#include "includes.h"
#include "requestHandlingAndRouting.h"

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;
namespace ssl = boost::asio::ssl;

class Session : public std::enable_shared_from_this<Session> {
private:
    tcp::socket socket_;
    beast::flat_buffer buffer_;
    http::request<http::string_body> req_;

    void do_read();
    void do_write(http::response<http::string_body> res);

public:
    explicit Session(tcp::socket socket) : socket_(std::move(socket)) {}
    void run() {do_read();}
};

class Listener : public std::enable_shared_from_this<Listener> {
private:
    net::io_context &ioc_;
    tcp::acceptor acceptor_;

    void do_accept();

public:
    Listener(net::io_context& ioc, tcp::endpoint endpoint);
};

#endif //SERVER_H
