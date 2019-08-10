#include "preprocessor.h"
#include "kmaps.h"
#include "renderer.h"

#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define     SIF(str1, str2)   if (!strcmp(str1, str2))
#define     EIF(str1, str2)   else if (!strcmp(str1, str2))

int deadloopCheck (kbindList *kbindTable)
{
    int val = 0;

    /* 建立链表枚举框架 */
    kbindList *list = kbindTable->next;
    /* 迭代查找时保存结果 */
    kbindList *searched;
    /* 在迭代过程中生成的回溯追踪信息，通过对成员的逐个访问，可追溯到整个迭代过程 */
    int backtrace[1024];

    /* 进入链表搜索 */
    while (list->next != NULL)
    {
        /* 忽略 monitor 类型的节点 */
        if (list->operation == OPERAT_MONITOR)
        {
            list = list->next;
            continue;
        }

        /* 初始化迭代参数 */
        searched = list;
        backtrace[0] = list->trigger;
        backtrace[1] = list->target;
        /* 迭代查找 */
        for (int index = 2; (searched = searchKBindList (kbindTable, searched->target)) != NULL; index++)
        {
            backtrace[index] = searched->target;

            if (searched->operation == OPERAT_MONITOR)
                break;
            
            /**
             * 该算法的依据如下。我们逐个访问链表里的每个节点，每个节点都作为一个入口。
             * 接下来程序保存该入口指针，进入迭代查找循环。在该循环中，查找入口指针
             * 对应的 target，再将 target 作为新的入口指针。如果不存在死循环，入口
             * 指针最终一定会变成 null。如果存在死循环，入口指针在某一时刻一定会等于
             * 最开始的入口指针。
             */
            if (searched == list)
            {
                /* 输出死循环警告并回溯迭代过程，打印出整个死循环流程 */
                dri (DCR_WARNING, "May be Dead-Loop (");
                for (int i = 0; i != index; i++)
                {
                    if (i)
                        fprintf (stderr, " -> ");
                    fprintf (stderr, "%s", desearchKMap(backtrace[i]));
                }

                fprintf (stderr, ")\n");
                val = 1;
                break;
            }
        }

        list = list->next;
    }

    return val;
}

