// 编写一个程序，能根据依次输入的数字，创建如下二叉搜索树，并且支持删除操作。


#include <stdio.h>
#include <stdlib.h>

// 二叉搜索树的节点体
typedef struct TreeNode
{
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

//新建一个节点(newNode)
TreeNode *newNode(int val)
{
    TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));
    node->val = val;
    node->left = NULL;
    node->right = NULL;
    return node;
}

//将节点插入二叉树
TreeNode *bst_insert(TreeNode *root, int val)
{
    if(root == NULL)
        return newNode(val);
    if(val < root->val)
        root->left = bst_insert(root->left, val);
    else
        root->right = bst_insert(root->right, val);
    return root;
}


// 删除指定节点
TreeNode *bst_delete(TreeNode *root, int val){
    if(root == NULL)
        return root;
    if(val < root->val)
        root->left = bst_delete(root->left, val);
    else if(val > root->val)
        root->right = bst_delete(root->right, val);
    else{
        if(root->left == NULL)
            return root->right;
        else if(root->right == NULL)
            return root->left;
        else{
            TreeNode *temp = root->left;
            while(temp->right != NULL)
                temp = temp->right;
            root->val = temp->val;
            root->left = bst_delete(root->left, temp->val);
        }
    }
    return root;
}

//前序遍历
void preorder(TreeNode *root)
{
    if(root == NULL)
        return;
    printf("%d ", root->val);
    preorder(root->left);
    preorder(root->right);
}

int main()
{
    TreeNode *root = NULL;
    int n;
    while(1)
    {
        scanf("%d", &n);
        if(n == 0)
            break;
        if(n > 0){
            root = bst_insert(root, n);
            preorder(root);
            printf("\n");   
        }else{
            root = bst_delete(root, -n);
            preorder(root);
            printf("\n");   
        }
    }
    return 0;
}