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
				pipex_utils.c \
				pipex_splitter.c


SRCB =			pipex_bonus.c \
				pipex_utils_bonus.c \
				pipex_splitter_bonus.c


OBJ =			$(SRC:.c=.o)
OBJB =			$(SRCB:.c=.o)

NAME =			pipex

CC =			cc
CFLAGS =		-Wall -Wextra -Werror

LIBFT =			libft/libft.a
LIBFT_DIR =		libft

all:			$(LIBFT)
				@if [ -f .bonus ]; then \
					rm -f $(NAME); \
					rm -f .bonus; \
				fi; \
				make $(NAME)

$(NAME):		$(OBJ)
				$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT)

bonus:			.bonus

.bonus:			$(LIBFT) $(OBJB)
				$(CC) $(CFLAGS) -o $(NAME) $(OBJB) $(LIBFT)
				@touch .bonus

$(LIBFT):
				make -C $(LIBFT_DIR)

clean:
				rm -f $(OBJ) $(OBJB) .bonus
				make clean -C $(LIBFT_DIR)

fclean:			clean
				rm -f $(NAME)
				make fclean -C $(LIBFT_DIR)

re:				fclean all

.PHONY:			bonus all clean fclean re
