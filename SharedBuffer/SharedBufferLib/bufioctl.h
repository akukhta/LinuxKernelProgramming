#include <linux/ioctl.h>

#define SHARED_BUFFER_IOCTL 0x0f0f0f0f
#define SHARED_BUFFER_GET_SIZE_OF_CURR_HEAD _IOR(SHARED_BUFFER_IOCTL, 0, size_t)
#define SHARED_BUFFER_GET_COUNT_OF_ELEMENTS _IOR(SHARED_BUFFER_IOCTL, 1, size_t)
#define SHARED_BUFFER_GET_TOTAL_SIZE _IOR(SHARED_BUFFER_IOCTL, 2, size_t);
#define SHARED_BUFFER_SET_NEW_HEAD _IOW(SHARED_BUFFER_IOCTL, 3, size_t);
#define SHARED_BUFFER_SET_STACK_MODE _IOW(SHARED_BUFFER_IOCTL, 4, bool);