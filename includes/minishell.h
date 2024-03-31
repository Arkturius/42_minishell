/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                         :+:      :+:    :+:  */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 16:16:19 by marvin            #+#    #+#             */
/*   Updated: 2024/01/16 16:16:19 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* INCLUDES ***************************************************************** */

# include <unistd.h>

# include <stdio.h>

# include <stdlib.h>

# include <stdarg.h>

# include <fcntl.h>

# include <signal.h>

# include <errno.h>

# include <readline/readline.h>

# include <readline/history.h>

# include <sys/types.h>

# include <sys/wait.h>

# include <sys/stat.h>

# include <termios.h>

# include <dirent.h>

# include "libft.h"

# include "ms_enumerates.h"

# include "ms_data_structures.h"

# include "ms_parsing.h"

# include "ms_builtins.h"

# include "ms_file_manager.h"

# include "ms_execution.h"

/* PROMPTS ****************************************************************** */

// Success error prompt head.
# ifndef P_SUCCESS
#  define P_SUCCESS "\001\033[32;1m\002$?\001\033[0m\002 "
# endif

// Fail error prompt head.
# ifndef P_FAIL
#  define P_FAIL "\001\033[31;1m\002$?\001\033[0m\002 "
# endif

// Tail of the prompt.
# ifndef P_TAIL
#  define P_TAIL "\001\033[37;1m\002 Minishell$\001\033[0m\002 "
# endif

// Single quote prompt.
# ifndef P_SDQUOTE
#  define P_SDQUOTE "\001\033[36;1m\002''  dquote:\001\033[0m\002 > "
# endif

// Double quote prompt.
# ifndef P_DDQUOTE
#  define P_DDQUOTE "\001\033[34;1m\002\"\"  dquote:\001\033[0m\002 > "
# endif

// Heredoc prompt.
# ifndef P_HEREDOC
#  define P_HEREDOC "\001\033[35;1m\002HD  here-doc:\001\033[0m\002 > "
# endif

// Error prompt display.
# ifndef P_ERROR
#  define P_ERROR "\001\033[31;1m\002/!\\ ERROR! > "
# endif

// Warning prompt display.
# ifndef P_WARNING
#  define P_WARNING "\001\033[33;1m\002/!\\ WARNING! > "
# endif

/* ERROR MESSAGES *********************************************************** */

# ifndef ES_NOTSET
#  define ES_NOTSET "%scd: %s not set\n"
# endif

# ifndef ES_NOFORD
#  define ES_NOFORD "%s%s no such file or directory\n"
# endif

# ifndef ES_TMARGS
#  define ES_TMARGS "%s%s: too many arguments\n"
# endif

# ifndef ES_NOTNUM
#  define ES_NOTNUM "%s%s: numeric argument required\n"
# endif

# ifndef ES_NOTVAL
#  define ES_NOTVAL "%sexport: `%s': not a valid identifier\n"
# endif

# ifndef ES_NOPERM
#  define ES_NOPERM "%s%s: Permission denied\n"
# endif

# ifndef ES_ISADIR
#  define ES_ISADIR "%s%s: Is a directory\n"
# endif

# ifndef ES_ISNDIR
#  define ES_ISNDIR "%s%s: Not a directory\n"
# endif

# ifndef ES_INVOPT
#  define ES_INVOPT "%s%c%c: Invalid option\n"
# endif

# ifndef ES_HDSTOP
#  define ES_HDSTOP "%s here-document delimited by end-of-file, (wanted `%s')\n"
# endif

# ifndef ES_DQSTOP
#  define ES_DQSTOP "%sunexpected EOF while looking for matching `%s\'\n"
# endif

# ifndef ES_SYNTXQ
#  define ES_SYNTXQ "%ssyntax error%s\n"
# endif

# ifndef ES_SYNTXT
#  define ES_SYNTXT "%ssyntax error near unexcepted token `%s'\n"
# endif

# ifndef ES_SYNTXL
#  define ES_SYNTXL "%ssyntax error\n"
# endif

# ifndef ES_HLIMIT
#  define ES_HLIMIT "%smaximum here-document count exceeded\n"
# endif

