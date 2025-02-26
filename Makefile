##
## EPITECH PROJECT, 2024
## Makefile
## File description:
## Makefile
##

.PHONY: all clean fclean re tests_run

%.o: %.cpp
	@echo "Compiling $<"
	@g++ $(CPPFLAGS) -c $< -o $@
	@echo "Generated $@ successfully"

NAME			=	myftp

MAIN_SRC		=	src/Main.cpp

SRC				=	src/server/Server.cpp	\
					src/client/Client.cpp	\

TESTS_SRC		=	tests/tests.cpp			\

OBJ				=	$(SRC:.cpp=.o)

MAIN_OBJ		=	$(MAIN_SRC:.cpp=.o)

INCLUDES		=	-I ./include

CPPFLAGS		+=	-std=c++20 -Wall -Wextra -Werror $(INCLUDES) -O2 -g

CPPTESTFLAGS	=	--coverage -lcriterion $(CPPFLAGS)

all: $(NAME)

$(NAME):	$(OBJ) $(MAIN_OBJ)
	@echo "Building $(NAME)..."
	@g++ -o $(NAME) $(OBJ) $(MAIN_OBJ)

run: re
	@echo "Running $(NAME)..."
	@echo "-----------------------------------"
	@./$(NAME)

valgrind: $(NAME)
	@echo "Running Valgrind..."
	@valgrind --leak-check=full --show-leak-kinds=all ./$(NAME)

tests_run:
	@echo "Running tests..."
	@g++ -o unit_tests $(SRC) $(TESTS_SRC) $(CPPTESTFLAGS)
	@./unit_tests
	@gcovr --exclude tests/

clean:
	@echo "Cleaning up..."
	@rm -f $(OBJ) $(MAIN_OBJ)
	@rm -f *.gcda
	@rm -f *.gcno
	@rm -f vgcore.*

fclean: clean
	@echo "Removing binaries..."
	@rm -f $(NAME)
	@rm -f unit_tests

re: fclean all
