#include <linux/delay.h>
#include <linux/module.h>
#include "nuc970_cap.h"

static struct nuvoton_vin_sensor tw9912;

struct TW_RegValue{
	__u8	uRegAddr;
	__u8	uValue;
};

#define _REG_TABLE_SIZE(nTableName)	sizeof(nTableName)/sizeof(struct TW_RegValue)

static struct TW_RegValue Init_RegValue[] = 
{	
	//NTSC INIT
	{0x88, 0x14},{0x02, 0x48},//From YIN2 
	{0x03, 0x20},{0x05, 0x01},{0x07, 0x02},{0x08, 0x12},{0x09, 0xF0},
	{0x0A, 0x14},{0x0B, 0xD0},{0x0C, 0xCC},{0x11, 0x64},{0x1C, 0x0F},
	{0x1E, 0x08},{0x27, 0x38},{0x30, 0x00},{0x37, 0x28},{0x38, 0xAF},
	{0xC0, 0x01},{0xC2, 0x01},{0xC3, 0x03},{0xC4, 0x5A},{0xC5, 0x00},
	{0xC6, 0x20},{0xCB, 0x30},{0xCC, 0x00},{0xD7, 0x70},{0xD9, 0x04},
	{0xE1, 0x05},{0xE2, 0xD9},{0xE6, 0x00},{0xE8, 0x0F},{0xE9, 0x00},
};

static struct TW_RegValue Output_RegValue[] = 
{	
#if 1 //NTSC PRO
	{0x88, 0x02},
	{0x02, 0x48},//From YIN2 
	{0x03, 0x20},//add	
	{0x04, 0x00},{0x05, 0x1E},{0x06, 0x03},
	//{0x08, 0x0A},//{0x08, 0x12},
	//{0x08, 0x17},	
#if 0	
	{0x07, 0x12},//{0x07, 0x02},				   	/* Fix insufficient 480 horizontal line */	
	{0x09, 0x05},//{0x09, 0xFC},//{0x09, 0xF0},   	/* Fix insufficient 480 horizontal line */
#else
	//{0x07, 0x12},//{0x07, 0x02},				   	/* Fix insufficient 480 horizontal line */	
	//{0x09, 0x40},//{0x09, 0xFC},//{0x09, 0xF0},   	/* Fix insufficient 480 horizontal line */
	//D1 TRY
	//{0x07, 0x03},
	//{0x09, 0xF0},
#endif	
	//{0x0A, 0x49},
	//{0x0B, 0x34},
	//{0x0C, 0xCC},
	{0x07, 0x02},{0x08, 0x14},{0x09, 0xF9},{0x0A, 0x2b},{0x0B, 0xf4},
	{0x0C, 0xDC},{0x0D, 0x15},{0x11, 0x64},{0x12, 0x11},{0x13, 0x80},	
	{0x14, 0x80},{0x15, 0x00},{0x17, 0x30},{0x18, 0x44},{0x1A, 0x10},
	{0x1B, 0x00},{0x1C, 0x0F},{0x1D, 0x7F},{0x1E, 0x08},{0x1F, 0x00},
	{0x20, 0x50},{0x21, 0x42},{0x22, 0xF0},{0x23, 0xD8},{0x24, 0xBC},
	{0x25, 0xB8},{0x26, 0x44},{0x27, 0x38},{0x28, 0x00},{0x29, 0x00},
	{0x2A, 0x78},{0x2B, 0x44},{0x2C, 0x30},{0x2D, 0x14},{0x2E, 0xA5},
	{0x2F, 0x26},{0x30, 0x00},{0x31, 0x10},{0x32, 0x00},{0x33, 0x05},
	{0x34, 0x1A},{0x35, 0x00},{0x36, 0xe2},
	{0x37, 0x01}, //D1
	//{0x37, 0x2D}, //VGA
	{0x38, 0x01},{0x40, 0x00},{0x41, 0x80},{0x42, 0x00},{0xC0, 0x01},
	{0xC1, 0x07},{0xC2, 0x01},{0xC3, 0x03},{0xC4, 0x5A},{0xC5, 0x00},
	{0xC6, 0x20},{0xC7, 0x04},{0xC8, 0x00},{0xC9, 0x06},{0xCA, 0x06},
	{0xCB, 0x30},{0xCC, 0x00},{0xCD, 0x54},{0xD0, 0x00},{0xD1, 0xF0},
	{0xD2, 0xF0},{0xD3, 0xF0},{0xD4, 0x00},{0xD5, 0x00},{0xD6, 0x10},
	{0xD7, 0x70},{0xD8, 0x00},{0xD9, 0x04},{0xDA, 0x80},{0xDB, 0x80},	
	{0xDC, 0x20},{0xE0, 0x00},{0xE1, 0x49},{0xE2, 0xD9},{0xE3, 0x00},
	{0xE4, 0x00},{0xE5, 0x00},{0xE6, 0x00},{0xE7, 0x2A},{0xE8, 0x0F},
	{0xE9, 0x6D},//{0xE9, 0x61},
//	{0x2F, 0x8B} /* test mode force blue */	

#else //PAL

