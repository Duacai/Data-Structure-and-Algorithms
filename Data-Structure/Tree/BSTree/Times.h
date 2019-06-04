#ifndef TIMES_H
#define TIMES_H

#include <iostream>
#ifdef _WIN32
#include <windows.h>
#endif
#if defined(linux) || defined(__MINGW64__) || defined(__GNUC__)
#include <sys/time.h>
#endif

// intrinsics
#if defined(__GNUC__)    // GCC
#include <cpuid.h>
#elif defined(_MSC_VER)    // MSVC
    #if _MSC_VER >=1400    // VC2005
#include <intrin.h>
    #endif    // #if _MSC_VER >=1400
#else
#error Only supports MSVC or GCC.
#endif    // #if defined(__GNUC__)

namespace Viclib
{
void getcpuid(unsigned int CPUInfo[4], unsigned int InfoType)
{
#if defined(__GNUC__)		// GCC
	__cpuid(InfoType, CPUInfo[0],CPUInfo[1],CPUInfo[2],CPUInfo[3]);
#elif defined(_MSC_VER)		// MSVC
    #if _MSC_VER>=1400		// VC2005才支持__cpuid
	    __cpuid((int*)(void*)CPUInfo, (int)InfoType);
    #else
	    getcpuidex(CPUInfo, InfoType, 0);
    #endif
#endif    // #if defined(__GNUC__)
}

// 取得CPU商标（Brand）.
//
// result: 成功时返回字符串的长度（一般为48）。失败时返回0.
// pbrand: 接收商标信息的字符串缓冲区。至少为49字节.
int cpu_getbrand(char* pbrand)
{
	unsigned int dwBuf[4];
	if ( nullptr == pbrand ) return 0;
	// Function 0x80000000: Largest Extended Function Number
	getcpuid(dwBuf, 0x80000000U);
	if ( dwBuf[0] < 0x80000004U ) return 0;
	// Function 80000002h,80000003h,80000004h: Processor Brand String
	getcpuid(reinterpret_cast<unsigned int *>(&pbrand[0]), 0x80000002U);    // 前16个字符.
	getcpuid(reinterpret_cast<unsigned int *>(&pbrand[16]), 0x80000003U);    // 中间16个字符.
	getcpuid(reinterpret_cast<unsigned int *>(&pbrand[32]), 0x80000004U);    // 最后16个字符.
	pbrand[48] = '\0';

	return 48;
}

unsigned long long getCpuFrq(void)
{
	char info[49];	// Intel(R) Core(TM) i7-3820 CPU @ 3.60GHz
	info[48] = '\0';

	if ( cpu_getbrand(info) )
	{
		strtok(info, "@");					// var[0]="Intel(R) Core(TM) i7-3820 CPU "; var[1]=" 3.60GHz";
		char * p = strtok(nullptr, "G");	// var[1] <==> p=" 3.60";
		p = strtok(p, " ");					// p="3.60";
		return static_cast<unsigned long long>(atof(p)*1000*1000*1000);
	}

	return 0;
}

// CPU频率计时器
#if defined (__i386__)
static inline unsigned long long GetCycleCount(void)
{
		unsigned long long int x;
		__asm__ volatile("rdtsc":"=A"(x));
		return x;
}
#elif defined (__x86_64__)
static inline unsigned long long GetCycleCount(void)
{
	    unsigned hi, lo;
		__asm__ volatile("rdtsc":"=a"(lo),"=d"(hi));
		return (static_cast<unsigned long long>(lo))|(static_cast<unsigned long long>(hi)<<32);
}
#endif

static inline void timing(bool b){
	if ( b ) goto GET;
#if defined(linux) || defined(__MINGW64__) || defined(__GNUC__)
	static struct timeval tv_begin, tv_end;
	gettimeofday(&tv_begin, nullptr);
#endif

#ifdef _WIN32
	static LARGE_INTEGER li_freq, li_start, li_stop;
	QueryPerformanceFrequency(&li_freq);
	QueryPerformanceCounter(&li_start);
#endif

	static uint64_t t1;
	t1 = GetCycleCount();
	static uint64_t frq = getCpuFrq();

	std::cout << "启动计时..." << std::endl;
	goto END;

GET:
#if defined(linux) || defined(__MINGW64__) || defined(__GNUC__)
	gettimeofday(&tv_end, nullptr);
	std::cout << "Linux:  \t" << std::fixed << (tv_end.tv_sec+tv_end.tv_usec/1000000.0)-(tv_begin.tv_sec+tv_begin.tv_usec/1000000.0) << " s" << std::endl;
#endif

#ifdef _WIN32
	QueryPerformanceCounter(&li_stop);
	std::cout << "Windows:\t" << (li_stop.QuadPart-li_start.QuadPart) * 1.0 / li_freq.QuadPart << " s" << std::endl;
#endif

	std::cout << "CPU:    \t" << (GetCycleCount() - t1)*1.0/frq << " s" << std::endl;

END:
	;
}

static void timingStart()
{
	timing(false);
}

static void timingEnd()
{
	timing(true);
}

}

#endif // TIMES_H
