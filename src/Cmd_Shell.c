#include "Cmd_Shell.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#ifdef _DEBUG
#define ASSERT(Condition,arg)      \
 if(Condition) {\
     printf("[%s:%d]:%s\r\n", __FILE__, __LINE__, arg);\
     assert(0);\
 }
#else
#define ASSERT(Condition,arg)     \

#endif

const char *logo = \
" ______  __    __  __    __   ______          ______   __    __  ________  __        __\r\n\
/      |/  \\  /  |/  |  /  | /      \\        /      \\ /  |  /  |/        |/  |      /  |\r\n\
$$$$$$/ $$  \\ $$ |$$ | /$$/ /$$$$$$  |      /$$$$$$  |$$ |  $$ |$$$$$$$$/ $$ |      $$ |\r\n\
  $$ |  $$$  \\$$ |$$ |/$$/  $$ \\__$$/       $$ \\__$$/ $$ |__$$ |$$ |__    $$ |      $$ |\r\n\
  $$ |  $$$$  $$ |$$  $$<   $$      \\       $$      \\ $$    $$ |$$    |   $$ |      $$ |\r\n\
  $$ |  $$ $$ $$ |$$$$$  \\   $$$$$$  |       $$$$$$  |$$$$$$$$ |$$$$$/    $$ |      $$ |\r\n\
 _$$ |_ $$ |$$$$ |$$ |$$  \\ /  \\__$$ |      /  \\__$$ |$$ |  $$ |$$ |_____ $$ |_____ $$ |_____\r\n\
/ $$   |$$ | $$$ |$$ | $$  |$$    $$/       $$    $$/ $$ |  $$ |$$       |$$       |$$       |\r\n\
$$$$$$/ $$/   $$/ $$/   $$/  $$$$$$/         $$$$$$/  $$/   $$/ $$$$$$$$/ $$$$$$$$/ $$$$$$$$/\r\n";


Cmd_Str* Cmd_Link_Head	= NULL;
Cmd_Str* Cmd_Link_end = NULL;

void Cmd_Help(int argc, char** argv);

Cmd_Str* Cmd_FoundKeyword(const char *keyword) {

	/***********************
	*	传入参数合法性检测	*
	***********************/

	ASSERT((Cmd_Link_Head == NULL), "Never Create KeyWord");
	ASSERT((*keyword == "\0"), "Keyword is NULL");		//未输入命令名

	/***************
	*	链表的遍历	*
	****************/

	Cmd_Str* Cmd_KeywordFound = Cmd_Link_Head;
	while (Cmd_KeywordFound != NULL) {
		if (strcmp(Cmd_KeywordFound->Cmd_Keyword, keyword) == 0) {
			return Cmd_KeywordFound;
		}
		Cmd_KeywordFound = Cmd_KeywordFound->next;
	}

	return (Cmd_Str *)NULL;
}



void Shell_KeywordAdd(const char *keyword, pfunc pfuncCmd ) 
{

	/***********************
	*	传入参数合法性检测	*
	***********************/

	ASSERT((*keyword == "\0"), "Keyword is NULL");		//未输入命令名
	ASSERT(pfuncCmd == NULL, "funcCmd is NULL");		//未指定命令回调函数

	/************************
	*	申请并初始化结构体	*
	************************/

	Cmd_Str *New_Cmd = malloc(sizeof(Cmd_Str)); //申请内存
	ASSERT(New_Cmd == NULL, "Cmd malloc failed");	//内存申请失败


	memset(New_Cmd, 0, sizeof(Cmd_Str)-1);	//初始化刚刚申请的内存

	New_Cmd->Cmd_Keyword = malloc(strlen(keyword));
	ASSERT(New_Cmd->Cmd_Keyword == NULL, "Cmd malloc failed");	//内存申请失败
	memset(New_Cmd->Cmd_Keyword, 0, strlen(New_Cmd->Cmd_Keyword));	//初始化刚刚申请的内存

	memcpy(New_Cmd->Cmd_Keyword, keyword, strlen(keyword));	//将命令名拷贝到结构体
	New_Cmd->pFuncCommand = pfuncCmd;	//将命令回调函数指针赋值到结构体

	/***************
	*	链表的链接	*
	***************/

	if (Cmd_Link_Head == NULL) {	//第一次创建
		Cmd_Link_Head = New_Cmd;	//更新链表头
		Cmd_Link_end = New_Cmd;		//更新链表尾
		Shell_KeywordAdd("help", Cmd_Help);
		Shell_HelpArgAdd("help", "CMD_LIST");
	}
	else {
		Cmd_Link_end->next = New_Cmd;	//将本次创建的结构体连接到链表尾
		Cmd_Link_end = New_Cmd;	//更新现在的链表尾
	}

}


