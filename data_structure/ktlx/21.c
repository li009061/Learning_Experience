// 这棵二叉树，按照前序、中序、后序方式，编程实现输出其各个节点的值。

#include <stdio.h>
#include <stdlib.h>

struct node
{
    int n;
    struct node *left;
    struct node *right;
};

struct queue
{
    struct node *data;
    struct queue *prev;
    struct queue *next;
};

struct node *bst_insert(struct node *root, int n)
{
    if(root == NULL)
    {
        struct node *new_node = (struct node *)malloc(sizeof(struct node));
        if(new_node != NULL)
        {
          new_node->n = n;
          new_node->left = NULL;
          new_node->right = NULL;
        }
        return new_node;
    }

    if(n < root->n)
        root->left = bst_insert(root->left, n);
    else
        root->right = bst_insert(root->right, n);

    return root;
}


// 前序遍历
void pre_travel(struct node *root)
{
    if(root == NULL)
        return;

    // 1. 访问根节点
    printf("%d ", root->n);

    // 2. 访问左子树
    pre_travel(root->left);

    // 3. 访问右子树
    pre_travel(root->right);
}


//中序遍历(左根右)
void in_travel(struct node *root){
    if (root == NULL)
        return;
    
    //访问左子树
    in_travel(root->left);

    //访问根节点
    printf("%d ", root->n);

    //访问右子树
    in_travel(root->right);
}

//后序遍历(左右根)
void post_travel(struct node *root){
    if (root == NULL)
        return;
    
    //访问左子树
    post_travel(root->left);

    //访问右子树
    post_travel(root->right);
    
    //访问根节点
    printf("%d ", root->n);
}

// 判断队列是否为空
int is_empty(struct queue *head) {
    return head == NULL;
}

// 入队（尾插）
struct queue *inqueue(struct queue *head, struct node *node) {
    struct queue *new_node = malloc(sizeof(struct queue));
    if (new_node == NULL) return head;
    new_node->data = node;
    new_node->next = NULL;
    new_node->prev = NULL;

    if (head == NULL) {
        // 空队列，新节点即第一个节点
        return new_node;
    }

    // 找到尾节点
    struct queue *tail = head;
    while (tail->next != NULL) {
        tail = tail->next;
    }
    tail->next = new_node;
    new_node->prev = tail;
    return head;
}

// 出队（头删），并返回出队的节点指针
struct node *dequeue(struct queue **head) {
    if (*head == NULL) return NULL;

    struct queue *first = *head;
    struct node *ret = first->data;

    *head = first->next;
    if (*head != NULL) {
        (*head)->prev = NULL;
    }
    free(first);
    return ret;
}

//初始化
struct queue *init_queue() {
    return NULL;
}

//层序遍历
void level_travel(struct node *root) {
    if (root == NULL) return;

    struct queue *head = init_queue();
    head = inqueue(head, root);

    while (!is_empty(head)) {
        struct node *cur = dequeue(&head);
        printf("%d ", cur->n);

        if (cur->left != NULL)
            head = inqueue(head, cur->left);
        if (cur->right != NULL)
            head = inqueue(head, cur->right);
    }
}

struct node *bst_delete(struct node *root, int n)
{
    if(root == NULL)
        return root;

    if(n < root->n)
        root->left = bst_delete(root->left, n);
    else if(n > root->n)
        root->right = bst_delete(root->right, n);
    else{
        if(root->left != NULL){
            struct node *tmp = root->left;
            while(tmp->right != NULL)
                tmp = tmp->right;
            root->n = tmp->n;
            root->left = bst_delete(root->left, tmp->n);
        }else if(root->right != NULL){
            struct node *tmp = root->right;
            while(tmp->left != NULL)
                tmp = tmp->left;
            root->n = tmp->n;
            root->right = bst_delete(root->right, tmp->n);
        }else{
            free(root);
            root = NULL;
        }
    }
    return root;
}


int main(void)
{
    struct node *root = NULL;

    int n;
    printf("请输入节点值（输入0结束）：\n");
    while(1)
    {
        scanf("%d", &n);
        if(n == 0)
            break;

        if(n < 0)
            root = bst_delete(root, -n);
        else
            root = bst_insert(root, n);

        // 遍历二叉树
        
        // 1. 前序遍历
        pre_travel(root); 
        printf("\n");
    
        // 2. 中序遍历
        in_travel(root); 
        printf("\n");
    
        // 3. 后序遍历
        post_travel(root); 
        printf("\n");
    
        // 4. 层序遍历
        level_travel(root); 
        printf("\n");
    }

    return 0;
}