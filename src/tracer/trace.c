/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trace.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlehmann <hlehmann@student.42wolfsburg.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 11:30:42 by hlehmann          #+#    #+#             */
/*   Updated: 2022/05/16 11:30:43 by hlehmann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minirt.h"

/**
 * find the object closest to the camera
 * put the representing color to the position in the image
 * @param x X-coordinate of the current pixel to trace
 * @param y Y-coordinate of the current pixel to trace
 * @param mrt our struct containing objects and camera
 * @param scr information needed to create the ray
 * @param img the resulting image
 * note ot colors: the color is defined by
 * 	1 the object
 * 	2 the ambient light
 * 	3 the angle at which the light hits the object
 */
double	hittem(t_mrt *mrt, t_obj *obj, t_vec ray)
{
	double	d;

	if (obj->id == 'S')
		d = hit_sphere(obj->cor, obj->rad, mrt->cam->cor, ray) / 2;
	if (obj->id == 'P')
		d = hit_plane(mrt->cam->cor, ray, obj);
	if (obj->id == 'C')
		d = hit_circle(mrt->cam->cor, ray, obj);
	if (obj->id == 'O')
		d = hit_line(mrt->cam->cor, ray, obj);
	if (obj->id == 'Z')
		d = hit_cylinder(*obj, mrt->cam->cor, ray);
	return (d);
}

double	nearest(t_mrt *mrt, t_vec ray, t_obj **near, t_bool p)
{
	double	old_d;
	double	d;
	int		i;

	i = 0;
	*near = NULL;
	old_d = RENDER_DISTANCE;
	while (mrt && mrt->obj && mrt->obj[i] && mrt->obj[i]->id)
	{
		d = 0;
		d = hittem(mrt, mrt->obj[i], ray);
		if (d && d > 0 && d < old_d)
		{
			if (!*near || (*near)->id != 'O')
			{
				*near = mrt->obj[i];
				old_d = d;
			}
		}
		if (p && d)
			printf("hit %c\nDistance %lf\n", mrt->obj[i]->id, d);
		i++;
	}
	return (old_d);
}

double	lumen(t_mrt *mrt, t_obj *near, t_vec ray, double d)
{
	t_vec	impact;
	t_vec	light;
	t_vec	norm;
	double	bright;

	impact = v_sum(mrt->cam->cor, v_product(v_unit(ray), d));
	light = v_unit(connect(impact, mrt->l->cor));
	norm = init_vec(0, 1, 0);
	if (near->id == 'S')
		norm = connect(near->cor, impact);
	else if (near->id == 'P' || near->id == 'C')
		norm = near->v_o;
	else if (near->id == 'Z')
		norm = v_invert(cross(near->v_o, cross(near->v_o, \
			connect(near->cor, impact))));
	if ((near->id == 'P' || near->id == 'C') && hell(&norm, &light, &ray))
		return (0);
	bright = 1 - (2 * fabs(angle(light, norm)) / (PI));
	limit(&bright, mrt->l->lr, 0);
	if (near->id == 'O')
		bright = 1;
	return (bright);
}

t_bool	shaed(t_mrt *mrt, t_vec ray, double d)
{
	t_vec	light;
	t_vec	impact;
	int		i;

	impact = v_sum(mrt->cam->cor, v_product(v_unit(ray), d));
	light = v_unit(connect(impact, mrt->l->cor));
	i = 0;
	while (mrt && mrt->obj && mrt->obj[i] && mrt->obj[i]->id)
	{
		d = 0;
		if (mrt->obj[i]->id == 'S')
			d = hit_sphere(mrt->obj[i]->cor, mrt->obj[i]->rad, impact, \
			light) / 2;
		if (mrt->obj[i]->id == 'P')
			d = hit_plane(impact, light, mrt->obj[i]);
		if (mrt->obj[i]->id == 'C')
			d = hit_circle(impact, light, mrt->obj[i]);
		if (mrt->obj[i]->id == 'Z')
			d = hit_cylinder(*mrt->obj[i], impact, light);
		if (d > 0.001 && d < veclen(connect(impact, mrt->l->cor)))
			return (TRUE);
		i++;
	}
	return (FALSE);
}

int	nachfolger(int cord[2], t_mrt *mrt, t_vec *scr, t_bool p)
{
	double	d[3];
	t_obj	*near;
	t_vec	ry;
	int		c;

	ry = single_ray(cord[0] - (WDTH / 2), cord[1] - (HGHT / 2), mrt->cam, scr);
	unit(&ry);
	c = create_trgb(0, 0, 0, 0);
	d[0] = nearest(mrt, ry, &near, p);
	if (!near)
		my_mlx_pixel_put(&mrt->img, cord[0], cord[1], c);
	if (!near)
		return (0);
	d[1] = lumen(mrt, near, ry, d[0]);
	d[2] = mrt->al->lr + mrt->l->lr;
	if (shaed(mrt, ry, d[0]) == FALSE && d[1] > 0)
	c = create_trgb(0, color(mrt, near, 'r', d[1]), color(mrt, near, \
		'g', d[1]), color(mrt, near, 'b', d[1]));
	else
		c = create_trgb(0, i_limit((mrt->al->lr / d[2] * mrt->al->r), 255 * \
		d[2], 0), i_limit((mrt->al->lr / d[2] * mrt->al->g), 255 * d[2], 0), \
		i_limit((mrt->al->lr / (mrt->al->lr + mrt->l->lr) * mrt->al->b), \
		255 * d[2], 0));
	my_mlx_pixel_put(&mrt->img, cord[0], cord[1], c);
	return (0);
}