	{0xFF, 0x00},//Page 0		
	//{0x88, 0x02}, //mark by paul's new sensor table
	{0x01, 0x79},{0x02, 0x48},//From YIN2 
	//{0x02, 0x40},//From YIN0 
	{0x03, 0x20},//add	
	{0x04, 0x00},{0x05, 0x1E},{0x06, 0x03},
	//D1 TRY
	{0x07, 0x12},{0x08, 0x14},{0x09, 0x20},{0x0A, 0x26},{0x0B, 0xFE},
//	{0x07, 0x12},
//	{0x08, 0x14},
//	{0x09, 0x20},
//	{0x0A, 0x0E},
//	{0x0B, 0xD0},
	{0x0C, 0xCC},{0x0D, 0x15},{0x11, 0x64},{0x12, 0x11},{0x13, 0x80},	
	{0x14, 0x80},{0x15, 0x00},{0x17, 0x30},{0x18, 0x44},{0x1A, 0x10},
	{0x1B, 0x00},{0x1C, 0x1F},//{0x1C, 0x0F},
	{0x1D, 0x7F},{0x1E, 0x18},//{0x1E, 0x08},
	{0x1F, 0x00},{0x20, 0x50},{0x21, 0x42},{0x22, 0xF0},{0x23, 0xD8},
	{0x24, 0xBC},{0x25, 0xB8},{0x26, 0x44},{0x27, 0x38},{0x28, 0x00},
	{0x29, 0x00},{0x2A, 0x78},{0x2B, 0x44},{0x2C, 0x30},{0x2D, 0x14},
	{0x2E, 0xA5},{0x2F, 0x26},{0x30, 0x00},{0x31, 0x10},{0x32, 0x00},
	{0x33, 0x05},{0x34, 0x1A},{0x35, 0x00},{0x36, 0xe2},
//	{0x37, 0x2D},//VGA
	{0x37, 0x01},//D1
	{0x38, 0x01},{0x40, 0x00},{0x41, 0x80},{0x42, 0x00},{0xC0, 0x01},
	{0xC1, 0x07},{0xC2, 0x01},{0xC3, 0x03},{0xC4, 0x5A},{0xC5, 0x00},
	{0xC6, 0x20},{0xC7, 0x04},{0xC8, 0x00},{0xC9, 0x06},{0xCA, 0x06},	
	{0xCB, 0x30},{0xCC, 0x00},{0xCD, 0x54},{0xD0, 0x00},{0xD1, 0xF0},
	{0xD2, 0xF0},{0xD3, 0xF0},{0xD4, 0x00},{0xD5, 0x00},{0xD6, 0x10},	
	{0xD7, 0x70},{0xD8, 0x00},{0xD9, 0x04},{0xDA, 0x80},{0xDB, 0x80},	
	{0xDC, 0x20},{0xE0, 0x00},{0xE1, 0x49},{0xE2, 0xD9},{0xE3, 0x00},
	{0xE4, 0x00},{0xE5, 0x00},{0xE6, 0x00},{0xE8, 0x0F},{0xE9, 0x6D},
	//{0xE9, 0x61},
	//{0xff, 0xff}
#endif
};

/************  I2C  *****************/
static struct i2c_client *save_client;
static char sensor_inited = 0;
	__u8 sensor_read_tw9912(__u8 uRegAddr)
{
		u8 val;		
		//printk("sensor_read_tw9912 i2c_smbus_read_byte uRegAddr=0x%x\n",uRegAddr);
		i2c_smbus_write_byte(save_client, uRegAddr);
		//printk("sensor_read_tw9912 i2c_smbus_write_byte\n");
		val = i2c_smbus_read_byte(save_client);		
		return val;
}

