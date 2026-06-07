ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	PRELOAD_VAR := DYLD_INSERT_LIBRARIES
	PRELOAD_FLAG := DYLD_FORCE_FLAT_NAMESPACE=1
else
	PRELOAD_VAR :=LD_LIBRARY_PATH=. LD_PRELOAD
	PRELOAD_FLAG :=
endif

CC := gcc
CFLAGS := -Wall -Wextra -Werror -g3 -pthread -fPIC
INCLUDE := -I include -I libft/include

SRC_DIR		:= src
OBJ_DIR		:= obj

SOURCES :=	malloc free realloc \
			malloc_utils malloc_find_zone malloc_allocate_block \
			free_utils free_convert_block_to_empty \
			realloc_utils realloc_try_to_extend_block \
			show_alloc_mem show_alloc_mem_ex

SRC = $(addsuffix .c, $(SOURCES))
SRC := $(addprefix $(SRC_DIR)/, $(SRC))
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

NAME := libft_malloc_${HOSTTYPE}.so
LINK := libft_malloc.so

LIBFT := libft/libft.a
LIBFT_OBJ := libft/obj/*.o
LINK_LIB := -L./libft -lft

TEST_NAME := test.out
TEST_DIR := tests
TST_OBJ_DIR := tests/obj
TESTS := main
TST = $(addsuffix .c, $(TESTS))
TST := $(addprefix $(TEST_DIR)/, $(TST))
TST_OBJ := $(TST:$(TEST_DIR)/%.c=$(TST_OBJ_DIR)/%.o)

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

test: $(TEST_NAME)

$(TEST_NAME): $(NAME) $(TST_OBJ_DIR) $(TST_OBJ)
	$(CC) $(CFLAGS) $(INCLUDE) $(TST_OBJ) -o $@ \
	-Wl,-rpath,'$$ORIGIN' -L. -lft_malloc $(LINK_LIB)

$(TST_OBJ_DIR):
	mkdir -p $(TST_OBJ_DIR)

$(TST_OBJ_DIR)/%.o: $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

run_test: test
	$(PRELOAD_VAR)=./$(LINK) $(PRELOAD_FLAG) ./$(TEST_NAME)

info:
	@echo "HOSTTYPE: [$(HOSTTYPE)]"
	@echo "UNAME_S: [$(UNAME_S)]"
	@echo "NAME: [$(NAME)]"
	@echo "LINK: [$(LINK)]"
	@echo "SOURCES: [$(SOURCES)]"
	@echo "OBJ: [$(OBJ)]"

help:
	@echo "Available targets:"
	@echo "  make all       - Build the malloc library"
	@echo "  make test      - Build test executable"
	@echo "  make run_test  - Run tests with library preload"
	@echo "  make clean     - Remove object files"
	@echo "  make fclean    - Remove everything"
	@echo "  make re        - Clean and rebuild"
	@echo "  make info      - Print build information"

.PHONY: all clean fclean re test run_test info help