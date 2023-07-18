#define CPU_CORE_NUMBERS                    8
#define CPU_STATS_MAXENTRIES                1024 * 10 * 2

typedef struct {
    unsigned long long ktime_ns;
    unsigned long long smp_id;
    unsigned long long uid_gid;
    unsigned long long pid_tgid;
    char comm[16];
} cpu_stats_record;

typedef unsigned long long                  CPU_STATS_BPFMAP_KEY;
typedef cpu_stats_record                    CPU_STATS_BPFMAP_VALUE;

struct switch_args {
    unsigned long long ignore;
    char prev_comm[16];
    int prev_pid;
    int prev_prio;
    long long prev_state;
    char next_comm[16];
    int next_pid;
    int next_prio;
};

