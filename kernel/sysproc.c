#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "date.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  if(myproc()->pid ==3)
  {
    printf("sys_sbrk.\n");
    vmprint(myproc()->pagetable);
  }
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;


  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// #define  LAB_PGTBL
#ifdef LAB_PGTBL

#define MAX_ACCESS_PAGES 32
pte_t * walk(pagetable_t pagetable, uint64 va, int alloc);
int 
sys_pgaccess(void)
{
  uint64 p_va;
  int pg_num;
  int access_bits = 0;
  uint64 p_user;
  pte_t *pte;
  int i;
  // lab pgtbl: your code here.
  // printf("sys_pgaccess.\n");
  // vmprint(myproc()->pagetable);
  if(argaddr(0, &p_va) < 0)
    return -1;
  // printf("va %p.\n",p_va);
  if(argint(1,&pg_num)<0)
    return -1;
  if(argaddr(2,&p_user)<0)
    return -1;
  if(pg_num > MAX_ACCESS_PAGES)
    return -1;
  for(i = 0; i <pg_num; i++ )
  {
    pte = walk(myproc()->pagetable, p_va+i*PGSIZE, 0);
    if(pte == 0)
      panic("sys_pgaccess");
    if(*pte & PTE_A)
    {
      access_bits |= (1<<i);
      *pte &= ~PTE_A; 
      // printf("pte va%p,  PTE_A set:%p\n",p_va+i*PGSIZE,*pte);
    } 
  }
  // printf("access_bits:%x.\n",access_bits);
  return copyout(myproc()->pagetable, p_user, (char*)(&access_bits), sizeof(access_bits));
}
#endif

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
