module object;

alias string = immutable(char)[];
alias size_t = typeof(int.sizeof);
alias ptrdiff_t = typeof(cast(void*) 0 - cast(void*) 0);

alias noreturn = typeof(*null);

static if ((void*).sizeof == 8) {
    alias uintptr = ulong;
} else static if ((void*).sizeof == 4) {
    alias uintptr = uint;
} else {
    static assert(0, "pointer size must be 4 or 8 bytes");
}

pragma(LDC_intrinsic, "ldc.bitop.vld") ubyte volatileLoad(ubyte* ptr);
pragma(LDC_intrinsic, "ldc.bitop.vld") ushort volatileLoad(ushort* ptr);
pragma(LDC_intrinsic, "ldc.bitop.vld") uint volatileLoad(uint* ptr);
pragma(LDC_intrinsic, "ldc.bitop.vld") ulong volatileLoad(ulong* ptr);
pragma(LDC_intrinsic, "ldc.bitop.vst") void volatileStore(ubyte* ptr, ubyte value);
pragma(LDC_intrinsic, "ldc.bitop.vst") void volatileStore(ushort* ptr, ushort value);
pragma(LDC_intrinsic, "ldc.bitop.vst") void volatileStore(uint* ptr, uint value);
pragma(LDC_intrinsic, "ldc.bitop.vst") void volatileStore(ulong* ptr, ulong value);


import uart;

void printElem(char c) {
    Uart.tx(c);
}

void printElem(string s) {
    foreach (c; s) {
        printElem(c);
    }
}

void print(Args...)(Args args) {
    foreach (arg; args) {
        printElem(arg);
    }
}

void println(Args...)(Args args) {
    print(args, '\n');
}