void parseConfig (char const *file, kbindList *kbindTable, optionTable *opts)
{
    char linebuf[4096];
    char tmp;
    int fd = open(file, O_RDONLY);
    if (fd < 0)
    {
        dri(DCR_ERROR, "Could open config file (%s)\n", strerror(errno));
        exit(1);
    }

    strcpy (opts->device, "none\0");

    char **tokens = malloc (sizeof(char *) * 4);
    for (int i = 0; i != 4; i++)
        tokens[i] = malloc (1024);

    while (1)
    {
        bzero (linebuf, sizeof(linebuf));
        for (int i = 0; i != sizeof(linebuf); i++)
        {
            if (read (fd, &tmp, 1) == 0)
                break;
            if (tmp == '\n')
                break;
            if (tmp == '#')
            {
                while (read(fd, &tmp, 1) == 1)
                    if (tmp == '\n')
                        break;
                break;
            }
            linebuf[i] = tmp;
        }

        if (linebuf[0] == '\0')
            continue;

        for (int i = 0, k = 0, p = 0; i != 4 && k != sizeof(linebuf) && linebuf[k] != '\0'; i++, p = 0)
        {
            while (linebuf[k] == ' ' || linebuf[k] == '\t')
                k++;
            while (linebuf[k] != ' ' && linebuf[k] != '\t' && linebuf[k] != '\0')
            {
                *(tokens[i] + p) = linebuf[k];
                k++;
                p++;
            }
        }

        SIF(tokens[0], "end")
        {
            break;
        }
        EIF(tokens[0], "option")
        {
            SIF(tokens[1], "runas")
            {
                SIF(tokens[2], "application")
                {
                    opts->runas = RUNAS_APPLICATION;
                }
                EIF(tokens[2], "daemon")
                {
                    opts->runas = RUNAS_DAEMON;
                }
                else
                {
                    dri (DCR_WARNING, "invalid configuration: option runas \033[31;1m%s\033[0m\n", tokens[2]);
                }
            }
            EIF(tokens[1], "quiet")
            {
                SIF(tokens[2], "true")
                {
                    opts->quiet = true;
                }
                EIF(tokens[2], "false")
                {
                    opts->quiet = false;
                }
                else
                {
                    dri (DCR_WARNING, "invalid configuration: option quiet \033[31;1m%s\033[0m\n", tokens[2]);
                }
            }
            EIF(tokens[1], "continuous-trigger")
            {
                SIF(tokens[2], "true")
                {
                    opts->continuousTrigger = true;
                }
                EIF(tokens[2], "false")
                {
                    opts->continuousTrigger = false;
                }
                else
                {
                    dri (DCR_WARNING, "invalid configuration: option continuous-trigger \033[31;1m%s\033[0m\n", tokens[2]);
                }
            }
            else
            {
                dri (DCR_WARNING, "invalid configuration: option \033[31;1m%s\033[0m\n", tokens[1]);
            }
        }
        EIF(tokens[0], "bind")
        {
            int code1 = searchKMap(tokens[1]);
            int code2 = searchKMap(tokens[3]);
            if (code1 == -1)
            {
                dri (DCR_WARNING, "invalid configuration: key \033[31;1m[%s]\033[0m\n", tokens[1]);
            }
            else if (code2 == -1)
            {
                dri (DCR_WARNING, "invalid configuration: key \033[31;1m[%s]\033[0m\n", tokens[3]);
            }
            else
            {
                SIF(tokens[2], "as")
                {
                    insertKBindList(kbindTable, code1, OPERAT_BIND, code2);
                }
                EIF(tokens[2], "to")
                {
                    insertKBindList(kbindTable, code2, OPERAT_BIND, code1);
                }
                else
                {
                    dri (DCR_WARNING, "invalid configuration: bind \033[31;1m%s\033[0m\n", tokens[2]);
                }
            }
        }
        EIF(tokens[0], "monitor")
        {
            if (opts->monitorFile == NULL)
            {
                dri (DCR_WARNING, "configuration: Need `option montior-file ...\'\n");
                dri (DCR_WARNING, "Use default: options.monitor-file = @stdout\n");
                opts->monitorFile = stdout;
            }

            int code = searchKMap(tokens[1]);
            if (code == -1)
            {
                dri (DCR_WARNING, "invalid configuration: key \033[31;1m[%s]\033[0m\n", tokens[1]);
            }
            else
            {
                insertKBindList(kbindTable, code, OPERAT_MONITOR, 0);
            }
        }
        EIF(tokens[0], "use")
        {
            SIF(tokens[1], "monitor-file")
            {
                SIF(tokens[2], "@stdout")
                {
                    opts->monitorFile = stdout;
                }
                EIF(tokens[2], "@stderr")
                {
                    opts->monitorFile = stderr;
                }
                else
                {
                    opts->monitorFile = fopen (tokens[2], "a");
                    if (opts->monitorFile == NULL)
                        dri (DCR_WARNING, "invalid configuration: monitor-file \033[31;1m%s\033[0m\n", tokens[2]);
                }
                dri (DCR_INFO, "Using monitor bridge: fd = %d\n", opts->monitorFile->_fileno);
            }
            EIF(tokens[1], "device")
            {
                dri (DCR_INFO, "Using HID-Device %s\n", tokens[2]);
                bzero (opts->device, 1024);
                strncpy (opts->device, tokens[2], 1024);
            }
            else
            {
                dri (DCR_WARNING, "invalid configuration: use \033[31;1m%s\033[0m\n", tokens[2]);
            }
        }
        else
        {
            dri (DCR_WARNING, "invalid configuration: \033[31;1m%s\033[0m\n", tokens[0]);
        }
        
        // printf ("%s, %s, %s, %s\n", tokens[0], tokens[1], tokens[2], tokens[3]);
        bzero (tokens[0], 1024);
        bzero (tokens[1], 1024);
        bzero (tokens[2], 1024);
        bzero (tokens[3], 1024);
    }

    for (int i = 0; i != 4; i++)
        free(tokens[i]);
    free(tokens);
    close (fd);
}