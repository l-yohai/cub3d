# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yohlee <yohlee@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/07/06 08:12:01 by yohlee            #+#    #+#              #
#    Updated: 2020/07/06 09:56:17 by yohlee           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3D

LIBFT = ./libft/libft.a

CC = gcc
CFLAGS = -Wall -Werror -Wextra -D BUFFER_SIZE=256 
INCLUDES = -I./ -I./libft -I./mlx_linux
INCLIB = -Lmlx_linux -lmlx -lXext -lX11 -lm -Llibft -lft
LIB = ./libft

SRCS = main.c\
		check_map.c\
		get_next_line.c\
		init.c\
		key_handling.c\
		parse.c\
		parse_map.c\
		texture.c\
		utils.c\
		raycasting.c\
		draw.c\
		sprite.c\
		sprite_utils.c\
		main_loop.c\
		bmp.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(MAKE) -C $(LIB) bonus
	$(CC) $(CFLAGS) $(INCLUDES) -o $(NAME) $(OBJS) $(INCLIB)

clean:
	$(MAKE) -C $(LIB) clean
	rm -f $(OBJS)

fclean: clean
	$(MAKE) -C $(LIB) fclean
	rm -f $(NAME)

# bonus: all

re: fclean all

.PHONY: test bonus all clean fclean re