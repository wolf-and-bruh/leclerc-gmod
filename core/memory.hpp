#pragma once
#include "../includes.hpp"

#define INRANGE(x, a, b) (x >= a && x <= b) 
#define GETBITS(x) (INRANGE((x & (~0x20)), 'A', 'F') ? ((x & (~0x20)) - 'A' + 0xA) : (INRANGE(x, '0', '9') ? x - '0' : 0))
#define GETBYTE(x) (GETBITS(x[0]) << 4 | GETBITS(x[1]))

namespace mem {
	inline std::uintptr_t PatternScan(const char* moduleName, const char* pattern) noexcept {
		HMODULE module = GetModuleHandleA(moduleName);

		MODULEINFO moduleInfo;
		GetModuleInformation(GetCurrentProcess(), module, &moduleInfo, sizeof(MODULEINFO));

		std::uintptr_t start = reinterpret_cast<std::uintptr_t>(module);
		std::uintptr_t end = start + moduleInfo.SizeOfImage;

		for (std::uintptr_t addr = start; addr < end; addr++) {
			bool found = true;

			int off = 0;
			for (const char* pat = pattern; *pat;) {
				if (*pat == ' ') {
					pat++;
					continue;
				}
				else if (*pat == '?') {
					pat += 2;
					off++;
					continue;
				}

				if (*reinterpret_cast<BYTE*>(addr + off) != GETBYTE(pat)) {
					found = false;
					break;
				}

				pat += 2;
				off++;
			}

			if (found)
				return addr;
		}

		return NULL;
	};

	template <typename Return, typename ... Arguments>
	constexpr Return Call(void* vmt, const std::uint32_t index, Arguments ... args) noexcept
	{
		using Function = Return(__thiscall*)(void*, decltype(args)...);
		return (*static_cast<Function**>(vmt))[index](vmt, args...);
	}

	constexpr void* Get(void* vmt, const std::uint32_t index) noexcept
	{
		return (*static_cast<void***>(vmt))[index];
	}

    static void* GetAbsAddress(std::uintptr_t inst, std::uintptr_t instOffset = 3, std::uintptr_t instSize = 7) {
        int offset = *reinterpret_cast<int*>(inst + instOffset);
        std::uintptr_t rip = inst + instSize;

        return reinterpret_cast<void*>(rip + static_cast<std::uintptr_t>(offset));
    }
}