/*
libactor - A C Actor Library
http_server.c

A Simple HTTP Server. Just returns "Hello, World to the client". Doesn't do any handling of headers.

Copyright (C) 2009 Chris Moos

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/


#include <libactor/actor.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>


#define BUFFER_SIZE 512
#define HTTP_CLIENT_INFO 100


unsigned char *recvline(int sockfd) {
    unsigned char *buf = (unsigned char *)malloc(BUFFER_SIZE);
    unsigned int len = 0, bufLen = BUFFER_SIZE;
    int ret;

    for (;;) {
        ret = recv(sockfd, buf + len, 1, 0);
        if (ret <= 0) {
            free(buf);
            return NULL;
        } else {
            if (buf[len] == 0) {
                free(buf);
                return NULL;
            }
            len += ret;
            if (len >= 2 && (buf[len - 2] == '\r' && buf[len - 1] == '\n')) {
                buf[len - 2] = 0;
                return buf;
            }
            if (len == (BUFFER_SIZE - 1)) {
                buf = (unsigned char *)realloc(buf, bufLen + BUFFER_SIZE);
                bufLen += BUFFER_SIZE;
            }
        }
    }

    return buf;
}


ACTOR_FUNCTION(http_client, args) {
    actor_msg_t *msg;
    struct sockaddr_in *remote;
    int sock = (int)args;
    unsigned char *line = NULL;
    char *response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 12\r\n"
        "\r\n"
        "Hello, World!\r\n";

    msg = actor_receive();
    if (msg->type == HTTP_CLIENT_INFO) {
        remote = (struct sockaddr_in *)msg->data;
        while ((line = recvline(sock)) != NULL) {
            if (*line == 0) {
                free(line);
                break;
            }
            free(line);
        }
        send(sock, response, strlen(response), 0);
    }
    arelease(msg);
    close(sock);
    return 0;
}


ACTOR_FUNCTION(http_listener, arg) {
    struct sockaddr_in local, remote;
    int sockfd;
    int socklen = sizeof(struct sockaddr_in);

    int port = (int)arg;

    local.sin_family = AF_INET;
    local.sin_addr.s_addr = INADDR_ANY;
    local.sin_port = htons(port);
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    /* Set SO_REUSEADDR */
    int sockoption = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&sockoption, sizeof(sockoption));

    int bind_result = bind(sockfd, (struct sockaddr *)&local, sizeof(struct sockaddr_in));

    if (bind_result == -1) {
        perror("bind");
        return 0;
    }

    listen(sockfd, 0);
    printf("HTTP server listening on port: %d\n", port);

    while (1) {
        int clientsock = accept(sockfd, (struct sockaddr *)&remote, &socklen);
        if (clientsock == -1) break;

        actor_id aid = spawn_actor(http_client, (void *)clientsock);
        actor_send_msg(aid, HTTP_CLIENT_INFO, (void *)&remote, sizeof(struct sockaddr_in));
    }

    return 0;
}


ACTOR_FUNCTION(main_func, args) {
    struct actor_main *amain = (struct actor_main *)args;

    if (amain->argc < 2)
        printf("usage: %s port\n", amain->argv[0]);
    else
        spawn_actor(http_listener, (void *)atoi(amain->argv[1]));

    return 0;
}

DECLARE_ACTOR_MAIN(main_func)
