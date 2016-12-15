#include "regmap.h"
#include "params.h"



const TPARAMS PARAMS[COUNT_REG]={
//   min     max      def    copy_flash                            ������������                           reg
    2000  , 2050  ,  2016   ,   2   ,//  ������� ����, ���                                                 0
     101  , 3112  ,   101   ,   2   ,//  ������� ���� (� ������� �����:����)                               1
      0   , 2359  ,    0    ,   2   ,//  ������� ����� (� ������� ���:���)                                 2
      0   ,  59   ,    0    ,   2   ,//  ������� �����, �������                                            3
      0   ,0xFFFF ,    0    ,   0   ,//  ���, ���������� ��������                                          4
      0   ,0xFFFF ,    0    ,   0   ,//  ���, ���������� ��������                                          5
      0   ,0xFFFF ,    0    ,   0   ,//  ���-���, ���������� ��������                                      6
      0   ,0xFFFF ,   100   ,   0   ,//  ���-������, ���������� ��������                                   7
      0   ,0xFFFF ,    0    ,   0   ,//  ������ �������                                                    8
      0   ,  100  ,    0    ,   0   ,//  ����� ������������                                                9
      0   ,0xFFFF ,    0    ,   0   ,//  ���� ���                                                          10
      0   ,0x00FF , 0x00FF  ,   0   ,//  ������ � ������ �������                                           11
      1   ,  1000  ,   1    ,   0   ,//  ����� ����� ��� �������, �������                                  12 ///���� ������� ��� �������������� "��� ��������"
      0   ,0xFFFF ,    0    ,   0   ,//  ������ ��                                                         13
      0   ,0xFFFF ,    0    ,   1   ,//  ��������� �������, ��                                             14
      0   ,0xFFFF ,    1    ,   1   ,//  ������                                                            15
      1   ,   8   ,    1    ,   1   ,//  �������, ����� ���������                                          16
      1   ,0x0010 ,    1    ,   1   ,//  �������, ������ ��� ������ ���������                              17
      1   ,0x0010 ,    1    ,   1   ,//  �������, ������ ��� ������ ��������                               18
      1   ,0x0010 ,    1    ,   1   ,//  �������, ������ ��� ������ �����������                            19
      1   ,0x0020 ,    1    ,   1   ,//  �������, �������� ��� ������ ��������                             20
      1   ,0x0010 ,    1    ,   1   ,//  �������, �������� ��� ������ ��������                             21
      2   ,0x0010 ,    2    ,   1   ,//  �������, �������� ��� ������ �����������                          22
      0   ,0xFFFF ,    0    ,   1   ,//  ������                                                            23
      0   ,0xFFFF ,    1    ,   1   ,//  ����� ���������� ����������� �����                                24
     100  , 9000  ,  1000   ,   1   ,//  �������� ��������������                                           25
     0  	, 0xFFFF   ,  0   ,   1   ,//  													                                         26
     0  	, 0xFFFF   ,  0   ,   1   ,//  																																	 27		 
		 0  	,      1   ,  0   ,   1   ,//																									 									 28 
		 0  	,      1   ,  0   ,   1   ,/// ������� ������� 																									 29 
		 0  	, 		 1   ,  0   ,   1   ,/// ��������������																									   30 		 
		 0  	, 	   1   ,  0   ,   1   ,/// ��������� �������  																							 31										 
		 0  	, 	   127 ,  0   ,   1   ,/// ���������� �����	  																							 32										 
};

