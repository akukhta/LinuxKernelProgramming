cmd_/home/alex/LinuxKernelProgramming/ScullDevice/Module.symvers := sed 's/ko$$/o/' /home/alex/LinuxKernelProgramming/ScullDevice/modules.order | scripts/mod/modpost -m -a   -o /home/alex/LinuxKernelProgramming/ScullDevice/Module.symvers -e -i Module.symvers   -T -
