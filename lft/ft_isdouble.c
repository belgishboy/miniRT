/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdouble.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vheymans <vheymans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 17:41:36 by vheymans          #+#    #+#             */
/*   Updated: 2022/05/11 22:33:26 by vheymans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * checks a string is a valid double
 * @param num [char *] string to be checked
 * @return [int] 0 if a valid double; 1 if fail
*/
int	ft_isdouble(char *num)
{
	int	i;

	i = 0;
	while (num[i])
	{
		if (i == 0)
		{
			if (!ft_isdigit(num[i]) && num[i] != '-' && num[i] != '+')
				return (1);
		}
		else if (!ft_isdigit(num[i]) && num[i] != '.')
			return (1);
		i++;
	}
	return (0);
}
