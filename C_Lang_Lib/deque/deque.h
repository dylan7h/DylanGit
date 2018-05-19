#ifndef __DEQUE_H__
#define __DEQUE_H__

#include <stdint.h>
#include <stdbool.h>

typedef void* PTR;

typedef struct DEQUE {
	short	m_size;		// 데이터 크기 단위.
	long	m_len;		// 현재 저장된 데이터의 개수.
	long	m_capacity;	// 현재 가용 버퍼의 길이.
	PTR*	m_Buf;		// 버퍼 주소.

	// 사용자 정의 메모리 해제 함수.
	void(*Deletor)(PTR Target);

	// 모든 원소 제거.
	void(*clear)(struct DEQUE* hKey);
	// 첫 번째와 마지막 원소의 주소 반환.
	PTR(*begin)(struct DEQUE* hKey);
	PTR(*end)(struct DEQUE* hKey);
	// 인덱스 pos의 주소 반환.
	PTR(*at)(struct DEQUE* hKey, long pos);
	// 공간을 n으로 변경.
	bool(*resize)(struct DEQUE* hKey, long n);
	// deque이 비었는지 검사.
	bool(*empty)(struct DEQUE* hKey);
	// capacity: 버퍼 용량, size: 저장된 개수.
	long(*capacity)(struct DEQUE* hKey);
	long(*size)(struct DEQUE* hKey);
	// pos에 data 삽입.
	bool(*insert)(struct DEQUE* hKey, long pos, PTR data);
	// pos의 데이터 제거.
	bool(*erase)(struct DEQUE* hKey, long pos);
	// 컨테이너 뒤쪽에서 데이터 삽입/제거
	bool(*push_back)(struct DEQUE* hKey, PTR data);
	bool(*pop_back)(struct DEQUE* hKey, PTR data);
	// 컨테이너 앞쪽에서 데이터 삽입/제거
	bool(*push_front)(struct DEQUE* hKey, PTR data);
	bool(*pop_front)(struct DEQUE* hKey, PTR data);
} deque;

typedef struct ITERATOR {
	const deque*	m_deque_key;
	const bool		m_bReverse;
	const PTR(*reset)(struct ITERATOR* hKey);
	const PTR(*next)(struct ITERATOR* hKey, long step);
	const bool(*check)(struct ITERATOR* hKey);
	long			m_cur;
} iterator;

deque* DequeConstructor(short elemSize, void(*Deletor)(PTR Target));
void DequeDestructor(deque* hKey);

iterator* IteratorConstructor(const deque* deque_key, const bool bReverse);
void IteratorDestructor(iterator* hKey);

#define deque_clear(hKey)				(hKey)->clear(hKey)
#define deque_begin(hKey)				(hKey)->begin(hKey)
#define deque_end(hKey)					(hKey)->end(hKey)
#define deque_at(hKey, pos)				(hKey)->at(hKey, pos)
#define deque_resize(hKey, n)			(hKey)->resize(hKey, n)
#define deque_empty(hKey)				(hKey)->empty(hKey)
#define deque_capacity(hKey)			(hKey)->capacity(hKey)
#define deque_size(hKey)				(hKey)->size(hKey)
#define deque_insert(hKey, pos, data)	(hKey)->insert(hKey, pos, data)
#define deque_erase(hKey, pos)			(hKey)->erase(hKey, pos)
#define deque_push_back(hKey, data)		(hKey)->push_back(hKey, data)
#define deque_pop_back(hKey, data)		(hKey)->pop_back(hKey, data)
#define deque_push_front(hKey, data)	(hKey)->push_front(hKey, data)
#define deque_pop_front(hKey, data)		(hKey)->pop_front(hKey, data)

#define it_reset(hKey)					(hKey)->reset(hKey)
#define it_Check(hKey)					(hKey)->check(hKey)
#define it_next(hKey, step)				(hKey)->next(hKey, step)
#define it_cur(hKey)					(hKey)->next(hKey, 0)

#endif