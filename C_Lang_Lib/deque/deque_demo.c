#include <stdio.h>
#include "deque.h"

int main(void) {
	int idx, temp;
	deque* dequeObj = DequeConstructor(sizeof(int), NULL);
	iterator* ItDeque;

	puts("[Init Satus]");
	printf("Len:		%d\n", dequeObj->size(dequeObj));
	printf("ElemSize:	%d\n", dequeObj->m_size);
	printf("Capacity:	%d\n", dequeObj->capacity(dequeObj));

	for (idx = 0; idx < 10; idx++) {
		temp = idx + 5;
		dequeObj->push_front(dequeObj, &temp);
	}

	puts("\n[After push_front]");
	printf("Len:		%d\n", dequeObj->size(dequeObj));
	printf("ElemSize:	%d\n", dequeObj->m_size);
	printf("Capacity:	%d\n\n", dequeObj->capacity(dequeObj));

	ItDeque = IteratorConstructor(dequeObj, false);
	for (it_reset(ItDeque); it_Check(ItDeque); it_next(ItDeque, 1)) {
		temp = *(int*)it_cur(ItDeque);
		printf("%d ", temp);
	}
	puts("\n");

	puts("[After Used Iterator]");
	printf("Len:		%d\n", dequeObj->size(dequeObj));
	printf("ElemSize:	%d\n", dequeObj->m_size);
	printf("Capacity:	%d\n\n", dequeObj->capacity(dequeObj));
	IteratorDestructor(ItDeque);

	for (idx = 0; idx < 10; idx++) {
		dequeObj->pop_front(dequeObj, &temp);
		printf("%d ", temp);
	}

	puts("\n\n[After pop_front]");
	printf("Len:		%d\n", dequeObj->size(dequeObj));
	printf("ElemSize:	%d\n", dequeObj->m_size);
	printf("Capacity:	%d\n", dequeObj->capacity(dequeObj));

	DequeDestructor(dequeObj);

	return 0;
}