/*avlt.h*/

//
// << Gurleen Kaur >>
// U. of Illinois, Chicago
// CS 251: Spring 2020
// Project #05: Threaded AVL tree
// 03/12/2020
//
// Threaded AVL tree: This program let's the user insert values and
// makes a threaded avl tree out of the values given. If there is nothing to
// the right of the key then it's threaded to nullptr and if there is other key
// to the right then it's threaded to that key. This functions also takes in
// account of the heights and if the tree start getting heavy on one side then
// it does the right or left rotation accoringly. This function also allows to
// search a key and find value of key using [] operator and find key to the right
// using () operator. This also checks if two trees are equal.
//

#pragma once

#include <iostream>
#include <vector>
#include <stack>

using namespace std;

template<typename KeyT, typename ValueT>
class avlt
{
private:
    struct NODE
    {
        KeyT   Key;
        ValueT Value;
        NODE*  Left;
        NODE*  Right;
        bool   isThreaded; // true =&gt; Right is a thread, false =&gt; non-threaded
        int    Height;     // height of tree rooted at this node
    };

    NODE* Root;            // pointer to root node of tree (nullptr if empty)
    int   Size;            // # of nodes in the tree (0 if empty)
    NODE* Next;            // used by threaded inorder traversal to denote next key


    //
    // insert copy
    //
    // Inserts function for copy constructor that inserts the keys
    // but doesn't rotate
    //
    // Time complexity:  O(lgN) on average
    //
    void _insertCopy(KeyT key, ValueT value)
    {
        NODE* prev = nullptr;
        NODE* cur = Root;

        //
        // stack the nodes we visit so we can walk back up
        // the search path later, adjusting heights:
        //
        stack<NODE*> updatingHeight;

        //
        // Search to see if tree already contains key
        //
        while (cur != nullptr)
        {
            // check if key is already in tree
            if (key == cur->Key)
                return;

            updatingHeight.push(cur);

            // if key is smaller then cur key then check left
            // and set prev to cur
            if (key < cur->Key)
            {
                prev = cur;
                cur = cur->Left;
            }
            else
            {
                // if cur is threaded then set cur to nullptr
                // and prev to cur
                if (cur->isThreaded) {
                    prev = cur;
                    cur = nullptr;
                }
                    // else check right of the tree and set prev to cur
                else{
                    prev = cur;
                    cur = cur->Right;
                }//else{
            }//else
        }//while

        //
        // so allocate a new node to insert
        //
        NODE* newNode = new NODE();
        newNode->Key = key;
        newNode->Value = value;
        newNode->Left = nullptr;
//         newNode->Right = nullptr;
//         newNode->Height = 0;

        //
        // link in the new node
        //
        // cur is null, and prev denotes node where
        // we fell out of the tree.  if prev is null, then
        // the tree is empty and the Root pointer needs
        // to be updated and increment size.
        //
        if(prev == nullptr){
            Root = newNode;
            newNode->Right = nullptr;
        }
        else{
            // if key is smaller then prev key
            if(key < prev->Key)
            {
                prev->Left = newNode;          // set left of prev to newNode
                newNode->Right = prev;         // and prev to right of newNode
                newNode->isThreaded = true;    // and set newNode to be threaded
            }
            else
            {
                newNode->Right = prev->Right;  // set right of newNode to prev right
                prev->Right = newNode;         // set right of prev to newNode
                newNode->isThreaded = true;    // set newNode to be threaded
                prev->isThreaded = false;      // set prev to not threaded
            }
        }//else

        //
        // 4. update size:
        //
        Size++;

        //
        // 5. walk back up tree using stack and update heights.
        //
        while (!updatingHeight.empty())
        {
            cur = updatingHeight.top();
            updatingHeight.pop();

            // set prev to top of the stack
            if(updatingHeight.empty())
                prev = nullptr;
            else
                prev = updatingHeight.top();

            // set the height of the cur
            int hL = (cur->Left == nullptr) ? -1 : cur->Left->Height;
            int hR = (cur->Right == nullptr) ? -1 : (cur->isThreaded) ? -1 : cur->Right->Height;
            int hCur = 1 + std::max(hL, hR);

            if (cur->Height == hCur)
                break;
            else
                cur->Height = hCur;

        }//while
    }//void insert


