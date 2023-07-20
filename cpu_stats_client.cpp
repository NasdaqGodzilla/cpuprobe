#include <android-base/macros.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>

#include <bpf/BpfMap.h>
#include <bpf/BpfUtils.h>
#include <libbpf_android.h>
#include "libbpf.h"
#include <netdutils/Status.h>

#include "cpu_stats.h"

#define CPU_STATS_PROG_PATH "/sys/fs/bpf/prog_cpu_stats_tracepoint_sched_sched_switch"
#define CPU_STATS_BPFMAP_PATH "/sys/fs/bpf/map_cpu_stats_cpu_stats_map"

typedef std::function<const android::netdutils::Status(const CPU_STATS_BPFMAP_KEY& key, const CPU_STATS_BPFMAP_VALUE& value,
        const android::bpf::BpfMap<CPU_STATS_BPFMAP_KEY, CPU_STATS_BPFMAP_VALUE>& map)>
        CPU_STATS_MAP_ITERATOR;

auto cpu_stats_client_print_records = [] (const CPU_STATS_BPFMAP_KEY& key, const CPU_STATS_BPFMAP_VALUE& value,
        const android::bpf::BpfMap<CPU_STATS_BPFMAP_KEY, CPU_STATS_BPFMAP_VALUE>&) {
    (void) key;

    printf("%-16llu %-8llu %-8llu %-8llu %-32s %-4llu\n",
            value.ktime_ns,
            value.uid_gid & 0xFFFF,
            value.pid_tgid & 0xFFFF,
            value.pid_tgid >> 32,
            value.comm,
            value.smp_id);

    return android::netdutils::Status(0);
};

int main(int argc, char* argv[]) {
    constexpr const char cpu_stats_prog_path[] = CPU_STATS_PROG_PATH;
    constexpr const char cpu_stats_bpfmap_path[] = CPU_STATS_BPFMAP_PATH;
    const int m_prog = bpf_obj_get(cpu_stats_prog_path);
    const int m_map = bpf_obj_get(cpu_stats_bpfmap_path);

    int interval_us = 1 * 1000 * 1000;
    if (2 == argc && nullptr != argv)
        interval_us = atoi(*argv);

    if (0 > m_prog || 0 > m_map) {
        printf("cpu_stats (prog: %d, map: %d) init failed\n", m_prog, m_map);
        return m_map;
    }

    bpf_attach_tracepoint(m_prog, "sched", "sched_switch");
    // sleep(0);

    android::bpf::BpfMap<CPU_STATS_BPFMAP_KEY, CPU_STATS_BPFMAP_VALUE> m_bpf_map(m_map);

    for (;;) {
        (void) m_bpf_map.iterateWithValue(cpu_stats_client_print_records);
        (void) m_bpf_map.clear();
        usleep(interval_us);
    }

    return 0;
}
