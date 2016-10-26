

typedef struct
{
 char StringName_1[15];
 //char StringName_2[20];
	unsigned char Number;
} TNameElement;


s8 rod_CreateFile(void);
s8 rod_GET_NameElement(TNameElement * Element, unsigned char num);
s8 rod_GET_NameElement_NEXT(TNameElement * Element, unsigned char num);
s8 rod_INIT(void);
s8 rod_create_files ( void );
//void AddPointToRoad ( void );
void rod_DEINIT ( void );
extern TNameElement NEl;
extern long rod_CreateFile_edit (void);

