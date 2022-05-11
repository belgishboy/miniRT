/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vheymans <vheymans@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 17:44:47 by vheymans          #+#    #+#             */
/*   Updated: 2022/05/11 22:33:26 by vheymans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

double	i_limit(double var, double upper, double lower)
{
	if (var > upper)
		return (upper);
	if (var < lower)
		return (lower);
	return (var);
}

void	limit(double *var, double upper, double lower)
{
	if (*var > upper)
		*var = upper;
	if (*var < lower)
		*var = lower;
}

int	create_trgb(int t, int r, int g, int b)
{
	return (t << 24 | r << 16 | g << 8 | b);
}

int	color(t_mrt *mrt, t_obj *near, char c, double bright)
{
	double	alp;
	double	lp;

	alp = mrt->al->lr / (mrt->al->lr + mrt->l->lr);
	lp = mrt->l->lr / (mrt->al->lr + mrt->l->lr) * bright;
	limit(&alp, 1, 0);
	limit(&lp, 1, 0);
	if (c == 'r')
		return (i_limit((mrt->al->r * alp + near->r * lp), 255, 0));
	if (c == 'g')
		return (i_limit((mrt->al->g * alp + near->g * lp), 255, 0));
	if (c == 'b')
		return (i_limit((mrt->al->b * alp + near->b * lp), 255, 0));
	return (0);
}

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}
