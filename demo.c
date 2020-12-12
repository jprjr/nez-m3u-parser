#define NEZ_M3U_IMPLEMENTATION
#define NEZ_M3U_STATIC
#include "nez-m3u-parser.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static const char *m3u_line =
  "some-file::nsf,7,Title 1,3:00,,10\r\n"
  "some-file::nsf,8,Title 2,1:30,,8\r\n";

static void dump_info(nez_m3u_t *t) {
    char *buffer;
    unsigned int title_len;
    if(t->linetype != NEZ_M3U_TRACK) return;
    title_len = nez_m3u_title(t,NULL,0);
    if(title_len) {
        buffer = (char *)malloc(title_len+1);
        if(buffer == NULL) abort();
        nez_m3u_title(t,buffer,title_len+1);
        printf("Title: %s\n",buffer);
        free(buffer);
    }
    printf("tracknum: %d\n",t->tracknum);
    printf("length: %d\n",t->length);
    printf("fade: %d\n",t->fade);
    printf("intro: %d\n",t->intro);
    printf("loop: %d\n",t->loop);
    printf("loops: %d\n",t->loops);
}

int main(void) {
    nez_m3u_t parser;

    nez_m3u_init(&parser);
    while(nez_m3u_parse(&parser,m3u_line,strlen(m3u_line))) {
        dump_info(&parser);
    }
    return 0;
}

