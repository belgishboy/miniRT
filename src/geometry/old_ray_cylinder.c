#include "../../inc/minirt.h"

#define EPSILON 0.00001

double	*get_vector(double *point_one, double *point_two)
{
	double	*vec;

	vec = malloc(sizeof(double) * 3);
	vec[0] = point_two[0] - point_one[0];
	vec[1] = point_two[1] - point_one[1];
	vec[2] = point_two[2] - point_one[2];
	return (vec);
}

t_vec	vec_product(t_vec a, double m)
{
	t_vec	res;
 
	res.x = a.x * m;
	res.y = a.y * m;
	res.z = a.z * m;
	return (res);
}

int	cylinder_coefficient(t_vec *pos, t_vec *dir, double radius, double *x, t_vec *ray_or, t_vec *ray_dir)
{
	t_vec	v;
	t_vec	u;
	t_vec	normalized;

	normalized = *dir;
	unit(&normalized);
	v = vec_product(normalized, calculate_dot(ray_dir, &normalized));
	v = connect(v, *ray_dir);
	t_vec tmp = connect(*pos, *ray_or);
	u = vec_product(normalized, calculate_dot(&tmp, &normalized));
	u = connect(u, connect(*pos, *ray_or));
	double a = calculate_dot(&v, &v);
	double b = 2 * calculate_dot(&v, &u);
	double c = calculate_dot(&u, &u) - pow(radius, 2);
	x[0] = (-b + sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
	x[1] = (-b - sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
	if ((x[0] != x[0] && x[1] != x[1]) || (x[0] < EPSILON && x[1] < EPSILON))
	{
		x[0] = INFINITY;
		x[1] = INFINITY;
		return (0);
	}
	return (1);
}

t_vec	calc_cy_normal(double *x, t_vec pos, t_vec dir, double *dist, t_vec ray_or, t_vec ray_dir, double height)
{
	double	d;
	double	t;
	t_vec	normalized;

	normalized = dir;
	unit(&normalized);
	if ((dist[0] >= 0 && dist[0] <= height && x[0] > EPSILON) && (dist[1] >= 0 && dist[1] <= height && x[1] > EPSILON))
	{
		d = x[0] < x[1] ? dist[0] : dist[1];
		t = x[0] < x[1] ? x[0] : x[1];
	}
	else if(dist[0] >= 0 && dist[0] <= height && x[0] > EPSILON)
	{
		d = dist[0];
		t = x[0];
	}
	else
	{
		d = dist[1];     
		t = x[1];
	}
	x[0] = t;
	t_vec	tmp = connect(connect(ray_or, pos), connect(vec_product(normalized, d), vec_product(ray_dir, t)));
	unit(&tmp);
	return (tmp);
}

double	new_cylinder_intersect(t_vec *pos, t_vec *dir, double radius, double height, t_vec *ray_or, t_vec *ray_dir)
{
	double	x[2];
	double	dist[2];
	t_vec	normalized;

	normalized = *dir;
	unit(&normalized);
	if (cylinder_coefficient(pos, dir, radius, x, ray_or, ray_dir) == 0)
		return (0);

	t_vec helper = connect(connect(*ray_or, *pos), vec_product(*ray_dir, x[0]));
	dist[0] = calculate_dot(&normalized, &helper);
	helper = connect(connect(*ray_or, *pos), vec_product(*ray_dir, x[1]));
	dist[1] = calculate_dot(&normalized, &helper);
	if (!((dist[0] >= 0 && dist[0] <= height && x[0] > EPSILON) || (dist[1] >= 0 && dist[1] <= height && x[0] > EPSILON)))
		return (0);
	calc_cy_normal(x, *pos, *dir, dist, *ray_or, *ray_dir, height);
	return (x[0]);
}
double	hit_cylinder(t_vec *posi, t_vec *dir, double radius, double height, t_vec *ray_or, t_vec *ray_dir)
{
	double	cylinder_inter;
	// double	cap_inter;

	cylinder_inter = new_cylinder_intersect(posi, dir, radius, height, ray_or, ray_dir);
	// limit(&cylinder_inter, RENDER_DISTANCE, 0);
	// cap_inter = cap_intersection(posi, dir, radius, height, ray_or, ray_dir);
	// limit(&cap_inter, RENDER_DISTANCE, 0);
	// if (cap_inter < cylinder_inter)
	// 	return (cap_inter);
	// else
		return (cylinder_inter);
}