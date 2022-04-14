
#ifndef RT_INPUT_H
# define RT_INPUT_H

# include "../../inc/minirt.h"

//UTILES

char	*trm_whtsp(char *s, int dir);
int		is_whspace(char *s, int dir);
int		ft_white(char c);

int		count_input(t_list *lst, int *count[6], char *tmp);
int		check_count(int count[6]);

int		init_cor(char **info, double *cor[3], int flag);
int		check_deci(char *s);

//FUNCTIONS

int		input(t_mrt *mrt, char *file);

#endif