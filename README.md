# Create - A simple and fast CLI task manager

* Create is a hobby project to help me and my friends with university deadlines

## 🛠️ Pre-requisites

To compilate and install **Create**, make shure you have this tools installed in your UNIX/LINUX system:

* **C++ Compiler (g++):** Compatible with C++17 or superior.
* **Make:** tool to get compiled with `Makefile`.
* **Git:** to clone repository.

## 📦 Instalation

### 1: Clone the repository

```terminal
git clone https://github.com/ViniScussel/Create
cd Create
```
### 2: Build
```terminal
make
```
### 3: Gloal Build (Recomended)
```terminal
sudo make install
```

## 💡 How to Use

Create was made to be simple and fast. To create a task you only have to use this command
```terminal
create -c {name} {dd/mm/aaaa} {h:min} {description}
```
To list all the tasks created, just use
```terminal
create -la
```
or, if you just want to see the tasks that are incomplete, use
```terminal
create -l
```
To erase a task, you only have to
```terminal
create -rm {number_of_the_task}
```
where the {number_of_the_task} can be found in -la parameter.
DO NOT CONFUNDE WITH -l PARAMETER.
to conclude a task is simple
```terminal
create -cl {number_of_the_task}
```
and if you want to remove all concluded and old tasks, use
```terminal
create -autoremove
```
