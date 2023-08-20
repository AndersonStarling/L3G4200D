#include <linux/module.h>
#include <linux/spi/spi.h>
#include <linux/printk.h>
#include <linux/miscdevice.h>

/**************************************************************************************** 
 * Definition
 ****************************************************************************************/

/* L3G4200D register */
#define WHO_AM_I 0x0F
#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define CTRL_REG4 0x23
#define CTRL_REG5 0x24
#define REFERENCE 0x25
#define OUT_TEMP 0x26
#define STATUS_REG 0x27
#define OUT_X_L 0x28
#define OUT_X_H 0x29
#define OUT_Y_L 0x2A
#define OUT_Y_H 0x2B
#define OUT_Z_L 0x2C
#define OUT_Z_H 0x2D
#define FIFO_CTRL_REG 0x2E
#define FIFO_SRC_REG 0x2F
#define INT1_CFG 0x30
#define INT1_SRC 0x31
#define INT1_TSH_XH 0x32
#define INT1_TSH_XL 0x33
#define INT1_TSH_YH 0x34
#define INT1_TSH_YL 0x35
#define INT1_TSH_ZH 0x36
#define INT1_TSH_ZL 0x37
#define INT1_DURATION 0x38

/* ioctl command */
#define L3G4200D_SWITCH_TO_NORMAL_MODE _IO(99, 1)


/**************************************************************************************** 
 * Function Prototype
 ****************************************************************************************/
static ssize_t l3g4200d_write(struct file *file, const char __user *buf, size_t len, loff_t *pos);
static ssize_t l3g4200d_read(struct file *file, char __user *buf, size_t len, loff_t *pos);
static int l3g4200d_open(struct inode *inode, struct file *file);
static int l3g4200d_close(struct inode *inodep, struct file *filp);
static long l3g4200d_ioctl(struct file * flip, unsigned int cmd, unsigned long arg);

u8 l3g4200d_init(void);

/**************************************************************************************** 
 * Variable
 ****************************************************************************************/
static const struct file_operations l3g4200d_fops = 
{
    .owner			= THIS_MODULE,
    .write			= l3g4200d_write,
	.read			= l3g4200d_read,
    .open			= l3g4200d_open,
    .release		= l3g4200d_close,
    .unlocked_ioctl = l3g4200d_ioctl
};

struct miscdevice l3g4200d_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "l3g4200d_sensor",
    .fops = &l3g4200d_fops,
};

struct spi_device *spi_global = NULL;


/**************************************************************************************** 
 * Static function
 ****************************************************************************************/

static ssize_t l3g4200d_write(struct file *file, const char __user *buf, size_t len, loff_t *pos)
{
    pr_info("%s %d", __func__, __LINE__);

    return 1;
}

static ssize_t l3g4200d_read(struct file *file, char __user *buf, size_t len, loff_t *pos)
{
    pr_info("%s %d\n", __func__, __LINE__);

    return 1;
}

static int l3g4200d_open(struct inode *inode, struct file *file)
{

    pr_info("%s %d\n", __func__, __LINE__);

    return 0;
}

static int l3g4200d_close(struct inode *inodep, struct file *filp)
{
    pr_info("%s %d\n", __func__, __LINE__);

    return 0;
}

static long l3g4200d_ioctl(struct file * flip, unsigned int cmd, unsigned long arg)
{
    int ret = -1;

    switch (cmd)
    {
        case L3G4200D_SWITCH_TO_NORMAL_MODE:
            l3g4200d_init();
            break;
            
        default:
            pr_info("Not support command\n");
            ret = -1;
    }

    return 0;
}

/* l3g4200d driver */
void l3g4200d_write_register(u8 register_address, u8 data_write)
{
    struct spi_message msg;
    struct spi_transfer xfer[2] = {0};
    u8 tx_buffer[2] = {register_address, data_write};
    int ret = -1;

    xfer[0].tx_buf = &tx_buffer[0];
    xfer[0].len = sizeof(tx_buffer);
    xfer[0].rx_buf = NULL;

    spi_message_init(&msg);
    spi_message_add_tail(&xfer[0], &msg);

    ret = spi_sync(spi_global, &msg);
    if(ret != 0)
    {
        pr_info("%s %d Failed\n", __func__, __LINE__);
    }
}

