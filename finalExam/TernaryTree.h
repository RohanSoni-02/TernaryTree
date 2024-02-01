
// COS30008, Final Exam, 2022

#pragma once

#include <stdexcept>
#include <algorithm>

template<typename T>
class TernaryTreePrefixIterator;

template<typename T>
class TernaryTree
{
public:
    
    using TTree = TernaryTree<T>;
    using TSubTree = TTree*;

private:
    
    T fKey;
    TSubTree fSubTrees[3];

    // private default constructor used for declaration of NIL
    TernaryTree() :
        fKey(T())
    {
        for ( size_t i = 0; i < 3; i++ )
        {
            fSubTrees[i] = &NIL;
        }
    }

public:

    using Iterator = TernaryTreePrefixIterator<T>;
        
    static TTree NIL;           // sentinel

    // getters for subtrees
    const TTree& getLeft() const { return *fSubTrees[0]; }
    const TTree& getMiddle() const { return *fSubTrees[1]; }
    const TTree& getRight() const { return *fSubTrees[2]; }

    // add a subtree
    void addLeft( const TTree& aTTree ) { addSubTree( 0, aTTree ); }
    void addMiddle( const TTree& aTTree ) { addSubTree( 1, aTTree ); }
    void addRight( const TTree& aTTree ) { addSubTree( 2, aTTree ); }
            
    // remove a subtree, may through a domain error
    const TTree& removeLeft() { return removeSubTree( 0 ); }
    const TTree& removeMiddle() { return removeSubTree( 1 ); }
    const TTree& removeRight() { return removeSubTree( 2 ); }

/////////////////////////////////////////////////////////////////////////
// Problem 1: TernaryTree Basic Infrastructure

private:

    // remove a subtree, may throw a domain error [22]
    const TTree& removeSubTree( size_t aSubtreeIndex )
    {
        if (!(fSubTrees[aSubtreeIndex]->empty()))
        {
              TTree* result = fSubTrees[aSubtreeIndex];
              fSubTrees[aSubtreeIndex] = &NIL;
              return *result;
        }
        
        else
        {
            throw std::domain_error("Subree is NIL ");
        }
    }
    
    // add a subtree; must avoid memory leaks; may throw domain error [18]
    void addSubTree( size_t aSubtreeIndex, const TTree& aTTree )
    {
        if (!empty())
        {
            if (aSubtreeIndex < 3)
            {
                if (fSubTrees[aSubtreeIndex]->empty())
                {
                    fSubTrees[aSubtreeIndex] = const_cast<TTree*>(&aTTree);
                }
                else
                    throw std::domain_error(" Subtree is not NILL");
            }
            else
            {
                throw std::domain_error("Illegal Subtree index");
            }
        }
        else
        {
            throw std::domain_error("Empty tree");
        }
    }
    
    
public:
    
    // TernaryTree l-value constructor [10]
	TernaryTree( const T& aKey ): fKey(aKey)
    {
        for ( size_t i = 0; i < 3; i++ )
        {
            fSubTrees[i] = &NIL;
        }
    }
    
    // destructor (free sub-trees, must not free empty trees) [14]
	~TernaryTree()
    {
        if (!fSubTrees[0]->empty())
        {
            removeLeft();
        }

        if (!fSubTrees[1]->empty())
        {
            removeMiddle();
        }

        if (!fSubTrees[2]->empty())
        {
            removeRight();
        }
    }
    
    // return key value, may throw domain_error if empty [2]
	const T& operator*() const
    {
        if (!empty())
        {
            return fKey;
        }
        else
        {
            throw std::domain_error("Empty TTree encountered.");
        }
    }

    // returns true if this ternary tree is empty [4]
	bool empty() const
    {
        return this == &NIL;
    }

    // returns true if this ternary tree is a leaf [10]
	bool leaf() const
    {
        return (fSubTrees[0]== &NIL && fSubTrees[1]== &NIL && fSubTrees[2]== &NIL);
    }

