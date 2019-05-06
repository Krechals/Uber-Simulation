// Copyright 2019 Topala Andrei
#include "./LinkedList.h"

template<typename T>
list_elem<T> * LinkedList<T>::front() {
	return pfirst;
}
template<typename T>
LinkedList<T>::LinkedList() {
	pfirst = plast = NULL;
}
// Add an element to start of list
template<typename T>
void LinkedList<T>::addFirst(T x) {
	struct list_elem<T> *paux;

	paux = new struct list_elem<T>;
	paux->info = x;
	paux->prev = NULL;
	paux->next = pfirst;

	if (pfirst != NULL) {
		pfirst->prev = paux;
	}
	pfirst = paux;

	if (plast == NULL) {
		plast = pfirst;
	}
}
// Add an element to end of list
template<typename T>
void LinkedList<T>::addLast(T x) {
	struct list_elem<T> *paux;

	paux = new struct list_elem<T>;
	paux->info = x;
	paux->prev = plast;
	paux->next = NULL;

	if (plast != NULL) {
		plast->next = paux;
	}
	plast = paux;
	if (pfirst == NULL) {
		pfirst = plast;
	}
}
// Remove element from start of the list
template<typename T>
void LinkedList<T>::removeFirst() {
	struct list_elem<T>* paux;

	if (pfirst != NULL) {
		paux = pfirst->next;
		if (pfirst == plast) {
			plast = NULL;
		}
		delete pfirst;
		pfirst = paux;
		if (pfirst != NULL) {
			pfirst->prev = NULL;
		}
	} else {
		return;
	}
}
// Check if list is empty
template<typename T>
bool LinkedList<T>::isEmpty() {
	if (pfirst == NULL) {
		return true;
	}
	return false;
}
