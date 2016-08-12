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
     4   ,"������� ����"             ,      "���:���"      ,"����"          ,""        ,0                            | O_RW  ,   19    ,    1    ,  0xff   ,        1         ,// 28
     4   ,"������� ���"              ,         ""          ,"���"           ,""        ,0                            | O_RW  ,    0    ,    1    ,  0xff   ,        0         ,// 29
     4   ,"���������"                ,  "��������������"   ,"���������"     ,""        ,0          | O_SZ            | O_RW  ,    3    ,    1    ,  0x80   ,        25        ,// 30
     4   ,"����� �������"            ,  "DV-2"   					 ,"����� DV"    ,""          ,0          | O_SZ            | O_RW  ,    0    ,    1    ,  0x80   ,        23        ,// 31
     4   ,"����� �������"            ,  "DV-2"   					 ,"����� DV"    ,""          ,0                      						   ,    0    ,    1    ,  0x7F   ,        23        ,// 32		 
     4   ,"������ ������������"      ,    "�����������"    ,"������ ��"     ,""        ,0                                    ,    2    ,    1    ,  0xff   ,        13        ,// 33
     4   ,"�������������"            ,         ""          ,""              ,""        ,0                            | O_RW  ,   10    ,    1    ,  0xff   ,        1         ,// 34
///		 4   ,"�������� ���������"       ,         ""          ,""              ,""        ,0                                    ,   0xfe  ,    1    ,  0xff   ,        6         ,// 35
     4   ,"���������"                ,   "�������, �."     ,"���������"     ,""        ,0                                    ,    0    ,    1    ,  0xff   ,        14        ,// 36
		 4   ,"���������� �������"       ,         ""          ,""              ,""        ,0                            | O_RW  ,   0xfe  ,    1    ,  0xff   ,        0xfd        ,// 
		 4   ,"������� �������"  				 ,   "���, �/�"        ,"�������"       ,""        ,0                            | O_RW  ,    26   ,    1    ,  0x80   ,        29        ,
		 4   ,"��������������"  				 ,   "����� 10 ���." 	 ,"��������."     ,""        ,0                            | O_RW  ,    26   ,    1    ,  0xff   ,        30        ,
     0   ," ����� ��������"          ,         ""          ,       ""       ,    ""    ,0                                    ,  0xfe   ,    0    ,  0x78   ,        5         ,// 37
		 5   ,"�������"         				 ,         ""          ,       ""       ,    ""    ,0                                    ,  130    ,    0    ,  0xff   ,        0xff      ,// 38
		 5   ,"������� ��������"         ,         ""          ,       ""       ,    ""    ,0                                    ,  18     ,    2    ,  0xff   ,        0xfe      ,// 39		

	0xff   ,"�������"        					 ,         ""          ,       ""       ,    ""    ,0                                    ,  0xfe   ,    1    ,  0xff   ,        0xf0      ,// 40	
///	0xff   ,"��������"         				 ,         ""          ,       ""       ,    ""    ,0                                    ,  0xfe   ,    1    ,  0xff   ,        0xf1      ,// 41	
	0xff   ,"�������"         				 ,         ""          ,       ""       ,    ""    ,0                                    ,  0xfe   ,    1    ,  0xff   ,        0xf2      ,// 42	
	
	0xf0   ,"��������"        				 ,         ""          ,       ""       ,    ""    ,0                                    ,  128   ,    1    ,  0xff   ,        0xfa       ,// 43	
///	0xf1   ,"���������"        				 ,         ""          ,       ""       ,    ""    ,0                                    ,  129   ,    1    ,  0xff   ,        0xfb       ,// 44		
//	0xf2   ,"��������"        				 ,         ""          ,       ""       ,    ""    ,0                                    ,  129   ,    1    ,  0xff   ,        0xfc       ,// 45		
		0xf2   ,"��������"        				 ,         ""          ,       ""       ,    ""    ,0                                    ,  129   ,    1    ,  0xff   ,        0xfc       ,// 45		
 
};
const TEnum_val Enum_val[COUNT_ENUM] = {
//� ������         ��������                �����������
    25   ,                          0,"������"             ,//               1
    25   ,                          1,"����"               ,//               2
    26   ,                          0,"����"               ,//               3
    26   ,                          1,"���"                ,//               4
};
