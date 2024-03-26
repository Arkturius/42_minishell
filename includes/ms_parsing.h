/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_parsing.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 14:23:29 by rgramati          #+#    #+#             */
/*   Updated: 2024/03/22 17:13:31 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MS_PARSING_H
# define MS_PARSING_H

/**
 * @brief			Quote state updater.
 * 
 * @param c			Current character.
 * @param qs		Current auote state adress.
 * 
 * @return			! if quote state changed, 0 otherwise.
*/
int			ms_qs_update(char c, t_qstate *qs);

/**
 * @brief			Tokenize a string.
 * 
 * @param str		String to tokenize.
 * @param qs		Quote state.
 * 
 * @return			t_token linked list.
*/
t_token		*ms_tokenizer(char *str, t_qstate qs, int (*tk)(char *, t_qstate));

/* STRING MANIPULATION ****************************************************** */

/**
 * @brief			Enforce strict quoting.
 * 
 * @param str		Pointer to string to check.
 * @param qs		Carried quote state.
*/
void		ms_quote_enforcer(char **str, int tmp_file_fd, t_qstate qs);

/**
 * @brief			Check for syntax error.
 * 
 * @param str		String to check.
 * @param end		String ending.
 * @param qs		Quote_state.
 * 
 * @return			Syntax error char.		
*/
t_qstate	ms_quote_error(char *str, char *end, t_qstate qs);

/**
 * @brief			Get quoted string from bad input.
 * 
 * @param line		Actual prompt line with wrong quote syntax.
 * @param envp		Environment adress to free it in child.
 * @param tmp		Temporary file name.
 * 
 * @return			Quoted string or NULL if error.
*/
int			ms_get_dquote(char *line, t_envvar **env, char *tmp);

/**
 * @brief			Get dquote line from dquote file
 * 
 * @param line		Actual line.
 * @param tmp_file	Temporary file name.
 * @param status	Error code from dquote fork.
 * 
 * @return			Dquote line.
*/
char		*ms_get_dquote_line(char *line, char *tmp_file, int status);

/**
 * @brief			Open dquote prompt, joins and write into tmp file.
 * 
 * @param tmp_fd	Temporary file descritpor.
 * @param qs		Quote state.
 * 
 * @return			Dquote readline return.
*/
char		*ms_open_dquote(int tmp_fd, t_qstate qs);

/**
 * @brief			Replace $VARS by their values if existing.
 * 
 * @param vars		Linked list.
 * @param str		String to format.
 * @param qs		Carried quote state.
*/
void	\
	ms_replace_vars(t_envvar *vars, char **str, t_qstate qs, int no_q);

/**
 * @brief			Insert environment variable into a string array.
 * 
 * @param vars		Linked list.
 * @param start		String to extract from.
 * @param n			Array to add to.
 * @param qs		Quote state.
*/
void	\
	ms_manage_vars(t_envvar *vars, char *start, char ***new, t_qstate qs);

/**
 * @brief			Dequote a string.
 * 
 * @param str		String adress.
 * @param qs		Quote state.
*/
void		ms_dequote_string(char **str, t_qstate qs);

/**
 * @brief			Check if a string starts by a token.
 * 
 * @param str		String to analyze.
 * @param qs		Carried quote state.
 * 
 * @return			1 if the string is headed by a token, 0 otherwise.
*/
int			ms_is_token(char *str, t_qstate qs);

/**
 * @brief			Check if a brace group contains a binary operator.
 * 
 * @param tokens	Token linked list.
 * 
 * @return			1 in case of valid group, 0 otherwise.
*/
int			ms_verif_binop_brace(t_token *tk);

/**
 * @brief			Update linked list, deleting useless brace tokens.
 * 
 * @param tokens	Linked list.
*/
void		ms_remove_braces(t_token **tokens);

/**
 * @brief			Check if a token linked list is valid.
 * 
 * @param tokens	Token linked list.
 * @param err_token	Error token string.
 * 
 * @return			1 if the linked list is valid, 0 otherwise.
*/
int			ms_verify_token(t_token **tokens, char **err_token);

/* WILDCARD ***************************************************************** */

/**
 * @brief			Verify if a string is a wildcard string.
 * 
 * @param str		String to check.
 * @param qs		Quote state.
 * 
 * @return			1 if the string contains only *, 0 otherwise.
*/
int			ms_verify_wildcard(char *str, t_qstate qs);

/**
 * @brief			Get current directory wildcard string.
 * 
 * @param wcstr		wilcard string rule.
 * 
 * @return			Wildcard string array, not sorted.
*/
char		**ms_wildcard_array(char *wcstr);

/**
 * @brief			Format a wildcard string array by sorting.
 * 
 * @param files		String Array pointer.
*/
char		*ms_format_wildcard(char ***str);

/**
 * @brief			Format tokens (wildcard && quotes).
 * 
 * @param tokens	Token linked list adress.
 * @param home		HOME t_envvar pointer.
*/
void		ms_tilde_expansion(t_token **tokens, t_envvar *home);

/**
 * @brief			Replace a string by its wildcard equivalent if there is one.
 * 
 * @param str		String adress.
*/
void		ms_replace_wildcard(char **str);

/* TREE ********************************************************************* */

/**
 * @brief			Recursively build a binary syntax tree.
 * 
 * @param tokens	Tokens linked list.
 * @param env		Environment linked list.
*/
t_node		*ms_build_tree(t_token *tokens, t_envvar **env);

/* UTILS ******************************************************************** */

/**
 * @brief			Get prompt total string.
 *
 * @return			Formatted total prompt string.
*/
char		*ms_get_prompt_string(void);

/**
 * @brief			Print MinishellRC logo.
 * 
 * @param envp		Envp linked list.
*/
void		ms_print_logo(t_envvar *envp);

/**
 * @brief			Unquoted string len.
 * 
 * @param str		String.
 * @param qs		Carried quote state.
 * 
 * @return			Unquoted len of the string.
*/
int			ms_dqstrlen(char *str, t_qstate qs);

/**
 * @brief			Skip a part of a string, updating its pointer and qs
 * 
 * @param str		String adress.
 * @param len		Len to skip.
 * @param qs		Quote state.
*/
void		ms_quoted_skip(char **str, int len, t_qstate *qs);

/**
 * @brief			Splits a string, respecting quoting rules.
 * 
 * @param str		string to split.
 * @param sep		string of separators to cut.
 * 
 * @return			A NULL-terminated string array of str splitted by seps.
*/
char		**ms_quoted_split(char *str, char *sep);

/* ************************************************************************** */

#endif