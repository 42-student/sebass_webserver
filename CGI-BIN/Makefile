# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: smargine <smargine@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/28 20:43:51 by sebastian         #+#    #+#              #
#    Updated: 2024/10/17 02:10:59 by smargine         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := cgi

CXX := c++

CXXFLAGS := -Wall -Wextra -Werror -std=c++98 -Wshadow -fsanitize=address

RM := rm -rf

SRC := main.cpp\
		cgiHandler.cpp\

OBJ := $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
			$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

clean:
		$(RM) $(OBJ)

fclean: clean
		$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
