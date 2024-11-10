/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerBlock.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smargine <smargine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 20:02:50 by smargine          #+#    #+#             */
/*   Updated: 2024/11/10 00:56:21 by smargine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerBlock.hpp"

ServerBlock::ServerBlock() {}

ServerBlock::~ServerBlock() {}

void	ServerBlock::addServer(const std::string& key, const std::vector<std::string>& values)
{
	_server_data[key] = values;
}

void	ServerBlock::addLocation(const std::string& path, const std::map< std::string, std::vector<std::string> >& loc_data)
{
	_locations[path] = loc_data;
}

void	ServerBlock::printData()
{
	std::map< std::string, std::vector<std::string> >::iterator it;

	for(it = _server_data.begin(); it != _server_data.end(); ++it)
	{
		std::cout << it->first << " = ";
		for(size_t i = 0; i < it->second.size(); ++i)
		{
			std::cout << it->second[i];
			if (i + 1 < it->second.size())
			{
				std::cout << ", ";
			}
		}
		std::cout << std::endl;
	}

	std::map< std::string, std::map< std::string, std::vector<std::string> > >::iterator loc_it;
	
	for (loc_it = _locations.begin(); loc_it != _locations.end(); ++loc_it)
	{
		std::cout << "Location path: " << loc_it->first << std::endl;
		std::map< std::string, std::vector<std::string> >::iterator loc_dir_it;
		for (loc_dir_it = loc_it->second.begin(); loc_dir_it != loc_it->second.end(); ++loc_dir_it)
		{
			std::cout << loc_dir_it->first << " = ";
			for (size_t i = 0; i < loc_dir_it->second.size(); ++i)
			{
				std::cout << loc_dir_it->second[i];
				if (i + 1 < loc_dir_it->second.size())
				{
					std::cout << ", ";
				}
			}
			std::cout << std::endl;
		}
	}
}
