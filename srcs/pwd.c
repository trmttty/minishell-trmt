/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttarumot <ttarumot@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/07 03:24:43 by ttarumot          #+#    #+#             */
/*   Updated: 2021/01/23 00:32:32 by ttarumot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		ft_pwd(char **args, t_list **env_lst)
{
    char    buf[MAXPATHLEN];
    char    *ret;

    if ((ret = getcwd(buf, MAXPATHLEN)) != NULL)
    {
        ft_putendl_fd(buf, 1);
        return (1);
    }
    return (0);
}
