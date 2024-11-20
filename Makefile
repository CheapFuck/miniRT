NAME = miniRT
CC = cc
CFLAGS = -Wall -Wextra -Werror -I./includes -I./MLX42/include
MLX_FLAGS = -L./MLX42/build -lmlx42 -ldl -lglfw -pthread -lm -g
LIBFTDIR 		:=	libft/
LIBMLX	:= ./MLX42
INCLUDE			:=	-L libft -lft


SRC = src/main.c \
      src/render/render.c \
      src/render/intersection.c \
      src/render/lighting.c \
      src/utils.c \
	  src/parser/parse_camera.c \
	  src/parser/parse_file.c \
	  src/parser/parse_light.c \
	  src/parser/parse_objects.c \
  	  src/parser/parse_plane.c \
	  src/parser/parse_cylinder.c \
  	  src/parser/parse_sphere.c \
	  
OBJ = $(SRC:.c=.o)

all: libmlx $(NAME)


libmlx:
	@cmake $(LIBMLX) -B $(LIBMLX)/build && make -C $(LIBMLX)/build -j4

$(NAME): $(OBJ)
	$(MAKE) -C $(LIBFTDIR)
	$(CC) $(CFLAGS) $(OBJ) $(MLX_FLAGS) -o $(NAME) $(INCLUDE) 

clean:
	rm -f $(OBJ)
	$(MAKE) -C $(LIBFTDIR) clean
	rm -rf $(LIBMLX)/build

fclean: clean

re: fclean all


# NAME = miniRT
# CC = cc
# CFLAGS  := -Wall -Wextra -Werror -Wunreachable-code -Ofast
# LIBMLX	:= ./MLX42
# HEADERS	:= -I ./includes -I $(LIBMLX)/include
# LIBS	:= $(LIBMLX)/build/libmlx42.a -ldl -lglfw -pthread -lm

# # MLX_FLAGS = -Iinclude -Lmlx42 -lmlx42 -ldl -lglfw3 -pthread -lm

# SRC = src/main.c \
# 	  src/utils.c \
#       src/parser/parse_file.c \
#       src/render/render.c \
#       src/render/intersection.c \
#       src/render/lighting.c \

# OBJ = $(SRC:.c=.o)

# all: libmlx $(NAME)

# libmlx:
# 	cmake $(LIBMLX) -B $(LIBMLX)/build && make -C $(LIBMLX)/build -j4

# %.o: %.c
# 	@$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS)  && printf "Compiling: $(notdir $<)\n"
# # %.o: %.c
# # 	@$(CC) $(CFLAGS) -o $@ -c $<

# $(NAME): $(OBJS)
# 	@$(CC) $(OBJS) $(LIBS) $(HEADERS) -o $(NAME)

# clean:
# 	rm -f $(OBJ)
# 	rm -rf $(LIBMLX)/build

# fclean: clean
# 	rm -f $(NAME)

# re: fclean all

