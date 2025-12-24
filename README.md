<div align=center>
    <img src ="Icon.png" width=50%>
    <h1>SMBHelper</h1>
    <p>Simple CLI tool made by Jexxy</p>
</div>

===================

**SMBHelper** is Free and open source CLI tool which helps generate Samba configurations.

# Usage

Just run the file and follow the program's instructions
(You might need it to run as SUDO to edit smb.conf)

# Manual installation

just run this command (Linux):

```bash
g++ main.cpp -static -o smbhelper
```

or if you're on Windows (Icon is optional):

```bash
windres icon.rc icon.o
```

```bash
g++ main.cpp icon.o -static -o smbhelper64.exe
```

# Features

Automatically generates config ready to copy-paste
Writes the changes into smb.conf (Only if the user agrees)
Automatic backups when writing into smb.conf
Preview the configuration before applying
Written in C++

===================
