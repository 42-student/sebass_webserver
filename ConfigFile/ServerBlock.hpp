/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerBlock.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smargine <smargine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 20:02:35 by smargine          #+#    #+#             */
/*   Updated: 2024/11/10 00:21:12 by smargine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERBLOCK_HPP
# define SERVERBLOCK_HPP

# include <iostream>
# include <vector>
# include <map>
# include <string>

class ServerBlock
{
	private:
			std::map< std::string, std::vector<std::string> > _server_data;
			std::map< std::string, std::map< std::string, std::vector<std::string> > > _locations;

			//ServerBlock(const ServerBlock& copy);
			//ServerBlock&	operator=(const ServerBlock& other);

	public:
			ServerBlock();
			~ServerBlock();

			void	addServer(const std::string& key, const std::vector<std::string>& values);
			void	addLocation(const std::string& path, const std::map< std::string, std::vector<std::string> >& loc_data);

			void	printData();

};

#endif
