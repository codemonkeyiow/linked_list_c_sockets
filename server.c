#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "linkedlist.h"

#define PORT 9090
#define BUF_SIZE 256

static void send_msg(const int fd, const char *msg)
{
	size_t len = strlen(msg);
	ssize_t sent = send(fd, msg, len, 0);
	if (sent < 0) {
		perror("send");
	}
}

static void handle_client(const int client_fd, LinkedList *list)
{
	char buf[BUF_SIZE];
	char reply[BUF_SIZE];

	send_msg(client_fd, "Welcome. Commands: PUSH <n> | POP | PRINT | QUIT\n");

	for (;;) {
		memset(buf, 0, sizeof(buf));
		ssize_t n = recv(client_fd, buf, sizeof(buf) - 1, 0);
		if (n <= 0) {
			break;
		}

		/* trim trailing newline/CR */
		while (n > 0 && (buf[n - 1] == '\n' || buf[n - 1] == '\r')) {
			buf[--n] = '\0';
		}

		if (n == 0) {
			continue;
		}

		printf("client cmd: '%s'\n", buf);

		if (strncasecmp(buf, "QUIT", 4) == 0) {
			send_msg(client_fd, "BYE\n");
			break;
		}

		if (strncasecmp(buf, "PUSH", 4) == 0) {
			int value = 0;
			if (sscanf(buf + 4, "%d", &value) != 1) {
				send_msg(client_fd, "ERR usage: PUSH <int>\n");
				continue;
			}
			push(value, list);
			snprintf(reply, sizeof(reply), "OK pushed %d (length %zu)\n",
				 value, list->length);
			send_msg(client_fd, reply);
			continue;
		}

		if (strncasecmp(buf, "POP", 3) == 0) {
			if (list->length == 0) {
				send_msg(client_fd, "EMPTY\n");
				continue;
			}
			int value = pop(list);
			snprintf(reply, sizeof(reply), "OK popped %d (length %zu)\n",
				 value, list->length);
			send_msg(client_fd, reply);
			continue;
		}

		if (strncasecmp(buf, "PRINT", 5) == 0) {
			print_list(list);
			if (list->length == 0) {
				send_msg(client_fd, "EMPTY\n");
			} else {
				snprintf(reply, sizeof(reply), "OK length %zu\n", list->length);
				send_msg(client_fd, reply);
			}
			continue;
		}

		send_msg(client_fd, "ERR unknown command\n");
	}
}

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	LinkedList *list = create_list();

	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0) {
		perror("socket");
		return 1;
	}

	int opt = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		perror("setsockopt");
		close(server_fd);
		return 1;
	}

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(PORT);

	if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("bind");
		close(server_fd);
		return 1;
	}

	if (listen(server_fd, 8) < 0) {
		perror("listen");
		close(server_fd);
		return 1;
	}

	printf("server listening on port %d\n", PORT);

	for (;;) {
		struct sockaddr_in client_addr;
		socklen_t client_len = sizeof(client_addr);
		int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
		if (client_fd < 0) {
			perror("accept");
			continue;
		}

		char ip[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &client_addr.sin_addr, ip, sizeof(ip));
		printf("client connected from %s:%d\n", ip, ntohs(client_addr.sin_port));

		handle_client(client_fd, list);
		close(client_fd);
		printf("client disconnected\n");
	}

	delete_list(&list);
	close(server_fd);
	return 0;
}
