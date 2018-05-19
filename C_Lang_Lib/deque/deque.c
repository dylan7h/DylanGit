#include <stdlib.h>
#include <string.h>
#include "deque.h"

// 모든 원소 제거.
static inline void clear(deque* hKey) {
	register long idx;

	for (idx = 0; idx < hKey->m_len; idx++)
		(hKey->Deletor == NULL) ? free(hKey->m_Buf[idx]) : hKey->Deletor(hKey->m_Buf[idx]);
}

// 첫 번째와 마지막 원소의 주소 반환.
static inline PTR begin(deque* hKey) {
	return hKey->m_Buf[0];
}

static inline PTR end(deque* hKey) {
	return hKey->m_Buf[hKey->m_len - 1];
}

// 인덱스 pos의 주소 반환.
static inline PTR at(deque* hKey, long pos) {
	if ((hKey->m_len <= pos) || (pos < 0) || (hKey->m_len == 0))
		return NULL;

	return hKey->m_Buf[pos];
}

// 공간을 n으로 변경.
static inline bool resize(deque* hKey, long n) {
	register long idx;

	// Exception Check1.
	if (n <= 0)
		return false;

	// Trash removal.
	if (hKey->m_len > n) {
		for (idx = n; idx < hKey->m_len; idx++)
			(hKey->Deletor == NULL) ? free(hKey->m_Buf[idx]) : hKey->Deletor(hKey->m_Buf[idx]);
	}

	// Memory Reallocation.
	hKey->m_Buf = (PTR)realloc(hKey->m_Buf, sizeof(PTR) * n);
	// Exception Check2.
	if (hKey->m_Buf == NULL)
		return false;

	hKey->m_capacity = n;

	return true;
}

// deque이 비었는지 검사.
static inline bool full(deque* hKey) {
	return (hKey->m_len < hKey->m_capacity) ? false : true;
}

// deque이 비었는지 검사.
static inline bool empty(deque* hKey) {
	return (hKey->m_len != 0) ? false : true;
}

// capacity: 버퍼 용량, size: 저장된 개수.
static inline long capacity(deque* hKey) {
	return hKey->m_capacity;
}
static inline long size(deque* hKey) {
	return hKey->m_len;
}

// pos에 data 삽입.
static inline bool insert(deque* hKey, long pos, PTR data) {
	if ((hKey->m_len <= pos) || (pos < 0))
		return false;

	if (full(hKey)) {
		// Memory Reallocation.
		hKey->m_Buf = (PTR)realloc(hKey->m_Buf, sizeof(PTR) * (hKey->m_capacity + 2));
		// Exception Check3.
		if (hKey->m_Buf == NULL)
			return false;
		// Increase Capacity.
		hKey->m_capacity += 2;
	}
	// Shift Container.
	memcpy(&hKey->m_Buf[pos + 1], &hKey->m_Buf[pos], sizeof(PTR) * (hKey->m_len - pos));

	// Create Memory Box.
	hKey->m_Buf[pos] = (PTR)malloc(hKey->m_size);
	if (hKey->m_Buf[pos] == NULL)
		return false;

	// Embark data in Memory box.
	memcpy(hKey->m_Buf[pos], data, hKey->m_size);

	//Increase queue count.
	hKey->m_len += 1;

	return true;
}

// pos의 데이터 제거.
static inline bool erase(deque* hKey, long pos) {
	if ((hKey->m_len <= pos) || (pos < 0) || (hKey->m_len == 0))
		return false;

	// Removing the memory box.
	(hKey->Deletor == NULL) ? free(hKey->m_Buf[pos]) : hKey->Deletor(hKey->m_Buf[pos]);
	// Shift Container.
	memcpy(&hKey->m_Buf[pos], &hKey->m_Buf[pos + 1], sizeof(PTR) * (hKey->m_len - pos - 1));
	hKey->m_len -= 1;

	return true;
}

// 컨테이너 뒤쪽에서 데이터 삽입/제거
static inline bool push_back(deque* hKey, PTR data) {
	if (full(hKey)) {
		// Memory Reallocation.
		hKey->m_Buf = (PTR)realloc(hKey->m_Buf, sizeof(PTR) * (hKey->m_capacity + 2));
		// Exception Check3.
		if (hKey->m_Buf == NULL)
			return false;
		// Increase Capacity.
		hKey->m_capacity += 2;
	}

	// Create Memory Box.
	hKey->m_Buf[hKey->m_len] = (PTR)malloc(hKey->m_size);
	if (hKey->m_Buf[hKey->m_len] == NULL)
		return false;

	// Embark data in Memory box.
	memcpy(hKey->m_Buf[hKey->m_len], data, hKey->m_size);
	
	//Increase queue count.
	hKey->m_len += 1;
	
	return true;
}
static inline bool pop_back(deque* hKey, PTR data) {
	if (empty(hKey))
		return false;

	// Take out the data from memory box.
	memcpy(data, hKey->m_Buf[hKey->m_len - 1], hKey->m_size);
	// Removing the memory box.
	(hKey->Deletor == NULL) ? free(hKey->m_Buf[hKey->m_len - 1]) : hKey->Deletor(hKey->m_Buf[hKey->m_len - 1]);
	//Decrease queue count.
	hKey->m_len -= 1;

	return true;
}

