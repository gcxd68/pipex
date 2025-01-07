# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gdosch <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/30 12:00:39 by gdosch            #+#    #+#              #
#    Updated: 2024/12/30 12:00:41 by gdosch           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC =			pipex.c \
				pipex_utils.c

SRCB =			pipex_bonus.c \
				pipex_utils_bonus.c

OBJ =			$(SRC:.c=.o)
OBJB =			$(SRCB:.c=.o)

NAME =			pipex
BONUS_NAME =	pipex_bonus

CC =			cc
CFLAGS =		-Wall -Wextra -Werror

LIBFT =			libft/libft.a
LIBFT_DIR = 	libft

all:			$(LIBFT) $(NAME)

$(NAME):		$(OBJ)
				$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT)

bonus:			$(LIBFT) $(BONUS_NAME)

$(BONUS_NAME):	$(OBJB)
				$(CC) $(CFLAGS) -o $(BONUS_NAME) $(OBJB) $(LIBFT)

$(LIBFT):
				make -C $(LIBFT_DIR)

clean:
				rm -f $(OBJ) $(OBJB)
				make clean -C $(LIBFT_DIR)

fclean:			clean
				rm -f $(NAME) $(BONUS_NAME)
				make fclean -C $(LIBFT_DIR)

re:				fclean all

.PHONY:			bonus all clean fclean re
