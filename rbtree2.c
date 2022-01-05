// Implementation of RB tree

#include<stdio.h>
#include<stdlib.h>

struct node
{
    int num;
    int color;           // 0 for red, 1 for black
    struct node *left;
    struct node *right;
    struct node *parent;
};

struct node *root1 = NULL;               // Points to root of tree T
struct node *root2 = NULL;

int max(int a, int b)
{
    if(a > b)
        return a;
    else
        return b;
}

struct node* rightRotate(struct node *root, struct node *x)  // Returns the root of tree
{
    struct node *y = x->left;
    if(y != NULL)
    {
      y->parent = x->parent;
      x->parent = y;
      x->left = y->right;
      y->right = x;  
      if(x->left != NULL)
        x->left->parent = x;
    }    
    else
    {
        printf("Error at right rotate");
        return root;
    }
    if(x != root)
    {
        if(y->parent->left == x)         
        {
            y->parent->left = y;
        }
        else
            y->parent->right = y;
    }    
    else
    {
        root = y;
    }
    return root;          
}


struct node* leftRotate(struct node *root, struct node *x)  // Returns the root
{
    struct node *y = x->right;
    if(y != NULL)
    {
      y->parent = x->parent;
      x->parent = y;
      x->right = y->left;
      y->left = x;  
      if(x->right != NULL)
        x->right->parent = x;
    }
    
    else
    {
        printf("Error at left rotate");
        return root;
    }
    if(x != root)
    {
        if(y->parent->left == x)         
        {
            y->parent->left = y;
        }
        else
            y->parent->right = y;
    }    
    else
    {
        root = y;
    }
    return root;
}

int findBlackHeight(struct node *root)
{
    if(root == NULL)
    {
        return 0;
    }
    else
    {
        return max(findBlackHeight(root->left),findBlackHeight(root->right)) + root->color;
    }
}

struct node* stopAtHeight(struct node *root, int height, int dir)  // dir = 0 means left, 1 means right
{                                                                  // Also, minimum value of height is 1  
    int totHeight = findBlackHeight(root);
    int curHeight = totHeight;                // Height of current node

    if(height > totHeight)
    {
        printf("Error in height parameter @ stopAtHeight()");
        return NULL;
    }
    else if(dir == 0)
    {
        while(curHeight > height)
        {
            curHeight = curHeight - root->color;
            root = root->left;
        }
        // Always stop at black node and not red node
        while(root->color == 0)
        {
            root = root->left;
        }
        return root;
    }
    else if(dir == 1)
    {
        while(curHeight > height)
        {
            curHeight = curHeight - root->color;
            root = root->right;
        }
        // Always stop at black node and not red node
        while(root->color == 0)
        {
            root = root->right;
        }
        return root;
    }
}

//Insertion

struct node* insert(int val, struct node *root)  // Returns pointer to root of tree
{

    // First, do BST insertion, then fix colors

    struct node *ptr = (struct node*)malloc(sizeof(struct node));
    ptr->num = val;
    ptr->color = 0;
    ptr->left = NULL;
    ptr->right = NULL;
    ptr->parent = NULL;
    if(root == NULL)
    {
        ptr->color = 1;  
        return ptr;
    }
    else 
    {
        struct node *cur = root;
        struct node *parent = root;
        while(cur != NULL)
        {
            parent = cur;
            if(cur->num == val)
            {
                printf("Duplicate node found");
                return root;
            }
            else if(val < cur->num)
            {
                cur = cur->left;
            }
            else
            {
                cur = cur->right;
            }
        }
        if(val < parent->num)
        {
            parent->left = ptr;
            ptr->parent = parent;            
        }
        else
        {
            parent->right = ptr;
            ptr->parent = parent;
        }

        // BST insertion finishes here
        // Now recolor the nodes

        struct node* insertColorFixup(struct node*, struct node*);  // Function prototype

        root = insertColorFixup(root,ptr);

        return root;
    }
    
}

