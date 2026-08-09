// 针对上面第2题，编程实现按层遍历算法，输出：4、3、8、1、5、9、12

#include <stdio.h>
#include <stdlib.h>

// 二叉搜索树的节点体
typedef struct TreeNode
{
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

//新建队列
typedef struct QNode {
    TreeNode *data;
    struct QNode *next;
} QNode;

typedef struct {
    QNode *front;
    QNode *rear;
} Queue;

//入队
void enqueue(Queue *q, TreeNode *node) {
    QNode *newNode = (QNode *)malloc(sizeof(QNode));
    newNode->data = node;
    newNode->next = NULL;
    if (q->rear) {
        q->rear->next = newNode;
        q->rear = newNode;
    } else {
        q->front = q->rear = newNode;
    }
}

//出队
TreeNode *dequeue(Queue *q) {
    if (!q->front) return NULL;
    QNode *tmp = q->front;
    TreeNode *data = tmp->data;
    q->front = q->front->next;
    if (!q->front) q->rear = NULL;
    free(tmp);
    return data;
}

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

//按层遍历
void levelorder(TreeNode *root)
{
    if(root == NULL)
        return;
    Queue q;
    q.front = q.rear = NULL;
    enqueue(&q, root);
    while(q.front)
    {
        TreeNode *node = dequeue(&q);
        printf("%d ", node->val);
        if(node->left != NULL)
            enqueue(&q, node->left);
        if(node->right != NULL)
            enqueue(&q, node->right);
    }
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
            // preorder(root);

        }else{
            root = bst_delete(root, -n);
            // preorder(root);
        }
        levelorder(root);
        printf("\n");
    }
    return 0;
}