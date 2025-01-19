NAME = miniRT
CC 			 = cc
CFLAGS 	     = -Ofast -Wall -Wextra -Werror -I./includes -I./MLX42/include -I./MLX/include/lodepng/lodepng.h
# CFLAGS 	     = -g3 -fsanitize=address -I./includes -I./MLX42/include -I./MLX/include/lodepng/lodepng.h
MLX_FLAGS 	 = -L./MLX42/build -lmlx42 -ldl -lglfw -pthread -lm
LIBFTDIR 	:=	libft/
LIBMLX		:= ./MLX42
INCLUDE		:=	-L libft -lft


SRC = src/main.c \
      src/render/render1.c \
	  src/render/render2.c \
	  src/render/render3.c \
	  src/render/render4.c \
	  src/render/render5.c \
	  src/render/render6.c \
	  src/render/render7.c \
	  src/render/render8.c \
	  src/render/render9.c \
      src/render/intersection.c \
      src/render/lighting1.c \
      src/render/lighting2.c \
      src/render/lighting3.c \
	  src/utils1.c \
 	  src/utils2.c \
	  src/parser/parse_camera.c \
	  src/parser/parse_file1.c \
	  src/parser/parse_file2.c \
	  src/parser/parse_light.c \
  	  src/parser/parse_plane.c \
	  src/parser/parse_cylinder1.c \
  	  src/parser/parse_cylinder2.c \
	  src/parser/parse_cylinder3.c \
  	  src/parser/parse_sphere.c \
	  src/render/safe2file.c \
	  MLX42/lib/png/lodepng.c \
	  src/parser/parser_utils.c \

OBJ = $(SRC:.c=.o)

all: libmlx $(NAME)


libmlx:
	@if [ ! -d "$(MLX_DIR)" ]; then \
		git clone https://github.com/codam-coding-college/MLX42.git; \
	fi
	cmake $(LIBMLX) -B $(LIBMLX)/build && make -C $(LIBMLX)/build

$(NAME): $(OBJ)
	$(MAKE) -C $(LIBFTDIR)
	$(CC) $(CFLAGS) $(OBJ) $(MLX_FLAGS) -o $(NAME) $(INCLUDE) 

clean:
	rm -f $(OBJ)
	$(MAKE) -C $(LIBFTDIR) clean
	rm -rf $(LIBMLX)/build

fclean: clean
	$(MAKE) -C $(LIBFTDIR) fclean
	rm -rf miniRT
	rm -rf MLX42/

re: fclean all