# ifndef ES_INVFDS
#  define ES_INVFDS "%s%s: no more fds. Aborting\n"
# endif

# ifndef ES_AMBRED
#  define ES_AMBRED "%s%s: ambiguous redirect\n"
# endif

# ifndef ES_NOARGS
#  define ES_NOARGS "%s%s: argument required\n"
# endif

/* OPENING MODES ************************************************************ */

// O_RDONLY = 00
# ifndef OPEN_READ
#  define OPEN_READ 00
# endif

// O_WRONLY = 01 | O_CREAT = 0100 | O_TRUNC = 01000
# ifndef OPEN_CREATE
#  define OPEN_CREATE 01101
# endif

// O_WRONLY = 01 | O_CREAT = 0100 | O_APPEND = 02000
# ifndef OPEN_APPEND
#  define OPEN_APPEND 02101
# endif

// O_WRONLY = 01 | O_EXCL = 0200 | O_CREAT = 0100 | O_TRUNC = 01000
# ifndef OPEN_EXCL
#  define OPEN_EXCL 01301
# endif

/* PROMPTING **************************************************************** */

/**
 * @brief			Handle quote syntax.
 * 
 * @param line		Line adress to handle.	
*/
t_error		ms_quote_handler(char **line, t_envvar **envp, int status);

/**
 * @brief			Handle line typing, history and quote-related errors.
 * 
 * @param envp		Environment linked list adress.
 * @param line		Line allocation adress. (from ms_prompt_handler).
 * 
 * @return			ERR_FAILED, ERR_DQSTOP if an error occurs,
 * 					ERR_NOERRS otherwise.
*/
t_error		ms_prompt_line(char **line, t_envvar **envp);

/**
 * @brief			Handle line to tokens transformation.
 * 
 * @param tokens	Tokens linked list adress. (from ms_prompt_handler).
 * @param line		Line from line handler.
 * @param envp		Environment linked list adress.
 * 
 * @return			ERR_FAILED in case of error during tokenization,
 * 					ERR_NOERRS otherwise.
*/
t_error		ms_to_tokens(t_token **tokens, char *line, t_envvar **envp);

/**
 * @brief			Handle tokens to tree transformation and execution.
 * 
 * @param tokens	Tokens linked list adress. (from ms_prompt_handler).
 * @param tree		Tree adress (from ms_prompt_handler).
 * @param envp		Environment linked list adress.
 * 
 * @return			ERR_HDSTOP in case of Heredoc problem,
 * 					ERR_NOERRS otherwise.
*/
t_error		ms_to_tree_exec(t_token **tokens, t_node **tree, t_envvar **envp);

/**
 * @brief			Handle prompt loop.
 * 
 * @param envp		Environment linked list adress.
*/
void		ms_prompt_handler(t_envvar **envp);

/* CORE UTILS *************************************************************** */

/**
 * @brief			Adress holder for dquotes allocations needed to be freed
 * 					by a signal handler.
 * 
 * @param addr		Adress of the element to save.
 * @param type		Type of the element (0 for line_save, 1 for dquote fd).
 * 
 * @return			Last known adress of the asked element.
*/
char		*ms_dq_holder(char *addr, int type);

/**
 * @brief			Adress holder for heredoc allocations needed to be freed
 * 					by a signal handler.
 * 
 * @param addr`		Adress of the element to save.
 * @param type		Type of the element (0 for filename save, 1 for delim save,
 * 					2 for heredoc fd)
 * 
 * @return			Last known adress of the asked element.
*/
char		*ms_hd_holder(char *addr, int type);

/**
 * @brief			Adress holder for the tree root.
 * 
 * @param reset		Reset boolean. 1 to reset.
 * @param root		Tree root's pointer.
 * 
 * @return			Last known adress for the root node.
*/
t_node		*ms_tree_holder(int reset, t_node *root);

/**
 * @brief			Signal handler switch.
 * 
 * @param toggle	Signal handler to branch on main process. see enum.
*/
void		ms_signal_state(int toggle);

/**
 * @brief			Error handler
 * 
 * @param err		Error code linked to the error.
 * @param str		String part for the actual message.
*/
void		ms_error_message(t_error err, char *str);

/* ************************************************************************** */

#endif