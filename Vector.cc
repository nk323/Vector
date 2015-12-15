// Implementation of the templated Vector class
// ECE6122
// Neha Kadam

#include <iostream> // debugging
#include "Vector.h"

// Your implementation here
// Fill in all the necessary functions below
using namespace std;

// Default constructor
template <typename T>
Vector<T>::Vector()
{
  elements = NULL;
  count = 0;
  reserved = 0;
}

// Copy constructor
template <typename T>
Vector<T>::Vector(const Vector& rhs)
{
  count = rhs.count;
  reserved = rhs.reserved;
  elements = (T*) malloc(count * sizeof(T));
 
  size_t i;
  for(i = 0; i < count; i++)
  {
    new (&elements[i]) T(rhs.elements[i]);	
  }

}

// Assignment operator
template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& rhs)
{
  count = rhs.count;
  reserved = rhs.reserved;
  elements = rhs.elements;
  return *this; // return reference to currrent object
}

#ifdef GRAD_STUDENT
// Other constructors
template <typename T>
Vector<T>::Vector(size_t nReserved)
{ // Initialize with reserved memory

  count = 0;
  reserved = nReserved;
  elements = (T*) malloc(reserved * sizeof(T));
}

template <typename T>
Vector<T>::Vector(size_t n, const T& t)
{ // Initialize with "n" copies of "t"

  count = n;
  reserved = n;

  elements = (T*) malloc(count * sizeof(T));
  size_t i;
  for(i = 0; i < count; i++)
  {
    new (&elements[i]) T(t);
  }
}

template <typename T>
void Vector<T>::Reserve(size_t n)
{
  reserved = n;
  elements = (T*) malloc(reserved * sizeof(T));
}
#endif

// Destructor
template <typename T>
Vector<T>::~Vector()
{
  //destroy each object individually
  size_t i;
  for(i = 0; i < count; i++)
  {
    elements[i].~T();
  }

  //now free allocated memory
  free(elements);
  count = 0;
}

// Add and access front and back
template <typename T>
void Vector<T>::Push_Back(const T& rhs)
{
  //first check if we have enough reserved memory
  if(count < reserved)
  {
    count++; //increment count
    new (&elements[count-1]) T(rhs); //last element at count-1
  }
  else
  {

    // enough memory not reserved. need to reallocate
    count++;
    //get a new block of memory
    T* temp = (T*) malloc(count * sizeof(T));
    if(temp != NULL) 
    {
	//allocation succesful
   	for(size_t i = 0; i < count-1; i++)
	{
		new (&temp[i]) T(elements[i]); //copy elements over to the new block
		elements[i].~T(); 	// destroy old elements
	}
	free(elements); 		// free previously alloctated memory 
	elements = temp; 		// assign elements to point to the new block
	new (&elements[count - 1]) T(rhs);// fill last slot with the new element, rhs
	reserved = count;		// update reserved
    }
    
    else if(temp == NULL) //malloc failed
    {
	    cout<<"Memory allocation failed to allocate block of size "<<count * sizeof(T)<<endl;
    }
 
  }
}

template <typename T>
void Vector<T>::Push_Front(const T& rhs)
{

  size_t i;
  //check if space is reserved 
  if(count < reserved)
  {
    // space already reserved. Only need to shift each element by 1 to the right
    count++;
    for(i = count-1; i > 0; i--)
    {
      new (&elements[i]) T(elements[i-1]); //move elements over to the right by 1
      elements[i-1].~T();		// destroy old elements
    }
    
    // Now push in new element at front using in place new
    new (&elements[0]) T(rhs);
  }

  else
  {
    // not enough space reserved. Need to allocate memory again.
    count++;
    T* temp = (T*) malloc(count * sizeof(T));
    if(temp != NULL)
    {
	for(i = count-1; i > 0; i--)
	{
		//do in place new for older elements 
		new (&temp[i]) T(elements[i-1]); // shift elements to thr right by 1
		elements[i-1].~T();  		// destroy old elements
	}
	free(elements); // free memory
	elements = temp; //assign elements pointer to new block of memory

	new (&elements[0]) T(rhs);// push new element to front using in place new
	reserved = count;	// update reserved
    }

    else if(temp == NULL)
    {
	//Memory allocation failed.
	cout<<"Memory allocation failed to allocate block of size "<<count * sizeof(T)<<endl;
    }
  }
}

template <typename T>
void Vector<T>::Pop_Back()
{ // Remove last element

  //destroy last element and update count
  elements[count-1].~T(); //last element at count-1
  count--;
}

