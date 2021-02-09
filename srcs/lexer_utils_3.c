/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils_3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttarumot <ttarumot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/18 12:26:55 by ttarumot          #+#    #+#             */
/*   Updated: 2021/02/07 13:54:57 by ttarumot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"
#include "token.h"

int			ft_isescape(t_lexer *lexer)
{
	if (lexer->c == '\\' && lexer->quote != '\'')
	{
		if (lexer->quote == 0)
			return (1);
		if (lexer->quote == '"' && ft_strchr("$`\"\\\n", lexer->nc))
			return (1);
	}
	return (0);
}

int			lexer_skip_quote(t_lexer *lexer)
{
	if (lexer->quote && lexer->quote == lexer->c && !lexer->env)
		return (1);
	else if (!lexer->quote && ft_isquote(lexer->c)
			&& !(lexer->nc == ' ' || lexer->nc == '\0') && !lexer->env)
		return (1);
	return (0);
}

int			lexer_escape_string(t_lexer *lexer, char **value)
{
	char	*str;
	size_t	size;

	if (lexer->quote == 0 && lexer->nc == '\n')
	{
		lexer_advance(lexer);
		lexer_advance(lexer);
	}
	else if (lexer->quote == 0)
		lexer_advance(lexer);
	else if (lexer->quote == '"' && ft_strchr("$`\"\\\n", lexer->nc))
		lexer_advance(lexer);
	str = lexer_get_current_char(lexer);
	size = ft_strlen(*value) + ft_strlen(str) + 1;
	if ((*value = ft_realloc(*value, size * sizeof(char))) == NULL)
		ft_perror("minishell");
	ft_strlcat(*value, str, size);
	free(str);
	if (!lexer->quote && ft_strchr(";|<>", lexer->nc))
		return (1);
	lexer_advance(lexer);
	return (0);
}

static void	update_contens(t_lexer *lexer, char **value)
{
	char	*tmp;

	tmp = lexer->contents;
	if (ft_isquote(lexer->c))
		lexer->i++;
	lexer->env = ft_strlen(*value);
	lexer->contents = ft_strjoin(*value, &lexer->contents[lexer->i]);
	free(tmp);
	free(*value);
	*value = ft_strdup("");
	lexer->i = 0;
	lexer->pc = 0;
	lexer->c = lexer->contents[0];
	if (ft_strlen(lexer->contents))
		lexer->nc = lexer->contents[1];
	else
		lexer->nc = 0;
	while (!lexer->quote && (lexer->c == ' ' || lexer->c == '\t'))
		lexer_skip_whitespace(lexer);
}

int			lexer_expand_env(t_lexer *lexer, char **value)
{
	while (lexer->c == '$' && lexer->quote != '\'')
		replace_environ(lexer, value);
	if (!lexer->quote && !ft_isquote(lexer->c) && ft_strcmp(*value, "$"))
	{
		if (ft_strlen(*value) == 0 && (ft_isquote(lexer->c)
			|| ft_strchr(" \t", lexer->c) || lexer->c == '\0'))
			return (1);
		update_contens(lexer, value);
	}
	return (0);
}