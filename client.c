#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 9090
#define BUF_SIZE 256

static int read_line(const int fd, char *buf, const size_t size)
{
	size_t i = 0;
	while (i + 1 < size) {
		char c;
		ssize_t n = recv(fd, &c, 1, 0);
		if (n <= 0) {
			return (int)n;
		}
		buf[i++] = c;
		if (c == '\n') {
			break;
		}
	}
	buf[i] = '\0';
	return (int)i;
}

int main(int argc, char *argv[])
{
	const char *host = "127.0.0.1";
	if (argc > 1) {
		host = argv[1];
	}

	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) {
		perror("socket");
		return 1;
	}

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	if (inet_pton(AF_INET, host, &addr.sin_addr) <= 0) {
		fprintf(stderr, "invalid address: %s\n", host);
		close(fd);
		return 1;
	}

	if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("connect");
		close(fd);
		return 1;
	}

	char buf[BUF_SIZE];
	if (read_line(fd, buf, sizeof(buf)) > 0) {
		printf("%s", buf);
	}

	printf("commands: push <n> | pop | print | quit\n");

	for (;;) {
		printf("> ");
		fflush(stdout);

		if (fgets(buf, sizeof(buf), stdin) == NULL) {
			break;
		}

		/* send as-is (includes newline) */
		if (send(fd, buf, strlen(buf), 0) < 0) {
			perror("send");
			break;
		}

		if (strncasecmp(buf, "quit", 4) == 0) {
			if (read_line(fd, buf, sizeof(buf)) > 0) {
				printf("%s", buf);
			}
			break;
		}

		if (read_line(fd, buf, sizeof(buf)) <= 0) {
			printf("server closed connection\n");
			break;
		}
		printf("%s", buf);
	}

	close(fd);
	return 0;
}
