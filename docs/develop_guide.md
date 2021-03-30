# chino os开发规范
## git commit message规范
feature:新功能<br>
fix:修复bug<br>
docs:文档<br>
refactor:重构<br>
style： 格式(不影响代码运行的变动)<br>

## 函数命名规范
总述<br>
函数命名, 变量命名, 文件命名要有描述性; 少用缩写。

### 通用命名规则
```c
int price_count_reader;    // 无缩写
int num_errors;            // "num" 是一个常见的写法
int num_dns_connections;   // 人人都知道 "DNS" 是什么

int n;                     // 毫无意义(在作为临时变量时，或者在循环中可用)
int nerr;                  // 含糊不清的缩写.
int n_comp_conns;          // 含糊不清的缩写.
int pc_reader;             // "pc" 有太多可能的解释了.
int cstmr_id;              // 删减了若干字母.
```

### 类型命名
类型命名分两种情况：<br>
1.不使用typedef关键字，直接使用struct,union或者enum的类型加两个下划线，如：__hashmap。<br>
2.使用typedef定义别名，则命名规则为每个单词首字母均大写, 不包含下划线，如:HashMap<br>

``` c
struct __hashmap {...};
typedef struct __hashmap HashMap;
```

### 变量命名
变量 (包括函数和函数参数) 和数据成员名一律小写, 单词之间用下划线连接. 类的成员变量以下划线结尾, 但结构体的就不用, 如: a_local_variable, a_struct_data_member, a_class_data_member_。<br>

``` c
int table_name;             // 好 - 用下划线.
void get_user_info(void);   // 好 - 用下划线.

int tableName;  // 差 - 混合大小写
void getUserInfo(void);   // 差 - 混合大小写.

```

### 常量命名
命名时以 “k” 开头, 大小写混合.

``` c
const int kDaysInAWeek = 7;
```

### 宏命名
全大写和单词之间用下划线隔开
``` c
#define GET_USER_INFO(current) ...
```

### 命名规则特例
在和开发组交流并取得同意后方可使用

### 注释编写规范
在对函数进行说明时应使用多行注释，并在第一行写上函数的功能(副作用)，在对函数的参数进行说明时应该@变量名称：变量说明
``` c
/*
 * 获取用户态的进程信息
 * @current:进程的结构
 */
Process *get_user_info(Process *process);
```
