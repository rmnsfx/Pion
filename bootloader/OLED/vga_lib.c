#include "ssd1303.h"
#include "vga_lib.h"


#define vga_MASK_X			127
#define vga_MASK_Y			63		

dr_mode_t 		vga_MODE = drMODE_NORMAL;

unsigned char 	text_pos_x = 0;		//глоабальная координата X для текста
unsigned char   text_pos_y = 0;		//глобальная координата Y для текста

unsigned char VGA_RAM[vga_COUNT_X_PIXELS][vga_COUNT_Y_PIXELS/8];			//видио память



void vga_INIT(void)
{
 	ssd_OLED_INIT();
	vga_CLEAR();
	text_pos_x = 0;
	text_pos_y = 0;
	vga_SET_DRAW_MODE(drMODE_NORMAL);
 	vga_UPDATE();
}

void vga_UPDATE(void)
{
 	unsigned short i, j;
        for(i = 0; i < 8; i++){
                ssd_SET_POS(0, i);				
                for(j = 0; j < 128; j++){
                        ssd_SEND_DATA8(VGA_RAM[j][i]);
                };
        }
}

void vga_CLEAR(void)
{
	unsigned short i, j;
        for(i = 0; i < 8; i++){
                ssd_SET_POS(0, i);				
                for(j = 0; j < 128; j++){
                        VGA_RAM[j][i] = 0x00;
                };
        } 
}


void vga_DRAW_DATA8(unsigned char x,unsigned char y,unsigned char data8)
{
 	unsigned char offset_y;

	if (x>vga_COUNT_X_PIXELS-1) return;
	if (y>vga_COUNT_Y_PIXELS-1) return;
 	//x&=vga_MASK_X;
 	//y&=vga_MASK_Y;

 
	//вычислить смещение внутри строки
	offset_y = y%8;
	//вычислить строку
 	y = y/8;

	if (offset_y>0)
	  {
	   VGA_RAM[x][y]&= ~(0xff<<offset_y);
	   VGA_RAM[x][y]|= (data8<<offset_y);
	   if (y<(vga_COUNT_Y_PIXELS/8-1)) 
	    {
		 y++;
		 offset_y = 8-offset_y;
		 VGA_RAM[x][y]&= ~(0xff>>offset_y);
		 VGA_RAM[x][y]|= (data8>>offset_y);
		}
	  }
	else
	   VGA_RAM[x][y] = data8;

}

void vga_DRAW_PIXEL(unsigned char x,unsigned char y)
{
 	unsigned char offset_y;
 	if (x>vga_COUNT_X_PIXELS-1) return;
	if (y>vga_COUNT_Y_PIXELS-1) return;
 
	//вычислить смещение внутри строки
	offset_y = y%8;
	//вычислить строку
 	y = y/8;

	if (vga_MODE==drMODE_NORMAL)
	 VGA_RAM[x][y]|= (0x01<<offset_y);
	else
	if (vga_MODE==drMODE_XOR)
	 {
	  offset_y = (0x01<<offset_y);
	  if (VGA_RAM[x][y]&offset_y)   VGA_RAM[x][y]&=~offset_y;
	  else							VGA_RAM[x][y]|=	offset_y;
	  //VGA_RAM[x][y]|=(VGA_RAM[x][y]&(~offset_y))|((~VGA_RAM[x][y])&offset_y);
	 // VGA_RAM[x][y]&= (VGA_RAM[x][y]&offset_y)^0xff;
	  }

}

//задать режим рисования
void vga_SET_DRAW_MODE(dr_mode_t mode)
{
 	vga_MODE = mode;
}
//запросить установленный режим рисования
dr_mode_t vga_GET_DRAW_MODE(void)
{
	return vga_MODE;
}

