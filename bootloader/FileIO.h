
//Structure to access Directory Entry in the FAT
typedef struct 
 {
  BYTE name[11];
  BYTE attrib; //file attributes
  BYTE NTreserved; //always 0
  BYTE timeTenth; //tenths of seconds, set to 0 here
  WORD createTime; //time file was created
  WORD createDate; //date file was created
  WORD lastAccessDate;
  WORD firstClusterHI; //higher word of the first cluster number
  WORD writeTime; //time of last write
  WORD writeDate; //date of last write
  WORD firstClusterLO; //lower word of the first cluster number
  DWORD fileSize; //size of file in bytes
 } DIR_TypeDef;

typedef struct
 {
   BYTE  		open;
   DWORD 		seek;
   WORD  		cur_claster;
   DIR_TypeDef *dir;

 } FILE_HANDEL_TypeDef;


#define START_DATA_AREA	 	0x0800 //начальный адрес области данных
#define START_FAT_AREA	 	0x0200 //начальный адрес FAT
#define START_MAINROOT_AREA 0x0600 //начальный адрес корневой директории
#define SIZE_CLASTER		512    //размер кластера в байтах


extern  BYTE Memory[];

BYTE fat_OPEN(FILE_HANDEL_TypeDef *FH, BYTE numfile);
void fat_CLOSE(FILE_HANDEL_TypeDef *FH);
BYTE fat_WRITE(FILE_HANDEL_TypeDef *FH, void *DATA, DWORD LEN);