    //
    // helper function of copy constructor
    //
    void _helperCopy(NODE* cur)
    {
        // if cur is nullptr then return
        if(cur == nullptr){
            return;
        }
            // else insert cur key and value and keep doing recursion
            // until you reach nullptr and then if the cur is threaded
            // then do recursion with cur->Right
        else{
            _insertCopy(cur->Key, cur->Value);
            _helperCopy(cur->Left);
            if(!(cur->isThreaded))
                _helperCopy(cur->Right);
        }//else{
    }//void _helperCopy(


    //
    // Helper function for destructor
    //
    void _helperDestroy(NODE* cur)
    {
        // if cur is nullptr then return
        if(cur == nullptr)
            return;
            // else keep doing post order transversal and delete cur
        else
        {
            // call helperDestroy function with recursion using cur to the left
            _helperDestroy(cur->Left);
            // if cur is threaded then call function using cur to the right
            if(!(cur->isThreaded)){
                _helperDestroy(cur->Right);
            }
            // deleting cur
            delete cur;
        }//else
    }//void _helperDestroy


    //
    // Helper function for function operator()
    //
    NODE* _helperOperator(KeyT key)const
    {
        NODE* cur = Root;

        //
        // Search to see if tree contains key
        //
        while (cur != nullptr)
        {
            // check if key is already in tree
            if (key == cur->Key){
                return cur;
            }

            // if key is smaller then cur key then check left
            // and set prev to cur
            if (key < cur->Key)
                cur = cur->Left;
                // else check right of the tree and set prev to cur
            else
            {
                if (cur->isThreaded)
                    cur = nullptr;
                else
                    cur = cur->Right;
            }//else
        }//while (cur

        return nullptr;
    }//NODE* _helperOperator


    //
    //From Hummel's code
    // Return the current node to the right
    //
    NODE* _getActualLeft(NODE* cur) const
    {
        return cur->Left;
    }

    NODE* _getActualRight(NODE* cur) const
    {
        if (cur->isThreaded)  // then actual Right ptr is null:
            return nullptr;
        else  // actual Right is contents of Right ptr:
            return cur->Right;
    }


    //
    // This function outputs they key value and the right key
    // depending on if it's threaded using inorder transversal
    //
    void _prompt(NODE* cur, ostream& output) const
    {
        int key;

        if (cur == nullptr)
            return;
        else
        {
            _prompt(cur->Left, output);

            if(cur->isThreaded){
                // if cur is threaded and cur to the right is nullptr then output key and value at cur
                if(cur->Right == nullptr){
                    output << "(" << cur->Key << "," << cur->Value << "," << cur->Height << ")" << endl;
                }
                    // else output key and value at cur and key at the right of cur
                else{
                    output << "(" << cur->Key << "," << cur->Value  << "," << cur->Height << "," << cur->Right->Key << ")" << endl;
                }
            }
                // else if cur is not threaded then output key and value at cur
            else if(!(cur->isThreaded)){
                output << "(" << cur->Key << "," << cur->Value << "," << cur->Height << ")" << endl;
                _prompt(cur->Right, output);
            }//else if(
        }//else
    }//void _prompt


    //
    // This function is the helper function for _checkForRotations
    // It balances the tree and returns the balance between left and right
    //
    int _balance(NODE* cur)
    {
        // if cur is null then height is 0
        if (cur == NULL)
            return 0;
        else{
            // if there is nothing to the left and right
            // then the height from left and right is -1
            if(cur->Left == NULL && cur->Right == NULL)
                return 0;
                // if there is nothing to the left of tree then add 1
                // to left height of the tree
            else if(cur->Left != NULL && cur->Right == NULL)
                return (cur->Left->Height+1);
                // if there is nothing to the right of the tree
                // then subtract 1 from - right height of tree
            else if(cur->Left == NULL && cur->Right != NULL)
                return (-1-cur->Right->Height);
                // else check for threaded and if not threaded then
                // subtract left height from right
            else
            {
                if(cur->isThreaded)
                    return (cur->Left->Height+1);
                else
                    return (cur->Left->Height - cur->Right->Height);
            }//else
        }//else{
    }//int _balance


