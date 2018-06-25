/*
来源：http://www.wowotech.net/kernel_synchronization/445.html

*/

typedef struct
{
	union
	{
		unsigned int slock;
		struct __raw_tickets
		{
			unsigned short int owner;
			unsigned short int next;
		}tickets;
	};

}arch_spinlock_t;

/***********************************************************************
Function:arch_spin_lock                  //函数名字
Desc:    申请锁操作                //函数功能描述
Input:   arch_spinlock_t * lock    // 要输入的参数
* @param lock -  当前锁的结构体指针
Output:  无                        // 对输出参数的说明。
Return:  无                        // 函数返回值的说明
Others:  无                        // 其它说明；没有就用nullptr表示
**************************************************************************/
static inline void arch_spin_lock(arch_spinlock_t * lock)
{
	arch_spinlock_t old_lock;
	old_lock.slock = lock->slock;
	lock->tickets.next++;
	while(old_lock.tickets.next != old_lock.tickets.owner)
	{
		//wfe(); //wfe()函数是指ARM64架构的WFE(wait for event)汇编指令。WFE是让ARM核进入低功耗模式的指令。当进程拿不到锁的时候，原地自旋不如cpu睡眠。节能。睡下去之后，什么时候醒来呢？就是等到持有锁的进程释放的时候，醒过来判断是否可以持有锁。如果不能获得锁，继续睡眠即可。这里就相当于顾客先小憩一会，等到广播下一位排队者的时候，醒来看看是不是自己。 
		old_lock.tickets.owner = lock->tickets.owner;
	}
}

/***********************************************************************
Function:arch_spin_unlock           //函数名字
Desc:    申请锁操作                //函数功能描述
Input:   arch_spinlock_t * lock    // 要输入的参数
* @param lock -  当前锁的结构体指针
Output:  无                        // 对输出参数的说明。
Return:  无                        // 函数返回值的说明
Others:  无                        // 其它说明；没有就用nullptr表示
**************************************************************************/
static inline void arch_spin_unlock(arch_spinlock_t *lock)
{
	lock->tickets.owner++;
	//sev() //sev()函数是指ARM64架构的SEV汇编指令。当进程无法获取锁的时候会使用WFE指令使CPU睡眠。现在释放锁了，自然要唤醒所有睡眠的CPU醒来检查自己是不是可以获取锁。
}