void Shell_FormatAdd(const char* keyword, const char* format) {
	uint8_t Format_Count = 0;
	Cmd_Format_init* Cmd_Format_Now = NULL;

	Cmd_Str* Cmd_Str_Found = NULL;

	/***************
	*	遍历命令表	*
	****************/

	Cmd_Str_Found = Cmd_FoundKeyword(keyword);
	if (Cmd_Str_Found == NULL) {
		return;
	}

	Cmd_Str_Found->Cmd_format_arg = malloc(strlen(format));
	ASSERT(Cmd_Str_Found->Cmd_format_arg == NULL, "Cmd malloc failed");	//内存申请失败

	memset(Cmd_Str_Found->Cmd_format_arg, 0, strlen(Cmd_Str_Found->Cmd_format_arg));	//初始化刚刚申请的内存
	memcpy(Cmd_Str_Found->Cmd_format_arg, format, strlen(format));

	if (Cmd_Str_Found->Cmd_Format != NULL) return;

	//解析关键字
	for ( ; *format != '\0'; format++) { //遍历到字符串尾
		
		if (*format == '[') {	// 检测到关键字
			Cmd_Format_init* Cmd_Format = NULL;	//
			Cmd_Format = malloc(sizeof(Cmd_Format_init));
			if (Cmd_Format == NULL) return;
			memset(Cmd_Format, 0, sizeof(Cmd_Format_init));

			Cmd_Format->Val_Type = 0; //初始化关键字类型

			format++; //切换到下一个字符
			for (int i=0; *format != ']'; format++,i++) { //取出关键字
				if (*format == '\0') {	// [] 包含关键字的方括号 不完整
					free(Cmd_Format);	//释放内存
					return;	//退出
				}
				Cmd_Format->Format[i] = *format; //将关键字存入结构体
			}
			if (Cmd_Str_Found->Cmd_Format == NULL) {	//是否为第一次
				Cmd_Str_Found->Cmd_Format = Cmd_Format;	//添加关键字
				Cmd_Format_Now = Cmd_Format;	//更新当前关键字指针
			}
			else { //不是第一次存储
				Cmd_Format_Now->next = Cmd_Format; //将新申请的结构体链接到关键字尾
				Cmd_Format_Now = Cmd_Format;	//更新当前关键字指针
			}

			Format_Count++;		//关键字数量加1

		}		
	}
	Cmd_Str_Found->Format_Count = Format_Count; //赋值关键字数量
}

void Shell_HelpArgAdd(const char* keyword, const char* help_arg) {

	Cmd_Str *Cmd_Str_Found = NULL;

	Cmd_Str_Found = Cmd_FoundKeyword(keyword);
	if (Cmd_Str_Found == NULL) {
		return ;
	}


	Cmd_Str_Found->Cmd_help_arg = malloc(strlen(help_arg));
	ASSERT(Cmd_Str_Found->Cmd_help_arg == NULL, "Cmd malloc failed");	//内存申请失败

	memset(Cmd_Str_Found->Cmd_help_arg, 0, strlen(Cmd_Str_Found->Cmd_help_arg));	//初始化刚刚申请的内存
	memcpy(Cmd_Str_Found->Cmd_help_arg, help_arg, strlen(help_arg));
}

int Shell_Keyparse(const char *buf) {
	char *cformat;
	char* argv[10] = {0};

	Cmd_Str *Cmd_keywordFound = NULL;
	Cmd_Format_init* Cmd_Format_now = NULL;

	/***************************
	*	提取命令并遍历命令表	*
	****************************/
	
	cformat = strtok((char *)buf, " ");
	if (cformat == NULL) return CMD_NOT_FOUND;  //无命令
	
	Cmd_keywordFound = Cmd_FoundKeyword(cformat); 
	if (Cmd_keywordFound == NULL) return CMD_NOT_FOUND; //无命令
	
	Cmd_Format_now = Cmd_keywordFound->Cmd_Format; 

	/*******************************
	*	提取输入参数并存入关键字	*
	********************************/

	buf = buf + (strlen(Cmd_keywordFound->Cmd_Keyword) + 1);

	if ((Cmd_keywordFound->Format_Count == 0)) {
		Cmd_keywordFound->pFuncCommand(0, argv);
		return CMD_OK;
	}

	if (*buf == '\0') return CMD_NOT_FOUND;


	for (int formet_count = 0; Cmd_Format_now != NULL; formet_count++) {  //填入 对应关键字的值
		cformat = strtok(NULL, " ");
		if (cformat == NULL) return CMD_FUNC_BUT_FORMAT_ERR;
		memcpy((void*)Cmd_Format_now->Val, cformat, sizeof(Cmd_Format_now->Val));

		argv[formet_count] = &Cmd_Format_now->Val[0];

		Cmd_Format_now = Cmd_Format_now->next;

	}

	/******************************
	*	将参数传给命令的回调函数	*
	*******************************/

	cformat = strtok(NULL, " "); //是否还有数据
	if (cformat == NULL) { //无数据 说明 参数输入正确
		Cmd_keywordFound->pFuncCommand(Cmd_keywordFound->Format_Count, argv); //命令函数回调
	}
	else {
		return CMD_FUNC_BUT_FORMAT_ERR;	//还有数据，说明输入参数数量 大于 设置时的参数数量，判定指令存在但配置参数错误
	}

	return CMD_OK;

}

void Cmd_Help(int argc, char** argv) {

	ASSERT((Cmd_Link_Head == NULL), "Never Create KeyWord");
	
	Cmd_Str* Cmd_Link = Cmd_Link_Head;
	printf("=====HELP_LIST=====\r\n");
	while (Cmd_Link != NULL) {
		printf("%s %s\t%s\r\n", 
					Cmd_Link->Cmd_Keyword, 
					(Cmd_Link->Cmd_format_arg != NULL)?Cmd_Link->Cmd_format_arg: "", 
					(Cmd_Link->Cmd_help_arg != NULL) ? Cmd_Link->Cmd_help_arg : "");
		Cmd_Link = Cmd_Link->next;
	}
	printf("========END========\r\n");
}
