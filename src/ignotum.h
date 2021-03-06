#ifndef _IGNOTUM_H
#define _IGNOTUM_H 1

/* information macros */
#define IGNOTUM_VERSION "0.1"

/* initialization macros */
#define DEFAULT_IGNOTUM_ADDR_RANGE (struct ignotum_addr_range){ 0, 0 }
#define DEFAULT_IGNOTUM_STRING (struct ignotum_string){ NULL, 0 }
#define DEFAULT_IGNOTUM_ADDR_LIST (struct ignotum_addr_list){ DEFAULT_IGNOTUM_ADDR_RANGE, 0, 0, 0, 0, DEFAULT_IGNOTUM_STRING, NULL }
#define DEFAULT_IGNOTUM_ELEMENTS (struct ignotum_elements){ 1, 0, 0, 0, 0, 0, 0 }
#define DEFAULT_IGNOTUM_MEMSEARCH (struct ignotum_mem_search){ 0, NULL }

/* size constant */
#define SIGNED_OVERFLOW_PID_T (pid_t)~((pid_t)1 << ((sizeof(pid_t)*8)-1)) /* 0b100000000000000... */
#define MAX10_PID_T_STR (10*sizeof(pid_t)*CHAR_BIT/33+3) /* note that it dont include minus signal */
#define PATHNAME_LEN 1024


/* function macros */
#define check_hex_digit(c) ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))
#define ignotum_free(x) __safefree((void **)&(x))

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>

typedef struct ignotum_string {
	char *ptr;
	size_t size;
} ignotum_string_t;

typedef struct ignotum_addr_range {
	off_t start_addr;
	off_t end_addr;
} ignotum_addr_range_t;

typedef struct ignotum_mapped_addr {
	ignotum_addr_range_t range;
	int perms;
	off_t offset;
	dev_t st_dev;
	ino_t st_ino;
	ignotum_string_t pathname;
	struct ignotum_mapped_addr *next;
} ignotum_mapped_addr_t;

typedef struct ignotum_elements {
	int first_hex;
	int second_hex;
	int perms;
	int offset;
	int dev;
	int inode;
	int pathname;
} ignotum_elements_t;

typedef struct ignotum_mem_search {
	size_t len;
	off_t *addrs;
} ignotum_mem_search_t;


typedef enum {
	IGNOTUM_SUCCESS,
	IGNOTUM_INVALID_PID_NUMBER,
	IGNOTUM_INVALID_PID_STR,
	IGNOTUM_OPEN_MAP_FAILED,
	IGNOTUM_OPEN_MEM_FAILED,
	IGNOTUM_PID_T_OVERFLOW,
	IGNOTUM_ERR_SIZE
} ignotum_status;

ignotum_status ignotum_openmap(pid_t pid_number, int *fd_out);
ignotum_status ignotum_openmapstr(const char *pid_str, int *fd_out);
ignotum_status ignotum_openmem(pid_t pid_number, int *fd_out, int mode, int attach_pid);
ignotum_status ignotum_openmemstr(const char *pid_str, int *fd_out, int mode, int attach_pid);

const char *ignotum_strerror(ignotum_status code);

size_t ignotum_getmappedaddr(int maps_fd, ignotum_mapped_addr_t **out);

int ignotum_memwrite(int mem_fd, off_t offset, const void *src, size_t n);
int ignotum_memsearch(const void *search, size_t search_size, int mem_fd, ignotum_addr_range_t range, ignotum_mem_search_t *out);
int ignotum_memread(int mem_fd, off_t offset, void *out, size_t n);

void free_ignotum_mapped_addr_t(ignotum_mapped_addr_t **);
void free_ignotum_mem_search(ignotum_mem_search_t *);


#endif /* _IGNOTUM_H */
