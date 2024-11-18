/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgiHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smargine <smargine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 21:49:05 by smargine          #+#    #+#             */
/*   Updated: 2024/10/25 16:40:55 by smargine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIHANDLER_HPP
# define CGIHANDLER_HPP

# include <iostream>
# include <sstream>
# include <string>
# include <vector>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <cstdlib>
# include <cstring>
# include <cstdio>

# define CGI_OUT_FILE ".cgi_output"
# define PY_BIN "/usr/bin/python3"
# define PY_EXT ".py"
# define READ_END 0
# define WRITE_END 1

class CGI
{
	private:
			std::string _cgi_path;
			std::string _request;
			std::vector<std::string> _env_var;
			std::string _upload_to;
			size_t _body_len;
			char** _av;
			char** _env;

			CGI(const CGI& copy);
			CGI&	operator=(const CGI& other);

			std::string	getScriptFileName();
			void	createAvAndEnv();
			std::string	checkRequestMethod(const std::string& str);
			bool	isHex(const std::string& str);
			int		hexToInt(const std::string& str);
			bool	isChunked() const;
			bool	unchunkRequest(const std::string& chunked, std::string& unchunked);
			void	runScript();

	public:
			CGI();
			CGI(const std::string& request, const std::vector<std::string>& env_var, const std::string& upload_to, size_t body_len);
			~CGI();

			class CGIException : public std::runtime_error
			{
				public:
						explicit CGIException(const std::string& msg) : std::runtime_error(msg) {}
			};
};

#endif
