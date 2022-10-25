/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmabel <jmabel@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/29 17:00:49 by jmabel            #+#    #+#             */
/*   Updated: 2022/10/25 20:58:52 by jmabel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include <fcntl.h>
# include <unistd.h>
# include <sys/types.h> 
# include <sys/uio.h> 
# include <stdio.h> 
# include <stdlib.h>
# include <math.h>

# include "../libft/libft.h"
# include "get_next_line.h"
# include "vector.h"
# include "scene.h"
# include "parser.h"
# include "../libmlx/mlx.h"

# define WIDTH 1920
# define HEIGHT 1080

# define BACKGROUND_COLOR 0

/* set mlx events */
# define ON_DESTROY 17
# define ON_KEYDOWN 2
# define ON_BUTTONPRESS 4

# define ESC 53
# define X_KEYHOOK 7
# define Y_KEYHOOK 16
# define Z_KEYHOOK 6
# define GREATER 47
# define LESS 43
# define UP 126
# define DOWN  125
# define LEFT 123
# define RIGHT 124

typedef struct s_img
{
	void	*img;
	char	*addr;
	int		bpp;
	int		line_length;
	int		endian;
}	t_img;

typedef struct s_global
{
	t_scene		*scene;
	void		*mlx;
	void		*window;
	void		*nearest_obj;
	int			nearest_type;
	int			prev_keyhook;
	t_img		img;
}	t_global;

typedef enum e_type_object
{
	NO_INTERSECT,
	PIPE,
	PLANE_BEGIN,
	PLANE_END,
	PLANE,
	SPHERE,
	CYLINDER,
}	t_type_object;

typedef struct s_pixel
{
	int			x;
	int			y;
	t_ray		ray;
	t_coord		d;
	float		length;
	t_coord		intersection;
	t_plane		*plane;
	t_sphere	*sphere;
	t_cylinder	*cylinder;
	int			cylinder_type;
	float		cylinder_m;
}	t_pixel;

t_scene		*parser(int argc, char **argv);
void		free_scene(t_scene *scene);
int			minirt_close(t_global *data);

void		rotate_objects(t_scene *scene, t_coord *coord);
void		translate_objects(t_scene *scene, t_coord *coord);

/* 	./image/image.c 
	functions for mlx management with image*/
void		ft_mlx_pixel_put_img(t_img	*img, int x, int y, int color);
void		init_image(t_global *data);
void		clean_image(void *img);

/* 	./image/hook.c
	manage events*/
int			hook(t_global *data);
int			key_print_hook(int keycode, t_global *data);

/* ./image/search_objects.c.c
	search objects for hook */
void		ft_search_objects(int x, int y, t_global *global);

/* ./image/resize_objects.c
	resize objects for hook */
void		ft_resize_object(int mousecode, t_global	*global);

/* ./image/traslate_objects.c */
void		translate_objects_keyhook(int keycode, t_global *global);

/*	./raytracing/check_intersection.c
	functions check intersection with objects */
void		check_intersection(t_global *global, t_pixel *pixel);

t_sphere	*check_for_spheres(t_global *global, t_ray *ray,
				t_coord *d, float *dist);
t_plane		*check_for_planes(t_global *global, t_ray *ray,
				t_coord *d, float *dist);
t_cylinder	*check_for_cylinder(t_global *global, t_pixel *pixel, float *dist);

/*	 ./raytracing/intersection.c
	 function returns distance to plane and sphere */
float		check_intersection_plane(t_plane *plane, t_ray *ray, t_coord *d);
float		check_intersection_sphere(t_sphere *sphere, t_ray *ray, t_coord *d);
float		nearest_distance(float	*points);

/* ./raytracing/intersection_cylinder.c */
float		check_intersection_cylinder(t_cylinder	*cylinder, t_pixel *pixel);

/*	./raytracer/rayracer.c */
void		raytracer(t_global *global);

/*	./raytracer/pixel_computing.c */
void		pixel_computing(t_global *global, t_pixel *pixel);
int			check_for_shadow(t_global *global, t_pixel *pixel_initial);

/*	./raytracer/compute_pixel_cyliner.c */

void		pixel_computing_cylinder(t_global *global, t_pixel *pixel);
/*	./raytracer/compute_pixel_sphere.c */
void		pixel_cleaning(t_pixel *pixel);
int			color_diffusal(int color_sum, int color1,
				int color2, float intensity);
void		pixel_computing_sphere(t_global *global, t_pixel *pixel);
float		pixel_computing_sphere_diffusal_recflect_ratio(t_global *global,
				t_pixel *pixel);

/*	./raytracer/compute_pixel_plane.c */
void		pixel_computing_plane(t_global *global, t_pixel *pixel);
float		pixel_computing_plane_diffusal_recflect_ratio(t_global *global,
				t_pixel *pixel);

/*	 ./raytracing/solver.c
	 solver of equations*/
int			solve_quadratic_equation(float a, float b, float c, float roots[2]);

/* utils */
void		print_scene(t_scene	*scene);
void		print_spheres(t_sphere	*sp, char type);
void		print_cylinders(t_cylinder	*cy, char type);
void		print_planes(t_plane	*pl, char type);
void		print_coordinate(t_coord *coord, char *endchar);

#endif