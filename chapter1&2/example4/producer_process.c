#include <sys/stat.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>





int main(){
/* the size (in bytes) of shared memory object */
const int SIZE = 4096;
/* name of the shared memory object */
const char *name = "OS";
/* strings written to shared memory */
const char *message_0 = "Hello";
const char *message_1 = "World!";
/* shared memory file descriptor */
int fd;
/* pointer to shared memory obect */
char *ptr;
/* create the shared memory object */
fd = shm_open (name,O_CREAT | O_RDWR,0666);
/* configure the size of the shared memory object */
ftruncate (fd, SIZE);
/* memory map the shared memory object */
ptr = (char *)
mmap (0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
/* write to the shared memory object */
sprintf(ptr,"%s",message_0);
ptr += strlen(message_0);
sprintf(ptr,"%s",message_1);
ptr += strlen(message_1);
return 0;
}
/*

The producer, shown in Figure 3.16, creates a shared-memory object named
OS and writes the infamous string "Hello World!" to shared memory. The
program memory-maps a shared-memory object of the specified size and
allows writing to the object. The flag MAP SHARED specifies that changes to the
shared-memory object will be visible to all processes sharing the object. Notice
that we write to the shared-memory object by calling the sprintf() function
and writing the formatted string to the pointer ptr . After each write, we must
increment the pointer by the number of bytes written.

*/