    //
    // This function is helper function for insert
    // It check which side is getting heavy then does
    // left and right rotation accoringly
    //
    void _checkForRotations(NODE* Parent, NODE* cur, KeyT curKey)
    {
		int balancingHeights;
		
		if(cur == nullptr)
			balancingHeights = 0;
		else
			balancingHeights = _balance(cur);

        // check if tree is getting heavy on the right
        if (balancingHeights > 1 && curKey < cur->Left->Key)
            _RightRotate(Parent, cur);

            // check is tree is getting heavy on the left
        else if (balancingHeights < -1 && curKey > cur->Right->Key)
            _LeftRotate(Parent, cur);

            // check is left right is getting heavy
        else if (balancingHeights > 1 && curKey > cur->Left->Key)
        {
            _LeftRotate(cur, cur->Left);
            _RightRotate(Parent, cur);
        }

            // check is right left is getting heavy
        else if (balancingHeights < -1 && curKey < cur->Right->Key)
        {
            _RightRotate(cur, cur->Right);
            _LeftRotate(Parent, cur);
        } // if (curKey
    }//void _checkForRotations


    //
    //This function Rotates the tree to the right and sets
    //the Parent to the new child after rotation
    //
    void _RightRotate(NODE* Parent, NODE* N)
    {
        NODE *initialLeft = N->Left;                 // set initialLeft to left of the N
        NODE *right = initialLeft->Right;            // right is child at right of initialLeft
        NODE *A = initialLeft->Left;                 // A is left child of initialLeft
        NODE *C = N->Right;                          // C is right child of N

        // check is initialLeft is threaded then set right to null
        if(initialLeft->isThreaded)
            right = nullptr;
        if(N->isThreaded)
            C = nullptr;

        // rotate the tree to right
        initialLeft->Right = N;
        N->Left = right;

        //update the parent
        if(Parent == nullptr){
            Root = initialLeft;
        }
        else if(Parent->Left == N){
            Parent->Left = initialLeft;
        }
        else{
            Parent->Right = initialLeft;
        }

        // set the height of N
        int HB = (right == NULL) ? -1 : right->Height;
        int HC = (C == NULL) ? -1 : C->Height;

        N->Height = 1 + max(HB, HC);

        // set the height for initialLeft
        int HA = (A == NULL) ? -1 : A->Height;
        initialLeft->Height = 1 + max(HA, N->Height);

        initialLeft->isThreaded = false;
    }//void _RightRotate


    //
    //This function Rotates the tree to the left and sets
    //the Parent to the new child after rotation
    //
    void _LeftRotate(NODE* Parent, NODE* N)
    {
        NODE *initialRight = N->Right;               // set initialRight to right of the N
        NODE *left = initialRight->Left;             // left is child at left of initialRight
        NODE *A = initialRight->Right;               // A is right child of initialRight
        NODE *C = N->Left;                           // C is left child of N

        // check is initialRight is threaded then set c to nullptr
        if(initialRight->isThreaded)
            C = nullptr;

        // Rotate the tree to left
        initialRight->Left = N;

        if(left != nullptr)
            N->Right = left;
        else{
            N->Right = initialRight;
            N->isThreaded = true;
        }

        // update the parent
        if(Parent == nullptr)
            Root = initialRight;
        else if(Parent->Key > initialRight->Key)
            Parent->Left = initialRight;
        else
            Parent->Right = initialRight;

        // set the height of N
        int HB = (left == NULL) ? -1 : left->Height;
        int HC = (C == NULL) ? -1 : C->Height;

        N->Height = 1 + max(HB, HC);

        // set the height of initialRight
        int HA = (A == NULL) ? -1 : A->Height;
        initialRight->Height = 1 + max(HA, N->Height);
    }//void _LeftRotate


