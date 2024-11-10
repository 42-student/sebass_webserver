/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smargine <smargine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 20:04:33 by smargine          #+#    #+#             */
/*   Updated: 2024/11/10 00:18:29 by smargine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigFile.hpp"

int	main(int ac, char**av)
{
	if (ac == 2)
	{
		std::string conf_file = av[1];
		ConfigFile config;
		
		config.parsConfFile(conf_file);
		config.printServers();
	}
	else
	{
		std::cerr << "Error: invalid arguments.\nTry ./webServer config_file.conf" << std::endl;
		exit(EXIT_FAILURE);
	}

	return (0);
}
