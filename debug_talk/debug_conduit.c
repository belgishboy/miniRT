#include "../inc/minirt.h"

typedef struct s_ack
{
	int	acked;
}			t_ack;

// i tried to implement my code but failed, then i took hendricks minitalk code
//	it workd for the first 7 characters si i guess i take that and run with it

int	ft__strlen(char *s)
{
	int i = 0;
	while (s[i])
		i++;
	return(i);
}

int	ft_atoi(const char *nptr)
{
	size_t	i;
	int		sign;
	int		num;

	i = 0;
	sign = 1;
	num = 0;
	while ((nptr[i] > 8 && nptr[i] < 14) || nptr[i] == 32)
		i++;
	if (nptr[i] == 43 || nptr[i] == 45)
	{
		if (nptr[i] == 45)
			sign *= -1;
		i++;
	}
	while (nptr[i] > 47 && nptr[i] < 58)
	{
		num = num * 10 + nptr[i] - '0';
		i++;
	}
	return (num * sign);
}

t_ack	g_ack;

void	send(int pid, char *msg, size_t len)
{
	int		shift;
	size_t	i;

	i = 0;
	while (i <= len)
	{
		shift = 0;
		while (shift <= 7)
		{
			g_ack.acked = 0;
			if ((msg[i] >> shift) & 1)
				kill(pid, SIGUSR2);
			else
				kill(pid, SIGUSR1);
			shift++;
			while (g_ack.acked == 0)
				pause();
			usleep(42);
		}
		i++;
	}
}

void	get_ack(int sigbit, siginfo_t *siginfo, void *context)
{
	(void) sigbit;
	(void) siginfo;
	(void) context;
	g_ack.acked = 1;
}

int	main(int argc, char **argv)
{
	struct sigaction	sig;
	int					pid;

	g_ack.acked = 1;
	sig.sa_sigaction = &get_ack;
	sig.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &sig, NULL);
	if (argc == 3)
	{
		pid = ft_atoi(argv[1]);
		send(pid, argv[2], ft__strlen(argv[2]));
	}
	else
		write(1, "Wrong input!\n", 14);
	return (0);
}
