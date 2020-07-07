# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yohlee <yohlee@student.42seoul.kr>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/07/06 08:12:01 by yohlee            #+#    #+#              #
#    Updated: 2020/07/07 12:31:17 by yohlee           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3D

LIBFT = ./libft/libft.a

CC = gcc
INCS = -I./includes -I./libft -I./mlx
INCLIB = -Lmlx -lmlx -framework OpenGL -framework Appkit -Llibft -lft
CFLAGS = -Wall -Werror -Wextra $(INCS)
LIBFT = ./libft

SRCS = ./srcs/main.c\
		./srcs/check_map.c\
		./srcs/get_next_line.c\
		./srcs/init.c\
		./srcs/key_handling.c\
		./srcs/parse.c\
		./srcs/parse_map.c\
		./srcs/texture.c\
		./srcs/utils.c\
		./srcs/raycasting.c\
		./srcs/draw.c\
		./srcs/sprite.c\
		./srcs/sprite_utils.c\
		./srcs/main_loop.c\
		./srcs/bmp.c\
		./srcs/exit.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(MAKE) -C $(LIBFT) bonus
	$(CC) $(CFLAGS) $(INCS) -o $(NAME) $(OBJS) $(INCLIB)

clean:
	$(MAKE) -C $(LIBFT) clean
	rm -f $(OBJS)

fclean: clean
	$(MAKE) -C $(LIBFT) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: test bonus all clean fclean re