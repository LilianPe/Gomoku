#variables

CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++17 -g3
NAME = Gomoku
INC_DIR = includes/
SRC_DIR = srcs/
OBJ_DIR = objs/
INCLUDES = -I includes/
SFML = libs/SFML-2.6.1
SFML_TAR = libs/SFML-2.6.1-linux-gcc-64-bit.tar.gz
SFML_INC = -I$(SFML)/include
SFML_LIB = -L$(SFML)/lib -lsfml-graphics -lsfml-window -lsfml-system

SRC_FILES = main Board Game Player Display Agent
INC_FILES = Gomoku Board Game Player Display Agent
TEMPLATE_FILES = 

INC	= $(addprefix $(INC_DIR), $(addsuffix .hpp, $(INC_FILES)))
TMP	= $(addprefix $(INC_DIR), $(addsuffix .tpp, $(TEMPLATE_FILES)))
SRC	= $(addprefix $(SRC_DIR), $(addsuffix .cpp, $(SRC_FILES)))
OBJ	= $(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRC_FILES)))
OBJF = .cache_exists

#colors

DEF_COLOR = \033[0;39m
GRAY = \033[0;90m
RED = \033[0;91m
GREEN = \033[0;92m
YELLOW = \033[0;93m
BLUE = \033[0;94m
MAGENTA = \033[0;95m
CYAN = \033[0;96m
WHITE = \033[0;97m

all : $(NAME)

$(NAME) : $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(INCLUDES) $(SFML_LIB)
	@echo "\n$(GREEN)$(NAME) ready!$(DEF_COLOR)"

$(OBJ_DIR)%.o:	$(SRC_DIR)%.cpp $(INC) $(TMP) | $(OBJF)
	@$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES) $(SFML_INC)

$(OBJF): $(SFML)
	@mkdir -p $(OBJ_DIR)

$(SFML) : $(SFML_TAR)
	@tar -xzf $(SFML_TAR) -C libs/

$(SFML_TAR) :
	@wget https://www.sfml-dev.org/files/SFML-2.6.1-linux-gcc-64-bit.tar.gz -P libs/

run: $(NAME)
	@LD_LIBRARY_PATH=$(SFML)/lib ./$(NAME)

clean :
	@rm -rf $(OBJ_DIR)

fclean : clean
	@rm -rf $(NAME)
	@rm -rf $(SFML)
	@rm -rf $(SFML_TAR)

re : fclean all
