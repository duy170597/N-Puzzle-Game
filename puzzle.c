#include <stdio.h>
#include <stdlib.h>


// Cau truc cua mot trang thai
struct node {
    int status[3][3];
    int f; // f(n) = g(n) + h(n)
    struct node *parent;
    int g;
    int h;
};
typedef struct node Node;

// Tao node
Node *makeNode(int status[][3]) {
    Node *p = (Node*) malloc(sizeof(Node));
    if(p == NULL) {
        printf("Loi cap phat o nho!]n");
        return NULL;
    }
    int i, j;
    for(i = 0; i < 3; i++) {
        for(j = 0; j < 3; j++) {
            p->status[i][j] = status[i][j];
        }
    }
    p->f = -1;
    p->g = -1;
    p->h = -1;
    p->parent = NULL;
    return p;
}

/* 
 * Open/Fringe: tap cac trang thai sinh ra nhung chua duoc xet den
*/
struct fringe {
    Node *number[362880];
    int count;
};
typedef struct fringe Fringe;

// Khoi tao hang doi rong
void initFringe(Fringe *F) {
    F->count = 0;
}

// Kiem tra Fringe rong
int isEmptyFringe(Fringe *F) {
    if(F->count == 0) return 1;
    return 0;
}

// Them mot trang thai vao Fringe
void pushFringe(Fringe *F, Node *N) {
    F->number[F->count] = N;
    F->count++;
}

Node *popFringe(Fringe *F) {
    if(isEmptyFringe(F)) {
        printf("Fringe rong!\n");
        return NULL;
    }
    else {
        // Tim Fringe min
        int i, j = 0;
        Node *fMin = F->number[0];
        for(i = 1; i < F->count; i++) {
           if((F->number[i]->f) < (fMin->f)) {
               fMin = F->number[i];
               j = i;   // luu lai chi so cua trang thai co f(n) nho nhat
           } 
        }
        //printf("j = %d\n", j);
        for(i = j; i < F->count-1; i++) {
            F->number[i] = F->number[i+1];           
        }
        F->count--;
        return fMin;
    }
}

/*
 * M: tap cac trang thai tiep theo cua n(i)
 */
struct list {
    Node *number[4];
    int count;
};
typedef struct list ListStatus;

void initListStatus(ListStatus *LS) {
    LS->count = 0;
}

void checkStatus(ListStatus *LS, Node *N) {
    // Tim kiem o trong
    int i, j;
    for(i = 0; i < 3; i ++) {
        for(j = 0; j < 3; j++) {
            if(N->status[i][j] == 0)   break;
        }
        if(j == 3) j--;
        if(N->status[i][j] == 0)   break;
    }

    int m, n;
    if(i != 0) {
        // sao chep
        LS->number[LS->count] = (Node*)malloc(sizeof(Node));
        for(m = 0; m < 3; m++) {
            for(n = 0; n < 3; n++) {
                LS->number[LS->count]->status[m][n] = N->status[m][n];
            }
        }
        
        // di chuyen o trong len tren
        LS->number[LS->count]->status[i][j] = LS->number[LS->count]->status[i-1][j];
        LS->number[LS->count]->status[i-1][j] = 0;
        LS->count++;
        LS->number[LS->count-1]->parent = N;    // luu lai cha cua cac trang thai
    }
   
    if(i != 2) {
        // sao chep
        LS->number[LS->count] = (Node*)malloc(sizeof(Node));
        for(m = 0; m < 3; m++) {
            for(n = 0; n < 3; n++) {
                LS->number[LS->count]->status[m][n] = N->status[m][n];
            }
        }
        
        // di chuyen o trong len tren
        LS->number[LS->count]->status[i][j] = LS->number[LS->count]->status[i+1][j];
        LS->number[LS->count]->status[i+1][j] = 0;
        LS->count++;
        LS->number[LS->count-1]->parent = N;
    }
    
    if(j != 0) {
        // sao chep
        LS->number[LS->count] = (Node*)malloc(sizeof(Node));
        for(m = 0; m < 3; m++) {
            for(n = 0; n < 3; n++) {
                LS->number[LS->count]->status[m][n] = N->status[m][n];
            }
        }
        
        // di chuyen o trong len tren
        LS->number[LS->count]->status[i][j] = LS->number[LS->count]->status[i][j-1];
        LS->number[LS->count]->status[i][j-1] = 0;
        LS->count++;
        LS->number[LS->count-1]->parent = N;
    }
    
    if(j != 2) {
        // sao chep
        LS->number[LS->count] = (Node*)malloc(sizeof(Node));
        for(m = 0; m < 3; m++) {
            for(n = 0; n < 3; n++) {
                LS->number[LS->count]->status[m][n] = N->status[m][n];
            }
        }
        
        // di chuyen o trong len tren
        LS->number[LS->count]->status[i][j] = LS->number[LS->count]->status[i][j+1];
        LS->number[LS->count]->status[i][j+1] = 0;
        LS->count++;
        LS->number[LS->count-1]->parent = N;
    }
}

