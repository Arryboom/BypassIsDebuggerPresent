### Simple Hook IsDebuggerPresent and always return  
**False**  
### So we can bypass some simple debugger detection by IsDebuggerPresent.
** only works on 32bit **  

Compiled with   

- VC 6  

File: \HookIsDebuggerPresent\Release\ISD.dll  
Size: 274485 bytes  
Modified: 2016-7-9, 20\:30\:22  
MD5: 89364B638CC99D2616C07D3AF7F46C09  
SHA1: C688D459783ADCE2321322DEFEBBC2512A7F4A0D  
CRC32: 0E6DDBC3  

- Check compiled dll from [Here](https://github.com/Arryboom/BypassIsDebuggerPresent/releases/download/1.0/ISD.dll "DLL").  
### Usage  
#### Simple Inject the dll to target process,here is a sample using Cheatengine to inject:
# IsDebuggerPreset_TEST.exe in this repo was for testing purpose.  
- 1   
  ![First Step](https://github.com/Arryboom/BypassIsDebuggerPresent/blob/master/Readme/1.png)
- 2  select the ISD.dll and inject
  ![Over](https://github.com/Arryboom/BypassIsDebuggerPresent/blob/master/Readme/2.png)