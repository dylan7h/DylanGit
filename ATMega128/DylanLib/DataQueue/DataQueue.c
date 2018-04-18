/*
 * DataQueue.c
 *
 * Created: 2018-04-14 오후 1:02:03
 *  Author: kazam
 */ 
#include <stdlib.h>
#include "DataQueue.h"

static inline uint8_t IsEmpty(hQueue* hKey){
	return (hKey->Len == 0) ? TRUE : FALSE;
}

static inline uint8_t IsFull(hQueue* hKey){
	return (hKey->Len == hKey->BufSize) ? TRUE : FALSE;
}

static inline uint8_t EnQueue(hQueue* hKey, uint8_t Data){
	if(IsFull(hKey))
		return FULL_BUF;
		
	hKey->pTail->data = Data;
	hKey->pTail = hKey->pTail->pNext;
	hKey->Len++;
		
	return NONE;
}

static inline uint8_t DeQueue(hQueue* hKey, uint8_t* Data){
	if(IsEmpty(hKey))
		return EMPTY_BUF;
	
	(*Data) = hKey->pFront->data;
	hKey->pFront = hKey->pFront->pNext;
	hKey->Len--;
		
	return NONE;
}

static inline uint8_t Peek(hQueue* hKey, uint8_t* Data){
	if(IsEmpty(hKey))
		return EMPTY_BUF;
	
	(*Data) = hKey->pFront->data;
	
	return NONE;
}

static inline uint8_t GetQueueLen(hQueue* hKey){
	return hKey->Len;
}

uint8_t InitQueue(hQueue* hKey, Node* newBuf, uint8_t BufSize){
	register uint8_t idx;
	
	if(hKey == NULL) return NULL_KEY;
	if(newBuf == NULL) return NULL_BUF;
	
	hKey->Buf = newBuf;
	hKey->BufSize = BufSize;
	hKey->Len = 0;
	
	// Create a circular queue in the form of a static list.
	for(idx = 0; idx < (BufSize - 1); idx++){
		hKey->Buf[idx].data = 0;
		hKey->Buf[idx].pNext = &hKey->Buf[idx + 1];
	}
	
	hKey->Buf[BufSize - 1].data = 0;
	hKey->Buf[BufSize - 1].pNext = hKey->Buf;
	
	// Initialize Position Index 0.
	hKey->pFront = hKey->Buf;
	hKey->pTail = hKey->Buf;
	
	// Linking Function Pointer
	hKey->IsFull = IsFull;
	hKey->IsEmpty = IsEmpty;
	hKey->EnQueue = EnQueue;
	hKey->DeQueue = DeQueue;
	hKey->GetQueueLen = GetQueueLen;
	hKey->Peek = Peek;
	
	return NONE;
}