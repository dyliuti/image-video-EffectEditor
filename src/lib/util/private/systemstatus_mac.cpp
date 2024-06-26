﻿#include "systemstatus.h"
#include "systemstatus_mac.h"
#include <QString>

#include <QBasicTimer>
#include <sys/types.h>
#include <sys/sysctl.h>

#define CP_USER 0
#define CP_SYS 1
#define CP_IDLE 2
#define CP_NICE 3
#define CP_STATES 4


SystemStatusPrivate::SystemStatusPrivate(QObject *parent)
    : SystemStatusBasePrivate(parent)
{

}

SystemStatusPrivate::~SystemStatusPrivate()
{

}

float SystemStatusPrivate::getSysMemUsePercentage()
{
    // These values are in bytes
    u_int64_t total_mem = 0;
    float used_mem = 0;

    vm_size_t page_size;
    vm_statistics_data_t vm_stats;

    // Get total physical memory
    int mib[] = {CTL_HW, HW_MEMSIZE};
    size_t length = sizeof(total_mem);
    sysctl(mib, 2, &total_mem, &length, NULL, 0);

    mach_port_t mach_port = mach_host_self();
    mach_msg_type_number_t count = sizeof(vm_stats) / sizeof(natural_t);
    if (KERN_SUCCESS == host_page_size(mach_port, &page_size)
        && KERN_SUCCESS
               == host_statistics(mach_port, HOST_VM_INFO, (host_info_t) &vm_stats, &count)) {
        used_mem = static_cast<float>((vm_stats.active_count + vm_stats.wire_count) * page_size);
    }

    uint usedMem = convert_unit(static_cast<float>(used_mem), MEGABYTES);
    uint totalMem = convert_unit(static_cast<float>(total_mem), MEGABYTES);
    return float((usedMem * 100) / totalMem);
}

float SystemStatusPrivate::getSysTotalMem()
{
    u_int64_t total_mem = 0;

    // Get total physical memory
    int mib[] = {CTL_HW, HW_MEMSIZE};
    size_t length = sizeof(total_mem);
    sysctl(mib, 2, &total_mem, &length, NULL, 0);

    return convert_unit(static_cast<float>(total_mem), MEGABYTES);
}

qint32 SystemStatusPrivate::getCurProcessMemUsed()
{
    struct task_basic_info info;
    mach_msg_type_number_t size = TASK_BASIC_INFO_COUNT; //sizeof(info);
    kern_return_t kerr = task_info(mach_task_self(), TASK_BASIC_INFO, (task_info_t) &info, &size);
    return (kerr == KERN_SUCCESS) ? info.resident_size / 1024.0 / 1024.0 : 0; // size in bytes
}

QPair<float, float> SystemStatusPrivate::getCpuUsePercentage()
{
    return { getSysCpuUsePercentage(), getProcessCpuUsePercentage() };
}


host_cpu_load_info_data_t _get_cpu_percentage()
{
    kern_return_t error;
    mach_msg_type_number_t count;
    host_cpu_load_info_data_t r_load;
    mach_port_t mach_port;

    count = HOST_CPU_LOAD_INFO_COUNT;
    mach_port = mach_host_self();
    error = host_statistics(mach_port, HOST_CPU_LOAD_INFO, (host_info_t) &r_load, &count);

    if (error != KERN_SUCCESS) {
        return host_cpu_load_info_data_t();
    }

    return r_load;
}

float SystemStatusPrivate::getSysCpuUsePercentage()
{
    host_cpu_load_info_data_t load1 = m_cpuLoad;
    host_cpu_load_info_data_t load2 = _get_cpu_percentage();
    m_cpuLoad = load2;

    if (m_isFirstGetData) {
        m_isFirstGetData = false;
        return -1;
    }

    // pre load times
    unsigned long long current_user = load1.cpu_ticks[CP_USER];
    unsigned long long current_system = load1.cpu_ticks[CP_SYS];
    unsigned long long current_nice = load1.cpu_ticks[CP_NICE];
    unsigned long long current_idle = load1.cpu_ticks[CP_IDLE];
    // Current load times
    unsigned long long next_user = load2.cpu_ticks[CP_USER];
    unsigned long long next_system = load2.cpu_ticks[CP_SYS];
    unsigned long long next_nice = load2.cpu_ticks[CP_NICE];
    unsigned long long next_idle = load2.cpu_ticks[CP_IDLE];
    // Difference between the two
    unsigned long long diff_user = next_user - current_user;
    unsigned long long diff_system = next_system - current_system;
    unsigned long long diff_nice = next_nice - current_nice;
    unsigned long long diff_idle = next_idle - current_idle;

    return static_cast<float>(diff_user + diff_system + diff_nice)
           / static_cast<float>(diff_user + diff_system + diff_nice + diff_idle) * 100.0;
}

float SystemStatusPrivate::getProcessCpuUsePercentage()
{
    float processValue = 0.0;
    QString cmdLine = QString("ps c -O %cpu -p %1 | awk '{print $2}' | tail -1").arg(m_appId);
    char cmd[512];
    strcpy(cmd, cmdLine.toStdString().c_str());
    FILE *fp = popen(cmd, "r");
    if (fp) {
        char pid[256];
        char line[1024];
        fgets(line, 1024, fp);
        sscanf(line, "%s", pid);
        processValue = QString(line).trimmed().toFloat();
        pclose(fp);
    }
    return processValue;
}

QPair<float, float> SystemStatusPrivate::getCpuInfo()
{
    return QPair<float, float>(getSysCpuUsePercentage(), getProcessCpuUsePercentage());
}

