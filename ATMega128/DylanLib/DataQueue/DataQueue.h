/*
 *  File Name   :   DataQueue.h
 *
 *  Created	    :   2018-04-14 P.M 1:02:26
 *  belong      :   Korea Polytechnic University
 *	                , Department of Energy-Electrical Engineering Student.
 *  Author      :   KOR DYLAN( Korean name: Jun Ki, Hong)
 *  YouTube     :   https://www.youtube.com/channel/UC9DTd1Rv730XKmWRTpqY8Rg?view_as=subscriber
 *  e-mail      :   dylan.7h@gmail.com
 *  Software    :   Atmel Studio 7
 *  Hardware    :   ATmega128, tested on ATmega128(jmod-128-1) at 16MHz.
 */ 

/*
*----------------------------------------------------------------------------
*  [ This queue data structure is a circular queue that uses static memory. ]
*----------------------------------------------------------------------------
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

/*
*  [ The node structure of the queue data structure. ]
*  >> data  : 1 byte data.
*  >> pNext : Address of Next Node
*/
typedef struct _NODE{
	uint8_t data;
	struct _NODE* pNext;
} Node;

/*
*  [ Queue data structure Handle Structure ]
*	- Member variable
*   1. BufSize  : The length of the queue buffer. 
*   2. Len      : The number of data currently stored in the queue buffer.
*   3. Buf      : The address of the buffer to be used for the queue.
*---------------------------------------------------------------------------------------------------------
*   << Note: This buffer memory must be a static memory of Node type assigned from an external source. >>
*---------------------------------------------------------------------------------------------------------
*   4. pFront   : The address of the next node to be ejected.
*   5. pTail    : The address of the next node to insert.
*
*   - Member Function.
*   1. IsEmpty
*   >> if queue Buffer is empty return 1 else return 0
*
*   2. IsFull
*   >> if queue Buffer is full return 1 else return 0
*
*   3. EnQueue
*   >> Put data into the queue buffer.(1 byte)
*
*   4. DeQueue
*   >> Pull data from the queue buffer.(1 byte)
*
*   5. Peek
*   >> Preview the next available data in the queue buffer.(1 byte)
*
*   6. GetQueueLen
*   >> Returns the number of data stored in the current queue buffer.
*/
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

/*  [ Initialize Queue Function ]
*   >> hKey         : Non Initialized hQueue type Handle variable Address.
*   >> newBuf       : The address of the buffer to be used for the queue.
*---------------------------------------------------------------------------------------------------------
*   << Note: This buffer memory must be a static memory of Node type assigned from an external source. >>
*---------------------------------------------------------------------------------------------------------
*   >> BufSize      : The size of the buffer to be used for the queue.
*/
uint8_t InitQueue(hQueue* hKey, Node* newBuf, uint8_t BufSize);

/* [ Macro for ease of use ] */
#define M_IsEmpty(hKey) (hKey)->IsEmpty(hKey)
#define M_IsFull(hKey) (hKey)->IsFull(hKey)
#define M_EnQueue(hKey, Data) (hKey)->EnQueue(hKey, Data)
#define M_DeQueue(hKey, pData) (hKey)->DeQueue(hKey, pData)
#define M_Peek(hKey, pData) (hKey)->Peek(hKey, pData)
#define M_GetQueueLen(hKey) (hKey)->GetQueueLen(hKey)

#endif /* DATAQUEUE_H_ */