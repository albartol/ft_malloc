CC := gcc

CFLAGS := -Wall -Wextra -Werror -O2

INCLUDE := -Iinclude

SRC_DIR		:= src
OBJ_DIR		:= obj

# SOURCES
SRC :=	ft_atoi.c ft_isalpha.c ft_isprint.c ft_memcpy.c ft_strchr.c  \
		ft_strlcpy.c ft_strnstr.c ft_toupper.c ft_calloc.c ft_isascii.c \
		ft_memchr.c ft_memmove.c ft_strdup.c ft_strlen.c ft_strrchr.c \
		ft_bzero.c ft_isalnum.c ft_isdigit.c ft_memcmp.c ft_memset.c \
		ft_strlcat.c ft_strncmp.c ft_tolower.c ft_putchar_fd.c ft_putstr_fd.c \
		ft_putendl_fd.c ft_putnbr_fd.c ft_substr.c ft_strjoin.c ft_strtrim.c \
		ft_itoa.c ft_split.c ft_strmapi.c ft_tolower.c ft_striteri.c

BNS :=	ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c \
		ft_lstadd_back.c ft_lstdelone.c ft_lstclear.c ft_lstiter.c ft_lstmap.c

GNL :=	get_next_line.c

PRI :=	ft_printf.c ft_putstr_n.c ft_putchar_n.c ft_putnbr_n.c \
		ft_putnbr_base_n.c ft_putptr_n.c

EXT :=	merge_sort.c quick_sort.c charjoin.c ft_atol.c ft_atoll.c \
		array_char_len.c array_to_str.c array_dup.c array_free.c array_len.c

SOURCES := $(SRC) $(BNS) $(GNL) $(PRI) $(EXT)

SRCS := $(addprefix $(SRC_DIR)/, $(SOURCES))

OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

NAME := libft.a

all: $(OBJ_DIR) $(NAME)

$(NAME): $(OBJS)
	ar -rcs $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all fclean clean re
# .SILENT: all fclean clean re $(NAME) $(OBJ_DIR)