static int32_t sensor_write_tw9912(__u8 uRegAddr, __u8 uData)
{
		int ret;
		ret=i2c_smbus_write_byte_data(save_client, uRegAddr, uData);			
		return ret;
}

static int sensor_probe(struct i2c_client *client,const struct i2c_device_id *did)
{
	ENTRY();
	sensor_inited = 1;
	client->flags = I2C_CLIENT_SCCB;
	save_client = client;
	LEAVE();
	return 0;
}
static int sensor_remove(struct i2c_client *client)
{	
	ENTRY();
	LEAVE();
	return 0;
}

static int tw9912_init(struct nuvoton_vin_device* cam)
{
	int err = 0;
	ENTRY();
	LEAVE();		
	return err;
}

#if 1 //NTSC
#define CROP_START_X	0x24
#define CROP_START_Y	0x28
#endif
static struct nuvoton_vin_sensor tw9912 = {
	.name = "tw9912",
	.init = &tw9912_init,
	.infmtord = (INORD_YVYU | INFMT_YCbCr | INTYPE_CCIR601),
	.polarity = (VSP_LO | HSP_LO | PCLKP_HI),
	.cropstart = ( CROP_START_Y | CROP_START_X<<16 ), /*( Vertical | Horizontal<<16 ) */
	.cropcap = {
		.bounds = {
			.left = CROP_START_X,
			.top = CROP_START_Y,
			.width = 720,
			.height = 480,
		},
		.defrect = {
			.left = 0,
			.top = 0,
			.width = 800,
			.height = 480,
		},
	},
	.pix_format	 = {
		.width = 720,
		.height = 480,
		.pixelformat = V4L2_PIX_FMT_YUYV,
		.priv = 16,
		.colorspace = V4L2_COLORSPACE_JPEG,
	},
};

int nuvoton_vin_probe_tw9912(struct nuvoton_vin_device* cam)
{
	int i,ret = 0;
	__u8 SensorID[1];
	struct TW_RegValue *psRegValue;
	struct TW_RegValue *psRegValue1;
	ENTRY();	
	nuvoton_vin_attach_sensor(cam, &tw9912);
	
	// if i2c module isn't loaded at this time
	if(!sensor_inited)
		return -1;

	psRegValue=Init_RegValue;
	for(i=0;i<_REG_TABLE_SIZE(Init_RegValue); i++, psRegValue++)
	{
		int32_t ret;
		printk(".");
		ret = sensor_write_tw9912((psRegValue->uRegAddr), (psRegValue->uValue));
		if(ret<0)
		{
			VDEBUG("Wrong to write register addr = 0x%x, write data = 0x%x , ret = %d\n", (psRegValue->uRegAddr), (psRegValue->uValue), ret);		
			printk("Wrong to write register addr = 0x%x, write data = 0x%x , ret = %d\n", (psRegValue->uRegAddr), (psRegValue->uValue), ret);		
		}	
	}
	printk("\n"); 	
	
	psRegValue1=Output_RegValue;
	for(i=0;i<_REG_TABLE_SIZE(Output_RegValue); i++, psRegValue1++)
	{
		int32_t ret;
		printk(".");
		ret = sensor_write_tw9912((psRegValue1->uRegAddr), (psRegValue1->uValue));
		if(ret<0)
		{
			VDEBUG("Wrong to write register addr = 0x%x, write data = 0x%x , ret = %d\n", (psRegValue1->uRegAddr), (psRegValue1->uValue), ret);		
		}	
	} 
	//----------Read sensor id-------------------------------------	        
	SensorID[0]=sensor_read_tw9912(0x00);  /* ID 0x60 */
	printk("Chip Version = 0x%02X(0x60) \n", SensorID[0]);	
	//-------------------------------------------------------------		
	printk("\n");
	if(ret>=0)
		printk("driver i2c initial done\n");
	else
		printk("driver i2c initial fail\n");	
	LEAVE();		
	return ret;	
}

static const struct i2c_device_id sensor_id[] = {{ "tw9912", 0 },};
MODULE_DEVICE_TABLE(i2c, sensor_id);

static struct i2c_driver sensor_i2c_driver = {
	.driver = { .name  = "tw9912", 
				.owner = THIS_MODULE,
						},
	.probe    = sensor_probe,
	.remove   = sensor_remove,	
	.id_table = sensor_id,
};

module_i2c_driver(sensor_i2c_driver);