	//
    // This function is a helper function for range_search
    // It's pushing the keys that are between upper and lower bound
    //
    void _helperRangeSearch(vector<KeyT>& keys, NODE* cur, int lower, int upper)
    {
        if(cur != NULL){
            // searches if lower is on the left of root then it search on the left of the root
            if(lower <= cur->Key)
                _helperRangeSearch(keys, cur->Left, lower, upper);
            // searches if key is in between lower and upper then push key to the vector
            if(lower <= cur->Key && upper >= cur->Key){
                keys.push_back(cur->Key);
            }
            // if upper is to the right then it searches on the right of the root
            if(upper >= cur->Key && (!(cur->isThreaded)))
                _helperRangeSearch(keys, cur->Right, lower, upper);
        }//if(cur != NULL)
    }//void _helperRangeSearch
	
public:
    //
    // default constructor:
    //
    // Creates an empty tree.
    //
    avlt()
    {
        Root = nullptr;
        Size = 0;
    }


    //
    // copy constructor
    //
    // NOTE: makes an exact copy of the "other" tree, such that making the
    // copy requires no rotations.
    //
    avlt (const avlt& other)
    {
        Root = nullptr;             // setting root to nullptr
        Size = 0;                   // setting size to 0
        _helperCopy(other.Root);    // calling helperCopy using other.Root
    }


    //
    // destructor:
    //
    // Called automatically by system when tree is about to be destroyed;
    // this is our last chance to free any resources / memory used by
    // this tree.
    //
    virtual ~avlt()
    {
        _helperDestroy(Root);
    }


    //
    // operator=
    //
    // Clears "this" tree and then makes a copy of the "other" tree.
    //
    // NOTE: makes an exact copy of the "other" tree, such that making the
    // copy requires no rotations.
    //
    avlt& operator=(const avlt& other)
    {
        (*this).clear();            // clear this pointer
        _helperCopy(other.Root);    // call helperCopy using other.Root

        // return this pointer
        return *this;
    }


    //
    // clear:
    //
    // Clears the contents of the tree, resetting the tree to empty.
    //
    void clear()
    {
        _helperDestroy(Root);        // call helperDestroy by passing Root
        Root = nullptr;              // set Root to nullptr
        Size = 0;                    // set size to 0
    }


    //
    // size:
    //
    // Returns the # of nodes in the tree, 0 if empty.
    //
    // Time complexity:  O(1)
    //
    int size() const
    {
        return Size;
    }


    //
    // height:
    //
    // Returns the height of the tree, -1 if empty.
    //
    // Time complexity:  O(1)
    //
    int height() const
    {
        if (Root == nullptr)
            return -1;
        else
            return Root->Height;
    }


    //
    // search:
    //
    // Searches the tree for the given key, returning true if found
    // and false if not.  If the key is found, the corresponding value
    // is returned via the reference parameter.
    //
    // Time complexity:  O(lgN) worst-case
    //
    bool search(KeyT key, ValueT& value) const
    {
        NODE* cur = Root;

        // while cur is not equal to nullptr
        while (cur != nullptr)
        {
            // check if key is already in tree
            if (key == cur->Key){
                value = cur->Value;            // set value to value of cur and
                return true;
            }
            // if key is smaller then cur key then check left of the tree
            if (key < cur->Key)
                cur = cur->Left;
            else{
                // if cur is threaded then set cur to nullptr
                if (cur->isThreaded)
                    cur = nullptr;
                    // else check right of the tree
                else
                    cur = cur->Right;
            }//else{
        }//while

        // if get here, not found
        return false;
    }


    //
    // range_search
    //
    // Searches the tree for all keys in the range [lower..upper], inclusive.
    // It is assumed that lower &lt;= upper.  The keys are returned in a vector;
    // if no keys are found, then the returned vector is empty.
    //
    // Time complexity: O(lgN + M), where M is the # of keys in the range
    // [lower..upper], inclusive.
    //
    // NOTE: do not simply traverse the entire tree and select the keys
    // that fall within the range.  That would be O(N), and thus invalid.
    // Be smarter, you have the technology.
    //
    vector<KeyT> range_search(KeyT lower, KeyT upper)
    {
        NODE* search = Root;
		vector<KeyT> keys;
		_helperRangeSearch(keys, search, lower, upper);
		
		return keys;
    }//vector<KeyT>


