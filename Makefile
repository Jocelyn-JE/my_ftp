##
## EPITECH PROJECT, 2024
## Makefile
## File description:
## Makefile
##

.PHONY: all clean fclean re tests_run vg cs

%.o: %.cpp
	@echo "Compiling $<"
	g++ $(CPPFLAGS) -c $< -o $@
	@echo "Generated $@ successfully"

NAME			=	myftp

MAIN_SRC		=	src/Main.cpp

SRC				=	src/server/Server.cpp			\
					src/server/RunServer.cpp		\
					src/server/PollFdList.cpp		\
					src/server/Client.cpp			\
					src/server/DirectoryUtility.cpp	\
					src/Parser.cpp					\
					src/Socket.cpp					\

TESTS_SRC		=	tests/DirectoryUtility_tests.cpp\

OBJ				=	$(SRC:.cpp=.o)

MAIN_OBJ		=	$(MAIN_SRC:.cpp=.o)

INCLUDES		=	-I ./include

CPPFLAGS		+=	-std=c++20 -Wall -Wextra -Werror $(INCLUDES) -O2 -g

CPPTESTFLAGS	=	--coverage -lcriterion $(CPPFLAGS)

VALGRIND_LOG	=	valgrind.log

VALGRIND_FLAGS	=	--leak-check=full					\
					--show-leak-kinds=definite			\
					--track-origins=yes					\
					--errors-for-leak-kinds=definite	\
					--log-file="$(VALGRIND_LOG)"		\

CPPLINTFLAGS	=	--root=./include									\
					--repository=. 										\
					--filter=-legal/copyright,-build/c++17,+build/c++20	\
					--recursive											\

all: $(NAME)

$(NAME):	$(OBJ) $(MAIN_OBJ)
	@echo "Building $(NAME)..."
	g++ -o $(NAME) $(OBJ) $(MAIN_OBJ)

run: re
	@echo "Running $(NAME)..."
	@echo "-----------------------------------"
	./$(NAME)

vg: $(NAME)
	@echo "Running Valgrind..."
	valgrind $(VALGRIND_FLAGS) ./$(NAME) 4242 tests
	cat $(VALGRIND_LOG)

tests_run:
	@echo "Running tests..."
	g++ -o unit_tests $(SRC) $(TESTS_SRC) $(CPPTESTFLAGS)
	./unit_tests
	@gcovr --exclude tests/
	@gcovr -e tests --branch

clean:
	@echo "Cleaning up..."
	rm -f $(OBJ) $(MAIN_OBJ)
	rm -f *.gcda
	rm -f *.gcno
	rm -f vgcore.*
	rm -f *.log

fclean: clean
	@echo "Removing binaries..."
	rm -f $(NAME)
	rm -f unit_tests

re: fclean all

cs:	clean
	coding-style . .
	cat coding-style-reports.log
	rm -f coding-style-reports.log

linter: clean
	cpplint $(CPPLINTFLAGS) ./include/ ./src/
