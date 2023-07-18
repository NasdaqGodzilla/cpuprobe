#include <linux/bpf.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <bpf_helpers.h>

#include "cpu_stats.h"

DEFINE_BPF_MAP(cpu_stats_map, LRU_HASH, CPU_STATS_BPFMAP_KEY, CPU_STATS_BPFMAP_VALUE, CPU_STATS_MAXENTRIES);

// DEFINE_BPF_PROG("tracepoint/sched/sched_switch", AID_ROOT, AID_NET_ADMIN, cpu_stats_tp_sched_switch) (struct switch_args* args) {
SEC("tracepoint/sched/sched_switch")
int cpu_stats_tp_sched_switch(struct switch_args* args) {
    CPU_STATS_BPFMAP_KEY ktime_ns = bpf_ktime_get_ns();
    CPU_STATS_BPFMAP_VALUE record = {
        .ktime_ns = ktime_ns,
        .smp_id = bpf_get_smp_processor_id(),
        .uid_gid = bpf_get_current_uid_gid(),
        .pid_tgid = bpf_get_current_pid_tgid(),
    };
    memset(record.comm, '\0', sizeof(record.comm));
    memcpy(record.comm, args->next_comm, sizeof(record.comm));

    bpf_cpu_stats_map_update_elem(&ktime_ns, &record, BPF_ANY);

    return 0;
}

char _license[] SEC("license") = "GPL";
