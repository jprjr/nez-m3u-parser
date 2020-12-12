# nez-m3u-parser

A no-dependency, header-only C89 library for parsing
[NEZPlug M3U files](http://www.vgmpf.com/Wiki/index.php/NEZ_Plug).

It allocates no memory and does not use any C library functions.
It works with all line endings, including strings with
no line ending.

## Example Usage

```c
#define NEZ_M3U_IMPLEMENTATION
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
```

## Details

All state is stored in a struct, `nez_m3u_t`.

Call `nez_m3u_init` to initialize the struct, then repeatedly call
`nez_m3u_parse` with your M3U data. If `nez_m3u_parse` returns a 1,
you can inspect the fields in the struct. A zero indicates
that `nez_m3u_parse` is finished.

`nez_m3u_t` will track the pointer to your string data,
if you're reusing buffers, you'll want to call `nez_m3u_init`
again.

In this example, `line1` and `line2` are different pointers,
so there's no need to call `nez_m3u_init`:

```c
const char *line1 = "a line";
const char *line2 = "pretend this is m3u data";
nez_m3u_init(&t);
while(nez_m3u_parse(&t,line1,strlen(line1))) {
  /* do stuff */
}
while(nez_m3u_parse(&t,line2,strlen(line2))) {
  /* do stuff */
}
```

In this example, `buffer` is being reused, so the
address never changes, you'll need to call `nez_m3u_init`
between calls to `fread` (or whatever you use to fill
your buffer):

```c
char buffer[2048];

fread(buffer,2048,1,file);
nez_m3u_init(&t);
while(nez_m3u_parse(&t,buffer,strlen(buffer))) {
  /* do stuff */
}

fread(buffer,2048,1,another_file);
nez_m3u_init(&t);
while(nez_m3u_parse(&t,buffer,strlen(buffer))) {
  /* do stuff */
}
```

Details on fields and their meaning are in the header file.

There's one additional function, `nez_m3u_title`, which is for
getting the length of the title field (after escaping characters),
and copying the title field out to some buffer (again, after escaping).

## LICENSE

Public Domain or MIT No Attribution, your choice.