struct node* insertColorFixup(struct node *root,struct node *z)   // Here root is either head1 or head2
{
    while(z->parent != NULL && z->parent->color == 0)   // After coming back - checking for black includes checking for null
    {
        if(z->parent == z->parent->parent->left)
        {                                                   
            struct node *y = z->parent->parent->right;

            if(y == NULL || y->color == 1) 
            {
                if(z == z->parent->right)
                {
                    z = z->parent;
                    root = leftRotate(root,z);
                }
                z->parent->color = 1;
                z->parent->parent->color = 0;
                root = rightRotate(root,z->parent->parent);
            }
            else if(y->color == 0)
            {
                z->parent->color = 1;
                y->color = 1;
                z->parent->parent->color = 0;
                z = z->parent->parent;
            }
        }
        else
        {
            struct node *y = z->parent->parent->left;

            if(y == NULL || y->color == 1) 
            {
                if(z == z->parent->left)
                {
                    z = z->parent;
                    root = rightRotate(root,z);
                }
                z->parent->color = 1;
                z->parent->parent->color = 0;
                root = leftRotate(root,z->parent->parent);
            }
            else if(y->color == 0)
            {
                z->parent->color = 1;
                y->color = 1;
                z->parent->parent->color = 0;
                z = z->parent->parent;
            }
        }
    }
    root->color = 1;
    return root;
}

struct node* rbTransplant(struct node *root, struct  node *u, struct node *v)
{
    if(u->parent == NULL)
    {
        root = v; /// How ? Check this line - Ans : node u has one or no child
    }
    else if(u == u->parent->left)
    {
        u->parent->left = v;
    }
    else
    {
        u->parent->right = v;
    }
    if(v != NULL)
        v->parent = u->parent;

    return root;
}

struct node* treeMin(struct node *root)
{
    struct node *cursor = root;
    while(cursor->left != NULL)
    {
        cursor = cursor->left;
    }
    return cursor;
}

struct node* treeMax(struct node *root)
{
    struct node *cursor = root;
    while(cursor->right != NULL)
    {
        cursor = cursor->right;
    }
    return cursor;
}



struct node* search(struct node *root, int ele)  // Returns pointer to node if it is found otherwise NULL
{
    struct node* cursor = root;
    while(cursor != NULL)
    {
        if(cursor->num == ele)
        {
            return cursor;
        }
        else if(cursor->num > ele)
            cursor = cursor->left;
        else
            cursor = cursor->right;
    }
    return cursor;
}


struct node* delete(struct node *root, int ele)
{
    struct node *z = search(root,ele);
    struct node *parentX = NULL;
    if(z == NULL)
        return root;

    struct node *y = z;
    struct node *x;
    int y_original_color = y->color;
    if(z->left == NULL)
    {
        x = z->right;
        parentX = z->parent;
        root = rbTransplant(root,z,z->right);           // Need to check the color of x @ end
    }
    else if(z->right == NULL)
    {
        x = z->left;
        parentX = z->parent;
        root = rbTransplant(root,z,z->left);
    }
    else
    {
        y = treeMin(z->right);
        y_original_color = y->color;
        x = y->right;
        if(y->parent == z)
        {
            z->num = y->num;
            rbTransplant(root,y,y->right);
            parentX = z;

            /*
            if(x != NULL)
                x->parent = y;
            parentX = y;
            */
        }
        else
        {
            z->num = y->num;
            parentX = y->parent;
            rbTransplant(root,y,y->right);
            /*
            parentX = y->parent;
            root = rbTransplant(root,y,y->right);
            y->right = z->right;
            if(y->right != NULL)
                y->right->parent = y;
            */
        }
        /*
        root = rbTransplant(root,z,y);
        y->left = z->left;
        if(y->left != NULL)
            y->left->parent = y;
        y->color = z->color;    
        */         
    }

    if(y_original_color == 1)
    {
        if(x != NULL && x->color == 0)
        {
            x->color = 1;
        }
        else if(root == NULL)
        {
            return root;
        }
        else if(x == root && x-> color == 0)
        {
            x->color = 1;
            return root;
        }
        else
        {
            struct node* rbDeleteFixup(struct node*,struct node*,struct node*);   // Function prototype
            root = rbDeleteFixup(root,x,parentX);    
        }
        
    }
    return root;
}

