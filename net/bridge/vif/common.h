//#define pr_fmt(fmt) KBUILD_MODNAME ":%s: " fmt, __func__

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/list.h>
#include <linux/string.h>
#include <linux/kvm_host.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/ip.h>
#include <linux/in.h>
#include <linux/io.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include </home/rpi2/qCon---rpi5-v6.8-/net/bridge/br_private.h>


//default value
#define CONFIG_BRIDGE_CREDIT_MODE
#define MAX_CREDIT 1000000	
#define MIN_CREDIT 100000

//pay function
#define PAY_SUCCESS	1
#define PAY_FAIL	0

#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#define MIN(a,b) (((a) < (b)) ? (a) : (b))

extern void (*fp_newvif)(struct net_bridge_port *p);
extern void (*fp_delvif)(struct net_bridge_port *p);
extern int (*fp_pay)(struct ancs_container *vif, struct sk_buff *skb);


struct credit_allocator{
	struct list_head active_vif_list;
	spinlock_t active_vif_list_lock;

	struct timer_list account_timer;

	unsigned int total_weight;
	unsigned int credit_balance;
	int num_vif;
};


int pay_credit(struct ancs_container *vif, struct sk_buff *skb);
void new_vif(struct net_bridge_port *p);
void del_vif(struct net_bridge_port *p);
static void credit_accounting(struct timer_list *timer);

struct proc_dir_vif{
	char name[10];
	int id;
	struct proc_dir_entry *dir;
	struct proc_dir_entry *file[10];
};
