#include "../../inc/minirt.h"

/**
 * @brief to initialize i use an imaginary screen plane, to which i point my rays
 * @param	cam [t_cam*] the camera in the scene
 * @return	[double**] [0]: vector on the screenplane, orthogonal to the g_z axis
 * 					[1]: vector on the screenplane, orthogonal to the vector in [0]
 * 					[2]: direction vector of the correct length
 * distance = acos (FOV/2) * ("wdth")
*/
double	**scream(t_cam *cam)
{
	double	**res;
	double	*tmp;

	res = malloc(3 * sizeof(double **));
	res[0] = malloc(3 * sizeof(double *));
	if (cam->cor[0] == 0 && cam->cor[1] == 0)
		res[0] = ray_alloc(1, 0, 0);
	else
	{
		tmp = ray_alloc(0, 0, 1);
		res[0] = cross(cam->v_o, tmp);
		unit(res[0]);
		free(tmp);
	}
	res[1] = malloc(3 * sizeof(double *));
	res[1] = cross(cam->v_o, res[0]);
	unit(res[1]);
	res[2] = malloc(3 * sizeof(double));
	res[2] = ray_alloc(cam->v_o[0], cam->v_o[1], cam->v_o[2]);
	resize(res[2], cos(cam->fov / 2) * (((wdth * divergence) / 2) / cos(pi / 2 - (cam->fov / 2))));
	// printf("sv1 %lf %lf %lf\nsv2 %lf %lf %lf\ndst %lf %lf %lf\nbreite %lf\n", res[0][0], res[0][1], res[0][2], res[1][0], res[1][1], res[1][2], res[2][0], res[2][1], res[2][2], (wdth * divergence) / 2);
	return(res);
}

/**
 * NOTE: the offset is from the middle ray to a projection screen
 * meaning that to represent all pixels x and y need to start at -(max / 2)
 * @param x [int] horizontal pixelcoordinate
 * @param y [int] vertical pixelcoordinate
 * @param cam [t_cam*] originating viewpoint
 * @param scr [double**] defining parameters of the projection screen
 * @brief return a ray representing a pixel with the xy offset given
*/
double	*single_ray(int x, int y, t_cam *cam, double **scr)
{
	double	addict[3];
	double	*tmp;
	double	*res;

	addict[0] = cam->cor[0];
	addict[1] = cam->cor[1];
	addict[2] = cam->cor[2];
	addto(addict, scr[2]);
	tmp = ray_alloc(scr[0][0], scr[0][1], scr[0][2]);
	product(tmp, x * divergence);
	addto(addict, tmp);
	free(tmp);
	tmp = ray_alloc(scr[1][0], scr[1][1], scr[1][2]);
	product(tmp, y * divergence);
	addto(addict, tmp);
	free(tmp);
	res = connect(cam->cor, addict);
	return (res);
}

/**
 * @brief calculate and set a ray foreach pixel to be displayed
 * @param mrt [t_mrt*] our main struct
*/
void	init_rays(t_mrt *mrt)
{
	int		x;
	int		y;
	double	**screen;

	screen = scream(mrt->cam);
	y = 0;//- hght / 2;
	mrt->ray = ft_calloc(hght + 3, sizeof(double **));
	if (!mrt->ray)
		printf("malloc_error\n");
	while(y < hght/* / 2*/)
	{
		mrt->ray[y] = ft_calloc(wdth + 3, sizeof(double *));
		if (!mrt->ray[y])
			printf("malloc_error\n");
		x = 0;//- wdth / 2;
		while (x < wdth/* / 2*/)
		{
			(((mrt->ray)[y])[x]) = single_ray(x - (wdth/2), y - (hght/2), mrt->cam, screen);
			x++;
		}
		y++;
	}
}