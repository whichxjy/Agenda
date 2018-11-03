#ifndef _SYNCHRONOUS_QUEUE_HPP_
#define _SYNCHRONOUS_QUEUE_HPP_

#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>


template <typename T>
class SynchronousQueue {
private:
	std::queue<T> myQueue;
	int queueMaxSize;
	mutable std::mutex myMutex;
	std::condition_variable notEmpty;
	std::condition_variable notFull;
	bool needToStop;
	
	/***********************************************************
	 *  A helper function that adds element to the queue,
	 *  waiting if queue is full.
	 ***********************************************************/
	template <typename Type>
	void putHelper(Type&& elem) {
		std::unique_lock<std::mutex> lock(myMutex);
		while (!needToStop && full()) {
			notFull.wait(lock);
		}
		if (needToStop)
			return;
		myQueue.emplace(std::forward<Type>(elem));
		notEmpty.notify_one();
	}

	/***********************************************************
	 *  Check whether the queue is full without lock.
	 ***********************************************************/
	bool full() const {
		return myQueue.size() == queueMaxSize;
	}

	/***********************************************************
	 *  Check whether the queue is empty without lock.
	 ***********************************************************/
	bool empty() const {
		return myQueue.size() == 0;
	}

public:
	SynchronousQueue(int maxSize) : queueMaxSize(maxSize), needToStop(false) {
	}

	~SynchronousQueue() {
		// If the queue wasn't stoped, then stop it. 
		stop();	
	}

	// No copy
	SynchronousQueue(const SynchronousQueue&) = delete;
	
	// No assignment
	SynchronousQueue& operator=(const SynchronousQueue&) = delete;
	
	/***********************************************************
	 *  Add the specified element to this queue, 
	 *  waiting if queue is full.
	 ***********************************************************/
	void put(const T& elem) {
		putHelper(elem);
	}
	
	void put(T&& elem) {
		putHelper(std::forward<T>(elem));
	}
	
	/***********************************************************
	 *  Retrieve and remove the head of this queue.
	 *  
	 *  There are two modes for this operation: Blocked or not.
	 *  If "Blocking" is true, then wait if the queue is empty.
	 *  If "Blocking" is false, then give up if the queue is empty.
	 ***********************************************************/
	bool take(T& elem, bool Blocking = true) {
		std::unique_lock<std::mutex> lock(myMutex);
		if (Blocking) {
			// blocking mode
			while (!needToStop && empty()) {
			    notEmpty.wait(lock);
			}
		}
		else {
			// non-blocking mode
			if (empty())
				return false; 
		}
		if (needToStop)
			return false;
		// Take element
		elem = myQueue.front();
		myQueue.pop();
		notFull.notify_one();
		return true;
	}
	
	/***********************************************************
	 *  Retrieve and remove all elements of this queue.
	 *  
	 *  There are two modes for this operation: Blocked or not.
	 *  If "Blocking" is true, then wait if the queue is empty.
	 *  If "Blocking" is false, then give up if the queue is empty.
	 ***********************************************************/
	bool takeAll(std::queue<T> &otherQueue, bool Blocking = true) {
		std::unique_lock<std::mutex> lock(myMutex);
		if (Blocking) {
			// blocking mode
			while (!needToStop && empty()) {
			    notEmpty.wait(lock);
			}
		}
		else {
			// non-blocking mode
			if (empty())
				return false; 
		}
		if (needToStop)
			return false;
		// Take all elements
		otherQueue = std::move(myQueue);
		notFull.notify_one();
		return true;
	}
	
	/***********************************************************
	 *  Start queue
	 ***********************************************************/
	void start() {
		std::lock_guard<std::mutex> lock(myMutex);
		// If the queue was stoped, then start it. 
		if (needToStop)
			needToStop = false;
	}
	
	/***********************************************************
	 *  Stop all operations
	 ***********************************************************/
	void stop() {
		std::lock_guard<std::mutex> lock(myMutex);
		// If the queue wasn't stoped, then stop it. 
		if (!needToStop) {
			needToStop = true;
			// Unblocks all threads waiting currently
			notFull.notify_all();
			notEmpty.notify_all();
		}
	}
	
	// Get the size of queue
	int getSize() const {
		std::lock_guard<std::mutex> lock(myMutex);
		return myQueue.size();
	}
	
	// Return true is the queue is full.
	bool isFull() const {
		std::lock_guard<std::mutex> lock(myMutex);
		return myQueue.size() == queueMaxSize;
	}
	
	// Return true is the queue is empty.
	bool isEmpty() const {
		std::lock_guard<std::mutex> lock(myMutex);
		return myQueue.size() == 0;
	}
	
	// Get the max size of the queue.
	int getMaxSize() const {
		std::lock_guard<std::mutex> lock(myMutex);
		return queueMaxSize;
	}
	
	// Set the max size of the queue
	void setMaxSize(int maxSize) {
		std::lock_guard<std::mutex> lock(myMutex);
		queueMaxSize = maxSize;
	}
	
	// Clear all the elements
	void clear() {
		std::lock_guard<std::mutex> lock(myMutex);
		while (!myQueue.empty())
			myQueue.pop();
	}

};


#endif
