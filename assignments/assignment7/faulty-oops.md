Unable to handle kernel NULL pointer dereference at virtual address 0000000000000000<br/>
Mem abort info:<br/>
 &nbsp; ESR = 0x96000045<br/>
&nbsp;  EC = 0x25: DABT (current EL), IL = 32 bits<br/>
 &nbsp; SET = 0, FnV = 0<br/>
 &nbsp; EA = 0, S1PTW = 0<br/>
Data abort info:<br/>
 &nbsp; ISV = 0, ISS = 0x00000045<br/>
 &nbsp; CM = 0, WnR = 1<br/>
user pgtable: 4k pages, 39-bit VAs, pgdp=0000000042094000<br/>
[0000000000000000] pgd=0000000000000000, p4d=0000000000000000, pud=0000000000000000<br/>
Internal error: Oops: 96000045 [#1] SMP<br/>
Modules linked in: hello(O) scull(O) faulty(O)<br/>
CPU: 0 PID: 150 Comm: sh Tainted: G           O      5.10.7 #1<br/>
Hardware name: linux,dummy-virt (DT)<br/>
pstate: 80000005 (Nzcv daif -PAN -UAO -TCO BTYPE=--)<br/>
pc : faulty_write+0x14/0x20 [faulty]<br/>
lr : vfs_write+0xf0/0x290<br/>
sp : ffffffc010cb3dc0<br/>
x29: ffffffc010cb3dc0 x28: ffffff80020b9980 <br/>
x27: 0000000000000000 x26: 0000000000000000 <br/>
x25: 0000000000000000 x24: 0000000000000000 <br/>
x23: 0000000000000000 x22: ffffffc010cb3e30 <br/>
x21: 0000005577251940 x20: ffffff8002059500 <br/>
x19: 0000000000000012 x18: 0000000000000000 <br/>
x17: 0000000000000000 x16: 0000000000000000 <br/>
x15: 0000000000000000 x14: 0000000000000000 <br/>
x13: 0000000000000000 x12: 0000000000000000 <br/>
x11: 0000000000000000 x10: 0000000000000000 <br/>
x9 : 0000000000000000 x8 : 0000000000000000 <br/>
x7 : 0000000000000000 x6 : 0000000000000000 <br/>
x5 : ffffff8002043848 x4 : ffffffc0086c0000 <br/>
x3 : ffffffc010cb3e30 x2 : 0000000000000012 <br/>
x1 : 0000000000000000 x0 : 0000000000000000 <br/>
Call trace:<br/>
 faulty_write+0x14/0x20 [faulty]<br/>
 ksys_write+0x68/0x100<br/>
 __arm64_sys_write+0x20/0x30<br/>
 el0_svc_common.constprop.0+0x94/0x1c0<br/>
 do_el0_svc+0x40/0xb0<br/>
 el0_svc+0x14/0x20<br/>
 el0_sync_handler+0x1a4/0x1b0<br/>
 el0_sync+0x174/0x180<br/>
Code: d2800001 d2800000 d503233f d50323bf (b900003f) <br/>
---[ end trace c80d8945c15ef6e9 ]---<br/>


ANALYSIS: 
When we echo "hello-world" the runqemu.sh goes in faulty. According to the first statement it says its unable to handle kernel NULL pointer dereference at address 0; which means we tried to access a NULL pointer. \
This is the cause for faulty_write to be called. We can also see that at "faulty_write+0x14/0x20" the faulty write was called and just before this qemu attempted to write at the address - x1 : 0000000000000000 x0 : 0000000000000000. When this error hits, the qemu re-starts.
