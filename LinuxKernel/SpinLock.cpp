/*
��Դ��http://www.wowotech.net/kernel_synchronization/445.html

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
Function:arch_spin_lock                  //��������
Desc:    ����������                //������������
Input:   arch_spinlock_t * lock    // Ҫ����Ĳ���
* @param lock -  ��ǰ���Ľṹ��ָ��
Output:  ��                        // �����������˵����
Return:  ��                        // ��������ֵ��˵��
Others:  ��                        // ����˵����û�о���nullptr��ʾ
**************************************************************************/
static inline void arch_spin_lock(arch_spinlock_t * lock)
{
	arch_spinlock_t old_lock;
	old_lock.slock = lock->slock;
	lock->tickets.next++;
	while(old_lock.tickets.next != old_lock.tickets.owner)
	{
		//wfe(); //wfe()������ָARM64�ܹ���WFE(wait for event)���ָ�WFE����ARM�˽���͹���ģʽ��ָ��������ò�������ʱ��ԭ����������cpu˯�ߡ����ܡ�˯��ȥ֮��ʲôʱ�������أ����ǵȵ��������Ľ����ͷŵ�ʱ���ѹ����ж��Ƿ���Գ�������������ܻ����������˯�߼��ɡ�������൱�ڹ˿���С�һ�ᣬ�ȵ��㲥��һλ�Ŷ��ߵ�ʱ�����������ǲ����Լ��� 
		old_lock.tickets.owner = lock->tickets.owner;
	}
}

/***********************************************************************
Function:arch_spin_unlock           //��������
Desc:    ����������                //������������
Input:   arch_spinlock_t * lock    // Ҫ����Ĳ���
* @param lock -  ��ǰ���Ľṹ��ָ��
Output:  ��                        // �����������˵����
Return:  ��                        // ��������ֵ��˵��
Others:  ��                        // ����˵����û�о���nullptr��ʾ
**************************************************************************/
static inline void arch_spin_unlock(arch_spinlock_t *lock)
{
	lock->tickets.owner++;
	//sev() //sev()������ָARM64�ܹ���SEV���ָ��������޷���ȡ����ʱ���ʹ��WFEָ��ʹCPU˯�ߡ������ͷ����ˣ���ȻҪ��������˯�ߵ�CPU��������Լ��ǲ��ǿ��Ի�ȡ����
}