struct node* rbDeleteFixup(struct node *root, struct node* x, struct node *parentX)
{
    struct node *w;
    while(x != root && (x == NULL || x->color == 1))
    {
        if(x != NULL)
            parentX = x->parent;

        if(x == parentX->left)
        {
            w = parentX->right;

            if(w->color == 0)
            {
                w->color = 1;
                parentX->color = 0;
                root = leftRotate(root,parentX);
                w = parentX->right;
            }
            if((w->left == NULL || w->left->color == 1) && (w->right == NULL || w->right->color == 1))
            {
                w->color = 0;
                x = parentX;            
            }
            else 
            {
                if (w->right == NULL || w->right->color == 1)
                {
                    w->left->color = 1;
                    w->color = 0;
                    root = rightRotate(root,w);
                    w = parentX->right;
                }
                w->color = parentX->color;
                parentX->color = 1;
                w->right->color = 1;
                root = leftRotate(root,parentX);
                x = root;
            }
        }
        else
        {
            w = parentX->left;

            if(w->color == 0)
            {
                w->color = 1;
                parentX->color = 0;
                root = rightRotate(root,parentX);
                w = parentX->left;
            }
            if((w->left == NULL || w->left->color == 1) && (w->right == NULL || w->right->color == 1))
            {
                w->color = 0;
                x = parentX;            
            }
            else 
            {
                if (w->left == NULL || w->left->color == 1)
                {
                    w->right->color = 1;
                    w->color = 0;
                    root = leftRotate(root,w);
                    w = parentX->left;
                }
                w->color = parentX->color;
                parentX->color = 1;
                w->left->color = 1;
                root = rightRotate(root,parentX);
                x = root;
            }
        }           
    }

    x->color = 1;
    return root;
}
// Merge function

struct node* mergeTrees(struct node *root1, struct node *root2) 
{
    struct node* merge(struct node*, struct node*,int,int,int);  // Function prototype

    if(root1 == NULL)
    {
        return root2;
    }
    else if(root2 == NULL)
    {
        return root1;
    }

    int height1,height2;
    struct node *delNode;
    int delNodeVal;
    
    if(root1->num < root2->num)
    {
        delNode = treeMin(root2);
        delNodeVal = delNode->num;
        root2 = delete(root2,delNodeVal);
    }
    else
    {
        delNode = treeMin(root1);
        delNodeVal = delNode->num;
        root1 = delete(root1,delNodeVal);
    }

    height1 = findBlackHeight(root1);
    height2 = findBlackHeight(root2);

    struct node *newRoot = (struct node*)malloc(sizeof(struct node));

    if(height1 == height2)
    {
        newRoot->num = delNodeVal;
        newRoot->color = 1;
        newRoot->left = root1;
        newRoot->right = root2;
        newRoot->parent = NULL;

        root1->parent = newRoot;
        root2->parent = newRoot;

    }
    else if(height1 > height2)
    {
        newRoot = merge(root1,root2,height1,height2,delNodeVal);
    }
    else
        newRoot = merge(root2,root1,height2,height1,delNodeVal);

    return newRoot;
}

