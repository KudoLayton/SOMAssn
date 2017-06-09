/*----------------------------------------------------------*
 *															*
 *					   Jokebox v4.051						*
 *															*
 * 				�̱����� ���� �ۼ��� Jokebox��				*
 *		ũ������Ƽ�� Ŀ���� ������ǥ��-�������Ǻ������		*
 * 		3.0 Unported ���̼����� ���� �̿��� �� �ֽ��ϴ�.	*
 *															*
 *					ORANGEYELLOW, 2013.						*
 *					iss300@postech.ac.kr					*
 *															*
 *----------------------------------------------------------*/

#ifndef FN_TIME
#define FN_TIME
	
typedef struct __declspec(dllexport) Gametime
{
	unsigned long elapsed;
	unsigned long total;
}GAMETIME;

unsigned long long _time_current();

//������ ������Ʈ�κ��� ���� �ð��� ms ������ ����.
unsigned long _time_elapsed();

//���� �������κ��� ���� �ð��� ms ������ ����.
unsigned long _time_total();

void _time_set_start();

//Update�� ������ ������Ʈ �ð� üũ.
void _time_set_update();

#endif
