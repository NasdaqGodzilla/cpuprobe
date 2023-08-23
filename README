# cpuprobe

An example of getting started with Android ebpf. Using ebpf, on Android, print out the switching status of the process on the CPU.

一个Android ebpf的入门示例，利用ebpf，在Android上，打印出进程在CPU上的切换情况。

# Usage

Build -> Install -> Reboot -> Run

## Build via AOSP

After lunch project:

```
m cpu_stats.o
m cpu_stats_client
```

## Install

With adb rooted Android device:

```
adb push cpu_stats.o /system/etc/bpf
adb push cpu_stats_client /system/bin/
```

Reboot is needed.

## Run

In adb, run command:

```
cpu_stats_client
```

# Output format

When the process is switched, print out the information of the process switched to the CPU core.

当进程切换时，打印出切换到该CPU核心上的进程的信息。

```
timestamp    uid    pid    tgid    command    running_cpu_core_id
```

## Example output:

```
844266752127     0        245      245      swapper/2                        2
844466688281     0        245      245      swapper/2                        2
844263030050     0        0        0        surfaceflinger                   5
844150573742     1000     1847     472      kworker/u16:2                    2
844430232281     0        235      235      swapper/1                        1
844288505434     0        1030     1030     swapper/3                        3
844235142742     0        0        0        mali-cmar-backe                  2
844453380357     0        0        0        kworker/u16:7                    1
844542420127     0        245      245      swapper/3                        3
844150695434     0        0        0        mali-cmar-backe                  2
844299940204     1000     2523     2198     swapper/3                        3
844381336434     0        0        0        mali-cmar-backe                  2
844504524819     0        0        0        kworker/u16:23                   0
844524850050     0        0        0        kworker/u16:23                   1
844483787127     1000     530      448      shell svc 4405                   0
844357043665     0        0        0        kworker/u16:7                    3
844350059588     0        0        0        shell svc 4405                   0
844297244204     0        0        0        kworker/u17:0                    2
844301078357     0        0        0        shell svc 4405                   1
844300169050     1000     2830     2198     kworker/u16:2                    5
```