template <typename T>
void Vector<T>::Pop_Front()
{ // Remove first element

  size_t i;
  
  // destroy 1st element and update count 
  elements[0].~T();
  count--;

  // now shift all elements to the left by 1
  for(i = 1; i <= count; i++)
  {
	new (&elements[i-1]) T(elements[i]);
	elements[i].~T();
  }

  // No changes to reserve as memory is not given back
}

// Element Access
template <typename T>
T& Vector<T>::Front() const
{
  return elements[0]; //return first element
}

// Element Access
template <typename T>
T& Vector<T>::Back() const
{
  return elements[count-1]; // return last element
}

template <typename T>
const T& Vector<T>::operator[](size_t i) const
{
  return elements[i]; // return 'i'th element
}

template <typename T>
size_t Vector<T>::Size() const
{
  return count;
}

template <typename T>
bool Vector<T>::Empty() const
{
  if(count == 0) 
    return true;
  else 
    return false;
}

template <typename T>
T&  Vector<T>::operator[](size_t i)
{
  return elements[i];
}

// Implement clear
template <typename T>
void Vector<T>::Clear()
{
  
  for(size_t i = 0; i < count; i++)
  {
    elements[i].~T();
  }
  //update count to indicate that vector is now empty
  count = 0;
}

// Iterator access functions
template <typename T>
VectorIterator<T> Vector<T>::Begin() const
{
  return VectorIterator<T>(elements);
}

template <typename T>
VectorIterator<T> Vector<T>::End() const
{
  return VectorIterator<T>(&elements[count]); // return ref to one beyond last element
}

#ifdef GRAD_STUDENT
// Erase and insert
template <typename T>
void Vector<T>::Erase(const VectorIterator<T>& it)
{

  VectorIterator<T> temp_itr;
  size_t index = 0;
  count--;
  for(temp_itr = elements; temp_itr != &elements[count-1]; temp_itr++)
  {
    index++;
    if(temp_itr == it) break;// break when a match if found
  }

  // element was found at index-1. destroy it.
  elements[index-1].~T();

  // now left shift the following elements by 1
  for(size_t i = index; i <= count; i++)
  {
    new (&elements[i-1]) T(elements[i]);
    elements[i].~T();
  }
  
  //No changes to reserve  
}

template <typename T>
void Vector<T>::Insert(const T& rhs, const VectorIterator<T>& it)
{
  count++;

  VectorIterator<T> temp_itr;
  size_t index = 0;
  size_t i;
  //get a new block of memory
  T* temp = (T*) malloc(count * sizeof(T));

  for(temp_itr = elements; temp_itr != &elements[count-1]; temp_itr++)
  {
    index++;
    if(temp_itr == it) break;
  }

  // element is to be inserted at index-1
  // copy elements to new vector till index-2
  for(i = 0; i < index-1; i++)
  {
    new (&temp[i]) T(elements[i]);
   elements[i].~T();
  }

  //now insert the new element, rhs, at index-1
  new (&temp[index-1]) T(rhs);

  //now copy the rest
  for(i = index; i < count; i++)
  {
    new (&temp[i]) T(elements[i-1]);
    elements[i-1].~T();
  }

  free(elements); // free the old memory
  elements = temp; // assign element pointer to new memory

}
#endif

// Implement the iterators

// Constructors
template <typename T>
VectorIterator<T>::VectorIterator()
{
  current = NULL;
}

template <typename T>
VectorIterator<T>::VectorIterator(T* c)
{
  current = c;
}

// Copy constructor
template <typename T>
VectorIterator<T>::VectorIterator(const VectorIterator<T>& rhs)
{
  current = rhs.current;
}

// Iterator defeferencing operator
template <typename T>
T& VectorIterator<T>::operator*() const
{
  return *current;
}

// Prefix increment
template <typename T>
VectorIterator<T>  VectorIterator<T>::operator++()
{
  //return incremented iterator 
  current++;
  return *this;
}

// Postfix increment
template <typename T>
VectorIterator<T> VectorIterator<T>::operator++(int)
{
  VectorIterator<T> temp_itr(current); //init temp_itr with current
  current++; 			// increment current 
  return temp_itr; 		// and return previous value of current i.e. before increment
}

// Comparison operators
template <typename T>
bool VectorIterator<T>::operator !=(const VectorIterator<T>& rhs) const
{

  if(current != rhs.current) 
    return true;
  else
    return false;
}

template <typename T>
bool VectorIterator<T>::operator ==(const VectorIterator<T>& rhs) const
{

  if(current == rhs.current)
    return true;
  else 
    return false;
}



