SET PATH=C:\MDK5\ARM\AC6.14\Bin;C:\Java\bin;C:\Java\jre\bin;E:\vs code\Microsoft VS Code\bin;E:\MATLAB\runtime\win64;E:\MATLAB\bin;E:\LabView\VISA\WinNT\Bin\;E:\LabView\VISA\WinNT\Bin;C:\Program Files\IVI Foundation\VISA\Win64\Bin\;C:\Program Files\Microsoft SQL Server\110\Tools\Binn\;e:\Program Files\Git\cmd;E:\Node.js\;E:\Android\android-sdk\tools\bin;E:\Android\android-sdk\platform-tools;E:\Android\apktool\dex2jar-2.0;F:\hg007\ffmpeg\ffmpeg-4.2.2-win64-static\bin;C:\Windows\System32\WindowsPowerShell\v1.0;E:\CodeBlocks\MinGW\bin;C:\Program Files\Intel\WiFi\bin\;C:\Program Files\Common Files\Intel\WirelessCommon\;C:\Users\admin\AppData\Local\Programs\Python\Python36\Scripts\;C:\Users\admin\AppData\Local\Programs\Python\Python36\;E:\Tools\WinNT;E:\MSDev98\Bin;E:\Tools;c:\bin;C:\Users\admin\AppData\Local\Microsoft\WindowsApps;E:\Mysql\mysql-8.0.13\bin;E:\PHP;E:\SQLite;E:\MQTT.fx\Fiddler;C:\Windows\System32;C:\Program Files\OpenSSH\OpenSSH-Win64;C:\Users\admin\AppData\Roaming\npm;E:\curl\AMD64;C:\Program Files\Intel\WiFi\bin\;C:\Program Files\Common Files\Intel\WirelessCommon\
SET CPU_TYPE=STM32F103ZE
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=Template
SET CPU_CLOCK=0x007A1200
"C:\MDK5\ARM\AC6.14\Bin\ArmAsm" --Via "..\obj\startup_stm32f10x_hd._ia"
"C:\MDK5\ARM\AC6.14\Bin\ArmClang.exe" @"..\obj\main.__i"
"C:\MDK5\ARM\AC6.14\Bin\ArmClang.exe" @"..\obj\delay.__i"
"C:\MDK5\ARM\AC6.14\Bin\ArmClang.exe" @"..\obj\sys.__i"
"C:\MDK5\ARM\AC6.14\Bin\ArmClang.exe" @"..\obj\adc.__i"
"C:\MDK5\ARM\AC6.14\Bin\ArmClang.exe" @"..\obj\gpio.__i"
"C:\MDK5\ARM\AC6.14\Bin\ArmClang.exe" @"..\obj\f_iic.__i"
"C:\MDK5\ARM\AC6.14\Bin\ArmClang.exe" @"..\obj\h_iic.__i"
"C:\MDK5\ARM\AC6.14\Bin\ArmClang.exe" @"..\obj\pwm.__i"
"C:\MDK5\ARM\AC6.14\Bin\ArmClang.exe" @"..\obj\rcc.__i"
"C:\MDK5\ARM\AC6.14\Bin\ArmClang.exe" @"..\obj\spi.__i"
"C:\MDK5\ARM\AC6.14\Bin\ArmClang.exe" @"..\obj\timer.__i"
"C:\MDK5\ARM\AC6.14\Bin\ArmClang.exe" @"..\obj\myuart.__i"
"C:\MDK5\ARM\AC6.14\Bin\ArmClang.exe" @"..\obj\uart_recive_handler.__i"
"C:\MDK5\ARM\AC6.14\Bin\ArmLink" --Via "..\OBJ\Template.lnp"
"C:\MDK5\ARM\AC6.14\Bin\fromelf.exe" "..\OBJ\Template.axf" --i32combined --output "..\OBJ\Template.hex"
C:\MDK5\ARM\AC6.14\bin\fromelf.exe --bin -o ..\OBJ\out.bin ..\OBJ\Template.axf
