/*
 *  File Name   :   DataQueue.c
 *
 *  Created	    :   2018-04-14 오후 1:02:03
 *  belong      :   Korea Polytechnic University
 *	                , Department of Energy-Electrical Engineering Student.
 *  Author      :   KOR DYLAN( Korean name: Jun Ki, Hong)
 *  YouTube     :   https://www.youtube.com/channel/UC9DTd1Rv730XKmWRTpqY8Rg?view_as=subscriber
 *  e-mail      :   dylan.7h@gmail.com
 *  IDE Software:   Atmel Studio 7
 *  Hardware    :   ATmega128, tested on ATmega128(jmod-128-1) at 16MHz.
 *  Note        :   This library is distributed in the hope that it will be useful. However, no warranty is given.
 *  place       :   In April 2018 at the Korea Polytechnic University, Technology Innovation Park 401 ...
 */ 

#include <stdlib.h>
#include "DataQueue.h"

static inline uint8_t IsEmpty(hQueue* const hKey){
	return (hKey->Len == 0) ? TRUE : FALSE;
}

static inline uint8_t IsFull(hQueue* const hKey){
	return (hKey->Len == hKey->BufSize) ? TRUE : FALSE;
}

static inline uint8_t EnQueue(hQueue* const hKey, const uint8_t Data){
	if(IsFull(hKey))
		return FULL_BUF;
		
	hKey->pTail->data = Data;
	hKey->pTail = hKey->pTail->pNext;
	hKey->Len++;
		
	return NONE;
}

static inline uint8_t DeQueue(hQueue* const hKey, uint8_t* const Data){
	if(IsEmpty(hKey))
		return EMPTY_BUF;
	
	(*Data) = hKey->pFront->data;
	hKey->pFront = hKey->pFront->pNext;
	hKey->Len--;
		
	return NONE;
}

static inline uint8_t Peek(hQueue* const hKey, uint8_t* const Data){
	if(IsEmpty(hKey))
		return EMPTY_BUF;
	
	(*Data) = hKey->pFront->data;
	
	return NONE;
}

static inline uint8_t GetQueueLen(hQueue* const hKey){
	return hKey->Len;
}

uint8_t InitQueue(hQueue* const hKey, Node* const newBuf, const uint8_t BufSize){
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