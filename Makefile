#variables

CC = c++
CFLAGS = -Wall -Wextra -Werror
NAME = Gomoku
INC_DIR = includes/
SRC_DIR = srcs/
OBJ_DIR = objs/
INCLUDES = -I includes/

SRC_FILES = main Board Game Player
INC_FILES = Gomoku Board Game Player
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
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(INCLUDES)
	@echo "\n$(GREEN)$(NAME) ready!$(DEF_COLOR)"

$(OBJ_DIR)%.o:	$(SRC_DIR)%.cpp $(INC) $(TMP) | $(OBJF)
	@$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

$(OBJF):
	@mkdir -p $(OBJ_DIR)

clean :
	@rm -rf $(OBJ_DIR)

fclean : clean
	@rm -rf $(NAME)

re : fclean all