/*!
  \file    shell.c
  \ingroup shell
  \author  Sota <jine0125@gmail.com>
  \date    2010/08/07

  \brief   デバッグ用シェル
 */

#include "install_option.h"
#ifdef __INSTALL_SHELL__

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "function_list.h"
#include "shell.h"
#include "timer_utility.h"
#include "util.h"
#include "odometry.h"

/* #if defined(compiled_by_h8300_elf) */
/* extern char rs_getc(void); */
/* extern short rs_gets(char *s, short len); */
/* #endif */
short r_gets(char *s, short len);

static void shell_exit(void);
static void shell_ls(void);
static void shell_echo(void);

static char _buffer[MAX_STR_LEN] = {};    //!< 入力用バッファ
static char _argc = 0;                   //!< 引数の数
static char *_argv[MAX_ARG_NUM] = {};    //!< 各引数
static int cmd_num = 0;                  //!< 現在登録されているコマンド数
static int max_cmd_len = 0;              //!< 現時点での最大コマンド長さ(ls用)

static struct{
    const char *name;
    void (*fp)(void);
} command_list[MAX_CMD_NUM]={};

//! シェルの初期化
/*!
  初期設定コマンドを追加する
 */
void init_shell(void)
{
    add_command("exit", shell_exit);
    add_command("ls", shell_ls);
    add_command("echo", shell_echo);
}

//! シェルにコマンドを追加する
/*!
  \return 0 成功
  \return 1 失敗
 */
int add_command(const char *name, void (*fp)(void))
{
    int n;
    if (cmd_num < MAX_CMD_NUM){
        command_list[cmd_num].fp = fp;
        command_list[cmd_num].name = name;
        cmd_num++;
        n = strlen(name);
        max_cmd_len = max_cmd_len > n ? max_cmd_len : n;
        return 0;
    }
    else{
        printf("#error: cannot add command '%s'\n",name);
        return 1;
    }
}

//! コマンド取得
/*!
  入力文字列を引数に分解する
  (例: "echo abc  123" -> "echo","abc","123")
  \param buffer 入力文字列
  \param argc   出力用ポインタ(実行後引数の個数)
  \param argv   出力用ポインタ(実行後各引数)
  \param len 入力文字列の長さ
  \param max_argc   argvの要素数
 */
void get_command(char buffer[], char *argc, char *argv[], int len, int argv_size)
{
    int i,h;

    if (argv!=NULL){
        for(i=0;i<argv_size;i++){
            argv[i] = buffer+len-1;
        }
    }

    for (i=h=0; h<argv_size && i<len; h++){
        while(i<len && isspace(buffer[i++]));
        if (argv!=NULL){
            argv[h] = buffer+i-1;
        }
        if (i>=len) break;
        while(i<len && !isspace(buffer[i++]));
        buffer[i-1] = '\0';
    }
    if (argc!=NULL){
        *argc = h;
    }
}

//! 引数の個数を取得
char get_argc(void)
{
    return _argc;
}

//! 引数へのポインタ配列を取得
char **get_argv(void)
{
    return _argv;
}

//! シェルを実行する
void execute_shell(void)
{
    int i;

    if (is_interrupting()!=0){
        printf("#error: cannot execute shell in interrupt\n");
        return;
    }

    do {
        printf("#robolib:~$ ");

        // 文字列取得
/* #if defined(compiled_by_arm_eabi) */
        get_command(_buffer, &_argc, _argv, r_gets(_buffer, MAX_STR_LEN-1), MAX_ARG_NUM);
/* #elif defined(compiled_by_h8300_elf) */
/*         get_command(_buffer, &_argc, _argv, rs_gets(_buffer, MAX_STR_LEN-1), MAX_ARG_NUM); */
/* #endif */

        printf("\r");

        if (_argc){
            for (i=0;i<cmd_num;i++){
                if (!strcmp(_argv[0], command_list[i].name)){
                    (command_list[i].fp)();
                    goto finish;
                }
            }
            printf("#%s: command not found\n", _argv[0]);
            printf("#Type 'ls' to search command\n");
        }
        finish:
        wait_cycles(1);
    }while(strcmp(_argv[0], "exit"));
}

//! exit exit用ダミー関数
static void shell_exit(void)
{
}

//! ls コマンドリストを表示
static void shell_ls(void)
{
    int i,n,col;

    col = LS_COL/(max_cmd_len+1);

    printf("#");
    for (i=0;i<cmd_num-1;i++){
        n = max_cmd_len+1 - printf(command_list[i].name);
        while(n--){
            printf(" ");
        }
        if (i%col==col-1){
            printf("\n#");
        }
    }

    printf("%s\n",command_list[cmd_num-1].name);
}


//! echo
static void shell_echo(void)
{
    int i;

    printf("#");
    for(i=1;i<_argc-1;i++){
        printf("%s ",_argv[i]);
    }
    printf("%s\n",_argv[_argc-1]);
}

//! 文字列受信
short r_gets(char *s, short len)
{
    short i = 0;
    char c;

    while (1) {
        c = r_getc();
        if ((c == '\n') || (c == '\r')) {
            s[i] = '\0'; if (i != 0) i++;
            r_putc('\n');
            break;
        } else if ((i > 0) && (c == '\b')) {
            r_puts("\b \b");
            i--;
        } else if ((i < len) && (i >= 0) && (c >= ' ') && (c <= '~')) {
            s[i++] = (char)c;
            r_putc(c);
        } else r_putc('\a');
    }
    return i;
}

#endif
