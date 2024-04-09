# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rems <rems@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/24 14:17:35 by rgramati          #+#    #+#              #
#    Updated: 2024/04/04 10:49:37 by rems             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

BLACK		=	\033[30;49;3m
RED			=	\033[31;49;3m
GREEN		=	\033[32;49;3m
YELLOW		=	\033[33;49;3m
BLUE		=	\033[34;49;3m
MAGENTA		=	\033[35;49;3m
CYAN		=	\033[36;49;3m
WHITE		=	\033[37;49;3m

BBLACK		=	\033[30;49;3;1m
BRED		=	\033[31;49;3;1m
BGREEN		=	\033[32;49;3;1m
BYELLOW		=	\033[33;49;3;1m
BBLUE		=	\033[34;49;3;1m
BMAGENTA	=	\033[35;49;3;1m
BCYAN		=	\033[36;49;3;1m
BWHITE		=	\033[37;49;3;1m

RESET		=	\033[0m

LINE_CLR	=	\33[2K\r

FILE		=	$(shell ls -lR . | grep -F .c | wc -l)
CMP			=	1

NAME        :=	phonebook

OBJS_DIR	:=	.objs

SRCS		:=	PhoneBook.cpp		\
				Contact.cpp			\
				main.cpp

OBJS		:=	$(addprefix $(OBJS_DIR)/, $(SRCS:%.cpp=%.o))

CC          :=	c++

CFLAGS      :=	-g -Wall -Wextra -Werror -std=c++98

RM          :=	rm -rf

MAKEFLAGS   +=	--no-print-directory

DIR_DUP     = mkdir -p $(@D)

# RULES ********************************************************************** #

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@printf "$(LINE_CLR)  ⭐$(BWHITE) $(NAME):\t PROJECT READY !$(RESET)\n\n"

$(OBJS_DIR)/%.o: %.cpp
	@$(DIR_DUP)
	if [ $(CMP) -eq '1' ]; then \
		printf "\n"; \
	fi;
	printf "$(LINE_CLR)$(WHITE)  🔄 $(NAME): $(CMP)/$(FILE)\t$(GREEN)compiling...$(RESET)\t$(BWHITE)$<$(RESET)"
	@$(CC) $(CFLAGS) -o $@ -c $^
	@$(eval CMP=$(shell echo $$(($(CMP)+1))))
	if [ $(CMP) -gt $(FILE) ]; then \
		printf "$(LINE_CLR)$(WHITE)  🔄 $(NAME): $$(($(CMP)-1))/$(FILE)\n$(LINE_CLR)$(BGREEN)  ✅ Compilation done !$(RESET)\n"; \
	fi \

clean:
	@printf "\n$(BWHITE)  🚫 $(NAME):\t$(BRED) .o files deleted.$(RESET)\n"
	@$(RM) $(OBJS)

dclean: clean
	@printf "$(BWHITE)  🚫 $(NAME):\t$(BRED) objs dir deleted.$(RESET)\n"
	@$(RM) $(OBJS_DIR)

fclean: dclean
	@printf "$(BWHITE)  🚫 $(NAME):\t$(BRED) binary deleted.$(RESET)\n"
	@$(RM) $(NAME)

re:
	@$(MAKE) fclean
	@$(MAKE) all

# **************************************************************************** #

.PHONY: all clean fclean dclean re
.SILENT:
