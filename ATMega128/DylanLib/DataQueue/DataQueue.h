/*
 * DataQueue.h
 *
 * Created: 2018-04-14 오후 1:01:56
 *  Author: kazam
 */ 


#ifndef DATAQUEUE_H_
#define DATAQUEUE_H_

#define NONE		0
#define FALSE		0
#define TRUE		1
#define FULL_BUF	2
#define EMPTY_BUF	3
#define NULL_KEY	4
#define NULL_BUF	5

#include <stdint.h>

typedef struct _NODE{
	uint8_t data;
	struct _NODE* pNext;
} Node;

typedef struct QUEUE_HANDLE {
	uint8_t BufSize;
	uint8_t Len;
	
	Node* Buf;
	Node* pFront;
	Node* pTail;
	
	uint8_t(*IsEmpty)(struct QUEUE_HANDLE* hKey);
	uint8_t(*IsFull)(struct QUEUE_HANDLE* hKey);
	uint8_t(*EnQueue)(struct QUEUE_HANDLE* hKey, uint8_t Data);
	uint8_t(*DeQueue)(struct QUEUE_HANDLE* hKey, uint8_t* Data);
	uint8_t(*Peek)(struct QUEUE_HANDLE* hKey, uint8_t* Data);
	uint8_t(*GetQueueLen)(struct QUEUE_HANDLE* hKey);		
} hQueue;

#define M_IsEmpty(hKey) (hKey)->IsEmpty(hKey)
#define M_IsFull(hKey) (hKey)->IsFull(hKey)
#define M_EnQueue(hKey, Data) (hKey)->EnQueue(hKey, Data)
#define M_DeQueue(hKey, pData) (hKey)->DeQueue(hKey, pData)
#define M_Peek(hKey, pData) (hKey)->Peek(hKey, pData)
#define M_GetQueueLen(hKey) (hKey)->GetQueueLen(hKey)

uint8_t InitQueue(hQueue* hKey, Node* newBuf, uint8_t BufSize);

#endif /* DATAQUEUE_H_ */