    //
    // insert
    //
    // Inserts the given key into the tree; if the key has already been insert then
    // the function returns without changing the tree.  Rotations are performed
    // as necessary to keep the tree balanced according to AVL definition.
    //
    // Time complexity:  O(lgN) worst-case
    //
    void insert(KeyT key, ValueT value)
    {
        NODE* prev = nullptr;
        NODE* cur = Root;

        //
        // stack the nodes we visit so we can walk back up
        // the search path later, adjusting heights:
        //
        stack<NODE*> updatingHeight;

        //
        // 1. Search to see if tree already contains key
        //
        while (cur != nullptr)
        {
            // check if key is already in tree
            if (key == cur->Key)
                return;

            updatingHeight.push(cur);

            // if key is smaller then cur key then check left
            // and set prev to cur
            if (key < cur->Key)
            {
                prev = cur;
                cur = cur->Left;
            }
            else
            {
                // if cur is threaded then set cur to nullptr
                // and prev to cur
                if (cur->isThreaded) {
                    prev = cur;
                    cur = nullptr;
                }
                    // else check right of the tree and set prev to cur
                else{
                    prev = cur;
                    cur = cur->Right;
                }//else{
            }//else
        }//while

        //
        // 2. so allocate a new node to insert
        //
        NODE* newNode = new NODE();
        newNode->Key = key;
        newNode->Value = value;
        newNode->Left = nullptr;
		newNode->Height = 0;
		//newNode->Right = nullptr;
        

        //
        // 3. link in the new node
        //
        // cur is null, and prev denotes node where
        // we fell out of the tree.  if prev is null, then
        // the tree is empty and the Root pointer needs
        // to be updated and increment size.
        //
        if(prev == nullptr){
            Root = newNode;
            newNode->Right = nullptr;
        }
        else{
            // if key is smaller then prev key
            if(key < prev->Key)
            {
                prev->Left = newNode;          // set left of prev to newNode
                newNode->Right = prev;         // and prev to right of newNode
                newNode->isThreaded = true;    // and set newNode to be threaded
            }
            else
            {
                newNode->Right = prev->Right;  // set right of newNode to prev right
                prev->Right = newNode;         // set right of prev to newNode
                newNode->isThreaded = true;    // set newNode to be threaded
                prev->isThreaded = false;      // set prev to not threaded
            }
        }//else

        //
        // 4. update size:
        //
        Size++;

        //
        // 5. walk back up tree using stack and update heights.
        //
        while (!updatingHeight.empty())
        {
            cur = updatingHeight.top();
            updatingHeight.pop();

            // set prev to top of the stack
            if(updatingHeight.empty())
                prev = nullptr;
            else
                prev = updatingHeight.top();

            // set the height of the cur
            int hL = (cur->Left == nullptr) ? -1 : cur->Left->Height;
            int hR = (cur->Right == nullptr) ? -1 : (cur->isThreaded) ? -1 : cur->Right->Height;
            int hCur = 1 + std::max(hL, hR);

            if (cur->Height == hCur)
                break;
            else
                cur->Height = hCur;

            _checkForRotations(prev, cur, key);
        }//while
    }//void insert


    //
    // From Hummel's code
    // []
    //
    // Returns the value for the given key; if the key is not found,
    // the default value ValueT{} is returned.
    //
    // Time complexity:  O(lgN) worst-case
    //
    ValueT operator[](KeyT key) const
    {
        ValueT  value = ValueT{};

		//
		// perform search, and either we find it --- and value is 
		// updated --- or we don't and we'll return default value:
		//
		this->search(key, value);

		return value;
    }//ValueT operator[]


