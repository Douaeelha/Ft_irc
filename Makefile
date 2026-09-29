NAME        = ircserv
NAME_BONUS  = ircserv_bonus
BOT_NAME    = ircbot

CXX         = c++
CXXFLAGS    = -Wall -Wextra -Werror -std=c++98

# --- STANDARD DIRECTORIES ---
SRC_DIR     = src
OBJ_DIR     = objs
INC_DIR     = includes
HEADERS     = $(wildcard $(INC_DIR)/*.hpp)

# --- BONUS DIRECTORIES ---
SRC_DIR_B   = bonus/src
OBJ_DIR_B   = bonus/objs
INC_DIR_B   = bonus/includes
HEADERS_B   = $(wildcard $(INC_DIR_B)/*.hpp)

# --- STANDARD FILES ---
SRCS        = main.cpp \
              $(SRC_DIR)/network/Server.cpp \
              $(SRC_DIR)/commands/Client.cpp \
              $(SRC_DIR)/commands/Channel.cpp \
              $(SRC_DIR)/commands/Dispatcher.cpp \
              $(SRC_DIR)/commands/Commands.cpp \
              $(SRC_DIR)/commands/Replies.cpp \
              $(SRC_DIR)/parser/Parser.cpp

OBJS        = $(SRCS:%.cpp=$(OBJ_DIR)/%.o)

# --- BONUS FILES ---
SRCS_B      = bonus/main_bonus.cpp \
              $(SRC_DIR_B)/network/Server_bonus.cpp \
              $(SRC_DIR_B)/commands/Client_bonus.cpp \
              $(SRC_DIR_B)/commands/Channel_bonus.cpp \
              $(SRC_DIR_B)/commands/Dispatcher_bonus.cpp \
              $(SRC_DIR_B)/commands/Commands_bonus.cpp \
              $(SRC_DIR_B)/commands/Replies_bonus.cpp \
              $(SRC_DIR_B)/parser/Parser_bonus.cpp

OBJS_B      = $(SRCS_B:%.cpp=$(OBJ_DIR_B)/%.o)

# --- BOT FILES ---
BOT_SRCS    = bonus/bot_bonus.cpp
BOT_OBJS    = $(BOT_SRCS:%.cpp=$(OBJ_DIR_B)/%.o)

# --- STANDARD RULES ---
all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -I $(INC_DIR) $(OBJS) -o $(NAME)
	@echo "Build successful: $(NAME) is ready!"
	@printf "%s\n" "$$ART"

$(OBJ_DIR)/%.o: %.cpp $(HEADERS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -I $(INC_DIR) -c $< -o $@

# --- BONUS RULES ---
bonus: $(NAME_BONUS) $(BOT_NAME)

$(NAME_BONUS): $(OBJS_B)
	$(CXX) $(CXXFLAGS) -I $(INC_DIR_B) $(OBJS_B) -o $(NAME_BONUS)
	@echo "Bonus Build successful: $(NAME_BONUS) is ready!"
	@printf "%s\n" "$$ART"

$(BOT_NAME): $(BOT_OBJS)
	$(CXX) $(CXXFLAGS) -I $(INC_DIR_B) $(BOT_OBJS) -o $(BOT_NAME)
	@echo "Bot Build successful: $(BOT_NAME) is ready!"

$(OBJ_DIR_B)/%.o: %.cpp $(HEADERS_B)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -I $(INC_DIR_B) -c $< -o $@

# --- CLEANUP ---
clean:
	rm -rf $(OBJ_DIR) $(OBJ_DIR_B)
	@echo "Object files cleaned."

fclean: clean
	rm -f $(NAME) $(NAME_BONUS) $(BOT_NAME)
	@echo "Executables cleaned."

re: fclean all

.PHONY: all clean fclean re bonus

define ART

⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⢠⣿⣄⣤⣤⣤⣤⣼⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⡇⠀⠀⠀⠀
⠀⠀⠀⠀⣠⣾⣿⣻⡵⠖⠛⠛⠛⢿⣿⣶⣴⣿⠟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣰⠏⢷⡄⠀⠀⠀
⠀⣤⣤⡾⣯⣿⡿⠋⠀⠀⠀⠀⠀⠀⠈⠙⢿⣿⣷⣤⣴⣾⠆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⠏⠀⠈⢻⣦⡀⠀
⠀⢹⣿⣴⣿⡏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠙⢿⣿⣿⣄⡀⢀⣤⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣶⠀⠈⠻⣦⠀⠀⣼⠋⠀⠀
⠀⣼⢉⣿⡿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠛⢿⣿⣿⣿⣥⠤⠴⣶⣶⣶⣶⣶⣶⣶⣶⣾⣿⠿⣿⣿⣿⣿⡇⣸⠋⠻⣿⣷
⢰⡏⢸⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠛⢿⣿⣶⣶⣿⣟⣿⣟⣛⣭⣉⣩⣿⣿⡀⣼⣿⣿⣿⣿⣿⣄⠀⣸⣿
⢿⡇⢸⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠛⣿⣿⣿⠿⠿⠛⠛⠛⠛⠛⠻⣿⣿⣭⣉⢉⣿⣿⠟⣰⣿⡟
⠈⣷⠸⣇⣷⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⡴⠞⠛⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⠀⠀⠉⣿⣿⡏⢀⣿⡟⠀
⠀⠹⣦⣿⣿⣿⣦⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⠞⠋⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⣤⣀⠀⠀⠀⠀⣼⣿⡿⢫⣿⣿⡁⠀
⠀⠀⠀⠙⣿⡿⣿⣿⣷⣤⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⡾⠁⠀⠀⠀⠀⠀⠀⠀⣀⣤⠶⠿⢯⡈⠙⣧⡀⠀⠀⣿⣄⣴⣿⣿⠉⠻⣦
⠀⠀⠀⠰⠿⠛⠛⠻⣿⣿⣿⣷⣦⣀⠀⠀⠀⠀⠀⠀⣴⠏⠀⠀⠀⠀⠀⠀⠀⣰⣿⠉⠀⠀⠀⠚⣷⠀⠘⡇⠀⠀⠀⠙⠛⠉⠁⠀⠀⠈
⠀⠀⠀⠀⠀⠀⠀⠀⠀⣹⣿⣽⡿⣿⣷⣦⣀⠀⠀⢰⡟⠀⠀⠀⠀⠀⠀⠀⠀⣿⠽⣄⠀⠀⠀⣠⠟⠀⢀⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠉⠉⠙⠻⣿⣿⣟⣷⣦⣼⡇⠀⠀⠀⠀⠀⠀⠀⠀⠛⢧⡉⠛⠛⠛⠁⠀⣠⡾⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⡟⢉⣿⣿⣿⣿⣷⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⠳⠶⠶⠶⠛⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⡇⠈⠉⠉⠉⣻⣿⣇⡀⠀⠀⠀⠀⠀⣤⡶⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢷⣄⠀⣠⣾⡿⠁⠙⢷⣦⣦⣤⣴⣿⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⢀⣴⠶⣆⠀⠀⠀⣾⠉⢻⣿⣿⡀⠀⠀⢿⣿⢉⡿⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⢀⣿⠁⢠⡟⠀⠀⠀⣿⠀⠘⣯⠉⠃⠀⠀⠈⢁⣸⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⣀⣼⡿⠀⠘⣷⠀⠀⠀⣿⠀⠀⢻⡶⠞⢛⡶⠚⢻⡟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⢀⡾⠋⠁⣀⠀⠀⠈⠳⣄⠀⢸⡆⠀⠈⢷⣄⠟⢁⣠⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣰⡇⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⢸⡇⠀⠀⠈⢻⡄⠀⠀⠘⢷⣤⣷⡀⠀⠀⠙⠛⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⣧⠀⠀⠀⠀⣿⡀⠀⠀⠀⠈⢻⣷⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⣇⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⢹⣄⠀⠀⢀⣿⠁⡀⠀⠀⠀⠀⠻⢷⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠹⣆⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠉⠛⠛⠛⠉⠻⣿⡦⠀⠀⠀⠀⠈⢻⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠹⡇⠀⠀⠀⠀⠀
      __      ___      ___        ______             __      ___      ___        ______    
     /""\    |"  |    |"  |      /    " \           /""\    |"  |    |"  |      /    " \   
    /    \   ||  |    ||  |     // ____  \         /    \   ||  |    ||  |     // ____  \  
   /' /\  \  |:  |    |:  |    /  /    ) :)       /' /\  \  |:  |    |:  |    /  /    ) :) 
  //  __'  \  \  |___  \  |___(: (____/ //       //  __'  \  \  |___  \  |___(: (____/ //  
 /   /  \\  \( \_|:  \( \_|:  \\        /       /   /  \\  \( \_|:  \( \_|:  \\        /   
(___/    \___)\_______)\_______)\"_____/       (___/    \___)\_______)\_______)\"_____/⠀

endef
export ART