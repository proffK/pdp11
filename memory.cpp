#include"memory.hpp"

#define IS_ROM(x) ((RAM_SIZE + VRAM_SIZE) <= x && x < (MEM_SIZE - IO_SIZE))
#define IS_VRAM(x) ((RAM_SIZE + (VRAM_SIZE / 2) * _vram_mode) <= x && x < (RAM_SIZE + VRAM_SIZE - (VRAM_SIZE / 2) * !_vram_mode))

Timings* Timings::_timings = 0;

int Memory::read(uint32_t addr, uint32_t* dest) {
        if (addr >= sizeof(MemoryMap))
        {
                abort();
        }
        if (addr == 0xff00) {
                *dest = rand();
                return Timings::timings()->get_reg();
        }
        *dest = (0xFFFF & (uint32_t) _mem->wmem[addr / 2]);
        if (addr < MEM_SIZE) {
                return Timings::timings()->get_mem_r();
        }
        return Timings::timings()->get_reg();
}

int Memory::write(uint32_t addr, uint32_t* src)
{
        if (addr >= sizeof(MemoryMap))
        {
                return -EFAULT;
        }
        if (IS_ROM(addr)) {
                _violated = true;
                return Timings::timings()->get_mem_r();
        }
        if (addr == 0xf000) {
                if (*src == 1) {
                        _vram_mode = 1;
                        _vram_dirty = true;
                } else if (*src == 0) {
                        _vram_mode = 0;
                        _vram_dirty = true;
                } else {
                        _violated = true;
                }
                return Timings::timings()->get_mem_r();
        }
        _mem->wmem[addr / 2] = (uint16_t) (0xFFFF & *src);
        if (IS_VRAM(addr)) {
                _vram_dirty = true;
                return Timings::timings()->get_mem_r();
        }
        if (addr < MEM_SIZE) {
                return Timings::timings()->get_mem_r();
        }
        return Timings::timings()->get_reg();
}

int Memory::read(uint32_t addr, uint16_t* dest)
{
        if (addr >= sizeof(MemoryMap))
        {
                abort();
        }
        if (addr == 0xff00) {
                *dest = rand();
                return Timings::timings()->get_reg();
        }
        *dest = _mem->wmem[addr / 2];
        if (addr < MEM_SIZE) {
                return Timings::timings()->get_mem_r();
        }
        return Timings::timings()->get_reg();
}

int Memory::write(uint32_t addr, uint16_t* src)
{
        if (addr >= sizeof(MemoryMap))
        {
                return -EFAULT;
        }
        if (IS_ROM(addr)) {
                _violated = true;
                return Timings::timings()->get_mem_r();
        }
        if (addr == 0xf000) {
                if (*src == 1) {
                        _vram_mode = 1;
                        _vram_dirty = true;
                } else if (*src == 0) {
                        _vram_mode = 0;
                        _vram_dirty = true;
                } else {
                        _violated = true;
                }
                return Timings::timings()->get_mem_r();
        }
        _mem->wmem[addr / 2] = *src;
        if (IS_VRAM(addr)) {
                _vram_dirty = true;
                return Timings::timings()->get_mem_r();
        }
        if (addr < MEM_SIZE) {
                return Timings::timings()->get_mem_r();
        }
        return Timings::timings()->get_reg();
}

int Memory::read(uint32_t addr, uint8_t* dest)
{
        if (addr >= sizeof(MemoryMap))
        {
                abort();
        }
        if (addr == 0xff00) {
                *dest = rand();
                return Timings::timings()->get_reg();
        } 
        *dest = _mem->mem[addr];
        if (addr < MEM_SIZE) {
                return Timings::timings()->get_mem_r();
        }
        return Timings::timings()->get_reg();
}

int Memory::write(uint32_t addr, uint8_t* src)
{
        if (addr >= sizeof(MemoryMap))
        {
                return -EFAULT;
        }
        if (IS_ROM(addr)) {
                _violated = true;
                return Timings::timings()->get_mem_r();
        }
        if (addr == 0xf000) {
                if (*src == 1) {
                        _vram_mode = 1;
                        _vram_dirty = true;
                } else if (*src == 0) {
                        _vram_mode = 0;
                        _vram_dirty = true;
                } else {
                        _violated = true;
                }
                return Timings::timings()->get_mem_r();
        }
        _mem->mem[addr] = *src;
        if (IS_VRAM(addr)) {
                _vram_dirty = true;
                return Timings::timings()->get_mem_r();
        }
        if (addr < MEM_SIZE) {
                return Timings::timings()->get_mem_r();
        }
        return Timings::timings()->get_reg();
}
