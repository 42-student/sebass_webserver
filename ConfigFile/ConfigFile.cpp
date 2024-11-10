/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smargine <smargine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 21:06:58 by smargine          #+#    #+#             */
/*   Updated: 2024/11/10 13:47:23 by smargine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigFile.hpp"

ConfigFile::ConfigFile() {}

ConfigFile::~ConfigFile() {}

void	ConfigFile::error_exit(const std::string& message)
{
	std::cerr << "Error: " << message << std::endl;
	exit(EXIT_FAILURE);
}

std::string	ConfigFile::trimString(const std::string& str)
{
	size_t start = str.find_first_not_of(WHITESPACE);
	if (start == std::string::npos)
	{
		return ("");
	}
	size_t end = str.find_last_not_of(WHITESPACE);
	
	return (str.substr(start, end - start + 1));
}

std::vector<std::string>	ConfigFile::splitLine(const std::string& line)
{
	std::vector<std::string> tokens;
	std::istringstream	iss(line);
	std::string	token;
	
	while (iss >> token)
	{
		tokens.push_back(token);
	}

	return (tokens);
}

bool	ConfigFile::isSkip(const std::string& token)
{
	std::string arr[] = {"server", "location", "{", "}", ";"};
	std::set<std::string> skip_tokens(arr, arr + 5);
	
	if (skip_tokens.find(token) != skip_tokens.end())
	{
		return (true);
	}
	return (false);
}

bool	ConfigFile::isValid(const std::string& token)
{
	std::string arr[] = {"listen", "server_name", "root", "index", "error_page", "client_max_body_size", "allow_methods", "autoindex"};
	std::set<std::string> valid_tokens(arr, arr + 8); 
	if (valid_tokens.find(token) != valid_tokens.end())
	{
		return (true);
	}	
	return (false);
}

bool	ConfigFile::checkSemicolon(std::string& token)
{
	if (token[token.size() - 1] != ';')
	{
		error_exit("no semicolon find at the end of the line.");
	}
	return (true);
}

void	ConfigFile::checkBrackets(std::string& conf_file)
{
	int counter = 0;
	std::ifstream file(conf_file.c_str());
	if (!file.is_open())
	{
		error_exit("unable to open file.");
	}
	
	std::string line;
	while(std::getline(file, line))
	{
		for (size_t i = 0; i < line.length(); ++i)
		{
			char c = line[i];
			
			if (c == '{')
			{
				++counter;
			}
			else if (c == '}')
			{
				--counter;
			}
		}
	}
	file.close();
	if (counter != 0)
	{
		error_exit("imbalance brackets.");
	}
}

void	ConfigFile::checkConfFile(std::string& conf_file)
{
	if (conf_file.rfind(".conf") != std::string::npos && conf_file.rfind(".conf") == conf_file.length() - 5)
	{
		parsConfFile(conf_file);
	}
	else
	{
		std::cerr << "Error: conf_file extension wrong." << std::endl;
		exit(1);
	}
	
}

void	ConfigFile::parseLocationDirectives(std::string& loc_token, std::vector<std::string>& loc_tokens, std::map< std::string, std::vector<std::string> >& loc_data)
{
	if (isValid(loc_token))
	{
		std::string& loc_last_token = loc_tokens.back();
		
		if (checkSemicolon(loc_last_token))
		{
			loc_last_token.erase(loc_last_token.size() - 1);
		}

		std::string loc_key = loc_token;
		itVec it = loc_tokens.begin();
		++it;

		while(it != loc_tokens.end())
		{
			loc_data[loc_key].push_back(*it);
			++it;
		}
	}
	else
	{
		error_exit("invalid token in location block.");
	}
}

void	ConfigFile::parseLocationBlock(itVec& it, std::vector<std::string>& tokens, std::ifstream& file)
{
	++it;
	if (it == tokens.end())
	{
		error_exit("location path expected.");
	}

	std::string loc_path = *it;
	++it;

	if (it == tokens.end() || *it != "{")
	{
		error_exit("'{' expected after location path.");
	}

	std::map< std::string, std::vector<std::string> > loc_data;
	std::string line;
	
	while (std::getline(file, line))
	{
		line = trimString(line);
		
		if (line.empty() || line[0] == '#')
		{
			continue ;
		}

		std::vector<std::string> loc_tokens = splitLine(line);
		std::string loc_token = loc_tokens.front();

		if (loc_token == "}")
		{
			break;
		}
		
		parseLocationDirectives(loc_token, loc_tokens, loc_data);
	}
	
	_current_server.addLocation(loc_path, loc_data);
}

void	ConfigFile::parseServerDirectives(itVec& it, std::vector<std::string>& tokens)
{
	std::string key = *it;
	++it;
	std::vector<std::string> values;

	std::string& last_token = tokens.back();

	if (checkSemicolon(last_token))
	{
		last_token.erase(last_token.size() - 1);
	}

	while (it != tokens.end())
	{
		values.push_back(*it);
		++it;
	}
	_current_server.addServer(key, values);
}

void	ConfigFile::parseTokens(std::vector<std::string>& tokens, std::ifstream& file, bool& in_server_block)
{
	itVec it = tokens.begin();
	std::string token = *it;

	if (token == "server")
	{
		if (*(++it) == "{")
		{
			in_server_block = true;
		}
		else
		{
			error_exit("Error: { expected after 'server'.");
		}
	}
	else if (in_server_block)
	{
		if (isSkip(token))
		{
			if (token == "location")
			{
				parseLocationBlock(it, tokens, file);
			}
		}
		else if (isValid(token))
		{
			parseServerDirectives(it, tokens);
		}
		else
		{
			error_exit("invalid token in server block.");
		}
	}
}

void	ConfigFile::parsConfFile(std::string& conf_file)
{
	std::ifstream file(conf_file.c_str());
	
	if (!file.is_open())
	{
		error_exit("unable to open file.");
	}
	
	checkBrackets(conf_file);
	
	std::string line;
	bool in_server_block = false;

	while (std::getline(file, line))
	{
		line = trimString(line);
		
		if (line.empty() || line[0] == '#')
		{
			continue ;
		}

		std::vector<std::string> tokens = splitLine(line);
		parseTokens(tokens, file, in_server_block);
		
		if (in_server_block && tokens.size() == 1 && tokens[0] == "}")
		{
			_servers.push_back(_current_server);
			_current_server = ServerBlock();
			in_server_block = false;
		}
	}
	
	file.close();
}

void	ConfigFile::printServers()
{
	int server_count = 1;
	std::vector<ServerBlock>::iterator it;
	
	for (it = _servers.begin(); it != _servers.end(); ++it)
	{
		std::cout << "Server Block >>> " << server_count++ << std::endl;
		it->printData();
		std::cout << std::endl;
	}
}

// void	ConfigFile::parsConfFile(std::string& conf_file)
// {
// 	std::ifstream file(conf_file.c_str());
	
// 	if (!file.is_open())
// 	{
// 		error_exit("unable to open file.");
// 	}

// 	std::string line;
// 	std::vector<std::string> tokens;
// 	std::string	token;
	
// 	checkBrackets(conf_file);
// 	while (std::getline(file, line))
// 	{
// 		line = trimString(line);
		
// 		if (line.empty() || line[0] == '#')
// 		{
// 			continue ;
// 		}
// 		tokens = splitLine(line);
// 		itVec it = tokens.begin();
// 		std::string& lastToken = tokens.back();
// 		while (it != tokens.end())
// 		{
// 			token = *it;
// 			std::string key;
			
// 			if (isSkip(token) || isValid(token))
// 			{
// 				if (isSkip(token))
// 				{
// 					if (token == "location")
// 					{
// 						++it;
// 						if (it == tokens.end())
// 						{
// 							error_exit("location path expected.");
// 						}
						
// 						std::string location_path = *it;
// 						++it;

// 						if (it == tokens.end() || *it != "{")
// 						{
// 							error_exit("'{' expected after location path.");
// 						}

// 						std::map< std::string, std::vector<std::string> > location_data;

// 						while (std::getline(file, line))
// 						{
// 							line = trimString(line);
// 							if (line.empty() || line[0] == '#')
// 							{
// 								continue ;
// 							}

// 							std::vector<std::string> loc_tokens = splitLine(line);
// 							std::string loc_token = loc_tokens.front();

// 							if (loc_token == "}")
// 							{
// 								break;
// 							}

// 							if (isValid(loc_token))
// 							{
// 								std::string& loc_lastToken = loc_tokens.back();
// 								if (checkSemicolon(loc_lastToken))
// 								{
// 									loc_lastToken.erase(loc_lastToken.size() - 1);
// 								}

// 								std::string loc_key = loc_token;
// 								itVec itLoc = loc_tokens.begin();
// 								++itLoc;
								
// 								while (itLoc != loc_tokens.end())
// 								{
// 									location_data[loc_key].push_back(*itLoc);
// 									++itLoc;
// 								}
// 							}
// 							else
// 							{
// 								error_exit("invalid token in location block.");
// 							}
// 						}
// 						_locations[location_path] = location_data;
// 						++it;
// 						continue;
// 					}
// 					++it;
// 					continue ;
// 				}
// 				else if (isValid(token))
// 				{
// 					if (checkSemicolon(lastToken))
// 					{
// 						lastToken.erase(lastToken.size() - 1);
// 					}
// 					key = token;
// 					++it;
// 					while (it != tokens.end())
// 					{
// 						_server_data[key].push_back(*it);
// 						++it;
// 					}
// 					break ;
// 				}
// 			}
// 			else
// 			{
// 				error_exit("this token makes no sense :).");
// 			}
// 		}
// 	}
// 	file.close();
// }
