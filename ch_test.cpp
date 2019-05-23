#define CH_IMPLEMENTATION
#include "ch_stl.h"

int main() {
    u8* memory = (u8*)Memory::alloc(1024);
    memory[0] = 5;

    return memory[12];
}