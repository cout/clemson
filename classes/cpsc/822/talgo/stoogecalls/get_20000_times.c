#include<linux/sched.h>
#include<linux/kernel.h>
#include<linux/kernel_stat.h>
#include<linux/fs.h>
#include<linux/version.h>
#include<linux/module.h>
#include<linux/pci.h>
#include<linux/major.h>
#include<linux/mm.h>
#include<linux/malloc.h>
#include<linux/delay.h>
#include<asm/io.h>
#include<asm/mman.h>
#include<asm/uaccess.h>
#include<asm/processor.h>

#include <linux/t_algo.h>

extern struct t_algo_stats_s t_algo_stats[];
extern unsigned int t_algo_flag;
extern unsigned int t_algo_index;
extern unsigned int get_times_index;
extern unsigned int num_of_reqs_completed;

int get_20000_times(struct t_algo_stats_s **disktime)
{
  int i, oldfsuid;
  struct t_algo_stats_s *phys_addr = NULL;
  struct t_algo_stats_s *user_virt_addr = NULL;
  struct file *fmem;


  phys_addr = (struct t_algo_stats_s *)virt_to_phys(t_algo_stats); 
  if( !phys_addr)
    printk("Error converting virt_to_phys\n");
    
  oldfsuid = current->fsuid; 
  current->fsuid = 0;
  cap_raise( current->cap_effective, CAP_SYS_RAWIO);
  fmem = filp_open( "/dev/mem", O_RDWR, 0);
  if(!fmem)
    printk("Error opening /dev/memi\n");
  user_virt_addr = (struct t_algo_stats_s *)do_mmap( fmem, 0,
			sizeof(struct t_algo_stats_s) * T_NUM_OF_REQS_MSRD,
			PROT_WRITE | PROT_READ | PROT_EXEC,
			MAP_SHARED,
			phys_addr);

  if( !user_virt_addr)
    printk("Error converting phys_to_user_virt\n");

  copy_to_user( disktime, &user_virt_addr, 4);

  

  return 0;
}

int start_20000_times(int algo) {
  int i;
  if( algo == 0)
    printk("Starting cscan timing\n");
  if( algo == 1)
    printk("Starting greedy timing\n");
  if( algo == 2)
    printk("Starting t_algo timing\n");

  for(i = 0; i < T_NUM_OF_REQS_MSRD; i++)
  {
    t_algo_stats[i].start_of_wait.tv_sec = 0;
    t_algo_stats[i].start_of_wait.tv_usec = 0;
    t_algo_stats[i].start_of_service.tv_sec = 0;
    t_algo_stats[i].start_of_service.tv_usec = 0;
    t_algo_stats[i].service_comp.tv_sec = 0;
    t_algo_stats[i].service_comp.tv_usec = 0;
  }
  /* For our reference:
   * t_algo_flag = 0		cscan/fcfs
   * t_algo_flag = 1		greedy
   * t_algo_flag = 2		algorithm t (optimal)
   */
  t_algo_flag = algo;
  t_algo_index = 0;
  get_times_index = 0;
  num_of_reqs_completed = 0;

  return 0;
}
