/*
 * Copyright (C) 2013  Maxim Noah Khailo
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QApplication>

#include "gui/mainwin.hpp"
#include "network/util.hpp"

#include <string>
#include <cstdlib>

#include <boost/asio/ip/host_name.hpp>
#include <boost/program_options.hpp>

namespace po = boost::program_options;
namespace ip = boost::asio::ip;
namespace fg = fire::gui;
namespace fn = fire::network;
namespace fu = fire::util;

po::options_description create_descriptions()
{
    po::options_description d{"Options"};

    std::string user = std::getenv("HOME");
    if(user.empty()) user = ".";
    const std::string home = user + "/.firestr";

    std::string host = ip::host_name();

    std::string ip = fn::get_lan_ip();
    if(!ip.empty()) host = ip;

    const std::string port = "6060";

    d.add_options()
        ("help", "prints help")
        ("home", po::value<std::string>()->default_value(home), "configuration directory")
        ("host", po::value<std::string>()->default_value(host), "host/ip of this machine") 
        ("port", po::value<std::string>()->default_value(port), "port this machine will receive messages on");

    return d;
}

po::variables_map parse_options(int argc, char* argv[], po::options_description& desc)
{
    po::variables_map v;
    po::store(po::parse_command_line(argc, argv, desc), v);
    po::notify(v);

    return v;
}

bool user_setup(const std::string& home)
{
    auto user = fg::setup_user(home);
    return user != nullptr;
}

int main(int argc, char *argv[])
{
    auto desc = create_descriptions();
    auto vm = parse_options(argc, argv, desc);
    if(vm.count("help"))
    {
        std::cout << desc << std::endl;
        return 1;
    }

    QApplication a{argc, argv};

    fg::main_window_context c;
        
    c.home = vm["home"].as<std::string>();
    c.host = vm["host"].as<std::string>();
    c.port = vm["port"].as<std::string>();

    if(!user_setup(c.home)) return 0;

    fg::main_window w{c};

    w.show();

    return a.exec();
}