struct node* merge(struct node *root1, struct node *root2, int height1, int height2, int delNodeVal) // Assumption : height1 > height2
{
    if(root2 == NULL)
    {
        root1 = insert(delNodeVal,root1);
        return root1;
    }
    if(root1->num < root2->num)  // tree with root1 has values lesser than tree with root2 
    {
        struct node *cursor = root1;
        int curHeight = height1;        // Height of node pointed to by cursor
        while(curHeight > height2 + 1 || cursor->color != 1)
        {
            curHeight -= cursor->color;
            cursor = cursor->right;
        }

        // Case 1: Checking for BB case

        if(cursor->right == NULL || cursor->right->color == 1)
        {
            struct node *leftSubTree = cursor->right;
            struct node *delNode = (struct node*)malloc(sizeof(struct node));
            delNode->num = delNodeVal;
            delNode->parent = cursor;
            cursor->right = delNode;
            delNode->left = leftSubTree;
            delNode->right = root2;
            root2->parent = delNode;
            leftSubTree->parent = delNode;
            return root1;
        }
        else  // This is BR case
        {
            if(cursor->left->color == 0)           // Case 2: Checking if uncle of leftSubtree is red
            {
                struct node *leftSubTree = cursor->right;
                struct node *delNode = (struct node*)malloc(sizeof(struct node));
                delNode->num = delNodeVal;
                delNode->parent = cursor;
                cursor->right = delNode;
                delNode->left = leftSubTree;
                delNode->right = root2;
                root2->parent = delNode;
                leftSubTree->parent = delNode;

                root1 = insertColorFixup(root1,leftSubTree);
                return root1;
            }
            else if(cursor->left == NULL || cursor->left->color == 1)  // Case 3: Uncle of left subtree is black
            {

                struct node *leftSubTree = cursor->right;
                struct node *delNode = (struct node*)malloc(sizeof(struct node));
                delNode->num = delNodeVal;
                delNode->parent = cursor;
                cursor->right = delNode;
                delNode->left = leftSubTree;
                delNode->right = root2;
                root2->parent = delNode;
                leftSubTree->parent = delNode;

                struct node *b = cursor->right->left;
                struct node *c = cursor->right;

                root1 = rightRotate(root1,c);
                root1 = leftRotate(root1,cursor);

                // Recoloring the nodes

                cursor->color = 0;
                b->color = 1;
                c->color = 0;

                return root1;
            }
        }
    }
    else if(root1->num > root2->num)
    {
        struct node *cursor = root1;
        int curHeight = height1;        // Height of node pointed to by cursor
        while(curHeight > height2 + 1 || cursor->color != 1)
        {
            curHeight -= cursor->color;
            cursor = cursor->left;
        }

        // Case 1: Checking for BB case

        if(cursor->left == NULL || cursor->left->color == 1)
        {
            struct node *rightSubTree = cursor->left;
            struct node *delNode = (struct node*)malloc(sizeof(struct node));
            delNode->num = delNodeVal;
            delNode->parent = cursor;
            cursor->left = delNode;
            delNode->right = rightSubTree;
            delNode->left = root2;
            root2->parent = delNode;
            rightSubTree->parent = delNode;
            return root1;
        }
        else  // This is BR case
        {
            if(cursor->right->color == 0)           // Case 2: Checking if uncle of leftSubtree is red
            {
                struct node *rightSubTree = cursor->left;
                struct node *delNode = (struct node*)malloc(sizeof(struct node));
                delNode->num = delNodeVal;
                delNode->parent = cursor;
                cursor->left = delNode;
                delNode->right = rightSubTree;
                delNode->left = root2;
                root2->parent = delNode;
                rightSubTree->parent = delNode;

                root1 = insertColorFixup(root1,rightSubTree);
                return root1;
            }
            else if(cursor->right == NULL || cursor->right->color == 1)  // Case 3: Uncle of left subtree is black
            {

                struct node *rightSubTree = cursor->right;
                struct node *delNode = (struct node*)malloc(sizeof(struct node));
                delNode->num = delNodeVal;
                delNode->parent = cursor;
                cursor->left = delNode;
                delNode->right = rightSubTree;
                delNode->left = root2;
                root2->parent = delNode;
                rightSubTree->parent = delNode;
                struct node *b = cursor->right->left;
                struct node *c = cursor->right;

                root1 = leftRotate(root1,c);
                root1 = rightRotate(root1,cursor);

                // Recoloring the nodes

                cursor->color = 0;
                b->color = 1;
                c->color = 0;

                return root1;
            }
        }        
    }

}

void insertMult(int tmin,int tmax,int umin,int umax)
{
    int i;
    for(i=tmin;i<=tmax;i++)
    {
        root1 = insert(i,root1);
    }
    for(i=umin;i<=umax;i++)
    {
        root2 = insert(i,root2);
    }
    printf("Inserted nodes successfully\n");
}

int checkRBtree(struct node *root, struct node *cursor, int depth)   // depth here means black depth
{
    int x,y;
    if(cursor != NULL)
    {
        x = checkRBtree(root,cursor->left,depth + cursor->color);
        if(cursor->left == NULL && cursor->right == NULL)
        {
            if((depth + cursor->color) != findBlackHeight(root))
            {
                if(root == root1)
                    printf("T is not / a RB tree");
                else
                    printf("U is not / a RB tree");
                return 0;
            }
        }
        if(cursor->color == 0)
        {
            if((cursor->left != NULL && cursor->left->color == 0) || (cursor->right != NULL && cursor->right->color == 0))
            {
                if(root == root1)
                    printf("T is not a RB tree");
                else
                    printf("U is not a RB tree"); 
                return 0;               
            }
        }
        y = checkRBtree(root,cursor->right,depth + cursor->color);
        if(x && y)
            return 1;
        else
            return 0;        
    }  
    return 1;  
}



