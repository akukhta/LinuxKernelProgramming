cmd_/home/alex/LinuxKernelProgramming/Lesson2/modules.order := {   echo /home/alex/LinuxKernelProgramming/Lesson2/sparrow.ko; :; } | awk '!x[$$0]++' - > /home/alex/LinuxKernelProgramming/Lesson2/modules.order
