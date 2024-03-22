# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/24 14:17:35 by rgramati          #+#    #+#              #
#    Updated: 2024/03/22 16:44:25 by rgramati         ###   ########.fr        #
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

FILE	=	$(shell ls -lR srcs/ | grep -F .c | wc -l)
CMP		=	1

NAME        := minishell

LFT_DIR 	:= ./libft

LFT			:= $(LFT_DIR)/libft.a

SRCS_DIR	:=	srcs

OBJS_DIR	:=	.objs


SRC_DATA	:=	data_structures/ms_constructors.c	\
				data_structures/ms_envvar.c			\
				data_structures/ms_envvar_utils.c	\
				data_structures/ms_token.c			\
				data_structures/ms_node.c			\
				data_structures/ms_command.c		\
				data_structures/ms_pid.c			\
				data_structures/ms_pipes.c			\
				data_structures/ms_redir.c			\
				data_structures/ms_executer.c

SRC_BUILTIN :=	builtins/ms_cd.c					\
				builtins/ms_echo.c					\
				builtins/ms_exit.c					\
				builtins/ms_pwd.c					\
				builtins/ms_env.c					\
				builtins/ms_export.c				\
				builtins/ms_unset.c					\
				builtins/ms_builtins_utils.c

SRC_PARSING	:=  parsing/ms_tree_build.c				\
				parsing/ms_parse_dollars.c			\
				parsing/ms_parse_quotes.c			\
				parsing/ms_parse_wildcards.c		\
				parsing/ms_token_auth.c				\
				parsing/ms_tokenizer.c				\
				parsing/ms_parsing_utils.c

SRC_EXEC	:=	execution/ms_manage_close.c			\
				execution/ms_manage_files.c			\
				execution/ms_manage_heredocs.c 		\
				execution/ms_manage_commands.c		\
				execution/ms_exec_binops.c			\
				execution/ms_exec_builtins.c		\
				execution/ms_exec_command.c			\
				execution/ms_exec_pipeline.c		\
				execution/ms_exec_utils.c

SRC_PROMPT	:=  prompt/ms_prompt.c					\
				prompt/ms_manage_dquote.c			\
				prompt/ms_holder.c

SRC_SETUP	:=  utils/ms_setup_env.c				\
				utils/ms_handle_signals.c			\
				utils/ms_error_handling.c			\
				utils/ms_utils.c

ALL_SRCS	:=	$(SRC_DATA) 						\
				$(SRC_BUILTIN)						\
				$(SRC_PARSING)						\
				$(SRC_PROMPT)						\
				$(SRC_EXEC)							\
				$(SRC_SETUP)						\
				minishell.c
				
HEADERS		:=	includes/minishell.h 				\
				includes/ms_enumerates.h			\
				includes/ms_data_structures.h 		\
				includes/ms_parsing.h				\
				includes/ms_builtins.h 				\
				includes/ms_file_manager.h			\
				includes/ms_execution.h

SRCS		:=	$(ALL_SRCS:%=$(SRCS_DIR)/%)

OBJS		:=	$(addprefix $(OBJS_DIR)/, $(SRCS:%.c=%.o))

CC          := cc

CFLAGS      := -g3 -Wall -Wextra -Werror

ifeq ($(DEBUG), 1)
	CFLAGS 	+= -fsanitize=address -static-libsan
endif

IFLAGS	    := -I $(LFT_DIR)/includes -I ./includes


RM          := rm -rf

MAKEFLAGS   += --no-print-directory

DIR_DUP     = mkdir -p $(@D)

# RULES ********************************************************************** #

all: $(NAME)

$(NAME): $(LFT) $(OBJS) $(HEADERS)
	@$(CC) $(CFLAGS) $(IFLAGS) $(OBJS) $(LFT_DIR)/libft.a -o $(NAME) -lreadline
	@printf "$(LINE_CLR)  ⭐$(BWHITE) $(NAME):\t PROJECT READY !$(RESET)\n\n"

$(LFT):
	@make -C $(LFT_DIR) -j

$(OBJS_DIR)/%.o: %.c
	@$(DIR_DUP)
	if [ $(CMP) -eq '1' ]; then \
		printf "\n"; \
	fi;
	printf "$(LINE_CLR)$(WHITE)  🔄 $(NAME): $(CMP)/$(FILE)\t$(GREEN)compiling...$(RESET)\t$(BWHITE)$<$(RESET)"
	@$(CC) $(CFLAGS) $(IFLAGS) -o $@ -c $^
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
	@$(MAKE) -C $(LFT_DIR) fclean

re:
	@$(MAKE) fclean
	@$(MAKE) all

# **************************************************************************** #

.PHONY: all clean fclean dclean re
.SILENT:
