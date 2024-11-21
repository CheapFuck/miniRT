# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: thivan-d <thivan-d@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2022/11/12 13:49:09 by thivan-d      #+#    #+#                  #
#    Updated: 2022/11/24 15:03:47 by thivan-d      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

CC = gcc
CFLAGS = -Wall -Wextra -Werror
AR = ar rcs
RM = rm -f


FILES = ft_isalpha \
		ft_isdigit \
		ft_isalnum \
		ft_isascii \
		ft_isprint \
		ft_strlen \
		ft_memset \
		ft_bzero \
		ft_memcpy \
		ft_memmove \
		ft_strlcpy \
		ft_strlcat \
		ft_toupper \
		ft_tolower \
		ft_strchr \
		ft_strrchr \
		ft_strncmp \
		ft_memchr \
		ft_memcmp \
		ft_strnstr \
		ft_atoi \
		ft_atof \
		ft_calloc \
		ft_strdup \
		ft_substr \
		ft_strjoin \
		ft_strtrim \
		ft_split \
		ft_itoa \
		ft_strmapi \
		ft_striteri \
		ft_putchar_fd \
		ft_putstr_fd \
		ft_putendl_fd \
		ft_putnbr_fd \
		ft_arraylen \
		ft_free_split \
		ft_clean_2d_array \
		ft_safe_free \
		
FILES_B = 	ft_lstnew_bonus \
	  		ft_lstadd_front_bonus \
	  		ft_lstsize_bonus \
			ft_lstlast_bonus \
	  		ft_lstadd_back_bonus \
	  		ft_lstdelone_bonus \
	  		ft_lstclear_bonus \
	  		ft_lstiter_bonus \
	  		ft_lstmap_bonus


MAIN = $(addsuffix .o, $(FILES))
BONUS = $(addsuffix .o, $(FILES)) $(addsuffix .o, $(FILES_B))

ifdef plusbonus
TODO = $(BONUS)
else
TODO = $(MAIN)
endif

$(NAME): $(TODO)
	@$(AR) $@ $^

all:	$(NAME)

%.o: %.c
	@$(CC) -c $(CFLAGS) -o $@ $<

bonus: 
	@$(MAKE) plusbonus=1

clean:
	$(RM) $(BONUS)

fclean: clean
	$(RM) $(NAME)

re: 
	$(MAKE) fclean
	$(MAKE) all

.PHONY: bonus all clean fclean re