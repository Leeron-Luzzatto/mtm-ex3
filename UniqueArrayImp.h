#ifndef MTMPARKINGLOT_UNIQUEARRAYIMP_H
#define MTMPARKINGLOT_UNIQUEARRAYIMP_H

template <class Element, class Compare>
UniqueArray<Element, Compare>::UniqueArray(unsigned int size):
	data(new Element*[size]), size(size){
	for (unsigned int i = 0; i < size; ++i) {
		data[i] = nullptr;
	}
}

template <class Element, class Compare>
UniqueArray<Element, Compare>::UniqueArray(const UniqueArray& other):
	data(new Element*[other.size]), size(other.size) {
	for (int i = 0; i < size; ++i) {
		if (other.data[i] == NULL) {
			data[i] = nullptr;
		}
		else {
			data[i] = new Element(*(other.data[i]));
		}
	}
}

template <class Element, class Compare>
UniqueArray<Element, Compare>::~UniqueArray<Element, Compare>() {
	for (int i = 0; i < size; ++i) {
		delete data[i];
	}
	delete[] data;
}

template <class Element, class Compare>
bool UniqueArray<Element, Compare>::availableIndex(unsigned int& index) {
	for (int i = 0; i < size; ++i) {
		if (data[i] == nullptr) {
			index = i;
			return true;
		}
	}
	return false;
}

template <class Element, class Compare>
unsigned int UniqueArray<Element, Compare>::insert(const Element& element) {
	unsigned int index = 0;
	if (getIndex(element, index)) {  // Checks if the element is already exists
		return index;
	}
	if (!availableIndex(index)) { // Checks if the array is full
		throw UniqueArrayIsFullException();
	}
	data[index] = new Element(element);
	return index;
}

template <class Element, class Compare>
unsigned int UniqueArray<Element, Compare>::insert_by_reference(Element& element){
    unsigned int index = 0;
	if (getIndex(element, index)) {  // Checks if the element is already exists
		return index;
	}
	if (!availableIndex(index)) { // Checks if the array is full
		throw UniqueArrayIsFullException();
	}
	data[index] = &(element);
	return index;
}

template <class Element, class Compare>
bool UniqueArray<Element, Compare>::getIndex(const Element& element,
	unsigned int& index) const{
	for (int i = 0; i < size; ++i) {
		if (data[i] != nullptr && Compare()(*data[i] ,element)) {
			index = i;
			return true;
		}
	}
	return false;
}

template <class Element, class Compare>
const Element* UniqueArray<Element, Compare>::operator[]
(const Element& element) const{
	unsigned int index=0;
	if (!getIndex(element, index)){
		return nullptr;
	}
	return data[index];
}

template <class Element, class Compare>
bool UniqueArray<Element, Compare>::remove(const Element& element) {
	unsigned int index=0;
	if (!getIndex(element, index)) { // Means it doesn't exist in the array
		return false;
	}
	delete data[index];
	data[index] = nullptr;
	return true;
}

template <class Element, class Compare>
unsigned int UniqueArray<Element, Compare>::getCount() const{
	unsigned int count = 0;
	for (int i = 0; i < size; ++i) {
		if (data[i] != nullptr) {
			++count;
		}
	}
	return count;
}

template <class Element, class Compare>
unsigned int UniqueArray<Element,Compare>::getSize() const{
	return size;
}

template <class Element, class Compare>
UniqueArray<Element, Compare> UniqueArray<Element, Compare>::
filter(const Filter& f) const {
	UniqueArray<Element, Compare> filtered_array(*this);
	for (int i = 0; i < size; ++i) {
		if (data[i] != nullptr && !f(*data[i])) {
			filtered_array.remove(*data[i]);
		}
	}
	return filtered_array;
}

template <class Element, class Compare>
int UniqueArray<Element, Compare>::iterator::initializeIterator(int start)
const {
	for (int i = start; i < unique_array->size; ++i) {
		if (unique_array->data[i] != nullptr) {
			return i;
		}
	}
	return end_unique_array;
}

template <class Element, class Compare>
UniqueArray<Element, Compare>::iterator::
iterator(UniqueArray* unique_array, int index) :
	unique_array(unique_array), index(index) {
}

template <class Element, class Compare>
UniqueArray<Element,Compare>::iterator::iterator(UniqueArray* unique_array):
	unique_array(unique_array), index(initializeIterator()){
}

template <class Element, class Compare>
Element& UniqueArray<Element, Compare>::iterator::operator*() const {
	return *(unique_array->data[index]);
}

template <class Element, class Compare>
typename UniqueArray<Element, Compare>::iterator&
UniqueArray<Element, Compare>::iterator::operator++(){
	index= initializeIterator(index+1);
	return *this;
}

template <class Element, class Compare>
bool UniqueArray<Element, Compare>::iterator::operator==(const iterator& other)
const{
	return unique_array == other.unique_array && index == other.index;
}

template <class Element, class Compare>
bool UniqueArray<Element, Compare>::iterator::operator!=(const iterator& other)
const{
	return !(*this == other);
}

template <class Element, class Compare>
typename UniqueArray<Element, Compare>::iterator
UniqueArray<Element, Compare>::begin(){
	return UniqueArray<Element, Compare>::iterator(this);
}

template <class Element, class Compare>
typename UniqueArray<Element, Compare>::iterator
UniqueArray<Element, Compare>::end(){
	return UniqueArray<Element, Compare>::iterator
	(this, iterator::end_unique_array);
}

template <class Element, class Compare>
int UniqueArray<Element, Compare>::const_iterator::initializeIterator(int start)
const {
	for (int i = start; i < unique_array->size; ++i) {
		if (unique_array->data[i] != nullptr) {
			return i;
		}
	}
	return end_unique_array;
}

template <class Element, class Compare>
UniqueArray<Element, Compare>::const_iterator::
const_iterator(const UniqueArray* const unique_array, int i) :
	unique_array(unique_array), index(i) {
}

template <class Element, class Compare>
UniqueArray<Element, Compare>::const_iterator::
const_iterator(const UniqueArray* const unique_array) :
	unique_array(unique_array), index(initializeIterator()) {
}

template <class Element, class Compare>
const Element& UniqueArray<Element, Compare>::const_iterator::operator*() const {
	return *(unique_array->data[index]);
}

template <class Element, class Compare>
typename UniqueArray<Element, Compare>::const_iterator&
UniqueArray<Element, Compare>::const_iterator::operator++() {
	index = initializeIterator(index + 1);
	return *this;
}

template <class Element, class Compare>
bool UniqueArray<Element, Compare>::const_iterator::operator==(const const_iterator& other)
const {
	return unique_array == other.unique_array && index == other.index;
}

template <class Element, class Compare>
bool UniqueArray<Element, Compare>::const_iterator::operator!=(const const_iterator& other)
const {
	return !(*this == other);
}

template <class Element, class Compare>
typename UniqueArray<Element, Compare>::const_iterator
UniqueArray<Element, Compare>::const_begin() const {
	return UniqueArray<Element, Compare>::const_iterator(this);
}

template <class Element, class Compare>
typename UniqueArray<Element, Compare>::const_iterator
UniqueArray<Element, Compare>::const_end() const {
	return UniqueArray<Element, Compare>::const_iterator
	(this, const_iterator::end_unique_array);
}

#endif //MTMPARKINGLOT_UNIQUEARRAYIMP_H