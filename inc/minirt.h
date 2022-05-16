/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlehmann <hlehmann@student.42wolfsburg.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 11:16:32 by hlehmann          #+#    #+#             */
/*   Updated: 2022/05/16 11:16:34 by hlehmann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

/*
 * DEFINES
 */

# define RENDER_DISTANCE 20000
# define WDTH 640
# define HGHT 640
# define DIVERGENCE 100
# define PI 3.14159265359

/*
**	LIBRARY
*/

# include "../lft/libft.h"
# include "../mlx/mlx.h"

/*
**	INCLUDES
*/

# include <stdio.h>	//  printf, perror
# include <math.h>	//  math lol
# include <fcntl.h> // open
# include <unistd.h> // close, read, write
# include <stdlib.h> // malloc, free, exit
# include <string.h> // strerror

/*
**	STRUCTS
*/

# include "struct.h"

typedef struct s_data
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_data;

typedef struct s_mrt
{
	void	*mlx;
	void	*win;
	t_al	*al;
	t_lol	*l;
	t_cam	*cam;
	t_obj	**obj;
	t_data	img;
}				t_mrt;

/*
 * FUNCTIONS
*/

//GEOMETRY-

t_vec	cross(t_vec a, t_vec b);
t_vec	init_vec(double x, double y, double z);
t_vec	*vec_alloc(t_vec clone);
double	veclen(t_vec a);
t_vec	connect(t_vec a, t_vec b);
void	addto(t_vec *a, t_vec b);
t_vec	v_sum(t_vec a, t_vec b);
void	product(t_vec *a, double m);
t_vec	v_product(t_vec a, double m);
void	resize(t_vec *a, double m);
void	unit(t_vec	*a);
t_vec	v_unit(t_vec	a);
double	angle(t_vec a, t_vec b);
double	calculate_dot(t_vec *a, t_vec *b);
t_vec	v_invert(t_vec a);
t_vec	reflect(t_vec in, t_vec norm);

//TRACER

t_vec	*scream(t_cam *cam);
t_vec	single_ray(int x, int y, t_cam *cam, t_vec	scr[3]);
double	hit_sphere(t_vec sph_org, double sph_rad, t_vec ray_or, t_vec ray_dir);
double	hit_plane(t_vec ray_or, t_vec ray, t_obj *plane);
double	hit_circle(t_vec ray_or, t_vec ray, t_obj *plane);
double	hit_line(t_vec ray_or, t_vec ray, t_obj *plane);
double	new_cylinder_intersect(t_obj cyl, t_vec *ray_or, t_vec *ray_dir);
double	cap_intersection(t_obj cyl, t_vec ray_or, t_vec ray, int d);
double	cap(t_obj cyl, t_vec ray_org, t_vec ray, double off);
double	hit_cylinder(t_obj cyl, t_vec ray_or, t_vec ray_dir);

t_bool	shadow(t_mrt *mrt, t_vec impact, char p);
int		colorme(t_mrt *mrt, t_obj *obj, t_vec ray);
int		nachfolger(int cord[2], t_mrt *mrt, t_vec *scr, t_bool p);

//SRC

int		main(int argc, char **argv);

// EXIT

void	rt_er_exit(t_mrt *mrt, int good, int count);
int		rt_exit(t_mrt *mrt);

// INPUT

void	*extract_line(char **lines, t_mrt *mrt);//proto

//UTILES

int		create_trgb(int t, int r, int g, int b);
void	my_mlx_pixel_put(t_data *data, int x, int y, int color);

char	*trm_whtsp(char *s, int dir);
int		is_whspace(char *s, int dir);
int		ft_white(char c);
char	**split_wh(char const *s);

int		count_input(t_list *lst, int *count, char *tmp);
int		check_count(int *count);
void	limit(double *var, double upper, double lower);
double	i_limit(double var, double upper, double lower);

//INITIALIZATION

int		check_deci(char *s);
int		init_cor(char **info, t_vec *cor, int flag);
int		init_rgb(char **info, int *r, int *g, int *b);
int		init_fov(char *info, double *fov);
int		init_dim(char *info, double *dim, int flag);

int		init_al(t_al *al, char **info);
int		init_cam(t_cam *ca, char **info);
int		init_lol(t_lol *l, char **info);
int		init_lol_b(t_lol_b *l, char **info);

int		init_sph(t_obj **sp, char **info, int p);
int		init_pl(t_obj **pl, char **info, int p);
int		init_cyl(t_obj **cy, char **info, int p);

//FUNCTIONS

int		input(t_mrt *mrt, char *file);

//DEBUG

void	controls(void);
void	print_config(t_mrt *mrt, int i);
void	debug(t_mrt *mrt);
void	print_object(t_obj *obj);
void	move(t_mrt *mrt, int key);
void	camera(t_mrt *mrt, int key);
void	light(t_mrt *mrt, int key);
void	origin(t_mrt *mrt, int i);
t_obj	*line_obj(t_vec	origin, t_vec direction, char color);
void	expand_obj(t_mrt *mrt);
void	flip(t_mrt *mrt);
int		color(t_mrt *mrt, t_obj *near, char c, double bright);
void	calc(t_mrt *mrt);
double	hell(t_vec *norm, t_vec *light, t_vec *ray);

#endif
