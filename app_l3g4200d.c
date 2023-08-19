#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/ioctl.h>

/* ioctl command */
#define L3G4200D_SWITCH_TO_NORMAL_MODE _IO(99, 1)
#define L3G4200D_DEVICE_FILE "/dev/l3g4200d_sensor"

/**************************************************************************************** 
 * Definition
 ****************************************************************************************/
int main(void)
{
    int fd = -1;
    int ret = -1;

    fd = open(L3G4200D_DEVICE_FILE, O_RDWR);
    if(fd < 0)
    {
        printf("Open device file failed\n");
        return -1;
    }

    ret = ioctl(fd, L3G4200D_SWITCH_TO_NORMAL_MODE);
    if(ret != 0)
    {
        printf("ioctl error\n");
        return -1;
    }

    return 0;
}





