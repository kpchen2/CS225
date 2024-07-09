/**
 * @file list.cpp
 * Doubly Linked List (MP 3).
 */

template <class T>
List<T>::List() { 
  // @TODO: graded in mp_lists part 1
    head_ = NULL;
    tail_ = NULL;
    length_ = 0;
}

/**
 * Returns a ListIterator with a position at the beginning of
 * the List.
 */
template <typename T>
typename List<T>::ListIterator List<T>::begin() const {
  // @TODO: graded in mp_lists part 1
  return List<T>::ListIterator(head_);
}

/**
 * Returns a ListIterator one past the end of the List.
 */
template <typename T>
typename List<T>::ListIterator List<T>::end() const {
  // @TODO: graded in mp_lists part 1
  return List<T>::ListIterator(nullptr);
}


/**
 * Destroys all dynamically allocated memory associated with the current
 * List class.
 */
template <typename T>
void List<T>::_destroy() {
  /// @todo Graded in mp_lists part 1
  if (head_ != nullptr) {
   ListNode* temp = head_->next;
   while (temp != nullptr) {
     delete head_;
     head_ = temp;
     temp = temp->next;
   }
   delete head_;
 }
}

/**
 * Inserts a new node at the front of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <typename T>
void List<T>::insertFront(T const & ndata) {
  /// @todo Graded in mp_lists part 1
  ListNode* newNode = new ListNode(ndata);
  newNode->next = head_;
  newNode->prev = nullptr;
  if (head_ != nullptr) {
    head_->prev = newNode;
  }
  head_ = newNode;
  if (tail_ == nullptr) {
    tail_ = newNode;
  }
 length_++;
}

/**
 * Inserts a new node at the back of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <typename T>
void List<T>::insertBack(const T & ndata) {
  /// @todo Graded in mp_lists part 1
  ListNode* newNode = new ListNode(ndata);
  newNode->next = nullptr;
  newNode->prev = tail_;
  if (tail_ != NULL) {
    tail_->next = newNode;
  }
  tail_ = newNode;
  if (head_ == nullptr) {
    head_ = newNode;
  }
  length_++;
}

/**
 * Helper function to split a sequence of linked memory at the node
 * splitPoint steps **after** start. In other words, it should disconnect
 * the sequence of linked memory after the given number of nodes, and
 * return a pointer to the starting node of the new sequence of linked
 * memory.
 *
 * This function **SHOULD NOT** create **ANY** new List or ListNode objects!
 *
 * This function is also called by the public split() function located in
 * List-given.hpp
 *
 * @param start The node to start from.
 * @param splitPoint The number of steps to walk before splitting.
 * @return The starting node of the sequence that was split off.
 */
template <typename T>
typename List<T>::ListNode * List<T>::split(ListNode * start, int splitPoint) {
  /// @todo Graded in mp_lists part 1
  ListNode* curr = start;

 for (int i = 0; i < splitPoint; i++) {
   if (curr == nullptr) {
     return nullptr;
   }
   curr = curr->next;
 }

 if (curr != nullptr) {
  if (curr->prev == nullptr) {
    return start;
  }
  curr->prev->next = nullptr;
  curr->prev = nullptr;
  return curr;
  }

  return nullptr;
}

/**
 * Modifies the List using the waterfall algorithm.
 * Every other node (starting from the second one) is removed from the
 * List, but appended at the back, becoming the new tail. This continues
 * until the next thing to be removed is either the tail (**not necessarily
 * the original tail!**) or NULL.  You may **NOT** allocate new ListNodes.
 * Note that since the tail should be continuously updated, some nodes will
 * be moved more than once.
 */
template <typename T>
void List<T>::waterfall() {
  /// @todo Graded in part 1
  ListNode* cur = head_;
  unsigned count = 0;
  while (cur != nullptr && cur != tail_) {
    if (count%2 == 1) {
      ListNode* temp = cur;
      cur->prev->next = cur->next;
      cur->next->prev = cur->prev;

      tail_->next = cur;
      cur->prev = tail_;
      cur = cur->next;
      temp->next = nullptr;
      tail_ = temp;
    } else {
      cur = cur->next;
    }
    count++;
  }
}


/**
 * Reverses the current List.
 */
template <typename T>
void List<T>::reverse() {
  reverse(head_, tail_);
}

/**
 * Helper function to reverse a sequence of linked memory inside a List,
 * starting at startPoint and ending at endPoint. You are responsible for
 * updating startPoint and endPoint to point to the new starting and ending
 * points of the rearranged sequence of linked memory in question.
 *
 * @param startPoint A pointer reference to the first node in the sequence
 *  to be reversed.
 * @param endPoint A pointer reference to the last node in the sequence to
 *  be reversed.
 */