// 컨테이너 앞쪽에서 데이터 삽입/제거
static inline bool push_front(deque* hKey, PTR data) {
	if (full(hKey)) {
		// Memory Reallocation.
		hKey->m_Buf = (PTR)realloc(hKey->m_Buf, sizeof(PTR) * (hKey->m_capacity + 2));
		// Exception Check3.
		if (hKey->m_Buf == NULL)
			return false;
		// Increase Capacity.
		hKey->m_capacity += 2;
	}
	// Shift Container.
	memcpy(&hKey->m_Buf[1], &hKey->m_Buf[0], sizeof(PTR) * hKey->m_len);

	// Create Memory Box.
	hKey->m_Buf[0] = (PTR)malloc(hKey->m_size);
	if (hKey->m_Buf[0] == NULL)
		return false;

	// Embark data in Memory box.
	memcpy(hKey->m_Buf[0], data, hKey->m_size);

	//Increase queue count.
	hKey->m_len += 1;

	return true;
}
static inline bool pop_front(deque* hKey, PTR data) {
	if (empty(hKey))
		return false;

	// Take out the data from memory box.
	memcpy(data, hKey->m_Buf[0], hKey->m_size);
	// Removing the memory box.
	(hKey->Deletor == NULL) ? free(hKey->m_Buf[0]) : hKey->Deletor(hKey->m_Buf[0]);
	//Decrease queue count.
	hKey->m_len -= 1;
	// Shift Container.
	memcpy(&hKey->m_Buf[0], &hKey->m_Buf[1], sizeof(PTR) * hKey->m_len);

	return true;
}

deque* DequeConstructor(short elemSize, void(*Deletor)(PTR Target)) {
	// Create deque Object Handle.
	deque* retObj = (deque*)malloc(sizeof(deque));
	// Exception Check!
	if (retObj == NULL)
		return NULL;

	/* Initialize Object Handle. */
	// Initialize Member Variable.
	retObj->m_capacity = 2;
	retObj->m_len = 0;
	retObj->m_size = elemSize;
	// Initialize Object Handle Buf.
	retObj->m_Buf = (PTR*)malloc(sizeof(PTR*) * 10);
	// Exception Check!
	if (retObj->m_Buf == NULL) {
		free(retObj);
		return NULL;
	}

	/* Link Memeber Function */
	retObj->clear = clear;
	retObj->begin = begin;
	retObj->end = end;
	retObj->at = at;
	retObj->resize = resize;
	retObj->empty = empty;
	retObj->capacity = capacity;
	retObj->size = size;
	retObj->insert = insert;
	retObj->erase = erase;
	retObj->push_back = push_back;
	retObj->pop_back = pop_back;
	retObj->push_front = push_front;
	retObj->pop_front = pop_front;
	retObj->Deletor = Deletor;

	return retObj;
}

void DequeDestructor(deque* hKey) {
	hKey->clear(hKey);
	free(hKey);
}

static inline PTR reset(iterator* hKey) {
	hKey->m_cur = hKey->m_bReverse ? (hKey->m_deque_key->m_len - 1) : 0;
	return hKey->m_deque_key->m_Buf[hKey->m_cur];
}

static inline PTR next(iterator* hKey, long step) {
	switch (hKey->m_bReverse) {
	case true:
		hKey->m_cur -= step;
		return (hKey->m_cur < 0) ? NULL : hKey->m_deque_key->m_Buf[hKey->m_cur];
	case false:
		hKey->m_cur += step;
		return (hKey->m_cur >= hKey->m_deque_key->m_len) ? NULL : hKey->m_deque_key->m_Buf[hKey->m_cur];
	}

	return NULL;
}

static inline bool check(iterator* hKey) {
	return (next(hKey, 0) != NULL) ? true : false;
}

iterator* IteratorConstructor(const deque* deque_key, const bool bReverse) {
	iterator temp = { deque_key, bReverse, reset, next, check };
	iterator* retObj = (iterator*)malloc(sizeof(iterator));
	if (retObj == NULL)
		return NULL;

	memcpy(retObj, &temp, sizeof(iterator));

	return retObj;
}

void IteratorDestructor(iterator* hKey) {
	free(hKey);
}