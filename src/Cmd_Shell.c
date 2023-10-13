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
/      |/  \\  /  |/  |  /  | /      \\        /      \\ /  |  /  |/        |/  |      /  |\n\
$$$$$$/ $$  \\ $$ |$$ | /$$/ /$$$$$$  |      /$$$$$$  |$$ |  $$ |$$$$$$$$/ $$ |      $$ |\n\
  $$ |  $$$  \\$$ |$$ |/$$/  $$ \\__$$/       $$ \\__$$/ $$ |__$$ |$$ |__    $$ |      $$ |\n\
  $$ |  $$$$  $$ |$$  $$<   $$      \\       $$      \\ $$    $$ |$$    |   $$ |      $$ |\n\
  $$ |  $$ $$ $$ |$$$$$  \\   $$$$$$  |       $$$$$$  |$$$$$$$$ |$$$$$/    $$ |      $$ |\n\
 _$$ |_ $$ |$$$$ |$$ |$$  \\ /  \\__$$ |      /  \\__$$ |$$ |  $$ |$$ |_____ $$ |_____ $$ |_____\n\
/ $$   |$$ | $$$ |$$ | $$  |$$    $$/       $$    $$/ $$ |  $$ |$$       |$$       |$$       |\n\
$$$$$$/ $$/   $$/ $$/   $$/  $$$$$$/         $$$$$$/  $$/   $$/ $$$$$$$$/ $$$$$$$$/ $$$$$$$$/\n";


Cmd_Str* Cmd_Link_Head	= NULL;
Cmd_Str* Cmd_Link_end = NULL;

void Cmd_Help(int argc, char** argv);

Cmd_Str* Cmd_FoundKeyword(const char *keyword) {

	/***********************
	*	��������Ϸ��Լ��	*
	***********************/

	ASSERT((Cmd_Link_Head == NULL), "Never Create KeyWord");
	ASSERT((*keyword == "\0"), "Keyword is NULL");		//δ����������

	/***************
	*	����ı���	*
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
	*	��������Ϸ��Լ��	*
	***********************/

	ASSERT((*keyword == "\0"), "Keyword is NULL");		//δ����������
	ASSERT(pfuncCmd == NULL, "funcCmd is NULL");		//δָ������ص�����

	/************************
	*	���벢��ʼ���ṹ��	*
	************************/

	Cmd_Str *New_Cmd = malloc(sizeof(Cmd_Str)); //�����ڴ�
	ASSERT(New_Cmd == NULL, "Cmd malloc failed");	//�ڴ�����ʧ��


	memset(New_Cmd, 0, sizeof(Cmd_Str));	//��ʼ���ո�������ڴ�

	New_Cmd->Cmd_Keyword = malloc(strlen(keyword));
	ASSERT(New_Cmd->Cmd_Keyword == NULL, "Cmd malloc failed");	//�ڴ�����ʧ��
	memset(New_Cmd->Cmd_Keyword, 0, strlen(New_Cmd->Cmd_Keyword));	//��ʼ���ո�������ڴ�

	memcpy(New_Cmd->Cmd_Keyword, keyword, strlen(keyword));	//���������������ṹ��
	New_Cmd->pFuncCommand = pfuncCmd;	//������ص�����ָ�븳ֵ���ṹ��

	/***************
	*	���������	*
	***************/

	if (Cmd_Link_Head == NULL) {	//��һ�δ���
		Cmd_Link_Head = New_Cmd;	//��������ͷ
		Cmd_Link_end = New_Cmd;		//��������β
		Shell_KeywordAdd("help", Cmd_Help);
		Shell_HelpArgAdd("help", "����");
	}
	else {
		Cmd_Link_end->next = New_Cmd;	//�����δ����Ľṹ�����ӵ�����β
		Cmd_Link_end = New_Cmd;	//�������ڵ�����β
	}

}


