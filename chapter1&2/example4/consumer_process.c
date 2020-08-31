#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
int main(){
/* the size (in bytes) of shared memory object */
const int SIZE = 4096;
/* name of the shared memory object */
const char *name = "OS";
/* shared memory file descriptor */
int fd;
/* pointer to shared memory obect */
char *ptr;
/* open the shared memory object */
fd = shm_open (name, O_RDONLY, 0666);
/* memory map the shared memory object */
ptr = (char *)
mmap (0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
/* read from the shared memory object */
printf("%s",(char *)ptr);
/* remove the shared memory object */
shm_unlink(name);
}

/*
The consumer process, shown in Figure 3.17, reads and outputs the con-
tents of the shared memory. The consumer also invokes the shm_unlink()
function, which removes the shared-memory segment after the consumer has
accessed it. We provide further exercises using the POSIX shared-memory API in
the programming exercises at the end of this chapter. Additionally, we provide
more detailed coverage of memory mapping in Section 13.5.
*/