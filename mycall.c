#include <linux/module.h>
#include <linux/unistd.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/kallsyms.h>
#include "process_tree.c"

#define __NR_my_call 360
#define MAX_LENGTH 8096
static int (*tmp_saved)(void);
void clear_cr0(void);
void reset_cr0(void);
static int __init init_mycall(void);
static void __exit exit_mycall(void);

/**
 * @brief 清除写保护位
 *
 */
void clear_cr0(void)
{
    unsigned int cr0 = 0;
    // 将cr0寄存器内容移动到rax寄存器中，并保存到cr0变量
    asm volatile("movq %%cr0, %%rax"
                 : "=a"(cr0));
    // 按位与，将17位置0
    cr0 &= 0xfffeffff;
    // 将cr0置回cr0寄存器
    asm volatile("movq %%rax, %%cr0" ::"a"(cr0));
}

/**
 * @brief 恢复写保护位
 *
 */
void reset_cr0(void)
{
    // 将cr0的17置回1
    unsigned int cr0 = 0;
    asm volatile("movq %%cr0, %%rax"
                 : "=a"(cr0));
    cr0 |= 0x00010000;
    asm volatile("movq %%rax, %%cr0" ::"a"(cr0));
}

/**
 * @brief 加载模块调用
 *
 * @return int
 */
static int __init init_mycall(void)
{
    // 获取sys_call_table地址
    unsigned long **sys_call_table = (unsigned long **)kallsyms_lookup_name("sys_call_table");
    // 清除写保护位
    clear_cr0();
    // 保存原始系统调用
    tmp_saved = (int (*)(void))(sys_call_table[__NR_my_call]);
    // 修改为自定义的服务地址
    sys_call_table[__NR_my_call] = (unsigned long *)mycall;
    // 恢复写保护位
    reset_cr0();
    return 0;
}

/**
 * @brief 卸载模块调用
 *
 */
static void __exit exit_mycall(void)
{
    // 获取sys_call_table地址
    unsigned long **sys_call_table = (unsigned long **)kallsyms_lookup_name("sys_call_table");
    // 清除写保护位
    clear_cr0();
    // 恢复原始系统调用
    sys_call_table[__NR_my_call] = (unsigned long *)tmp_saved;
    // 恢复写保护位
    reset_cr0();
}

module_init(init_mycall);
module_exit(exit_mycall);
MODULE_LICENSE("GPL");