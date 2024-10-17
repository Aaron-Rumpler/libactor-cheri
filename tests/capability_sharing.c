#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <cheriintrin.h>

#include <libactor/actor.h>

struct message {
    char *buf;
    size_t size;
};

void *printer_actor(void *args) {
    actor_msg_t *msg;
    msg = actor_receive();

    struct message *msg_inner = (struct message *)msg->data;

    char *buf = msg_inner->buf;

    printf("%#p (tag: %d, valid: %d)\n", buf, cheri_tag_get(buf), cheri_is_valid(buf));

    printf("%c\n", *buf);

    printf("%s\n", buf);

    arelease(msg);
    return NULL;
}

void *main_func(void *args) {
    struct actor_main *main = (struct actor_main *)args;

    char *init_message = "This is a test.";

    size_t buf_size = strlen(init_message) + 1;
    char *buf = amalloc(buf_size);
    memcpy(buf, init_message, buf_size);

    struct message msg = {
        .buf = buf,
        .size = buf_size
    };

    actor_id printer = spawn_actor(printer_actor, NULL);
    actor_send_msg(printer, 0, &msg, sizeof(msg));

    return NULL;
}

DECLARE_ACTOR_MAIN(main_func)
