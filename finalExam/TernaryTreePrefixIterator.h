
// COS30008, Final Exam, 2022

#pragma once

#include "TernaryTree.h"

#include <stack>

template<typename T>
class TernaryTreePrefixIterator
{
private:
    using TTree = TernaryTree<T>;
    using TTreeNode = TTree*;
    //using TTreeStack = std::stack<const TTreeNode>;
    using TTreeStack = std::stack<const TTree*>;
    
    const TTree* fTTree;                // ternary tree
    TTreeStack fStack;                  // traversal stack

public:

    using Iterator = TernaryTreePrefixIterator<T>;
       
    Iterator operator++(int)
    {
        Iterator old = *this;

        ++(*this);
           
        return old;
    }
       
    bool operator!=( const Iterator& aOtherIter ) const
    {
        return !(*this == aOtherIter);
    }

/////////////////////////////////////////////////////////////////////////
// Problem 4: TernaryTree Prefix Iterator

private:
    
    // push subtree of aNode
    void push_subtrees( const TTree* aNode )
    {
        while (!aNode->empty()) {
            fStack.push(aNode);
            aNode = &aNode->getLeft();
        }
    }

public:
    
	// iterator constructor
	TernaryTreePrefixIterator( const TTree* aTTree ): fTTree(aTTree), fStack()
    {
        push_subtrees(aTTree);
    }

	// iterator dereference
	const T& operator*() const
    {
        return **fStack.top();
    }

    // prefix increment
	Iterator& operator++()
    {
       fStack.pop();

       if (!fStack.empty()) {
           auto& current_frontier(fStack.top());
           if (fSubTrees[0] == 0)
           {
               fSubTrees[1] = 1;
               push_subtrees(fSubTrees[1]->getMiddle());
           }
           else if (fSubTrees[1] == 1)
           {
               fSubTrees[2] = 2;
               push_subtrees(fSubTrees[2]->getRight());
           }
       }
       return *this;
    }

	// iterator equivalence
	bool operator==( const Iterator& aOtherIter ) const
    {
        return fTTree == aOtherIter.fTTree && fStack.size() == aOtherIter.fStack.size();
    }

	// auxiliaries
	Iterator begin() const
    {
        Iterator iter = *this;
                
        iter.fStack = fStack;
        push_subtrees(aNode);
        return iter;
    }
    
	Iterator end() const
    {
        Iterator iter = *this;
        
        iter.fStack = fStack;
            
        return iter;
    }
};
