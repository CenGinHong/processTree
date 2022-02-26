#include <linux/sched/task.h>
#define MAX_LENGTH 8096

void process_tree(struct task_struct *cur_process, int cur_depth);
static int mycall(struct pt_regs *regs);
typedef struct
{
    int pid;
    int depth;
} Process_Node;

// 进程数信息
Process_Node process_array[MAX_LENGTH];
// 全局计数器
int counter;

/**
 * @brief
 *
 * @param cur_process 当前进程
 * @param cur_depth 进程树深度
 */
void process_tree(struct task_struct *cur_process, int cur_depth)
{
    struct task_struct *task;
    struct list_head *list;
    process_array[counter].pid = cur_process->pid;
    process_array[counter].depth = cur_depth;
    counter++;
    // 遍历子进程
    list_for_each(list, &cur_process->children)
    {
        task = list_entry(list, struct task_struct, sibling);
        process_tree(task, cur_depth + 1);
    }
}

/**
 * @brief 自定义服务
 *
 * @param regs 用户态传入的地址
 * @return int
 */
static int mycall(struct pt_regs *regs)
{
    //获取用户态传入的地址
    unsigned int *addr = (unsigned int *)regs->di;
    // 0号进程的描述符
    struct task_struct *process_0 = &init_task;
    counter = 0;
    // 获取进程树信息
    process_tree(process_0, 0);
    // 将获取的信息传回用户态
    copy_to_user(addr, process_array, MAX_LENGTH * sizeof(Process_Node));
    return 0;
}