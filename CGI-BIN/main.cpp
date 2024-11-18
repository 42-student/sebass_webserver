/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smargine <smargine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 02:08:58 by smargine          #+#    #+#             */
/*   Updated: 2024/11/18 18:15:15 by smargine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgiHandler.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>

void test_post_request() {
    // Simulate a POST request with form data
    std::string post_request = "name=John&age=30";
    std::vector<std::string> env_vars;
    env_vars.push_back("SCRIPT_FILENAME=/home/smargine/Desktop/webserver_in_progress/CGI-BIN/py-script.py");
    env_vars.push_back("REQUEST_METHOD=POST");

    // Convert size to string manually
    std::ostringstream content_length_stream;
    content_length_stream << post_request.size();
    env_vars.push_back("CONTENT_LENGTH=" + content_length_stream.str());

    size_t body_length = post_request.size();
    std::string upload_to = "/home/smargine/Desktop/webserver_in_progress/CGI-BIN/";

    try {
        CGI cgi(post_request, env_vars, upload_to, body_length);
        std::cout << "POST request test completed successfully." << std::endl;
    } catch (const CGI::CGIException &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void test_chunked_post_request() {
    // Simulate a chunked POST request
    std::string chunked_request =
        "4\r\n"
        "Wiki\r\n"
        "5\r\n"
        "pedia\r\n"
        "E\r\n"
        " in\r\nchunks.\r\n"
        "0\r\n\r\n";

    std::vector<std::string> env_vars;
    env_vars.push_back("SCRIPT_FILENAME=/home/smargine/Desktop/webserver_in_progress/CGI-BIN/py-script.py");
    env_vars.push_back("REQUEST_METHOD=POST");
    env_vars.push_back("HTTP_TRANSFER_ENCODING=chunked");

    // Convert size to string manually (no std::to_string in C++98)
    std::ostringstream content_length_stream;
    content_length_stream << chunked_request.size();
    env_vars.push_back("CONTENT_LENGTH=" + content_length_stream.str());

    size_t body_length = chunked_request.size();
    std::string upload_to = "/home/smargine/Desktop/webserver_in_progress/CGI-BIN/";

    try {
        CGI cgi(chunked_request, env_vars, upload_to, body_length);
        std::cout << "Chunked POST request test completed successfully." << std::endl;
    } catch (const CGI::CGIException &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void test_get_request() {
    // Simulate a GET request (without a body, just query string)
    std::string get_request = "name=John&age=30";
    std::vector<std::string> env_vars;
    env_vars.push_back("SCRIPT_FILENAME=/home/smargine/Desktop/webserver_in_progress/CGI-BIN/py-script.py");
    env_vars.push_back("REQUEST_METHOD=GET");

    // For GET requests, CONTENT_LENGTH is typically not set, but can be added if needed
    env_vars.push_back("CONTENT_LENGTH=0");

    size_t body_length = 0; // GET requests don't have a body
    std::string upload_to = "/home/smargine/Desktop/webserver_in_progress/CGI-BIN/";

    try {
        CGI cgi(get_request, env_vars, upload_to, body_length);
        std::cout << "GET request test completed successfully." << std::endl;
    } catch (const CGI::CGIException &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

int main() {
    // std::cout << "Testing POST request..." << std::endl;
    // test_post_request();

    std::cout << "Testing chunked POST request..." << std::endl;
    test_chunked_post_request();

    // std::cout << "Testing GET request..." << std::endl;
    // test_get_request();

    return 0;
}
