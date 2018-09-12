/*
 * parserInfo.h
 *
 *  Created on: Aug 25, 2018
 *      Author: charles
 */

#ifndef PARSERINFO_H_
#define PARSERINFO_H_

#include <iostream>
//#include <string>
#include <cstring>
//#include "network.h"


using namespace std;

#define D_HEADER_SIZE 11
#define D_MAX_PROC_SEQ 255

typedef int (*callbackFunc)(void *param);

typedef struct transPackinfo {
	unsigned char cmd_type;
	unsigned char action_type;
	unsigned char item_id;
	unsigned char cell_num;
	unsigned char process_num;
	unsigned char accuracy;
	unsigned char order_size;
	char          *order_num;
	unsigned int  image_size;
	char          *image_data;

}packInfo_tx;

typedef struct revPackinfo {
	unsigned char cmd_type;
	unsigned char action_type;
	unsigned char item_id;
	unsigned char cell_num;
	unsigned char process_num;
	unsigned int coordinate_x;
	unsigned int coordinate_y;
	unsigned char matching_rate;
	unsigned char data_size;
    char          data[128];


}packInfo_rx;


typedef enum command_type
{
	CMD_TYPE_REQUEST = 0x20,
	CMD_TYPE_ACK = 0x21,
	CMD_TYPE_NACK = 0x22,
	CMD_TYPE_END

}command_type_t;

typedef enum action_type
{
	ACT_BARCODE1D = 0x01,
	ACT_BARCODE2D,
	ACT_BARCODEQR,
	ACT_EXIST,
	ACT_JUDGEMENT,
	ACT_END
}action_type_t;

typedef enum work_type
{
	WORK_ORDER = 0x00,
	WORK_ITEM1,
	WORK_ITEM2,
	WORK_ITEM3,
	WORK_ITEM4,
	WORK_END
}work_type_t;

typedef enum jobStatus
{
	JS_IDLE,
	JS_READY,
	JS_PROCESSING,
	JS_ERROR,
	JS_NONE
}jobStatus_t;

typedef struct jobinfo
{
	char *txData;
	packInfo_tx *txPackInfo;
	packInfo_rx *rxPackInfo;
	callbackFunc callback;
	jobStatus_t state;

}jobInfo_t;




extern bool setSendRequest(packInfo_tx *sInfo);
extern void getDevOrderNumber(char *num);
extern int buildPacket(packInfo_tx *info);
//extern int transfer_proc_init(void);
//extern int transfer_data_proc(void);
//extern void setNetworkHandler(Network* net);

#endif /* PARSERINFO_H_ */
