NAME        = ircserv
CXX         = c++
CXXFLAGS    = -Wall -Wextra -Werror -std=c++98 -I $(INC_DIR)

SRC_DIR     = src
OBJ_DIR     = objs
INC_DIR     = includes

SRCS        = main.cpp \
			  $(SRC_DIR)/network/Server.cpp \
			  $(SRC_DIR)/commands/Client.cpp \
			  $(SRC_DIR)/commands/Channel.cpp \
			  $(SRC_DIR)/commands/Dispatcher.cpp \
			  $(SRC_DIR)/commands/Commands.cpp \
			  $(SRC_DIR)/commands/Replies.cpp \
			  $(SRC_DIR)/parser/Parser.cpp
OBJS        = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
	@echo "Build successful: $(NAME) is ready!"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	@echo "Object files cleaned."

fclean: clean
	rm -f $(NAME)
	@echo "Executable cleaned."

re: fclean all

.PHONY: all clean fclean re