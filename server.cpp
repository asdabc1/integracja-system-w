#include "server.h"

void Session::do_read() {
    auto self(shared_from_this());
    http::async_read(socket_, buffer_, req_, [this, self](beast::error_code ec, std::size_t) {
        if (!ec) {
            do_write(handle_request(req_));
        }
    });
}

void Session::do_write(http::response<http::string_body> res) {
    auto self(shared_from_this());
    auto sp = std::make_shared<http::response<http::string_body> >(std::move(res));
    http::async_write(socket_, *sp, [this, self, sp](beast::error_code ec, std::size_t) {
        socket_.shutdown(tcp::socket::shutdown_send, ec);
    });
}

Listener::Listener(net::io_context &ioc, tcp::endpoint endpoint)
        : ioc_(ioc), acceptor_(net::make_strand(ioc)) {
    beast::error_code ec;

    acceptor_.open(endpoint.protocol(), ec);
    if (ec) {
        std::cerr << "Open error: " << ec.message() << std::endl;
        return;
    }

    acceptor_.set_option(net::socket_base::reuse_address(true), ec);
    if (ec) {
        std::cerr << "Set option error: " << ec.message() << std::endl;
        return;
    }

    acceptor_.bind(endpoint, ec);
    if (ec) {
        std::cerr << "Bind error: " << ec.message() << std::endl;
        return;
    }

    acceptor_.listen(net::socket_base::max_listen_connections, ec);
    if (ec) {
        std::cerr << "Listen error: " << ec.message() << std::endl;
        return;
    }

    do_accept();
}

void Listener::do_accept() {
    acceptor_.async_accept(net::make_strand(ioc_), [this](beast::error_code ec, tcp::socket socket) {
        if (!ec) {
            std::make_shared<Session>(std::move(socket))->run();
        }
        do_accept();
    });
}