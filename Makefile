ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	PRELOAD_VAR := DYLD_INSERT_LIBRARIES
	PRELOAD_FLAG := DYLD_FORCE_FLAT_NAMESPACE=1
else
	PRELOAD_VAR := LD_PRELOAD
	PRELOAD_FLAG :=
endif

CC := gcc
CFLAGS := -Wall -Wextra -Werror -g3 -pthread -fPIC
INCLUDE := -I include -I libft/include

SRC_DIR		:= src
OBJ_DIR		:= obj

SOURCES := malloc free realloc
SRC = $(addsuffix .c, $(SOURCES))
SRC := $(addprefix $(SRC_DIR)/, $(SRC))
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

NAME := libft_malloc_${HOSTTYPE}.so
LINK := libft_malloc.so

LIBFT := libft/libft.a

TEST_NAME := test.out
TEST_DIR := tests
TST_OBJ_DIR := tests/obj
TESTS := main
TST = $(addsuffix .c, $(TESTS))
TST := $(addprefix $(TEST_DIR)/, $(TST))
TST_OBJ := $(TST:$(TEST_DIR)/%.c=$(TST_OBJ_DIR)/%.o)
LINK_LIB := -L. -lft_malloc

all: $(NAME)

$(NAME): libft $(OBJ_DIR) $(OBJ)
	$(CC) -shared $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME)
	ln -sf $(NAME) $(LINK)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

libft: $(LIBFT)

$(LIBFT):
	$(MAKE) -C libft

clean:
	$(MAKE) clean -C libft
	rm -rf $(OBJ_DIR)
	rm -rf $(TST_OBJ_DIR)

fclean: clean
	$(MAKE) fclean -C libft
	rm -f $(NAME) $(LINK) $(TEST_NAME)

re: fclean all

test: all $(TEST_NAME)
	$(PRELOAD_VAR)=./$(LINK) $(PRELOAD_FLAG) ./$(TEST_NAME)

$(TEST_NAME): $(TST_OBJ_DIR) $(TST_OBJ)
	$(CC) $(CFLAGS) $(INCLUDE) $(TST_OBJ) -o $@
# 	$(CC) $(CFLAGS) $(INCLUDE) $(TST_OBJ) -o $@ $(LINK_LIB)

$(TST_OBJ_DIR):
	mkdir -p $(TST_OBJ_DIR)

$(TST_OBJ_DIR)/%.o: $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

print_info:
	@echo HOSTTYPE: [$(HOSTTYPE)]
	@echo NAME: [$(NAME)]
	@echo OS: [$(UNAME_S)]

.PHONY: all clean fclean re test print_info