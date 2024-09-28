# What is machine emulation?

By simulating the processor, memory and other machine components (e.g. network card, storage, graphics card) the behavior of a real machine can be emulated. For example Jor1k emulates a complete machine, including a simple process ("OR1K") using Javascript and thus can run inside a web browser. 

# Emulating a CPU means emulating every possible op-code

To execute a single CPU instruction the emulator must first read a word from memory (translating virtual addresses into physical addresses), then decode the word in an opcode. The execution of each possible CPU opcode is implemented using a very large set of case statements or a lookup using a function table. The instruction encodes the opcode instruction (add, or, jump-to-new-address,etc) in upper bits (bits 26 to 31). The lower bits encode which CPU registers to for the and and or operations. 

```C
     ins = this.GetInstruction(this.pc<<2)
     // The CPU opcode is stored in 
     switch ((ins >> 26)&0x3F) {
        ...
        case 0x29:
            // andi
            r[(ins >> 21) & 0x1F] = r[(ins >> 16) & 0x1F] & (ins & 0xFFFF);
            break;

        case 0x2A:
            // ori
            r[(ins >> 21) & 0x1F] = r[(ins >> 16) & 0x1F] | (ins & 0xFFFF);
            break;
         ...
       }
       this.pc = this.nextpc++;

Source: https://github.com/s-macke/jor1k/blob/master/js/worker/or1k/safecpu.js
```

An emulator of the RISCV processor is shown below.
```C
// Emulation of the RISCV processor
                // addi, slti, sltiu, xori, ori, andi, slli, srli, srai
                rindex = (ins >> 7) & 0x1F;
                rs1 = r[(ins >> 15) & 0x1F];
                imm = (ins >> 20);
                switch((ins >> 12)&0x7) {
                    
                    case 0x00:
                        // addi
                        r[rindex] = rs1 + imm;
                        break;

                    case 0x06:
                        // ori
                        r[rindex] = rs1 | imm;
                        break;
                     ....
              }
Source: https://github.com/s-macke/jor1k/blob/master/js/worker/riscv/safecpu.js
```

In practice at least 10 to 100 real CPU instructions need to be executed to emulate the behavior of a single instruction of the virtual machine. Thus software run in emulated machines typically executes 10 to 100 times slower than native performance. 

# Advantages of emulation.

Full emulation does provide two important features - security and the ability to execute binary images that have been compiled for a different architecture. For example, an Android Linux operating system compiled for an ARM processor can be tested and debugged on a x86-64bit (Intel) host, even though the Intel CPU has c completely different instruction set.

# Are emulated machines secure?

Full system emulators such as JOR1K provide a relatively safe environment to run untrusted code. For example it would be difficult to write malicious code to escape the JOR1K environment and run arbitrary code on the host machine. 

However emulators are complex and provide virtual devices to support I/O. Virtual devices may still be used by malicious software to perform real attacks such as denial of service or password guessing on real services. If there are security bugs in the implemetation of virtual devices, then these vulnerabilities may be exploited to access resources (such as host files or the Internet) that otherwise should be prevented.

# Acccuracy

Todo : discuss performance vs accuracy tradeoffs.

# Qemu is a full system emulator.

QEMU is a generic and open source machine emulator and virtualizer, that can run within a Windows, Linux or OSX host. Similar to Jor1k emulator above, Qemu can emulate a complete system including CPU, memory, cdrom, audio, USB connections, network, storage, graphics, and keyboard I/O.
