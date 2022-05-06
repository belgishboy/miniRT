#include "../inc/minirt.h"


void	limit(double *var, double upper, double lower)
{
	if (*var > upper)
		*var = upper;
	if (*var < lower)
		*var = lower;
}

void	calc(t_mrt *mrt)
{
	t_data	img;
	int	y;
	int	x;
	t_vec	*scr;

	scr = scream(mrt->cam);
	img.img = mlx_new_image(mrt->mlx, WDTH, HGHT);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length, &img.endian);
	y = 0;
	while (y < HGHT)
	{
		x = 0;
		while (x < WDTH)
		{
			nachfolger(x, y, mrt, scr, &img, FALSE);
			x++;
		}
		y++;
	}
	mrt->img = img.img;
	// need to free scr
}

void	light(t_mrt *mrt, int key)
{
	if (key == 43)
		mrt->l->lr *= 1.125;
	if (key == 45)
		mrt->l->lr *= 0.875;
	if (key == 119)
		mrt->l->cor.z += 5;
	if (key == 97)
		mrt->l->cor.x += 5;
	if (key == 115)
		mrt->l->cor.z -= 5;
	if (key == 100)
		mrt->l->cor.x -= 5;
	if (key == 102)
		mrt->l->cor.y += 5;
	if (key == 98)
		mrt->l->cor.y -= 5;
	limit(&mrt->l->lr, 1, 0);
	calc(mrt);
}

void	camera(t_mrt *mrt, int key)
{
	t_vec	*scr;

	scr = scream(mrt->cam);
	unit(&mrt->cam->v_o);
	if (key == 65362) // UP
		addto(&mrt->cam->v_o, v_product(scr[1], -0.2));
	if (key == 65364) // DOWN
		addto(&mrt->cam->v_o, v_product(scr[1], 0.2));
	if (key == 65361) // left
		addto(&mrt->cam->v_o, v_product(scr[0], -0.2));
	if (key == 65363) // right
		addto(&mrt->cam->v_o, v_product(scr[0], 0.2));
	if (key == 228) // Ä
		mrt->cam->fov = mrt->cam->fov + (10 * (PI / 180));
	if (key == 35) // #
		mrt->cam->fov = mrt->cam->fov - (10 * (PI / 180));
	limit(&mrt->cam->fov, 179 * (PI / 180), 0);
	unit(&mrt->cam->v_o);
	calc(mrt);
}

void	controls(t_mrt *mrt)
{
	char	*red;
	char	*green;
	char	*yellow;
	char	*normal;

	red = "\033[1;31m";
	green = "\033[0;32m";
	yellow = "\033[1;33m";
	normal = "\033[1;30m";
	printf("\n");
	printf("\t%sControls :%s\n\n", red, normal);
	mlx_string_put(mrt->mlx, mrt->win, 10, HGHT + 10, create_trgb(0, -1, 0, 0), "Controls :");
	printf("%sCamera Tilt   %s┏┉┉┉┓%s  Field of vision %s┏┉┉┉┓%s Light                %s┏┉┉┉┓%s               %s┏┉┉┉┓%s\n", normal, green, normal, yellow, normal, red, normal, red, normal);
	printf("%s              %s┇ ⇧ ┇%s                  %s┇ Ä ┇%s      Position        %s┇ W ┇%s     Intensity %s┇ + ┇%s\n", normal, green, normal, yellow, normal, red, normal, red, normal);
	printf("%s          %s┏┉┉┉┛   ┗┉┉┉┓%s              %s┇ ↑ ┇%s                  %s┏┉┉┉┛ ↑ ┗┉┉┉┓%s           %s┇ ↑ ┇%s\n", normal, green, normal, yellow, normal, red, normal, red, normal);
	printf("%s          %s┇ ⇦       ⇨ ┇%s              %s┇   ┇%s   Front   Back   %s┇ A ←   → S ┇%s           %s┇   ┇%s\n", normal, green, normal, yellow, normal, red, normal, red, normal);
	printf("%s          %s┗┉┉┉┓   ┏┉┉┉┛%s              %s┇ ↓ ┇%s   %s┏┉┉┉┉┉┉┉┉┉┉┉┓  ┗┉┉┉┓ ↓ ┏┉┉┉┛%s           %s┇ ↓ ┇%s\n", normal, green, normal, yellow, normal, red, normal, red, normal);
	printf("%s              %s┇ ⇩ ┇%s                  %s┇ # ┇%s   %s┇ F ↙   ↗ B ┇      ┇ D ┇%s               %s┇ - ┇%s\n", normal, green, normal, yellow, normal, red, normal, red, normal);
	printf("%s              %s┗┉┉┉┛%s                  %s┗┉┉┉┛%s   %s┗┉┉┉┉┉┉┉┉┉┉┉┛      ┗┉┉┉┛%s               %s┗┉┉┉┛%s\n", normal, green, normal, yellow, normal, red, normal, red, normal);
	printf("\n");
	printf("T  Toggle Debug info\n");
	printf("C  Show this Menu\n");
	printf("R  Retrace the Pixel at current Mouse-Position\n");
	printf("ESC  Exit Process\n");
	printf("\n");
}

// Trace me baby one more time
void	debug(t_mrt *mrt)
{
	int	x;
	int	y;
	t_vec	ray;
	t_vec	*scr;

	mlx_mouse_get_pos(mrt->mlx, mrt->win, &x, &y);
	scr = scream(mrt->cam);
	printf("ray trough x %i, y %i\n", x, y);
	ray = single_ray(x - (WDTH/2), y - (HGHT/2), mrt->cam, scr);
	nachfolger(x, y, mrt, scr, NULL, TRUE);
}


int	render(t_mrt *mrt)
{
	mlx_put_image_to_window(mrt->mlx, mrt->win, mrt->img, 0, 0);
	return (0);
}

int	end(t_mrt *mrt)
{
	//exit functions
	if (mrt)
		exit(0);
	return (0);
}

int	key_hook(int key, t_mrt *mrt)
{
	static t_bool	swtch;

	if (key == 65307)
		end(mrt);
	if (key == 116) // T
	{
		printf("Toggle debugger\n");
		if (swtch)
			swtch = FALSE;
		else
			swtch = TRUE;
	}
	if (key == 99 && !swtch) // c
		controls(mrt);
	else if (key == 114 && !swtch) // r
		debug(mrt);
	else if (!swtch && (key == 65362 || key == 65364 || key == 65361 || key == 65363 || key == 228 || key == 35))
		camera(mrt, key);
	else if (!swtch && (key == 119 || key == 97 || key == 115 || key == 100 || key == 102 || key == 98 || key == 43 || key == 45))
		light(mrt, key);
	return (0);
}

int	main(int argc, char **argv)
{
	t_mrt	mrt;

	if (argc != 2)
	{
		printf("Error\nIncorrect number of Args\n");
		return (1);
	}
	if (input(&mrt, argv[1]))
		return (printf("ERROR INPUT\n"));
	mrt.mlx = mlx_init();
	mrt.win = mlx_new_window(mrt.mlx, WDTH, HGHT + 100, "I wanna be Tracer");
	calc(&mrt);
	mlx_mouse_show(mrt.mlx, mrt.win);
	mlx_loop_hook(mrt.mlx, render, &mrt);
	mlx_key_hook(mrt.win, key_hook, &mrt);
	mlx_hook(mrt.win, 33, (1L << 17), end, &mrt);
	mlx_loop(mrt.mlx);
	rt_exit(&mrt);
	return (0);
}