u8 l3g4200d_read_register(u8 register_address)
{
    struct spi_message msg;
    struct spi_transfer xfer[2] = {0};
    u8 tx_buffer[2] = {register_address | 0x80, 0x00};
    u8 rx_buffer[2] = {0};
    int ret = -1;

    xfer[0].tx_buf = &tx_buffer[0];
    xfer[0].len = sizeof(tx_buffer);
    xfer[0].rx_buf = &rx_buffer[0];

    spi_message_init(&msg);
    spi_message_add_tail(&xfer[0], &msg);

    ret = spi_sync(spi_global, &msg);
    if(ret != 0)
    {
        pr_info("%s %d Failed\n", __func__, __LINE__);
    }

    return rx_buffer[1];

}

u8 l3g4200d_init(void)
{
    u8 register_value = 0;
    u8 retval = 1;

    l3g4200d_write_register(CTRL_REG1, 0x0F);
    l3g4200d_write_register(CTRL_REG2, 0x00);
    l3g4200d_write_register(CTRL_REG4, 0x00);

    register_value = l3g4200d_read_register(STATUS_REG);
    if(!(register_value & (1 << 3)))
    {
        pr_info("Sensor is in power down mode\n");
        retval = 1;
    }

    return retval;
}


/**************************************************************************************** 
 * Function implementation
 ****************************************************************************************/

static int spi_protocol_example_probe(struct spi_device *spi) 
{
    struct spi_message msg;
    struct spi_transfer xfer[2] = {0};
    char tx_buf[2] = {WHO_AM_I | 0x80, 0x00};
    char rx_buf[2] = {0};
    int ret = -1;

    ret = misc_register(&l3g4200d_device);
    if (ret) {
        pr_err("can't misc_register\n");
        return -1;
    }

    pr_info("%s, %d\n", __func__, __LINE__);

    spi->bits_per_word = 8;
    spi->mode = SPI_MODE_0;
    spi->max_speed_hz = 1000000;
    spi_global = spi;

    ret = spi_setup(spi);
    if(ret != 0)
    {
        pr_info("SPI setup failed\n");
        return -1;
    }


    xfer[0].tx_buf = tx_buf;
    xfer[0].len = sizeof(tx_buf);

    xfer[1].rx_buf = rx_buf;
    xfer[1].len = sizeof(rx_buf);

    spi_message_init(&msg);
    spi_message_add_tail(&xfer[0], &msg);
    spi_message_add_tail(&xfer[1], &msg);

    ret = spi_sync(spi, &msg);
    if(ret != 0)
    {
        pr_info("Failed\n");
        return -1;
    }

    if(rx_buf[0] != 0xD3)
    {
        pr_info("Sensor is not respond. Read value is %d\n", rx_buf[0]);
    }

    return 0;
}

static int spi_protocol_example_remove(struct spi_device *spi) 
{
    pr_info("%s, %d\n", __func__, __LINE__);

    misc_deregister(&l3g4200d_device);

    return 0;
}

static const struct of_device_id spi_protocol_example_dt_ids[] = {
	{ .compatible = "st,l3g42000d"},
	{},
};

static struct spi_driver spi_protocol_example_driver = {
    .probe = spi_protocol_example_probe,
    .remove = spi_protocol_example_remove,
    .driver = {
        .name = "spi_protocol_example",
        .of_match_table = spi_protocol_example_dt_ids,
    },
};

module_spi_driver(spi_protocol_example_driver);

MODULE_AUTHOR("ThinhNH <huuthinh1603@gmail.com>");
MODULE_DESCRIPTION("SPI module");
MODULE_LICENSE("GPL v2");