void Shell_FormatAdd(const char* keyword, const char* format) {
	uint8_t Format_Count = 0;
	Cmd_Format_init* Cmd_Format_Now = NULL;

	Cmd_Str* Cmd_Str_Found = NULL;

	/***************
	*	���������	*
	****************/

	Cmd_Str_Found = Cmd_FoundKeyword(keyword);
	if (Cmd_Str_Found == NULL) {
		return;
	}

	Cmd_Str_Found->Cmd_format_arg = malloc(strlen(format));
	ASSERT(Cmd_Str_Found->Cmd_format_arg == NULL, "Cmd malloc failed");	//�ڴ�����ʧ��
	memset(Cmd_Str_Found->Cmd_format_arg, 0, strlen(Cmd_Str_Found->Cmd_format_arg));	//��ʼ���ո�������ڴ�
	memcpy(Cmd_Str_Found->Cmd_format_arg, format, strlen(format));

	if (Cmd_Str_Found->Cmd_Format != NULL) return;

	//�����ؼ���
	for ( ; *format != '\0'; format++) { //�������ַ���β
		
		if (*format == '[') {	// ��⵽�ؼ��� 	
			Cmd_Format_init* Cmd_Format = NULL;	//
			Cmd_Format = malloc(sizeof(Cmd_Format_init));
			if (Cmd_Format == NULL) return;
			memset(Cmd_Format, 0, sizeof(Cmd_Format_init));

			Cmd_Format->Val_Type = 0; //��ʼ���ؼ�������

			format++; //�л�����һ���ַ�
			for (int i=0; *format != ']'; format++,i++) { //ȡ���ؼ���
				if (*format == '\0') {	// [] �����ؼ��ֵķ����� ������
					free(Cmd_Format);	//�ͷ��ڴ�
					return;	//�˳�
				}
				Cmd_Format->Format[i] = *format; //���ؼ��ִ���ṹ��
			}
			if (Cmd_Str_Found->Cmd_Format == NULL) {	//�Ƿ�Ϊ��һ��
				Cmd_Str_Found->Cmd_Format = Cmd_Format;	//��ӹؼ���
				Cmd_Format_Now = Cmd_Format;	//���µ�ǰ�ؼ���ָ��
			}
			else { //���ǵ�һ�δ洢
				Cmd_Format_Now->next = Cmd_Format; //��������Ľṹ�����ӵ��ؼ���β
				Cmd_Format_Now = Cmd_Format;	//���µ�ǰ�ؼ���ָ��
			}

			Format_Count++;		//�ؼ���������1

		}		
	}
	Cmd_Str_Found->Format_Count = Format_Count; //��ֵ�ؼ�������
}

void Shell_HelpArgAdd(const char* keyword, const char* help_arg) {

	Cmd_Str *Cmd_Str_Found = NULL;

	Cmd_Str_Found = Cmd_FoundKeyword(keyword);
	if (Cmd_Str_Found == NULL) {
		return ;
	}


	Cmd_Str_Found->Cmd_help_arg = malloc(strlen(help_arg));
	ASSERT(Cmd_Str_Found->Cmd_help_arg == NULL, "Cmd malloc failed");	//�ڴ�����ʧ��
	memset(Cmd_Str_Found->Cmd_help_arg, 0, strlen(Cmd_Str_Found->Cmd_help_arg));	//��ʼ���ո�������ڴ�

	memcpy(Cmd_Str_Found->Cmd_help_arg, help_arg, strlen(help_arg));
}

int Shell_Keyparse(const char *buf) {
	char *cformat;
	char* argv[10] = {0};

	Cmd_Str *Cmd_keywordFound = NULL;
	Cmd_Format_init* Cmd_Format_now = NULL;

	/***************************
	*	��ȡ������������	*
	****************************/

	cformat = strtok((char *)buf, " ");
	if (cformat == NULL) return CMD_NOT_FOUND;  //������
	
	Cmd_keywordFound = Cmd_FoundKeyword(cformat); 
	if (Cmd_keywordFound == NULL) return CMD_NOT_FOUND; //������
	
	Cmd_Format_now = Cmd_keywordFound->Cmd_Format; 

	/*******************************
	*	��ȡ�������������ؼ���	*
	********************************/

	buf = buf + (strlen(Cmd_keywordFound->Cmd_Keyword) + 1);

	if ((Cmd_keywordFound->Format_Count == 0)) {
		Cmd_keywordFound->pFuncCommand(0, argv);
		return CMD_OK;
	}

	if (*buf == '\0') return CMD_NOT_FOUND;


	for (int formet_count = 0; Cmd_Format_now != NULL; formet_count++) {  //���� ��Ӧ�ؼ��ֵ�ֵ
		cformat = strtok(NULL, " ");
		if (cformat == NULL) return CMD_FUNC_BUT_FORMAT_ERR;
		memcpy((void*)Cmd_Format_now->Val, cformat, sizeof(Cmd_Format_now->Val));

		argv[formet_count] = &Cmd_Format_now->Val[0];

		Cmd_Format_now = Cmd_Format_now->next;

	}

	/******************************
	*	��������������Ļص�����	*
	*******************************/

	cformat = strtok(NULL, " "); //�Ƿ�������
	if (cformat == NULL) { //������ ˵�� ����������ȷ
		Cmd_keywordFound->pFuncCommand(Cmd_keywordFound->Format_Count, argv); //������ص�
	}
	else {
		return CMD_FUNC_BUT_FORMAT_ERR;	//�������ݣ�˵������������� ���� ����ʱ�Ĳ����������ж�ָ����ڵ����ò�������
	}

	return CMD_OK;

}

void Cmd_Help(int argc, char** argv) {

	ASSERT((Cmd_Link_Head == NULL), "Never Create KeyWord");
	
	Cmd_Str* Cmd_Link = Cmd_Link_Head;

	while (Cmd_Link != NULL) {
		printf("%s %s\t%s\r\n", 
					Cmd_Link->Cmd_Keyword, 
					(Cmd_Link->Cmd_format_arg != NULL)?Cmd_Link->Cmd_format_arg: " ", 
					(Cmd_Link->Cmd_help_arg != NULL) ? Cmd_Link->Cmd_help_arg : " ");
		Cmd_Link = Cmd_Link->next;
	}
}