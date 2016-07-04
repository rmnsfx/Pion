

typedef struct
{
 char StringName_1[15];
 //char StringName_2[20];
	unsigned char Number;
} TNameElement;


s8 rod_CreateFile(void);
s8 rod_GET_NameElement(TNameElement * Element, unsigned char num);
s8 rod_INIT(void);

extern TNameElement NEl;

