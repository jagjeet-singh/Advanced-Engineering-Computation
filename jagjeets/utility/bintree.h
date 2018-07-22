#include <stdio.h>
#include <cstddef>
#include <cmath>

template <class KeyClass,class ValueClass>
class BinaryTree
{
protected:
	class Node
	{
	public:
		KeyClass key;
		ValueClass value;
		Node *left,*right,*up;
		int height;
		Node() : left(nullptr),right(nullptr),up(nullptr),height(1)
		{
		}
	};
public:
	bool autoRebalance;
	class NodeHandle
	{
	friend BinaryTree <KeyClass,ValueClass>;
	private:
		Node *ptr;
	public:
		inline void Nullify(void)
		{
			ptr=nullptr;
		}
		inline bool IsNull(void) const
		{
			return ptr==nullptr;
		}
		inline bool IsNotNull(void) const
		{
			return ptr!=nullptr;
		}
		inline bool operator==(NodeHandle hd) const
		{
			return this->ptr==hd.ptr;
		}
		inline bool operator!=(NodeHandle hd) const
		{
			return this->ptr!=hd.ptr;
		}
		inline bool operator==(std::nullptr_t) const
		{
			return ptr==nullptr;
		}
		inline bool operator!=(std::nullptr_t) const
		{
			return ptr!=nullptr;
		}
	};
protected:

