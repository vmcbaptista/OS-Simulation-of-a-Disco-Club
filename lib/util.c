
/* Fun�es utilit�rias retiradas de "UNIX Networking Programming" */

/* Le nbytes de um ficheiro/socket.
   Bloqueia ate conseguir ler os nbytes ou dar erro */

int readn(int fd,char* ptr,int  nbytes)
{
	int nleft, nread;

	nleft = nbytes;
	while (nleft > 0) {
		nread = read (fd, ptr, nleft);
		if (nread < 0)
			return (nread);
		else if (nread == 0)
			break;

		nleft -= nread;
		ptr += nread;
	}
	return (nbytes - nleft);
}

/* Escreve nbytes num ficheiro/socket. 
   Bloqueia ate conseguir escrever os nbytes ou dar erro */

int writen(int fd,char* ptr,int nbytes)
{
	int nleft, nwritten;

	nleft = nbytes;
	while (nleft > 0) {
		nwritten = write(fd, ptr, nleft);
		if (nwritten <= 0)
			return(nwritten);

		nleft -= nwritten;
		ptr += nwritten;
	}
	return(nbytes - nleft);
}

/* L� uma linha de um ficheiro/socket (at� \n, maxlen ou \0).
   Bloqueia at� ler a linha ou dar erro.
   Retorna quantos caracteres conseguiu ler */

int readline(int fd,char* ptr,int maxlen)
{
	int n, rc;
	char c;

	for (n=1; n<maxlen; n++) {
		if ((rc = read(fd, &c, 1)) == 1) {
			*ptr++ = c;
			if (c == '\n')
				break;
		} else if (rc == 0) {
			if (n == 1)
				return(0);
			else
				break;
		} else
			return (-1);
	}

	/* N�o esquecer de terminar a string */

	*ptr = 0;

	/* Note-se que n foi incrementado de modo a contar
	   com o \n ou \0 */

	return (n);
}


/* Mensagem de erro */

err_dump(char* msg)
{
	perror(msg);
	exit(1);
}

/* Limpa array de ponteiros */

bzero(char * p,int n)
{
        for (; n>0; n--) {
                *p++ = 0;
        }
}

/* Copia array de ponteiros */

bcopy(char* p,char *q,int n)
{
        for (; n>0; n--) {
                *q++ = *p++;
        }
}