    //
    // ()
    //
    // Finds the key in the tree, and returns the key to the "right".
    // If the right is threaded, this will be the next inorder key.
    // if the right is not threaded, it will be the key of whatever
    // node is immediately to the right.
    //
    // If no such key exists, or there is no key to the "right", the
    // default key value KeyT{} is returned.
    //
    // Time complexity:  O(lgN) worst-case
    //
    KeyT operator()(KeyT key) const
    {
        NODE* result = _helperOperator(key);                     // set result to return value of helperOperator
        // if result is nullptr or right of result is nullptr then return default KeyT
        if(result == nullptr || result->Right == nullptr){
            return KeyT{ };
        }

        // if here then return key at right of result
        return result->Right->Key;
    }//KeyT operator()


    //
    // %
    //
    // Returns the height stored in the node that contains key; if key is
    // not found, -1 is returned.
    //
    // Example:  cout << tree%12345 << endl;
    //
    // Time complexity:  O(lgN) worst-case
    //
    int operator%(KeyT key) const
    {
        NODE* cur = Root;

        if(cur == NULL)
            return -1;

        // while cur is not equal to nullptr
        while (cur != nullptr)
        {
            // check if key is already in tree
            if (key == cur->Key)
            {
                return cur->Height;
            }
            // if key is smaller then cur key then check left of the tree
            if (key < cur->Key)
                cur = cur->Left;
            else{
                // if cur is threaded then set cur to nullptr
                if (cur->isThreaded)
                    cur = nullptr;
                    // else check right of the tree
                else
                    cur = cur->Right;
            }//else{
        }//while

        return -1;
    }//int operator%


    //
    // From Hummel's code
    // begin
    //
    // Resets internal state for an inorder traversal.  After the
    // call to begin(), the internal state denotes the first inorder
    // key; this ensure that first call to next() function returns
    // the first inorder key.
    //
    // Space complexity: O(1)
    // Time complexity:  O(lgN) worst-case
    //
    // Example usage:
    //    tree.begin();
    //    while (tree.next(key))
    //      cout &lt;&lt; key &lt;&lt; endl;
    //
    void begin()
    {
        Next = Root;

        if (Next != nullptr)  // advance as left as possible:
        {
            while (Next->Left != nullptr)
                Next = Next->Left;
        }
    }


    //
    //From Hummel's code
    // next
    //
    // Uses the internal state to return the next inorder key, and
    // then advances the internal state in anticipation of future
    // calls.  If a key is in fact returned (via the reference
    // parameter), true is also returned.
    //
    // False is returned when the internal state has reached null,
    // meaning no more keys are available.  This is the end of the
    // inorder traversal.
    //
    // Space complexity: O(1)
    // Time complexity:  O(lgN) on average
    //
    // Example usage:
    //    tree.begin();
    //    while (tree.next(key))
    //      cout << key << endl;
    //
    bool next(KeyT& key)
    {
        if (Next == nullptr)
        {
            //
            // no more keys:
            //
            return false;
        }

        //
        // we have at least one more, so grab it now,
        // advance, and return true:
        //
        key = Next->Key;

        if (_getActualRight(Next) == nullptr)
        {
            // follow the thread:
            Next = Next->Right;
        }
        else
        {
            Next = Next->Right;  // go right, and then left as far as we can:

            while (Next->Left != nullptr)
                Next = Next->Left;
        }

        return true;
    }


    //
    // dump
    //
    // Dumps the contents of the tree to the output stream, using a
    // recursive inorder traversal.
    //
    void dump(ostream& output) const
    {
        output << "**************************************************" << endl;
        output << "********************* AVLT ***********************" << endl;
        output << "** size: " << this->size() << endl;
        output << "** height: " << this->height() << endl;


        //
        // inorder traversal, with one output per line: either
        // (key,value,height) or (key,value,height,THREAD)
        //
        // (key,value,height) if the node is not threaded OR thread==nullptr
        // (key,value,height,THREAD) if the node is threaded and THREAD denotes the next inorder key
        //
        NODE* cur = Root;
        _prompt(cur, output);            // calling prompt by sending cur and output file as parameter
        //
        // TODO
        //

        output << "**************************************************" << endl;
    }//void dump
};//class avlt
