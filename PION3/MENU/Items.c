#include <Items.h>
//                                                                                       | O_LEFT  | O_SZ   | O_HR   | O_RW                                                           //����������
const TItem Items[MAX_ITEMS] = {
//� ������     ����� � ������ 0         ����� � ������ 1    ����.����������   ��.�����  ������                                ���/��������.�������  �����   ���.������/����� ��//
//                                                                                         �����    ����    ��� ���  ������                                                    //
                                                                                                                                                                               //
     0   ,       " ���������"        ,         ""          ,       ""       ,    ""    ,0                                    ,  0xfe   ,    0    ,  0xFF   ,        1         ,//  0
     1   ,"���������"                ,         ""          ,""              ,""        ,0          | O_SZ            | O_RW  ,   128   ,    1    ,  0xff   ,        16        ,//  1
     1   ,"��������"                 ,         ""          ,""              ,""        ,0          | O_SZ            | O_RW  ,   129   ,    1    ,  0xff   ,        16        ,//  2
     1   ,"�����������"              ,         ""          ,""              ,""        ,0          | O_SZ            | O_RW  ,   130   ,    1    ,  0xff   ,        16        ,//  3
     1   ,"������� �������"          ,         ""          ,""              ,""        ,0          | O_SZ            | O_RW  ,   131   ,    1    ,  0xff   ,        16        ,//  4
     0   ,   " ��������� ��������"   ,         ""          ,       ""       ,    ""    ,0                                    ,  0xfe   ,    0    ,  0x87   ,        2         ,//  5
     2   ," 5-1000  ��"              ,     "5-1000 ��"     ,""              ,""        ,0          | O_SZ            | O_RW  ,   128   ,    1    ,  0x81   ,        17        ,//  6  �������� ��� ������ ��������������
     2   ,"10-1000  ��"              ,    "10-1000 ��"     ,""              ,""        ,0          | O_SZ            | O_RW  ,   129   ,    1    ,  0x81   ,        17        ,//  7  �������� ��� ������ ��������������
     2   ,"10-2000  ��"              ,    "10-2000 ��"     ,""              ,""        ,0          | O_SZ            | O_RW  ,   130   ,    1    ,  0x81   ,        17        ,//  8  �������� ��� ������ ��������������
     2   ,"10-5000  ��"              ,    "10-5000 ��"     ,""              ,""        ,0          | O_SZ            | O_RW  ,   131   ,    1    ,  0x81   ,        17        ,// 9  �������� ��� ������ ��������������
//   2   ,"10-10000 ��"              ,    "10-10000 ��"    ,""              ,""        ,0          | O_SZ            | O_RW  ,   132   ,    1    ,  0x81   ,        17        ,// 10  �������� ��� ������ ��������������
     2   ," 5-1000  ��"              ,     "5-1000 ��"     ,""              ,""        ,0          | O_SZ            | O_RW  ,   128   ,    1    ,  0x82   ,        18        ,// 10  �������� ��� ������ �������������
     2   ,"10-1000  ��"              ,    "10-1000 ��"     ,""              ,""        ,0          | O_SZ            | O_RW  ,   129   ,    1    ,  0x82   ,        18        ,// 11  �������� ��� ������ �������������
     2   ,"10-2000  ��"              ,    "10-2000 ��"     ,""              ,""        ,0          | O_SZ            | O_RW  ,   130   ,    1    ,  0x82   ,        18        ,// 12  �������� ��� ������ �������������
     2   ,"10-5000  ��"              ,    "10-5000 ��"     ,""              ,""        ,0          | O_SZ            | O_RW  ,   131   ,    1    ,  0x82   ,        18        ,// 13  �������� ��� ������ �������������
     2   ,"10-300  ��"               ,     "10-300 ��"     ,""              ,""        ,0          | O_SZ            | O_RW  ,   128   ,    1    ,  0x84   ,        19        ,// 14  �������� ��� ������ ����������������
     0   ,     " �����/��������"     ,         ""          ,       ""       ,    ""    ,0                                    ,  0xfe   ,    0    ,  0x87   ,        3         ,// 15
     3   ,"���"                      ,        "���"        ,""              ,""        ,0          | O_SZ            | O_RW  ,   128   ,    1    ,  0x81   ,        20        ,// 16  �������� ��� ������ ��������������
     3   ,"���"                      ,        "���"        ,""              ,""        ,0          | O_SZ            | O_RW  ,   129   ,    1    ,  0x81   ,        20        ,// 17  �������� ��� ������ ��������������
     3   ,"���-���"                  ,      "���-���"      ,""              ,""        ,0          | O_SZ            | O_RW  ,   130   ,    1    ,  0x81   ,        20        ,// 18  �������� ��� ������ ��������������
     3   ,"���-������"               ,    "���-������"     ,""              ,""        ,0          | O_SZ            | O_RW  ,   131   ,    1    ,  0x81   ,        20        ,// 19  �������� ��� ������ ��������������
//   3   ,"������"                   ,      "������"       ,""              ,""        ,0          | O_SZ            | O_RW  ,   132   ,    1    ,  0x81   ,        20        ,// 20  �������� ��� ������ ��������������
     3   ,"���"                      ,        "���"        ,""              ,""        ,0          | O_SZ            | O_RW  ,   128   ,    1    ,  0x82   ,        21        ,// 20  �������� ��� ������ �������������
     3   ,"���"                      ,        "���"        ,""              ,""        ,0          | O_SZ            | O_RW  ,   129   ,    1    ,  0x82   ,        21        ,// 21  �������� ��� ������ �������������
     3   ,"���-���"                  ,      "���-���"      ,""              ,""        ,0          | O_SZ            | O_RW  ,   130   ,    1    ,  0x82   ,        21        ,// 22  �������� ��� ������ �������������
     3   ,"���-������"               ,    "���-������"     ,""              ,""        ,0          | O_SZ            | O_RW  ,   131   ,    1    ,  0x82   ,        21        ,// 23  �������� ��� ������ �������������
//   3   ,"������"                   ,      "������"       ,""              ,""        ,0          | O_SZ            | O_RW  ,   132   ,    1    ,  0x82   ,        21        ,// 24  �������� ��� ������ �������������
     3   ,"���"                      ,        "���"        ,""              ,""        ,0          | O_SZ            | O_RW  ,   129   ,    1    ,  0x84   ,        22        ,// 24  �������� ��� ������ ����������������
     3   ,"���-���"                  ,      "���-���"      ,""              ,""        ,0          | O_SZ            | O_RW  ,   130   ,    1    ,  0x84   ,        22        ,// 25  �������� ��� ������ ����������������
     0   ,         " ������"         ,         ""          ,       ""       ,    ""    ,0                                    ,  0xfe   ,    0    ,  0xff   ,        4         ,// 26
     4   ,"������� �����"            ,      "���:���"      ,"�����"         ,""        ,0                            | O_RW  ,   18    ,    1    ,  0xff   ,        2         ,// 27
     4   ,"����������� ��������"		 ,"�����, ���. � 30 ����","�����������"		,""        ,0                            | O_RW  ,   28     ,    1    ,  0xff   ,        32        ,// 		 
		 4   ,"������� ����"             ,      "���:���"      ,"����"          ,""        ,0                            | O_RW  ,   19    ,    1    ,  0xff   ,        1         ,// 28
     4   ,"������� ���"              ,         ""          ,"���"           ,""        ,0                            | O_RW  ,    0    ,    1    ,  0xff   ,        0         ,// 29     
     4   ,"����� �������"            ,  "DV-2"   					 ,"����� DV"    ,""          ,0                      						   ,    0    ,    1    ,  0x7F   ,        23        ,// 32		 
     4   ,"������ ������������"      ,    "�����������"    ,"������ ��"     ,""        ,0                                    ,    2    ,    1    ,  0xff   ,        13        ,// 33
     4   ,"�������������"            ,         ""          ,""              ,""        ,0                            | O_RW  ,   10    ,    1    ,  0xff   ,        1         ,// 34
     4   ,"���������"                ,   "�������, �."     ,"���������"     ,""        ,0                                    ,    0    ,    1    ,  0xff   ,        14        ,// 36
		 4   ,"���������� �������"       ,         ""          ,""              ,""        ,0                            | O_RW  ,   0xfe  ,    1    ,  0xC7   ,        0xfd        ,// 		 
		 4   ,"��������������"  				 ,   "����� 10 ���." 	 ,"��������������",""        ,0                            | O_RW  ,    26   ,    0    ,  0xff   ,        30        ,		 
     0   ," ����� ��������"          ,         ""          ,       ""       ,    ""    ,0                                    ,  0xfe   ,    0    ,  0x78   ,        5         ,// 37
		 5   ,"�������"         				 ,         ""          ,       ""       ,    ""    ,0                                    ,  130    ,    0    ,  0xff   ,        0xff      ,// 38
		 5   ,"������� ��������"         ,         ""          ,       ""       ,    ""    ,0                                    ,  18     ,    2    ,  0xff   ,        0xfe      ,// 39		
		 0   ,         " ���������"      ,         ""          ,       ""       ,    ""    ,0                                    ,  0xfe   ,    0    ,  0x80   ,        6         ,// 
		 6   ,"������� �������"  				 ,   "���, �/�"        ,"�������"       ,""        ,0                            | O_RW  ,    26   ,    1    ,  0x80   ,        29        ,
		 6   ,"����� �������"            ,  "DV-2"   					 ,"����� DV"      ,""        ,0          | O_SZ            | O_RW  ,    0    ,    1    ,  0x80   ,        23        ,// 31
		 6   ,"���������"                ,  "��������������"   ,"���������"     ,""        ,0          | O_SZ            | O_RW  ,    3    ,    1    ,  0x80   ,        25        ,// 30
		 6   ,"��������� �������" 	     ,   "���, �/�"        ,"���.�������"   ,""        ,0                            | O_RW  ,    27   ,    1    ,  0x80   ,        31        ,
		 

	0xff   ,"������ �� ��������"        					 ,         ""          ,       ""       ,    ""    ,0                                    ,  0xfe   ,    1    ,  0xff   ,        0xf0      ,// 40	
///	0xff   ,"��������"         				 ,         ""          ,       ""       ,    ""    ,0                                    ,  0xfe   ,    1    ,  0xff   ,        0xf1      ,// 41	
	0xff   ,"�������"         				 ,         ""          ,       ""       ,    ""    ,0                                    ,  0xfe   ,    1    ,  0xff   ,        0xf2      ,// 42	
	
	  0xf0   ,"��������"        				 ,         ""          ,       ""       ,    ""    ,0                                    ,  128   ,    1    ,  0xff   ,        0xfa       ,// 43	
		0xf2   ,"��������"        				 ,         ""          ,       ""       ,    ""    ,0                                    ,  129   ,    1    ,  0xff   ,        0xfc       ,// 45		
 
};
const TEnum_val Enum_val[COUNT_ENUM] = {
//� ������         ��������                �����������
    25   ,                          0,"������"             ,//               1
    25   ,                          1,"����"               ,//               2
    26   ,                          0,"����"               ,//               3
    26   ,                          1,"���"                ,//               4
		27   ,                          0,"0.6"                ,//               5
    27   ,                          1,"1.2"                ,//               6
	28	 ,													0,"0 �."									 ,
	28	 ,													1,"2 �."									 ,
	28	 ,													2,"5 �."									 ,
	28	 ,													3,"7 �."									 ,
	28	 ,													4,"10 �."									 ,
	28	 ,													5,"12 �."									 ,
	28	 ,													6,"15 �."									 ,
	28	 ,													7,"17 �."									 ,
	28	 ,													8,"20 �."									 ,
	28	 ,													9,"22 �."									 ,
	28	 ,													10,"25 �."									 ,
	28	 ,													11,"27 �."									 ,
	28	 ,													12,"30 �."									 ,
	28	 ,													13,"32 �."									 ,
	28	 ,													14,"35 �."									 ,
	28	 ,													15,"37 �."									 ,
	28	 ,													16,"40 �."									 ,
	28	 ,													17,"42 �."									 ,
	28	 ,													18,"44 �."									 ,
	28	 ,													19,"47 �."									 ,
	28	 ,													20,"49 �."									 ,
	28	 ,													21,"52 �."									 ,
	28	 ,													22,"54 �."									 ,
	28	 ,													23,"57 �."									 ,
	28	 ,													24,"59 �."									 ,
	28	 ,													25,"62 �."									 ,
	28	 ,													26,"64 �."									 ,
	28	 ,													27,"67 �."									 ,
	28	 ,													28,"69 �."									 ,
	28	 ,													29,"72 �."									 ,
	28	 ,													30,"74 �."									 ,
	28	 ,													31,"77 �."									 ,
	28	 ,													32,"79 �."									 ,
	28	 ,													33,"82 �."									 ,
	28	 ,													34,"84 �."									 ,
	28	 ,													35,"87 �."									 ,
	28	 ,													36,"89 �."									 ,
	28	 ,													37,"91 �."									 ,
	28	 ,													38,"94 �."									 ,
	28	 ,													39,"96 �."									 ,
	28	 ,													40,"99 �."									 ,
	28	 ,													41,"101 �."									 ,
	28	 ,													42,"104 �."									 ,
	28	 ,													43,"106 �."									 ,
	28	 ,													44,"109 �."									 ,
	28	 ,													45,"111 �."									 ,
	28	 ,													46,"114 �."									 ,
	28	 ,													47,"116 �."									 ,
	28	 ,													48,"119 �."									 ,
	28	 ,													49,"121 �."									 ,
	28	 ,													50,"124 �."									 ,
	28	 ,													51,"126 �."									 ,
	28	 ,													52,"129 �."									 ,
	28	 ,													53,"131 �."									 ,
	28	 ,													54,"133 �."									 ,
	28	 ,													55,"136 �."									 ,
	28	 ,													56,"138 �."									 ,
	28	 ,													57,"141 �."									 ,
	28	 ,													58,"143 �."									 ,
	28	 ,													59,"146 �."									 ,
	28	 ,													60,"148 �."									 ,
	28	 ,													61,"151 �."									 ,
	28	 ,													62,"153 �."									 ,
	28	 ,													63,"156 �."									 ,
	28	 ,													64,"158 �."									 ,
	28	 ,													65,"161 �."									 ,
	28	 ,													66,"163 �."									 ,
	28	 ,													67,"166 �."									 ,
	28	 ,													68,"168 �."									 ,
	28	 ,													69,"171 �."									 ,
	28	 ,													70,"173 �."									 ,
	28	 ,													71,"176 �."									 ,
	28	 ,													72,"178 �."									 ,
	28	 ,													73,"180 �."									 ,
	28	 ,													74,"183 �."									 ,
	28	 ,													75,"185 �."									 ,
	28	 ,													76,"188 �."									 ,
	28	 ,													77,"190 �."									 ,
	28	 ,													78,"193 �."									 ,
	28	 ,													79,"195 �."									 ,
	28	 ,													80,"198 �."									 ,
	28	 ,													81,"200 �."									 ,
	28	 ,													82,"203 �."									 ,
	28	 ,													83,"205 �."									 ,
	28	 ,													84,"208 �."									 ,
	28	 ,													85,"210 �."									 ,
	28	 ,													86,"213 �."									 ,
	28	 ,													87,"215 �."									 ,
	28	 ,													88,"218 �."									 ,
	28	 ,													89,"220 �."									 ,
	28	 ,													90,"222 �."									 ,
	28	 ,													91,"225 �."									 ,
	28	 ,													92,"227 �."									 ,
	28	 ,													93,"230 �."									 ,
	28	 ,													94,"232 �."									 ,
	28	 ,													95,"235 �."									 ,
	28	 ,													96,"237 �."									 ,
	28	 ,													97,"240 �."									 ,
	28	 ,													98,"242 �."									 ,
	28	 ,													99,"245 �."									 ,
	28	 ,													100,"247 �."									 ,
	28	 ,													101,"250 �."									 ,
	28	 ,													102,"252 �."									 ,
	28	 ,													103,"255 �."									 ,
	28	 ,													104,"257 �."									 ,
	28	 ,													105,"260 �."									 ,
	28	 ,													106,"262 �."									 ,
	28	 ,													107,"264 �."									 ,
	28	 ,													108,"267 �."									 ,
	28	 ,													109,"269 �."									 ,
	28	 ,													110,"272 �."									 ,
	28	 ,													111,"274 �."									 ,
	28	 ,													112,"277 �."									 ,
	28	 ,													113,"279 �."									 ,
	28	 ,													114,"282 �."									 ,
	28	 ,													115,"284 �."									 ,
	28	 ,													116,"287 �."									 ,
	28	 ,													117,"289 �."									 ,
	28	 ,													118,"292 �."									 ,
	28	 ,													119,"294 �."									 ,
	28	 ,													120,"297 �."									 ,
	28	 ,													121,"299 �."									 ,
	28	 ,													122,"302 �."									 ,
	28	 ,													123,"304 �."									 ,
	28	 ,													124,"307 �."									 ,
	28	 ,													125,"309 �."									 ,
	28	 ,													126,"311 �."									 ,
	28	 ,													127,"314 �."									 ,	
	
};