template <typename T>
void List<T>::reverse(ListNode *& startPoint, ListNode *& endPoint) {
  /// @todo Graded in mp_lists part 2
  ListNode* s_ptr = startPoint;
  ListNode* e_ptr = endPoint;
  
  while (s_ptr != nullptr && e_ptr != nullptr && s_ptr != e_ptr) {
    ListNode* s_next = s_ptr->next;
    ListNode* s_prev = s_ptr->prev;
    ListNode* e_next = e_ptr->next;
    ListNode* e_prev = e_ptr->prev;
    ListNode* ptr;

    if (s_ptr == startPoint && e_ptr == endPoint) {
      ptr = startPoint;
      startPoint = endPoint;
      endPoint = ptr;

      if (s_prev != nullptr) {
        s_prev->next = e_ptr;
      }
      if (e_next != nullptr) {
        e_next->prev = s_ptr;
      }
    } else {
      s_ptr->prev->next = e_ptr;
      e_ptr->next->prev = s_ptr;
    }

    if (s_ptr->next != e_ptr) {
      ptr = s_ptr->next;
      s_ptr->next = e_ptr->next;
      e_ptr->next = ptr;

      ptr = s_ptr->prev;
      s_ptr->prev = e_ptr->prev;
      e_ptr->prev = ptr;

      s_next->prev = e_ptr;
      e_prev->next = s_ptr;
    } else {
      ptr = s_ptr;
      s_ptr->next = e_ptr->next;
      e_ptr->next = ptr;

      ptr = s_ptr->prev;
      s_ptr->prev = e_ptr;
      e_ptr->prev = ptr;
    }

    s_ptr = s_next;
    if (s_ptr == e_ptr) {
      break;
    }
    e_ptr = e_prev;
  }
}

/**
 * Reverses blocks of size n in the current List. You should use your
 * reverse( ListNode * &, ListNode * & ) helper function in this method!
 *
 * @param n The size of the blocks in the List to be reversed.
 */
template <typename T>
void List<T>::reverseNth(int n) {
  /// @todo Graded in mp_lists part 2
  ListNode* begin = head_;
  ListNode* end = head_;
  int num = 1;

  if (n > 1 && end != nullptr) {
    while (end->next != nullptr) {
      if (num%n == 0) {
        if (num == n) {
          reverse(head_,end);
        } else {
          reverse(begin,end);
        }
        begin = end->next;
      }
      end = end->next;
      num++;
    }
    reverse(begin,tail_);
  }
}


/**
 * Merges the given sorted list into the current sorted list.
 *
 * @param otherList List to be merged into the current list.
 */
template <typename T>
void List<T>::mergeWith(List<T> & otherList) {
    // set up the current list
    head_ = merge(head_, otherList.head_);
    tail_ = head_;

    // make sure there is a node in the new list
    if (tail_ != NULL) {
        while (tail_->next != NULL)
            tail_ = tail_->next;
    }
    length_ = length_ + otherList.length_;

    // empty out the parameter list
    otherList.head_ = NULL;
    otherList.tail_ = NULL;
    otherList.length_ = 0;
}

/**
 * Helper function to merge two **sorted** and **independent** sequences of
 * linked memory. The result should be a single sequence that is itself
 * sorted.
 *
 * This function **SHOULD NOT** create **ANY** new List objects.
 *
 * @param first The starting node of the first sequence.
 * @param second The starting node of the second sequence.
 * @return The starting node of the resulting, sorted sequence.
 */
template <typename T>
typename List<T>::ListNode * List<T>::merge(ListNode * first, ListNode* second) {
  /// @todo Graded in mp_lists part 2
  if (first == nullptr) {
    return second;
  }
  if (second == nullptr) {
    return first;
  }

  if (second->data < first->data) {
    ListNode* temp = first;
    first = second;
    second = temp;
  }
  
  ListNode* ans = first;

  while (first->next != nullptr) {
    if (second == nullptr) {
      first = first->next;
    } else if (first->next->data < second->data) {
      first = first->next;
    } else {
      ListNode* first_next = first->next;
      ListNode* first_prev = first->prev;
      ListNode* second_next = second->next;
      ListNode* second_prev = second->prev;

      first->next = second;
      second->next = first_next;
      first_next->prev = second;
      second->prev = first;
      if (second_next != nullptr) {
        second_next->prev = nullptr;
      }
      second = second_next;
    }
  }
  while (second != nullptr) {
    ListNode* first_next = first->next;
    ListNode* first_prev = first->prev;
    ListNode* second_next = second->next;
    ListNode* second_prev = second->prev;

    first->next = second;
    second->next = first_next;
    if (first_next != nullptr) {
      first_next->prev = second;
    }
    second->prev = first;
    if (second_next != nullptr) {
      second_next->prev = nullptr;
    }
    second = second_next;
    first = first->next;
  }
  return ans;
}

/**
 * Sorts a chain of linked memory given a start node and a size.
 * This is the recursive helper for the Mergesort algorithm (i.e., this is
 * the divide-and-conquer step).
 *
 * Called by the public sort function in List-given.hpp
 *
 * @param start Starting point of the chain.
 * @param chainLength Size of the chain to be sorted.
 * @return A pointer to the beginning of the now sorted chain.
 */
template <typename T>
typename List<T>::ListNode* List<T>::mergesort(ListNode * start, int chainLength) {
  /// @todo Graded in mp_lists part 2
  if (chainLength == 1) {
    if (start->next != nullptr) {
      start->next->prev = nullptr;
      start->next = nullptr;
    }
    if (start->prev != nullptr) {
      start->prev->next = nullptr;
      start->prev = nullptr;
    }
    return start;
  }
  ListNode* cur = start;
  int mid = chainLength/2;
  for (int i = 0; i < mid; i++) {
    cur = cur->next;
  }

  ListNode* first = mergesort(start, mid);
  ListNode* second = mergesort(cur, chainLength-mid);

  return merge(first, second);
}
