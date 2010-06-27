#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

/* check wait status to see if child process finished abnormaly */
int wait_status_correct (int status)
{
	if (WEXITSTATUS(status)!= 0) {
		fprintf (stderr, "process finished abnormally. exit status: %d\n", WEXITSTATUS(status));
		return 0;
	}
	if (WIFSIGNALED(status)) {
		if (WCOREDUMP(status)) {
			fprintf (stderr, "process finished abnormally. core dumped\n");
			return 0;
		}

		int signum= WTERMSIG(status);
		if (signum== SIGSEGV) {
			fprintf (stderr, "process finished abnormally. segmentation fault\n");
			return 0;
		}
		if (signum== SIGBUS) {
			fprintf (stderr, "process finished abnormally. bus error\n");
			return 0;
		}
	}
	return 1;
}

int main (int argc, char **argv)
{

#ifndef NDEBUG
	printf ("Use: %d, Group: %d. Eff. usr: %d, Eff. group: %d\n", getuid(), getgid(), geteuid(), getegid());
#endif	

	/* set desired priority */
	if (nice(-11)== -1) {
		printf ("%s: this program needs root privileges\n", argv[0]);
		return -1;
	}

	/* privileges revocation */	
	setuid (getuid()); seteuid (getuid());
	setgid (getgid()); setegid (getgid());

	/* run eviacam until normal exit */
	for (;;) {
		/* create child process to run eviacam */
		pid_t pid= fork();
		if (pid== -1) {
			perror ("fork");
			return -1;
		}
		else if (pid== 0) {
			/* child process */
			return execl ("/usr/bin/eviacam", "eviacam", NULL);
			/* return execl ("./foo", "foo", NULL); */
		}
		else
		{
			/* parent process */
			int status;

			if (wait(&status)== -1) {
				perror ("wait");
				return -1;
			}

			/* check whether the child process finished abnormaly */
			if (!wait_status_correct(status)) {
				fprintf (stderr, "%s: abnormal process termination, restarting\n", argv[0]);
				sleep (1);
			}
			else	break;
		}

	}

#ifndef NDEBUG
	printf ("Use: %d, Group: %d. Eff. usr: %d, Eff. group: %d\n", getuid(), getgid(), geteuid(), getegid());
#endif	
	return 0;
}

