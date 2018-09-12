/*
 * parserTask.cpp
 *
 *  Created on: Aug 25, 2018
 *      Author: charles
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <queue>
//#include <mutex>
#include <fstream>
#include <QDebug>

#include "parserInfo.h"
#include "tcp_sock.h"
#include "network.h"
#include "parsemysql.h"


using namespace std;

//template <typename T
queue<jobInfo_t *> job_list;
//mutex mutex_;
//condition_variable cond_;
static jobInfo_t *currentJobData = nullptr;
static char order_num[128];
static tcp_client comm;

//public handle
packInfo_tx proc_seq_table[D_MAX_PROC_SEQ];
unsigned int tableCounter;


int appendJob(jobInfo_t *job);
void deleteJob(jobInfo_t *job);
extern int buildPacket(packInfo_tx *info);
bool parsePacket(packInfo_rx *info, char *data);
bool verifyPackInfo(packInfo_rx *info);
void test_makeRAWdata(char *img, unsigned int *size);
Network *netHander;

extern int transfer_data_proc(void)
{

	int rval;
    //tcp_client comm;
	string host;

    //qDebug() <<"Entered hostname : 10.1.31.57:5001";
    //cin>>host;

    host = "10.1.31.105";

#if 0
	//connect to host
	rval = comm.conn(host , 5001);
	if(rval < 0){
		cout << "connection fail" <<endl;
        return -1;
	}
#endif
#if DEBUG_I
	packInfo_tx pack;

	memset(&pack, 0, sizeof(packInfo_tx));


	pack.cmd_type = CMD_TYPE_REQUEST;
	pack.action_type = ACT_BARCODE1D;
	pack.item_id = WORK_ORDER;
    pack.cell_num = 1;
    pack.process_num = 1;
    pack.accuracy = 100;

    //char odr[8] = {'a','b','c','d','e','f','g','h'};//910913C00692BA21YBN01115
    //pack.order_size = sizeof(odr);
    //pack.order_num = &odr[0];

    char *image = (char *)malloc(1024*1014*1);

    unsigned int img_size =0;
    test_makeRAWdata(image, &img_size);

    pack.image_size = img_size;
    pack.image_data = image;
    printf("data: %p\n", pack.image_data);

	buildPacket(&pack);
#endif
#if 0
	//unique_lock<std::mutex> mlock(mutex_);
	while (job_list.empty()) // check condition to be safe against spurious wakes
	{
		//cond_.wait(mlock); // release lock and go join the waiting thread queue
		cout << "waiting" << endl;
	}
#endif

#if 1
    if(!job_list.empty())
	{

        qDebug() <<"Entered hostname : 10.1.31.57:5001";

        //connect to host
        rval = comm.conn(host , 5001);
        if(rval < 0){
            cout << "connection fail" <<endl;
            return -1;
        }

        currentJobData =(jobInfo_t *)job_list.front();

		//send some data
        if(!comm.send_data((jobInfo_t *)currentJobData))
		{
			cout << "send falure" << endl;
		}

		//here : receive packet
        comm.receive((jobInfo_t*)currentJobData);
		//deleteJob(temp);




	}

#endif

	return 1;

}

extern void setNetworkHandler(Network* net)
{
    netHander = net;
}

int receiveFunc(char *data)
{
	bool ret;
	cout << "[call]" << __FUNCTION__ <<endl;

    packInfo_rx *info = (packInfo_rx*)malloc(sizeof(packInfo_rx));

	ret = parsePacket(info, data);
	if(ret != true)
	{
		cout << "Error! parse on rev data" << endl;
		return false;
	}
	printf("____ recieved packet ___\n");
	printf("info->cmd_type: %x\n", info->cmd_type);
	printf("info->action_type: %x\n", info->action_type);
	printf("info->work_type: %x\n", info->item_id);
	printf("info->cell_num: %x\n", info->cell_num);
	printf("info->process_num: %x\n", info->process_num);
	printf("info->coord_x: %d\n", info->coordinate_x);
	printf("info->coord_y: %d\n", info->coordinate_y);
	printf("info->data_size: %d\n", info->data_size);    
    printf("info->data: %s\n", info->data);
    printf("__________________________\n");

	if(info->cmd_type == CMD_TYPE_ACK)
	{
		if(verifyPackInfo(info))
		{
			if(info->item_id == WORK_ORDER)
			{
                strncpy(order_num, info->data, info->data_size);
			}
			/* do something * */
            printf("current Model num of order: %s\n", order_num);
            netHander->setIpResults(200, 300, 1);

            deleteJob(currentJobData);
		}
	}
	else if(info->cmd_type == CMD_TYPE_NACK)
	{
		cout <<"received Error from server." <<endl;
        netHander->setIpResults(200, 300, 0);

        deleteJob(currentJobData);
	}
	else
	{
        /*in case of unkown cmd */
	}


	free(info);

    emit netHander->imgProcessFin();
    //temporarily call
    comm.close_sock();


	return 1;
}