void vga_LINE(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
{
    signed short dx, dy, i1, i2, i, kx, ky;
    signed short d;     
    signed short x, y;
    signed short flag;

    dy = y2 - y1;
    dx = x2 - x1;
    if (dx == 0 && dy == 0){
            vga_DRAW_PIXEL(x1,y1);    
            return;
    }
    kx = 1; 
    ky = 1; 

    if( dx < 0 ){ dx = -dx; kx = -1; }
    else if(dx == 0)        kx = 0;    

    if(dy < 0) { dy = -dy; ky = -1; }

    if(dx < dy){ flag = 0; d = dx; dx = dy; dy = d; }
    else         flag = 1;

    i1 = dy + dy; d = i1 - dx; i2 = d - dx;
    x = x1; y = y1;
	
    for(i=0; i < dx; i++){
            vga_DRAW_PIXEL(x,y);     

            if(flag) x += kx;
            else     y += ky;

            if( d < 0 ) 
                     d += i1;
            else{       
                     d += i2;
                     if(flag) y += ky; 
                     else     x += kx;
            }
    }
    vga_DRAW_PIXEL(x,y);
}
/*Функция рисования прямоугольника, координаты двух точек-верхний левый угол и нижни правый и заливка */
void vga_RECTANGLE(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, const dr_style_t style)
{
	if ((style==drRECT_NO_FILL)||(style==drRECT_FILL))	
	 {
      vga_LINE(x1, y1, x2, y1);
      vga_LINE(x1, y2, x2, y2);  
      vga_LINE(x1, (y1+1), x1, (y2-1));  
      vga_LINE(x2, (y1+1), x2, (y2-1));  
	 }
	else
	 {
	  vga_LINE(x1+1, y1, x2-1, y1);
      vga_LINE(x1+1, y2, x2-1, y2);  
      vga_LINE(x1, (y1+1), x1, (y2-1));  
      vga_LINE(x2, (y1+1), x2, (y2-1));  
	 }

    if ((style==drRECT_FILL)||(style==drRECT_ARC_FILL))
	 {
	  x1++;
      while(x1<x2)
	   	{
        	vga_LINE(x1, (y1+1), x1, (y2-1));
			x1++;
       	}
     }
}

void vga_CIRCLE(unsigned char xc,unsigned char yc,unsigned char r)
{
    signed short  x,y,d;
    y = r; 
    d = 3-2*r;
    x = 0;
  
    while(x <= y)
    {
     vga_DRAW_PIXEL(x+xc,y+yc);
     vga_DRAW_PIXEL(x+xc,-y+yc);
     vga_DRAW_PIXEL(-x+xc,-y+yc);
     vga_DRAW_PIXEL(-x+xc,y+yc);
     vga_DRAW_PIXEL(y+xc,x+yc);
     vga_DRAW_PIXEL(y+xc,-x+yc);
     vga_DRAW_PIXEL(-y+xc,-x+yc);
     vga_DRAW_PIXEL(-y+xc,x+yc);
     if (d<0) 
      {
       d = d+4*x+6;
      } 
     else
      {
       d = d+4*(x-y)+10;
       y--;
      }
    x++;
    };  
}

void vga_SET_POS_TEXT(unsigned char x, unsigned char y)
{
 	if (x < (vga_COUNT_X_PIXELS-1))
 		text_pos_x = x;
	else
		text_pos_x = vga_COUNT_X_PIXELS-1;

	if (y < (vga_COUNT_Y_PIXELS-1))
 		text_pos_y = y;
	else
		text_pos_y = vga_COUNT_Y_PIXELS-1;
}

void vga_PRINT_CHAR(const char symbol, const font_t *font)
{
  unsigned char i,j,s;
  s = (font->H+7)/8;
  for (j=0;j<s;j++)
   {
	for (i=0;i<font->W;i++)
		{
		 //s = font->FONT[symbol*font->W*s+j*font->W+i];
	     vga_DRAW_DATA8(text_pos_x+i,text_pos_y+j*8,font->FONT[symbol*font->W*s+j*font->W+i]);

		}
   }	
 if (text_pos_x < (vga_COUNT_X_PIXELS-font->W)) text_pos_x+=font->W;  	
 //if (text_pos_y < (vga_COUNT_Y_PIXELS-font->H)) text_pos_y+=font->H;
}


void vga_PRINT_TEXT(const char *str, unsigned char len, const font_t *font)
{
 if (len==0)
  while (*str)
   vga_PRINT_CHAR(*str++,font);	
 else
  while (len--)
   vga_PRINT_CHAR(*str++,font);	   
}

void vga_FILL_STR(const char fill_char, unsigned char len, const font_t *font)
{
 while (len--)
   vga_PRINT_CHAR(fill_char,font);	   
}