Node *popListStatus(ListStatus *LS, int i) {
    Node *p = LS->number[i];
    int j;
    for(j = 0; j< LS->count-1; j++) {
        LS->number[i] = LS->number[i+1];
    }
    LS->count--;
    return p;
}

/*
 * KQ: Tap trang thai ket qua, luu cac trang thai tu trang thai hien tai
       toi trang thai dich
 */

// Ham heuristic1
int heuristic1(int statusBegin[][3], int statusEnd[][3]) {
    int i, j, m, n, heuristic = 0, item;
    for(i = 0; i < 3; i++) {
        for(j = 0; j < 3; j++) {
            for(m = 0; m < 3; m++) {
                for(n = 0; n < 3; n++) {
                    if(statusBegin[i][j] == statusEnd[m][n])
                        heuristic += abs(i-m) + abs(j-n);
                }
            }
        }
    }
    return heuristic;
}


// Ham di chuyen o trong
void swap(int status[][3], char c) {
    // Tim kiem o trong
    int i, j;
    for(i = 0; i < 3; i ++) {
        for(j = 0; j < 3; j++) {
            if(status[i][j] == 0)   break;
        }
        if(j == 3) j--;
        if(status[i][j] == 0)   break;
    }
    //printf("i = %d\n", i);
    
    if(c == 'u') {
        if(i != 0) {
            status[i][j] = status[i-1][j];
            status[i-1][j] = 0;
        }
    }
    else if(c == 'd') {
        if(i != 2) {
            status[i][j] = status[i+1][j];
            status[i+1][j] = 0;
        }
    }
    else if(c == 'l') {
        if(j != 0) {
            status[i][j] = status[i][j-1];
            status[i][j-1] = 0;
        }
    }
    else if(c == 'r') {
        if(j != 2) {
            status[i][j] = status[i][j+1];
            status[i][j+1] = 0;
        }
    }
}

// Ham so sanh 2 trang thai
int compareStatus(int status1[][3], int status2[][3]) {
    int i, j, result = 1;
    for(i = 0; i < 3; i++) {
        for(j = 0; j < 3; j++) {
            if(status1[i][j] != status2[i][j]) {
                result = 0;
                return result;
            }
        }
    }
    return result;
}

// Ham A*
Node *aStar(Node *begin, Node *end, Fringe *F, ListStatus *LS) {
    begin->g = 0;
    begin->h = heuristic1(begin->status, end->status);
    begin->f = begin->g + begin->h;
    pushFringe(F, begin);
    int i, j, result;
    Node *p;
    //printf("isEmptyFringe = %d\n", isEmptyFringe(F));
    while(isEmptyFringe(F) == 0) {
        p = popFringe(F);
        if(compareStatus(p->status, end) == 1) {
            return p;
        }
        else{
            
            checkStatus(LS, p);
            for(i = 0; i < LS->count; i++) {
                // Tinh f = g +h
                LS->number[i]->g = p->g + 1;
                LS->number[i]->h = heuristic1(LS->number[i]->status, end);
                LS->number[i]->f = LS->number[i]->g + LS->number[i]->h;
                LS->number[i]->parent = p;
                
                // Kiem tra va day cac trang thai vao Fringe
                if(p->parent != NULL){
                    if(compareStatus(LS->number[i]->status, p->parent->status) == 0) {
                        result = 0;
                        for(j = 0; j < F->count; j++) {
                            if(compareStatus(LS->number[i]->status, F->number[j]) == 1) {
                                result = 1;
                                break;
                            }
                        }
                        if(result == 0) pushFringe(F, LS->number[i]);
                    }
                }
                else {
                    pushFringe(F, LS->number[i]);
                }
            }
            LS->count = 0;
        }
    }
    return NULL;
}


// Ham in mot trang thai
void printStatus(int status[][3]) {
    int i, j;
    for(i = 0; i < 3; i++) {
        for(j = 0; j < 3; j++) {
            if(status[i][j] == 0)   printf("  ");
            else printf("%d ", status[i][j]);
        }
        printf("\n");
    }
    printf("--------------\n");
}


void resultSearch(Node *end) {
    static int z=0;
    Node *p = end;
    if(p != NULL) {
        resultSearch(p->parent);
        printf("%d, f = %d, g = %d, h = %d\n", z, p->f, p->g, p->h);
        printStatus(p);
        z++;
    }
}

int main() {
    // Tao trang thai dau tien cua tro choi
    int statusBegin[3][3] = {{0, 2, 1}, {6, 7, 5}, {3, 8, 4}};
    // Tao trang thai cuoi cua tro choi
    int statusEnd[3][3] = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}};
    
    Node *star = makeNode(statusBegin);  
    Node *end = makeNode(statusEnd);
    
    Fringe *fringe = (Fringe*) malloc(sizeof(Fringe));
    if(fringe == NULL) {
        printf("loi cap phat o nho!\n");
        return;
    }
    initFringe(fringe);
    
    
    ListStatus *listStatus = (ListStatus*)malloc(sizeof(ListStatus));
    initListStatus(listStatus);
   
    Node *result = aStar(star, end, fringe, listStatus);
    resultSearch(result);
    
    
    return 0;
}