void inorder(struct node *root, struct node *cursor)
{
    if(cursor != NULL)
    {
        inorder(root,cursor->left);
        if(cursor != root)
        {
            if(cursor->color == 0)
                printf("(%d,red,p=%d) ",cursor->num,cursor->parent->num);
            else
                printf("(%d,black,p=%d) ",cursor->num,cursor->parent->num);
        }
        else
            printf("(%d,black,root) ",cursor->num);
        inorder(root,cursor->right);
    }
}

struct node* insertFile(struct node *root, char *filename)
{
    FILE *fp;
    fp = fopen(filename, "r");
    char c;
    char str[50];
    str[0] = '\0';
	while((c = fgetc(fp)) != EOF)
	{
		if(c == ' ' || c == '\n')
		{
            if(str[0] != '\0')
            {
                int num = atoi(str);
                root = insert(num,root);
                str[0] = '\0';
            }
		}
		else
		{
            int i;
			for(i=0;str[i] != '\0';i++);
            str[i] = c;
            str[i+1] = '\0';
		}
	}
	fclose(fp);
    return root;
}

void main()
{
    int choice,chosenTree,ele;
    char cont;
    int tmin,tmax,umin,umax;
    printf("Red Black Trees\n");
    printf("MENU\n");
    printf("\n1. Insert\n2. Delete\n3. Merge\n4. Inorder traversal\n5. Exit\n6. Insert multiple nodes\n7. Read from file");
    do
    {
        printf("\nEnter your choice");
        scanf("%d",&choice);

        switch(choice)
        {
            case 1: printf("Choose tree\n1. Tree T\n2. Tree U\nEnter 1 or 2 : ");
                    scanf("%d",&chosenTree);
                    printf("Enter element to be inserted : ");
                    scanf("%d",&ele);
                    if(chosenTree == 1)
                    {
                        root1 = insert(ele,root1);
                    }
                    else if(chosenTree == 2)
                    {
                        root2 = insert(ele,root2);
                    }
                    break;
            case 2: printf("Choose tree\n1. Tree T\n2. Tree U\nEnter 1 or 2 : ");
                    scanf("%d",&chosenTree);
                    printf("Enter element to be deleted : ");
                    scanf("%d",&ele);
                    if(chosenTree == 1)
                    {
                        root1 = delete(root1,ele);
                    }
                    else if(chosenTree == 2)
                    {
                        root2 = delete(root2,ele);
                    }
                    break;
            case 3: root1 = mergeTrees(root1,root2);
                    printf("Trees are merged. Resultant tree is stored in tree T.\n");
                    if(checkRBtree(root1,root1,0))
                        printf("T is a RB tree\n");
                    else
                        printf("T is not a RB tree\n");
                    break;
            case 4: printf("Choose tree\n1. Tree T\n2. Tree U\nEnter 1 or 2 : ");
                    scanf("%d",&chosenTree);
                    if(chosenTree == 1)
                    {
                        inorder(root1,root1);
                    }
                    else if(chosenTree == 2)
                    {
                        inorder(root2,root2);
                    }
                    break;
            case 5: exit(0);
                    break;
            case 6: printf("Enter tmin, tmax, umin and umax ");
                    scanf("%d%d%d%d",&tmin,&tmax,&umin,&umax);
                    insertMult(tmin,tmax,umin,umax);
                    if(checkRBtree(root1,root1,0))
                        printf("T is a RB tree");
                    if(checkRBtree(root2,root2,0))
                        printf("U is a RB tree");    
                    break;
            case 7: printf("Select tree to be constructed\n1. Tree T\n2. Tree U\nEnter 1 or 2 : ");
                    scanf("%d",&chosenTree);
                    if(chosenTree == 1)
                        root1 = insertFile(root1,"T.txt");
                    else
                        root2 = insertFile(root2,"U.txt");
                    break;
        }
        printf("Continue? (Y/N) ");
        scanf("%*c%c",&cont);
    }while(cont == 'y' || cont == 'Y');
}