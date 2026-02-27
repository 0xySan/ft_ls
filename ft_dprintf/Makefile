CC = cc
CFLAGS = -Wall -Wextra -Werror

SRCS_FILE = ft_dprintf.c ft_puts.c print_format.c put_unit_ptr.c ft_buffer.c

SRCS = $(addprefix ./,$(SRCS_FILE))

OBJ = $(SRCS:.c=.o)

NAME = dprintf.a

%.o: %.c ./ft_printf.h
	@cc -c -o $@ $< -Wall -Wextra -Werror

$(NAME): $(OBJ)
	@ar rc $@ $(OBJ)

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

all: $(NAME)

re: fclean all

.PHONY: all fclean clean re