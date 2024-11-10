/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smargine <smargine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 21:06:47 by smargine          #+#    #+#             */
/*   Updated: 2024/11/10 14:51:34 by smargine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGFILE_HPP
# define CONFIGFILE_HPP

# include <iostream>
# include <string>
# include <sstream>
# include <fstream>
# include <map>
# include <vector>
# include <set>
# include <cstdlib>
# include "ServerBlock.hpp"

# define WHITESPACE " \t\n\v\f\r"

typedef std::vector<std::string>::iterator itVec;
typedef std::map< std::string, std::vector<std::string> >::iterator itMap;
typedef std::map< std::string, std::map< std::string, std::vector<std::string> > >::iterator itMm;

class ConfigFile
{
	private:
			//std::map< std::string, std::vector<std::string> > _server_data;
			//std::map< std::string, std::map< std::string, std::vector<std::string> > > _locations;
			std::vector<ServerBlock> _servers;
			ServerBlock _current_server;

			//ConfigFile(const ConfigFile& copy);
			//ConfigFile&	operator=(const ConfigFile& other);

	public:
			ConfigFile();
			~ConfigFile();

			void	error_exit(const std::string& message);
			std::string	trimString(const std::string& str);
			std::vector<std::string>	splitLine(const std::string& line);
			bool	isSkip(const std::string& token);
			bool	isValid(const std::string& token);
			bool	checkSemicolon(std::string& token);
			void	checkBrackets(std::string& conf_file);
			void	checkConfFile(std::string& conf_file);

			void	parseLocationDirectives(std::string& loc_token, std::vector<std::string>& loc_tokens, std::map< std::string, std::vector<std::string> >& loc_data);
			void	parseLocationBlock(itVec& it, std::vector<std::string>& tokens, std::ifstream& file);
			void	parseServerDirectives(itVec& it, std::vector<std::string>& tokens);
			void	parseTokens(std::vector<std::string>& tokens, std::ifstream& file, bool& in_server_block);
			void	parsConfFile(std::string& conf_file);

			void	printServers();
};

#endif