int buildPacket(packInfo_tx *info)
{
	int buf_size = info->image_size+info->order_size+D_HEADER_SIZE;
	char *rq_data = (char*)malloc(sizeof(char)*buf_size);
	memset(rq_data, 0, sizeof(char)*buf_size);
	printf("buildPack: totalSize: %d\n", buf_size);

	rq_data[0] = info->cmd_type;
	rq_data[1] = info->action_type;
	rq_data[2] = info->item_id;
	rq_data[3] = info->cell_num;
	rq_data[4] = info->process_num;
	rq_data[5] = info->accuracy;
	rq_data[6] = info->order_size;
	memcpy(&rq_data[7], info->order_num, info->order_size);
	memcpy(&rq_data[7+info->order_size], &info->image_size, sizeof(unsigned int));
	memcpy(&rq_data[D_HEADER_SIZE+info->order_size], \
										info->image_data, info->image_size);

	printf("cmd: %d\n", info->cmd_type);
	printf("action: %d\n", info->action_type);
	printf("work: %d\n", info->item_id);
	printf("size: %d\n", info->image_size);

    jobInfo_t *newJob = (jobInfo_t *)malloc(sizeof(jobInfo_t));	
	newJob->txData = rq_data;
    newJob->txPackInfo = (packInfo_tx *)info;    
	newJob->callback = (callbackFunc)receiveFunc;
	//add job
	appendJob(newJob);

	return 1;
}


int appendJob(jobInfo_t *job)
{
	cout << "[push] new job" << endl;
	//unique_lockMedics mlock(mutex_);
	job_list.push((jobInfo_t*)job);
	//mlock.unlock();     // unlock before notificiation to minimize mutex contention
	//cond_.notify_one(); // notify one waiting thread

	return 1;
}


void deleteJob(jobInfo_t *job)
{
	cout << "[pop] used job" <<endl;
	job_list.pop();
    if(job->txData != nullptr){ free(job->txData); job->txData = nullptr; }
    if(job->txPackInfo != nullptr){ free(job->txPackInfo); job->txPackInfo = nullptr; }
    if(job != nullptr){ free(job); job = nullptr; }
}

bool parsePacket(packInfo_rx *info, char *data)
{	
	if(data != NULL)
	{
		char * rs_data = data;
		info->cmd_type = rs_data[0];
		info->action_type = rs_data[1];
		info->item_id = rs_data[2];
		info->cell_num = rs_data[3];
		info->process_num = rs_data[4];
		memcpy(&info->coordinate_x, &rs_data[5], sizeof(char)*4);
		memcpy(&info->coordinate_y, &rs_data[9], sizeof(char)*4);
		info->matching_rate = rs_data[13];
		info->data_size = rs_data[14];
        memcpy(info->data, &rs_data[15], sizeof(char)*info->data_size);
	}
	else return false;

	cout << "parser success" << endl;
	return true;
}

bool verifyPackInfo(packInfo_rx *info)
{
	bool rval = false;
    packInfo_tx *cPack = currentJobData->txPackInfo;

	if(cPack->cell_num == info->cell_num && (cPack->process_num == info->process_num) \
			&& cPack->action_type == info->action_type && (cPack->item_id == info->item_id))
	{
		rval = true;
	}

	return rval;
}

void getDevOrderNumber(char *num)
{
	strncpy(num, order_num, strlen(order_num)+1);
}

void test_makeRAWdata(char *img,  unsigned int *size)
{

    //char *image = (char *)malloc(1024*1014*1);
//    memcpy(image, 0, sizeof(char)*1024*1024*1);

    string filePath = "test.jpg";

    // read File
    unsigned int idx =0;

	ifstream openFile(filePath.data(), ios::in | ios::binary);
	if( openFile.is_open() ){

		while(!openFile.eof())
		{
			openFile.get(img[idx]);
			idx++;
		}
		openFile.close();
		*size = idx;
	}
	else
	{
		cout <<"can't find file" << endl;
	}

#if DEBUG_
	ofstream output;
	output.open("refile.jpg", ios::app);
	for(int f =0; f < idx; f++)
	{
		output << img[f];
	}
	output << endl;
	output.close();
#endif

}


extern int transfer_proc_init(void)
{
    int ret = -1;



    //memset(proc_seq_table, 0, sizeof(proc_seq_table)); //reset buffer to restore proc sequence table
    //tableCounter = 0;

    char *ordernum = "3029C003AA";
    char *process = "M1";
    ret = getProcessSeqFromDB(ordernum, process);

    return ret;
}


/*
 *
 * switch(info.action_type)
		{
			case ACT_BARCODE1D:
				break;
			case ACT_BARCODE2D:
				break;
			case ACT_BARCODEQR:
				break;
			case ACT_EXIST:
				break;
			case ACT_JUDGEMENT:
			default:
				break;
		}
		switch(info.work_type)
		{
			case WORK_ORDER:
				break;
			case WORK_ITEM1:
				break;
			case WORK_ITEM2:
				break;
			default:
				break;
		}
 */

