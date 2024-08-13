# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mfortuna <mfortuna@student.42.pt>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/21 11:05:07 by mfortuna          #+#    #+#              #
#    Updated: 2024/08/13 12:50:52 by mfortuna         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.INCLUDEDIRS	: /mandatory /mlx /mylibft

CC				= cc
CFLAGS			= -Wall -Werror -Wextra #-fsanitize=thread
RM				= rm -rf
NAME			= philo
MANDATORY		= mandatory/
SRC_M			= $(MANDATORY)src/
SRC				= $(addprefix $(SRC_M), main.c struct.c utils.c)
OBJS			= $(SRC:%.c=%.o)
AR				= ar rc
MAKE			= make -C


all: $(NAME) 

$(NAME) :  $(OBJS)
	$(CC) -g $(CFLAGS) -o $(NAME) $(SRC)
	
$(SRC_M)%.o: %.c
	$(CC) $(CFLAGS) -I . -c $<

clean:
	$(RM) $(SRC_M)*.o

fclean: clean
	$(RM) $(NAME)

re: fclean all
