cmd_/home/alex/LinuxKernelProgramming/ScullDevice/modules.order := {   echo /home/alex/LinuxKernelProgramming/ScullDevice/ScullDevice.ko; :; } | awk '!x[$$0]++' - > /home/alex/LinuxKernelProgramming/ScullDevice/modules.order
