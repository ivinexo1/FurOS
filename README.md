# <p align="center" dir="auto">FurOS</p>
<div align="center" dir="auto">
  <img src="https://img.shields.io/github/contributors/ivinexo1/FurOS"></img>
  <img src="https://img.shields.io/github/commit-activity/w/ivinexo1/FurOS"></img>
  <img src="https://img.shields.io/github/stars/ivinexo1/FurOS"></img>
</div>

<div align="center" dir="auto">
  <img src="https://github.com/user-attachments/assets/664b6144-a5d5-46b8-9370-30ffa85d64dd"></img>
  
  Open-source OS made from scratch
</div>

<div align="center" dir="auto">
  <img width="640" height="360" alt="image" src="https://github.com/user-attachments/assets/f95a2543-59b2-4ddb-a380-c23e38066a8b" />
</div>

# How to setup!!

This guide contains tutorials for both Linux and Windows 11

## Linux 🐧

1. Clone repository
```bash
git clone "https://github.com/ivinexo1/FurOS/"
```
2. Install QEMU (for Debian : ```apt-get install qemu-system```, Arch: ```pacman -S qemu-full```) or build it yourself
```bash
wget https://download.qemu.org/qemu-10.0.2.tar.xz
tar xvJf qemu-10.0.2.tar.xz
cd qemu-10.0.2
./configure
make
```
3. Install NASM (for Debian: ```apt-get install nasm```, Arch: ```pacman -S nasm```)
5. Install i386-elf-gcc (for Debian: add this later, Arch: ```yay i386-elf-gcc``` or ```paru i368-elf-gcc```)
6. Navigate to the repository and run ```./makeNrun.sh```

## Windows 11 🪟
1. Clone repository
```shell
git clone "https://github.com/ivinexo1/FurOS/"
```
2. Install [QEMU](https://download.qemu.org/)
3. Install NASM
```shell
winget install nasm -i
```
4. Add QEMU as an env path
5. Install WSL ```wsl --install```, check for available distros ```wsl -l -o```
6. Install archlinux (optional, but that's what this tutorial is focusing on) ```wsl --install -d archlinux```
7. Install a text editor (doesn't matter which) ```sudo pacman -S nano```
```
sudo pacman -S --needed git base-devel
```
8. Install yay
```
git clone https://aur.archlinux.org/yay.git
cd yay
```
9. (Just make sure you're not in root but some user)
```
makepkg -si
```
10. Go to ```sudo nano /etc/locale.gen``` and uncomment ```#en_US.UTF-8 UTF-8``` (remove #)
11. ```sudo locale-gen```
12. Before installing i386-elf-gcc make sure you have ENOUGH MEMORY!! It finally worked for me when I set it to 6gb
13. Check available filesize ```df -h```, memory can be changed here ```C:\Users\[your name]\.wslconfig```
```
sudo mount -o remount,size=6G /tmp
```
14. Before you install i386-elf-gcc just know that WSL is going to be using like 90% of your CPU
15. ```yay i386-elf-gcc``` and enter 1 (for the other options just press enter)
16. Once it's done check ```i386-elf-gcc -v```
17. If it's installed navigate to killmeOS (running `./makeNrun.sh` is optional, you can also copy the inside of the file and paste everything except line 15)
```
cd '/mnt/c/Users/[your name]/Documents/GitHub/FurOS/build'
./makeNrun.sh
```
18. Open ```nasmpath.bat``` (can be found here: ```C:\Program Files\NASM```)
```
cd C:\Users\[your name]\Documents\GitHub\killmeOS\build
qemu-system-i386 -drive format=raw,file="OS.bin",index=0,if=floppy -m 128M
```
19. If you've done everthing correctly, You should see QEMU instance running killmeOS!! Also make sure to join our [discord server](http://discord.gg/cTumjQQkUp) >:3