	bool isBalanced(NodeHandle ndHd)
	{
		auto leftHeight = GetHeight(Left(ndHd));
		auto rightHeight = GetHeight(Right(ndHd));
		// printf("Left:%d, Right:%d\n", leftHeight, rightHeight);
		if (abs(leftHeight-rightHeight)>1)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	void Rebalance(NodeHandle ndHd)
	{

		auto childBranch = 'L'; 
		auto grandChildBranch = 'L';
		auto firstImbalanced = RootNode();
		auto child = RootNode();
		auto grandChild = RootNode();

		while (ndHd.IsNotNull())
		{
			if (isBalanced(ndHd)==false)
			{
				// Performing tri-node-restructure
				firstImbalanced = ndHd;
				if (GetHeight(Left(firstImbalanced))>GetHeight(Right(firstImbalanced)))
				{
					child=Left(firstImbalanced);
				}
				else
				{
					child = Right(firstImbalanced);
					childBranch = 'R';
				}
				if (GetHeight(Left(child))>GetHeight(Right(child)))
				{
					grandChild = Left(child);
				}
				else if (GetHeight(Left(child))>GetHeight(Right(child)))
				{
					grandChild = Right(child);
					grandChildBranch = 'R';
				}
				else
				{
					grandChildBranch = childBranch;
					if (grandChildBranch=='R')
					{
						grandChild=Right(child);
					}
					else
					{
						grandChild=Left(child);
					}
				}
				// Left-Left case
				if (childBranch=='L' && grandChildBranch=='L')
				{
					RotateRight(firstImbalanced);
				}

				// Right-Right case
				else if (childBranch=='R' && grandChildBranch=='R') 
				{
					RotateLeft(firstImbalanced);
				}

				// Right-Left case
				else if (childBranch=='R' && grandChildBranch=='L')
				{
					RotateRight(child);
					RotateLeft(firstImbalanced);
				}
				// Left-Right case
				else
				{
					RotateLeft(child);
					RotateRight(firstImbalanced);
				}
			}
			else
			{
				ndHd = Up(ndHd);
			}	
		}
	}


	Node *GetNode(NodeHandle ndHd)
	{
		if(ndHd.IsNotNull())
		{
			return ndHd.ptr;
		}
		return nullptr;
	}
	const Node *GetNode(NodeHandle ndHd) const
	{
		if(ndHd.IsNotNull())
		{
			return ndHd.ptr;
		}
		return nullptr;
	}
	static NodeHandle MakeHandle(Node *nodePtr)
	{
		NodeHandle ndHd;
		ndHd.ptr=nodePtr;
		return ndHd;
	}
	bool UpdateHeight(Node *nodePtr)
	{
		int leftHeight=1,rightHeight=1;
		if(nullptr!=nodePtr->left)
		{
			leftHeight=nodePtr->left->height+1;
		}
		if(nullptr!=nodePtr->right)
		{
			rightHeight=nodePtr->right->height+1;
		}
		int newHeight=1;
		if(leftHeight>rightHeight)
		{
			newHeight=leftHeight;
		}
		else
		{
			newHeight=rightHeight;
		}
		if(newHeight!=nodePtr->height)
		{
			nodePtr->height=newHeight;
			return true;
		}
		return false;
	}
	void UpdateHeightCascade(Node *nodePtr)
	{
		bool first=true;
		while(nullptr!=nodePtr)
		{
			auto changed=UpdateHeight(nodePtr);
			if(true!=first && true!=changed)
			{
				break;
			}
			nodePtr=nodePtr->up;
			first=false;
		}
	}

private:
	Node *root;
	long long int nElem;

public:
	BinaryTree()
	{
		root=nullptr;
		nElem=0;
		autoRebalance=false;
	}
	~BinaryTree()
	{
		CleanUp();
	}
	void CleanUp(void)
	{
		CleanUp(GetNode(RootNode()));
	}
private:
	void CleanUp(Node *nodePtr)
	{
		if(nullptr!=nodePtr)
		{
			CleanUp(nodePtr->left);
			CleanUp(nodePtr->right);
			delete nodePtr;
		}
	}
public:
	static NodeHandle Null(void)
	{
		NodeHandle ndHd;
		ndHd.ptr=nullptr;
		return ndHd;
	}
	NodeHandle RootNode(void) const
	{
		return MakeHandle(root);
	}
	NodeHandle Left(NodeHandle ndHd) const
	{
		auto nodePtr=GetNode(ndHd);
		if(nullptr!=nodePtr)
		{
			return MakeHandle(nodePtr->left);
		}
		return Null();
	}
	NodeHandle Up(NodeHandle ndHd) const
	{
		auto nodePtr=GetNode(ndHd);
		if(nullptr!=nodePtr)
		{
			return MakeHandle(nodePtr->up);
		}
		return Null();
	}
	NodeHandle Right(NodeHandle ndHd) const
	{
		auto nodePtr=GetNode(ndHd);
		if(nullptr!=nodePtr)
		{
			return MakeHandle(nodePtr->right);
		}
		return Null();
	}

	long long int GetN(void) const
	{
		return nElem;
	}
	const KeyClass &GetKey(NodeHandle ndHd) const
	{
		// This will crash if ndHd==nullptr.  Therefore, ndHd must be non-null to use this function.
		return GetNode(ndHd)->key;
	}
	ValueClass &GetValue(NodeHandle ndHd)
	{
		// This will crash if ndHd==nullptr.  Therefore, ndHd must be non-null to use this function.
		return GetNode(ndHd)->value;
	}
	const ValueClass &GetValue(NodeHandle ndHd) const
	{
		// This will crash if ndHd==nullptr.  Therefore, ndHd must be non-null to use this function.
		return GetNode(ndHd)->value;
	}
	NodeHandle FindNode(const KeyClass &key) const
	{
		auto ndHd=RootNode();
		while(nullptr!=ndHd)
		{
			if(key==GetKey(ndHd))
			{
				return ndHd;
			}
			if(key<GetKey(ndHd))
			{
				ndHd=Left(ndHd);
			}
			else
			{
				ndHd=Right(ndHd);
			}
		}
		return Null();
	}
	bool IsKeyIncluded(const KeyClass &key) const
	{
		return FindNode(key).IsNotNull();
	}
	int GetHeight(NodeHandle ndHd) const
	{
		auto nodePtr=GetNode(ndHd);
		if(nullptr!=nodePtr)
		{
			return nodePtr->height;
		}
		return 0;
	}

	NodeHandle Insert(const KeyClass &key,const ValueClass &value)
	{
		auto newNode=new Node;
		newNode->key=key;
		newNode->value=value;

		auto ndHd=RootNode();
		if(ndHd.IsNull())
		{
			root=newNode;
		}
		else
		{
			while(ndHd.IsNotNull())
			{
				if(key<GetKey(ndHd))
				{
					if(Left(ndHd)!=nullptr)
					{
						ndHd=Left(ndHd);
					}
					else
					{
						GetNode(ndHd)->left=newNode;
						newNode->up=GetNode(ndHd);
						break;
					}
				}
				else
				{
					if(Right(ndHd)!=nullptr)
					{
						ndHd=Right(ndHd);
					}
					else
					{
						GetNode(ndHd)->right=newNode;
						newNode->up=GetNode(ndHd);
						break;
					}
				}
			}
		}
		UpdateHeightCascade(newNode);
		nElem++;
		if (autoRebalance==true)
		{
			Rebalance(MakeHandle(newNode));
		}
		return MakeHandle(newNode);
	}

	NodeHandle Last(void) const
	{
		auto ndHd=RootNode();
		while(Right(ndHd).IsNotNull())
		{
			ndHd=Right(ndHd);
		}
		return ndHd;
	}

	NodeHandle First(void) const
	{
		auto ndHd=RootNode();
		while(Left(ndHd).IsNotNull())
		{
			ndHd=Left(ndHd);
		}
		return ndHd;
	}
	NodeHandle FindNext(NodeHandle ndHd) const
	{
		auto rightHd=Right(ndHd);
		if(rightHd.IsNotNull())
		{
			// Has a right sub-tree.
			// The next node is the left-most of the right sub-tree.
			ndHd=Right(ndHd);
			while(Left(ndHd).IsNotNull())
			{
				ndHd=Left(ndHd);
			}
			return ndHd;
		}
		else
		{
			// Does not have a right sub-tree.
			// Go up until it goes up from the left.
			while(ndHd.IsNotNull())
			{
				auto upHd=Up(ndHd);
				if(upHd.IsNotNull() && ndHd==Left(upHd))
				{
					return upHd;
				}
				ndHd=upHd;
			}
			return Null();
		}
	}

	NodeHandle FindPrev(NodeHandle ndHd) const
	{
		auto leftHd=Left(ndHd);
		if(leftHd.IsNotNull())
		{
			// Has a left sub-tree.
			// The next node is the right-most of the left sub-tree.
			ndHd=Left(ndHd);
			while(Right(ndHd).IsNotNull())
			{
				ndHd=Right(ndHd);
			}
			return ndHd;
		}
		else
		{
			// Does not have a left sub-tree.
			// Go up until it goes up from the right.
			while(ndHd.IsNotNull())
			{
				auto upHd=Up(ndHd);
				if(upHd.IsNotNull() && ndHd==Right(upHd))
				{
					return upHd;
				}
				ndHd=upHd;
			}
			return Null();
		}
	}


private:
	NodeHandle RightMostOf(NodeHandle ndHd)
	{
		while(Right(ndHd).IsNotNull())
		{
			ndHd=Right(ndHd);
		}
		return ndHd;
	}
	bool SimpleDetach(NodeHandle ndHd)
	{
		if(ndHd.IsNotNull())
		{
			auto upHd=Up(ndHd);
			auto rightHd=Right(ndHd);
			auto leftHd=Left(ndHd);
			if(rightHd.IsNull() && leftHd.IsNull())
			{
				if(upHd.IsNull()) // ndHd is a root.
				{
					root=nullptr;
				}
				else
				{
					auto upPtr=GetNode(upHd);
					if(Left(upHd)==ndHd)
					{
						upPtr->left=nullptr;
					}
					else if(Right(upHd)==ndHd)
					{
						upPtr->right=nullptr;
					}
					else
					{
						fprintf(stderr,"Error! Internal Tree Data Structure is broken.\n");
						return false;
					}
				}
				UpdateHeightCascade(GetNode(upHd));
				return true;
			}
			else if(rightHd.IsNull())
			{
				if(upHd.IsNull())
				{
					root=GetNode(leftHd);
					root->up=nullptr;
					return true;
				}
				else
				{
					// Connect upHd and leftHd
					auto upPtr=GetNode(upHd);
					auto leftPtr=GetNode(leftHd);
					if(Left(upHd)==ndHd)
					{
						upPtr->left=leftPtr;
						leftPtr->up=upPtr;
						UpdateHeightCascade(GetNode(upHd));
						return true;
					}
					else if(Right(upHd)==ndHd)
					{
						upPtr->right=leftPtr;
						leftPtr->up=upPtr;
						UpdateHeightCascade(GetNode(upHd));
						return true;
					}
					else
					{
						fprintf(stderr,"Error! Internal Tree Data Structure is broken.\n");
						return false;
					}
				}
			}
			else if(leftHd.IsNull())
			{
				if(upHd.IsNull())
				{
					root=GetNode(rightHd);
					root->up=nullptr;
					return true;
				}
				else
				{
					// Connect upHd and rightHd
					auto upPtr=GetNode(upHd);
					auto rightPtr=GetNode(rightHd);
					if(Left(upHd)==ndHd)
					{
						upPtr->left=rightPtr;
						rightPtr->up=upPtr;
						UpdateHeightCascade(GetNode(upHd));
						return true;
					}
					else if(Right(upHd)==ndHd)
					{
						upPtr->right=rightPtr;
						rightPtr->up=upPtr;
						UpdateHeightCascade(GetNode(upHd));
						return true;
					}
					else
					{
						fprintf(stderr,"Error! Internal Tree Data Structure is broken.\n");
						return false;
					}
				}
			}
			else
			{
				return false;
			}
		}
		return false;
	}
public:
	bool Delete(NodeHandle ndHd)
	{
		if(true==SimpleDetach(ndHd))
		{
			auto actionPos = Up(ndHd);
			delete GetNode(ndHd);
			--nElem;
			if (autoRebalance==true)
			{
				Rebalance(actionPos);
			}
			return true;
		}
		else if(ndHd.IsNotNull())
		{
			// Right most of left. Always Simple-Detachable.
			// Also, since SimpleDetach of itself has failed, it must have a left sub-tree.
			auto RMOL=RightMostOf(Left(ndHd));
			auto actionPos = Up(RMOL);
			if (RMOL==Left(ndHd))
			{
				actionPos = RMOL;
			}

			if(true==SimpleDetach(RMOL))
			{
				// Now, RMOL needs to take position of ndHd.
				auto RMOLptr=GetNode(RMOL);
				auto upPtr=GetNode(Up(ndHd));
				auto leftPtr=GetNode(Left(ndHd));
				auto rightPtr=GetNode(Right(ndHd));

				auto upOfRMOLptr=RMOLptr->up;
				if(upOfRMOLptr==GetNode(ndHd))
				{
					upOfRMOLptr=RMOLptr;	// Now it is correct.
				}

				if(nullptr==upPtr)
				{
					root=RMOLptr;
					root->up=nullptr;
				}
				else if(upPtr->left==GetNode(ndHd))
				{
					upPtr->left=RMOLptr;
					RMOLptr->up=upPtr;
				}
				else if(upPtr->right==GetNode(ndHd))
				{
					upPtr->right=RMOLptr;
					RMOLptr->up=upPtr;
				}
				else
				{
					fprintf(stderr,"Error! Internal Tree Data Structure is broken.\n");
					return false;
				}

				RMOLptr->left=leftPtr;
				if(nullptr!=leftPtr)
				{
					leftPtr->up=RMOLptr;
				}
				RMOLptr->right=rightPtr;
				if(nullptr!=rightPtr)
				{
					rightPtr->up=RMOLptr;
				}

				UpdateHeightCascade(RMOLptr);
				delete GetNode(ndHd);
				--nElem;
				if (autoRebalance==true)
				{
					Rebalance(actionPos);
				}
				return true;
			}
		}

		return false; // Cannot delete a null node.
	}

	bool RotateLeft(NodeHandle ndHd)
	{
		auto nodePtr=GetNode(ndHd);
		if(nullptr!=nodePtr && nullptr!=nodePtr->right)
		{
			auto rightPtr=nodePtr->right;
			auto leftOfRight=nodePtr->right->left;

			if(nullptr==nodePtr->up)
			{
				root=rightPtr;
				rightPtr->up=nullptr;
			}
			else
			{
				rightPtr->up=nodePtr->up;
				if(nodePtr->up->left==nodePtr)
				{
					nodePtr->up->left=rightPtr;
				}
				else
				{
					nodePtr->up->right=rightPtr;
				}
			}

			rightPtr->left=nodePtr;
			nodePtr->up=rightPtr;

			nodePtr->right=leftOfRight;
			if(nullptr!=leftOfRight)
			{
				leftOfRight->up=nodePtr;
			}
			UpdateHeight(nodePtr);
			UpdateHeightCascade(rightPtr);
			return true;
		}
		return false;
	}

	bool RotateRight(NodeHandle ndHd)
	{
		auto nodePtr=GetNode(ndHd);
		if(nullptr!=nodePtr && nullptr!=nodePtr->left)
		{
			auto leftPtr=nodePtr->left;
			auto rightOfLeft=nodePtr->left->right;

			if(nullptr==nodePtr->up)
			{
				root=leftPtr;
				leftPtr->up=nullptr;
			}
			else
			{
				leftPtr->up=nodePtr->up;
				if(nodePtr->up->left==nodePtr)
				{
					nodePtr->up->left=leftPtr;
				}
				else
				{
					nodePtr->up->right=leftPtr;
				}
			}

			leftPtr->right=nodePtr;
			nodePtr->up=leftPtr;

			nodePtr->left=rightOfLeft;
			if(nullptr!=rightOfLeft)
			{
				rightOfLeft->up=nodePtr;
			}
			UpdateHeight(nodePtr);
			UpdateHeightCascade(leftPtr);
			return true;
		}
		return false;
	}

	void TreeToVine()
	{
		auto ctr=0;
		if(nullptr==root)
		{
			return;
		}
		auto vine_tail=root;
		auto tempRoot = vine_tail;
		while(nullptr!=vine_tail)
		{
			auto leftOfVine = vine_tail->left;
			while (nullptr!=leftOfVine)
			{
				RotateRight(MakeHandle(vine_tail));
				vine_tail = leftOfVine;
				leftOfVine = vine_tail->left;
			}
			if(ctr==0)
			{
				tempRoot = vine_tail;
				ctr=1;
			}
			vine_tail=vine_tail->right;
		}
		root = tempRoot;
	}

	void Compress(long long int size)
	{
		auto newRoot = root->right;
		auto tmp = root;
		for (int i=0;i<size;i++)
		{	
			auto rightChild = tmp->right;
			auto ndHd = MakeHandle(tmp);
			RotateLeft(ndHd);
			tmp = rightChild->right;
		}
		root = newRoot;
	}

	void VineToTree()
	{
		auto currNode = root;
		while(nullptr!=currNode)
		{
			if (nullptr!=currNode->left)
			{
				printf("%s\n","Not a Vine! Cannot convert to tree");
				return;
			}
			currNode=currNode->right;
		}
		auto size = GetN();
		auto exp_term = (int)(log(size+1)/log(2));
		auto leaves = size+1-pow(2,exp_term);
		Compress(leaves);
	 	size = size-leaves;
		while(size>1)
		{
			size = (int)size/2;
			Compress(size);
		}
	}

};


