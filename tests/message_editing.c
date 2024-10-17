#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <cheriintrin.h>

#include <libactor/actor.h>

void *editor_actor(void *args) {
    actor_msg_t *msg;
    msg = actor_receive();

    sleep(2);

    char *buf = (char *)msg->data;

    printf("editor_actor()\n");
    printf("%#p (tag: %d, valid: %d)\n", buf, cheri_tag_get(buf), cheri_is_valid(buf));

    printf("%s\n", buf);

    strlcpy(buf, "Hello, World!", msg->size);

    printf("Editied\n");

    return NULL;
}

void *printer_actor_1(void *args) {
    actor_msg_t *msg;
    msg = actor_receive();

    char *buf = (char *)msg->data;

    printf("printer_actor_1()\n");
    printf("%#p (tag: %d, valid: %d)\n", buf, cheri_tag_get(buf), cheri_is_valid(buf));
    printf("%s\n", buf);

    return NULL;
}

void *printer_actor_2(void *args) {
    actor_msg_t *msg;
    msg = actor_receive();

    sleep(4);

    char *buf = (char *)msg->data;

    printf("printer_actor_2()\n");
    printf("%#p (tag: %d, valid: %d)\n", buf, cheri_tag_get(buf), cheri_is_valid(buf));
    printf("%s\n", buf);

    return NULL;
}

void *main_func(void *args) {
    struct actor_main *main = (struct actor_main *)args;

    char *init_message = "This is a test.";

    size_t buf_size = strlen(init_message) + 1;
    char *buf = amalloc(buf_size);
    memcpy(buf, init_message, buf_size);

    printf("main_func: %#p (tag: %d, valid: %d)\n", buf, cheri_tag_get(buf), cheri_is_valid(buf));

    actor_id editor = spawn_actor(editor_actor, NULL);
    actor_id printer_1 = spawn_actor(printer_actor_1, NULL);
    actor_id printer_2 = spawn_actor(printer_actor_2, NULL);
    actor_broadcast_msg(0, buf, buf_size);
    sleep(6);

    printf("main_func: %99s\n", buf);

    return NULL;
}

DECLARE_ACTOR_MAIN(main_func)

