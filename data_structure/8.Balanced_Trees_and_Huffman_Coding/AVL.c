#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef  int datatype;

//定义一颗BST树
typedef struct treenode{
    datatype data;
    int height;

    struct treenode *rchild;
    struct treenode *lchild;
}bsttree;



//定义一些新节点
bsttree * newNode(datatype data){
    bsttree * root = calloc(1, sizeof(bsttree));
    if (root == NULL)
        return NULL;

    root->data = data;
    root->lchild = root->rchild = NULL;
    root->height = 1;

    return root;
}

bool is_Empty(bsttree *root){
    return (root == NULL);
}

//树的高度
int height(bsttree *root){
    return ((root == NULL) ? 0 : (root->height));
}

//最大值
int MAX(int r, int l){
    return ((r > l) ? (r) : (l));
}

//左旋
bsttree * tree_left(bsttree *root){
    bsttree *tmp = root->rchild;
    root->rchild = tmp->lchild;
    tmp->lchild = root;

    root->height = (MAX(height(root->lchild), height(root->rchild)) + 1);
    tmp->height = (MAX(height(tmp->lchild), height(tmp->rchild)) + 1);

    return tmp;
}

//右旋
bsttree * tree_right(bsttree *root){
    bsttree *tmp = root->lchild;
    root->lchild = tmp->rchild;
    tmp->rchild = root;

    root->height = (MAX(height(root->lchild), height(root->rchild)) + 1);
    tmp->height = (MAX(height(tmp->lchild), height(tmp->rchild)) + 1);

    return tmp;
}

//左右旋
bsttree * tree_leftright(bsttree *root){
    root->lchild = tree_left(root->lchild);
    return tree_right(root);
}
//右左旋
bsttree * tree_rightleft(bsttree *root){
    root->rchild = tree_right(root->rchild);
    return tree_left(root);
}

//将新节插入到树中
bsttree * insertree(bsttree *root, bsttree *new){
    if(is_Empty(root))
        return new;

    if(root->data > new->data)
        root->lchild = insertree(root->lchild, new);
    else if (root->data < new->data)
        root->rchild = insertree(root->rchild, new);
    else{
        printf("数据已存在\n");
        return root;
    }

    int balance = height(root->lchild) - height(root->rchild);
    if (balance > 1) {
        if (new->data < root->lchild->data)
            root = tree_right(root);
        else
            root = tree_leftright(root);
    } else if (balance < -1) {
        if (new->data > root->rchild->data)
            root = tree_left(root);
        else
            root = tree_rightleft(root);
    }

    root->height = (MAX(height(root->lchild), height(root->rchild)) + 1);
    return root;
}

//从树中删除节点
bsttree *removetree(bsttree *root, datatype data) {
    if (is_Empty(root))
        return NULL;

    if (data > root->data)
        root->rchild = removetree(root->rchild, data);
    else if (data < root->data)
        root->lchild = removetree(root->lchild, data);
    else {
        // 找到要删除的节点
        bsttree *tmp;
        
        if (root->lchild != NULL && root->rchild != NULL) {
            // 有两个孩子，找后继（右子树最小值）
            tmp = root->rchild;
            while (tmp->lchild != NULL)
                tmp = tmp->lchild;
            
            root->data = tmp->data;
            root->rchild = removetree(root->rchild, tmp->data);
        } else if (root->lchild != NULL) {
            // 只有左孩子，找前驱（左子树最大值）
            tmp = root->lchild;
            while (tmp->rchild != NULL)
                tmp = tmp->rchild;
            
            root->data = tmp->data;
            root->lchild = removetree(root->lchild, tmp->data);
        } else if (root->rchild != NULL) {
            // 只有右孩子
            tmp = root->rchild;
            root->data = tmp->data;
            root->rchild = removetree(root->rchild, tmp->data);
        } else {
            // 叶子节点
            free(root);
            return NULL;
        }
    }

    // 更新高度
    root->height = MAX(height(root->lchild), height(root->rchild)) + 1;

    // 平衡调整
    int balance = height(root->lchild) - height(root->rchild);
    if (balance > 1) {
        int left_balance = height(root->lchild->lchild) - height(root->lchild->rchild);
        if (left_balance >= 0)
            root = tree_right(root);
        else
            root = tree_leftright(root);
    } else if (balance < -1) {
        int right_balance = height(root->rchild->rchild) - height(root->rchild->lchild);
        if (right_balance >= 0)
            root = tree_left(root);
        else
            root = tree_rightleft(root);
    }

    return root;
}

//前序展示
void show(bsttree *root){
    if(is_Empty(root))
        return;

    printf("%d\t", root->data);

    show(root->lchild);

    show(root->rchild);
}

int main(int argc, char const *argv[])
{
    //定义一棵BST树
    bsttree *root = NULL;

    //输入数据(正数添加，0退出，负数删除)
    int n;
    while(1){
        scanf("%d", &n);
        if (n == 0)
            break;
        
        if (n > 0){
            root = insertree(root, newNode(n));
        }

        if (n < 0){
            root = removetree(root, -n);
        }

        show(root);
        printf("\n");
    }

    return 0;
}
