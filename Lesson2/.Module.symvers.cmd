cmd_/home/alex/LinuxKernelProgramming/Lesson2/Module.symvers := sed 's/ko$$/o/' /home/alex/LinuxKernelProgramming/Lesson2/modules.order | scripts/mod/modpost -m -a   -o /home/alex/LinuxKernelProgramming/Lesson2/Module.symvers -e -i Module.symvers   -T -