    // return height of ternary tree, may throw domain_error if empty [48]
    size_t height() const
    {
        if (empty())
           {
               throw std::domain_error("Operation not supported");
           }
           if (leaf())
           {
               return 0;
           }
           else
           {
               size_t lHeightLeft = 0;
               size_t lHeightRight = 0;
               size_t lHeightMiddle = 0;

               if (!fSubTrees[0]->empty())
               {
                   lHeightLeft = fSubTrees[0]->height();
               }
               if (!fSubTrees[1]->empty())
               {
                   lHeightMiddle = fSubTrees[1]->height();
               }
               if (!fSubTrees[2]->empty())
               {
                   lHeightRight = fSubTrees[2]->height();
               }
               return max(max(lHeightLeft, lHeightRight), lHeightMiddle) + 1;
           }
    }
    
/////////////////////////////////////////////////////////////////////////
// Problem 2: TernaryTree Copy Semantics
    
    // copy constructor, must not copy empty ternary tree
	TernaryTree( const TTree& aOtherTTree ): fKey(*aOtherTTree)
    {
        for (size_t i = 0; i < 3; i++)
        {
            fSubTrees[i] = &NIL;
        }
        
        if (aOtherTTree.empty())
            throw std::domain_error("Attemptted to copy empty tree");

        if (!fSubTrees[0]->empty())
            fSubTrees[0] = new TTree(aOtherTTree.getLeft());

        if (!fSubTrees[2]->empty())
            fSubTrees[2] = new TTree(aOtherTTree.getRight());

        if (!fSubTrees[1]->empty())
            fSubTrees[1] = new TTree(aOtherTTree.getMiddle());
    }

    // copy assignment operator, must not copy empty ternary tree
    // may throw a domain error on attempts to copy NIL
	TTree& operator=( const TTree& aOtherTTree )
    {
        if (!aOtherTTree.empty())
        {
            if (**this != *aOtherTTree)
            {
                fKey = *aOtherTTree;

                fSubTrees[0] = aOtherTTree.getLeft().clone();
                fSubTrees[2] = aOtherTTree.getRight().clone();
                fSubTrees[1] = aOtherTTree.getMiddle().clone();
            }
            return *this;
        }
        else
        {
            throw std::domain_error("NIL as source not permiitted.");
        }
    }
    
    // clone ternary tree, must not copy empty trees
	TSubTree clone() const
    {
        if (!empty())
        {
            return new TTree(*this);
        }
        else
        {
            throw std::domain_error("Cloning empty tree");
        }
    }

/////////////////////////////////////////////////////////////////////////
// Problem 3: TernaryTree Move Semantics

    // TTree r-value constructor
	TernaryTree( T&& aKey ): fKey(std::move(aKey))
    {
        for ( size_t i = 0; i < 3; i++ )
        {
            fSubTrees[i] = &NIL;
        }
    }

    // move constructor, must not copy empty ternary tree
	TernaryTree( TTree&& aOtherTTree )
    {
        if (aOtherTTree.empty())
        {
            throw std::domain_error("Attemptted to move empty tree");
        }
                    
        fKey = std::move(aOtherTTree.fKey);

        for (size_t i = 0; i < 3; i++)
        {
            fSubTrees[i] = &NIL;
        }
    }

    // move assignment operator, must not copy empty ternary tree
	TTree& operator=( TTree&& aOtherTTree )
    {
        
    
        if (!aOtherTTree.empty())
        {
            if (this != &aOtherTTree)
            {
                
                fKey = std::move(*aOtherTTree);
            
                fSubTrees[0] = aOtherTTree.fSubTrees[0]->getLeft().clone();
                fSubTrees[2] = aOtherTTree.fSubTrees[2]->getRight().clone();
                fSubTrees[1] = aOtherTTree.fSubTrees[1]->getMiddle().clone();

                aOtherTTree.fSubTrees[0] = &NIL;
                aOtherTTree.fSubTrees[1] = &NIL;
                aOtherTTree.fSubTrees[2] = &NIL;
                
            }
            return *this;
        }
        else
        {
            throw std::domain_error("Moving NIL");
        }
    }
    
/////////////////////////////////////////////////////////////////////////
// Problem 4: TernaryTree Prefix Iterator

    // return ternary tree prefix iterator positioned at start
	Iterator begin() const
    {
        Iterator iter(this);
        return iter.begin();
    }

    // return ternary prefix iterator positioned at end
	Iterator end() const
    {
        Iterator iter(this);
        return iter.end();
    }
};

template<typename T>
TernaryTree<T> TernaryTree<T>::NIL;
