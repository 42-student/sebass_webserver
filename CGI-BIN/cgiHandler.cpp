/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgiHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smargine <smargine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 20:27:20 by smargine          #+#    #+#             */
/*   Updated: 2024/11/12 00:36:02 by smargine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgiHandler.hpp"

CGI::CGI() {}

CGI::CGI(const std::string& request, const std::vector<std::string>& env_var, const std::string& upload_to, size_t body_len)
		: _request(request), _env_var(env_var), _upload_to(upload_to), _body_len(body_len)
{
	_cgi_path = getScriptFileName();
	createAvAndEnv();
	runScript();
}

CGI::~CGI()
{
	//remove(".cgi_output");
	
	if (_av)
	{
		for (size_t i = 0; _av[i]; ++i)
		{
			free(_av[i]);
		}
		delete[] _av;
	}

	if (_env)
	{
		for (size_t i = 0; _env[i]; ++i)
		{
			free(_env[i]);
		}
		delete[] _env;
	}
}

std::string	CGI::getScriptFileName()
{
	for (size_t i = 0; i < _env_var.size(); ++i)
	{
		if (_env_var[i].find("SCRIPT_FILENAME=") == 0)
		{
			return (_env_var[i].substr(16));
		}
	}
	throw CGIException("SCRIPT_FILENAME not provided.");
}

void	CGI::createAvAndEnv()
{
	if (_cgi_path.substr(_cgi_path.find_last_of('.')) != ".py")
	{
		throw CGIException("Invalid script extension.");
	}

	_av = new char*[4];
	_av[0] = strdup("/usr/bin/python3");
	_av[1] = strdup(_cgi_path.c_str());
	_av[2] = strdup(_upload_to.c_str());
	_av[3] = NULL;

	_env = new char*[_env_var.size() + 1];
	for (size_t i = 0; i < _env_var.size(); ++i)
	{
		_env[i] = strdup(_env_var[i].c_str());
	}
	_env[_env_var.size()] = NULL;
}

std::string	CGI::checkRequestMethod(const std::string& str)
{
	for (size_t i = 0; i < _env_var.size(); ++i)
	{
		if (_env_var[i].find(str + "=") == 0)
		{
			return (_env_var[i].substr(str.size() + 1));
		}
	}
	return ("");
}

bool	CGI::isHex(const std::string& str)
{
	for (size_t i = 0; i < str.size(); ++i)
	{
		char c = str[i];
		
		if (!(c >= '0' && c <= '9') && (c >= 'A' && c <= 'F') && (c >= 'a' && c <= 'f'))
		{
			return (false);
		}
	}
	
	return (true);
}

int		CGI::hexToInt(const std::string& str)
{
	if (!isHex(str))
	{
		throw std::invalid_argument("Error: invalid hexadecimal string.");
	}
	
	std::stringstream ss;
	int int_val;
	ss << std::hex << str;
	ss >> int_val;

	return (int_val);
}

bool	CGI::isChunked() const
{
	for (size_t i = 0; i < _env_var.size(); ++i)
	{
		if (_env_var[i].find("HTTP_TRANSFER_ENCODING=chunked") == 0)
		{
			return (true);
		}
	}
	
	return (false);
}

bool	CGI::unchunkRequest(const std::string& chunked, std::string& unchunked)
{
	std::istringstream iss(chunked);
	std::string line;
	int chunk_size;

	while(std::getline(iss, line))
	{
		bool is_empty = true;
		
		for (size_t i = 0; i < line.size(); ++i)
		{
			if (!isspace(line[i]))
			{
				is_empty = false;
				break ;
			}
		}
		
		if (is_empty)
		{
			continue ;
		}
		
		if (!isHex(line))
		{
			std::cerr << "Error: invalid hexadecimal string." << std::endl;
			return (false);
		}

		chunk_size = hexToInt(line);
		
		if (chunk_size == -1)
		{
			std::cerr << "Error: unchunkedRequest failed." << std::endl;
			return (false);
		}
		
		if (chunk_size == 0)
		{
			break ;
		}

		std::vector<char> buffer(chunk_size);
		iss.read(buffer.data(), chunk_size);
		unchunked.append(buffer.data(), chunk_size);

		std::cout << unchunked << std::endl; // for testing only!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	
		std::getline(iss, line);
	}
	
	return (!unchunked.empty());
}

void	CGI::runScript()
{
	pid_t pid;
	
	int out_fd = open(".cgi_output", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (out_fd == -1)
	{
		throw CGIException("open() failed.");
	}

	int pipedes[2];
	if (pipe(pipedes) == -1)
	{
		close(out_fd);
		throw CGIException("pipe() failed");
	}

	std::string request_method = checkRequestMethod("REQUEST_METHOD");
	std::string unchunk_body;

	if (request_method == "POST" && _body_len > 0)
	{
		if (isChunked())
		{
			if (!unchunkRequest(_request, unchunk_body))
			{
				close(pipedes[0]);
				close(pipedes[1]);
				close(out_fd);
				throw CGIException("unchunkRequest failed.");
			}
		}
		else
		{
			unchunk_body = _request;
		}
		
		if(fcntl(pipedes[1], F_SETPIPE_SZ, unchunk_body.size()) == -1)
		{
			close(pipedes[0]);
			close(pipedes[1]);
			close(out_fd);
			throw CGIException("fcntl() F_SETPIPE_SZ failed.");
		}
		write(pipedes[1], unchunk_body.data(), unchunk_body.size());
	}
	close(pipedes[1]);

	pid = fork();
	if (pid == -1)
	{
		close(pipedes[0]);
		close(out_fd);
		throw CGIException("fork() failed.");
	}
	
	if (pid == 0)
	{
		if (request_method == "POST")
		{
			dup2(pipedes[0], STDIN_FILENO);
		}
		close(pipedes[0]);
		
		dup2(out_fd, STDOUT_FILENO);
		close(out_fd);

		if (execve(_av[0], _av, _env) == -1)
		{
			std::cerr << "Erorr: execve() failed." << std::endl;
			exit(1);
		}
	}
	else
	{
		close(pipedes[0]);
		close(out_fd);
		waitpid(pid, NULL, 0);
	}
}
