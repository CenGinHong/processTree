#include <stdio.h>
#include <unistd.h> 

#define MAX_LENGTH 8096
typedef struct
{
    int pid;
    int depth;
} Process_Node;

Process_Node process_tree[MAX_LENGTH];
int main()
{
    if (syscall(360, process_tree) != 0)
    {
        return 1;
    }
    int count = 0;
    //打印结果
    for (int i = 0; i < MAX_LENGTH; i++)
    {
        count++;
        for (int j = 0; j < process_tree[i].depth; j++)
        {
            printf("| ");
        }
        printf("- ");
        printf("%d\n", process_tree[i].pid);
        if (i + 1 == MAX_LENGTH || process_tree[i + 1].pid == 0)
        {
            break;
        }
    }
    printf("total %d process\n", count);
    return 0;
}