#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

/* ioctl command */
#define L3G4200D_SWITCH_TO_NORMAL_MODE _IO(99, 1)
#define L3G4200D_GET_INTERNAL_TEMP     _IOR(99, 2, void *)
#define L3G4200D_GET_OUT_X_L           _IOR(99, 3, void *)
#define L3G4200D_GET_OUT_X_H           _IOR(99, 4, void *)
#define L3G4200D_GET_OUT_Y_H           _IOR(99, 5, void *)
#define L3G4200D_GET_OUT_Y_L           _IOR(99, 6, void *)
#define L3G4200D_GET_OUT_Z_H           _IOR(99, 7, void *)
#define L3G4200D_GET_OUT_Z_L           _IOR(99, 8, void *)
#define L3G4200D_DEVICE_FILE "/dev/l3g4200d_sensor"

/**************************************************************************************** 
 * Definition
 ****************************************************************************************/
int main(void)
{
    int fd = -1;
    int ret = -1;
    uint8_t temp = 0;

    fd = open(L3G4200D_DEVICE_FILE, O_RDWR);
    if(fd < 0)
    {
        printf("Open device file failed\n");
        return -1;
    }

    ret = ioctl(fd, L3G4200D_SWITCH_TO_NORMAL_MODE, NULL);
    if(ret != 0)
    {
        printf("ioctl error\n");
        return -1;
    }

    while(1)
    {
        printf("----------------------------------\n");
        ioctl(fd, L3G4200D_GET_OUT_X_L, &temp);
        printf("L3G4200D_GET_OUT_X_L = %d\n", temp);

        ioctl(fd, L3G4200D_GET_OUT_X_H, &temp);
        printf("L3G4200D_GET_OUT_X_H = %d\n", temp);

        ioctl(fd, L3G4200D_GET_OUT_Y_L, &temp);
        printf("L3G4200D_GET_OUT_Y_L = %d\n", temp);

        ioctl(fd, L3G4200D_GET_OUT_Y_H, &temp);
        printf("L3G4200D_GET_OUT_Y_H = %d\n", temp);

        ioctl(fd, L3G4200D_GET_OUT_Z_L, &temp);
        printf("L3G4200D_GET_OUT_Z_L = %d\n", temp);

        ioctl(fd, L3G4200D_GET_OUT_Z_H, &temp);
        printf("L3G4200D_GET_OUT_Z_H = %d\n", temp);

        printf("----------------------------------\n");

        sleep(1);
    }

    close(fd);

    return 0;
}





