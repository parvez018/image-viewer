/*****************************
 *PROJECT::IMAGE VIEWER      *
 *Created by MD.NASIM        *
 *Student ID:0905018         *
 *****************************/
# include "iGraphics2.h"


#define SCREEN_WIDTH 1300	//replace 1350 & 700 with your screen resolution for the software to act properly

#define SCREEN_HEIGHT 600

#define BUTTON_START_X (SCREEN_WIDTH-464)/2



typedef struct
               {
                 short bmtype;
                 long int bmsize;
                 short int bmreserved1;
                 short int bmreserved2;
                 long int bmofbyte;
               }fileheader;

typedef struct
              {
                long int bmheadersize;
                long int bmwidth;
                long int bmheight;
                short int bmplane;
                short int bmbitdepth;
                long int bmcompression;
                long int bmimagesize;
                long int bmXresln;
                long int bmYresln;
                long int bmusedcol;
                long int bmimpcolor;
              }fileinfo;

typedef struct
              {
                unsigned char id;
                unsigned char colour_map_type;
                unsigned char image_type;
                short int colour_map_first_entry;
                short int colour_map_length;
				short int horizontal_origin;
                short int vertical_origin;
                short int width;
                short int height;
                unsigned char pixel_depth;
              }tga_header;

fileinfo bmpi;
tga_header tgai;

char type;
char *filename;

char *show_filenm;

FILE *bm;
short int bmtype;

unsigned char ***pixelarray=NULL;

float zoom;
int drawing_mode=1;

int scroll_x=0;
int scroll_y=0;

char zin,zout,ccw,accw,delt;

int w,h;

int txt_c=1;

int txt_in=0;

int error_message;
int cursor_color;
int shift_cursor;

int i=0;


void readPixelInfo(FILE *fp);
void iInitializeAll();
fileinfo readFileinfo(FILE *fp);
tga_header read_tgainfo(FILE *f);
void freearray(unsigned char ***pixelarray);



void iDraw()
{
	
	int i,j;
	float p,q;
	
	iClear();


	
	iSetColor(230,230,230);
	iFilledRectangle(0,0,SCREEN_WIDTH,SCREEN_HEIGHT);


	 
	if(!txt_c)
	{

		if(drawing_mode==1)
		{
			for(i=0,p=0;i<h;i++,p=p+zoom)
			{
				for(j=0,q=0;j<w;j++,q=q+zoom)
				{
					iSetColor(pixelarray[i][j][2],pixelarray[i][j][1],pixelarray[i][j][0]);
					iPoint( q + (SCREEN_WIDTH-w*zoom)/2 + scroll_x  , p + (SCREEN_HEIGHT-h*zoom)/2 + 50 + scroll_y,zoom);	
					
					
				
				}
				
	
			}
	
		}


		else if(drawing_mode==2)
		{
			for(i=0,p=0;i<h;i++,p=p+zoom)
			{
				for(j=0,q=0;j<w;j++,q=q+zoom)
				{
					iSetColor(pixelarray[i][j][2],pixelarray[i][j][1],pixelarray[i][j][0]);
					iPoint(p+(SCREEN_WIDTH-h*zoom)/2 + scroll_x  ,w*zoom-q+(SCREEN_HEIGHT-w*zoom)/2+50+scroll_y,zoom);

					
				}
	
			}
	
		}

		else if(drawing_mode==3)
		{
			for(i=0,p=0;i<h;i++,p=p+zoom)
			{
				for(j=0,q=0;j<w;j++,q=q+zoom)
				{
					iSetColor(pixelarray[i][j][2],pixelarray[i][j][1],pixelarray[i][j][0]);
					iPoint(SCREEN_WIDTH-q-(SCREEN_WIDTH-w*zoom)/2+scroll_x ,SCREEN_HEIGHT-p-(SCREEN_HEIGHT-h*zoom)/2+50+scroll_y,zoom);
				}
	
			}
	
		}

		else if(drawing_mode==4)
		{
			for(i=0,p=0;i<h;i++,p=p+zoom)
			{
				for(j=0,q=0;j<w;j++,q=q+zoom)
				{
					iSetColor(pixelarray[i][j][2],pixelarray[i][j][1],pixelarray[i][j][0]);
					iPoint(SCREEN_WIDTH-(p+(SCREEN_WIDTH-h*zoom)/2)+scroll_x  ,SCREEN_HEIGHT-(w*zoom-q+(SCREEN_HEIGHT-w*zoom)/2)+50+scroll_y,zoom);

				}
	
			}
	
		}



		


		
	

	}
	iSetColor(230,230,230);
	iFilledRectangle(0,SCREEN_HEIGHT-30,SCREEN_WIDTH,SCREEN_HEIGHT);

	iSetColor(txt_c,txt_c,txt_c);
	iText(10,SCREEN_HEIGHT-20,"OPEN :",GLUT_BITMAP_HELVETICA_18);

	iSetColor(200,200,200);
	if(txt_in==0)iText(10+4*18,SCREEN_HEIGHT-20,"CLICK HERE & ENTER THE FILE NAME",GLUT_BITMAP_HELVETICA_18);

	iSetColor(cursor_color,cursor_color,cursor_color);
	if(txt_in==1)iLine(10+4*18+shift_cursor,SCREEN_HEIGHT-22,10+4*18+shift_cursor,SCREEN_HEIGHT-5);
	
	iSetColor(0,0,0);
	if(txt_in==1)iText(10+4*18,SCREEN_HEIGHT-20,filename,GLUT_BITMAP_9_BY_15);

	iSetColor(230,230,230);
	iFilledRectangle(0,0,SCREEN_WIDTH,94);

	if(zin==1)iShowBMP(BUTTON_START_X,25, "search-add-icon_g.bmp");
	else iShowBMP(BUTTON_START_X,25, "search-add-icon.bmp");
	
	if(zout==1)iShowBMP(BUTTON_START_X+100,25, "search-remove-icon_g.bmp");
	else iShowBMP(BUTTON_START_X+100,25, "search-remove-icon.bmp");
	
	if(ccw==1)iShowBMP(BUTTON_START_X+200,25, "rotate_a_g.bmp");
	else iShowBMP(BUTTON_START_X + 200,25, "rotate_a.bmp");
	
	if(accw==1)iShowBMP(BUTTON_START_X+300,25, "rotate_c_g.bmp");
	else iShowBMP(BUTTON_START_X + 300,25, "rotate_c.bmp");

	if(delt==1)iShowBMP(BUTTON_START_X+400,25, "delete_g.bmp");
	else iShowBMP(BUTTON_START_X + 400,25, "delete_b.bmp");

	iSetColor(0,0,0);
	iText(30,30,show_filenm,GLUT_BITMAP_HELVETICA_18);

	if(error_message)
	{
		iSetColor(0,0,0);
		if(txt_in==1)iText(400,350,"An error has occurred.Please try again",GLUT_BITMAP_TIMES_ROMAN_24);	
	
	}



}


void iMouseMove(int mx, int my)
{
	
}

/******************************************************************

 function iPassiveMouseMove is called again and again by the system

*******************************************************************/


void iPassiveMouseMove(int mx,int my)
{
	if( mx>BUTTON_START_X && mx<(BUTTON_START_X+ 64) && my>25 && my<89)zin=1;
	else zin=0;

	if( mx>(BUTTON_START_X + 100) && mx<(BUTTON_START_X + 164) && my>25 && my<89)zout=1;
	else zout=0;

	if( mx>(BUTTON_START_X + 200) && mx<(BUTTON_START_X + 264) && my>25 && my<89)ccw=1;
	else ccw=0;

	if( mx>(BUTTON_START_X + 300) && mx<(BUTTON_START_X + 364) && my>25 && my<89)accw=1;
	else accw=0;

	if( mx>(BUTTON_START_X + 400) && mx<(BUTTON_START_X + 464) && my>25 && my<89)delt=1;
	else delt=0;

}


/************************************************************************ 
	
	function iMouse() is called when the user presses/releases the mouse.
	(mx, my) is the position where the mouse pointer is.

*************************************************************************/


void iMouse(int button, int state, int mx, int my)
{
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		
		if( mx>BUTTON_START_X && mx<(BUTTON_START_X + 64) && my>25 && my<89)zoom+=.5;

		if( mx>(BUTTON_START_X + 100) && mx<(BUTTON_START_X + 164) && my>25 && my<89 && zoom>.5)
					
				{	
			
					zoom-=.5;
					scroll_x =0;
					scroll_y =0;
		
				}

		if( mx>(BUTTON_START_X + 200) && mx<(BUTTON_START_X + 264) && my>25 && my<89)
				
				{
					drawing_mode--;
					if(drawing_mode<1)drawing_mode=4;
				}

		if( mx>(BUTTON_START_X + 300) && mx<(BUTTON_START_X + 364) && my>25 && my<89)
		
				{
					drawing_mode++;
					if(drawing_mode>4)drawing_mode=1;
				}

		if( mx>(BUTTON_START_X + 400) && mx<(BUTTON_START_X + 464) && my>25 && my<89)
		
				{
					txt_c=1;
					remove(show_filenm);
					
				
					
					free(show_filenm);
					show_filenm=(char *)malloc(100*sizeof(char));
					iInitializeAll();


					
				}

		if(mx>82 && mx<514 && my>(SCREEN_HEIGHT-20) && my<SCREEN_HEIGHT)
				
				{
					txt_in=1;
					error_message=0;
				}
	
	}
	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		//Does nothing	
	}
}

/******************************************************************************
	
	  function iKeyboard() is called whenever the user hits a key in keyboard.
	  key- holds the ASCII value of the key pressed. 

*******************************************************************************/


void iSpecialKeyboard(unsigned char key);



void iKeyboard(unsigned char key)
{
		
	int temp1,temp2;
	
		
		if(key ==' ')
		{
			if(txt_c==0 && txt_in==1)
			{
				txt_c=1;
				iInitializeAll();
			}
		}
		

		else if(key=='\r' && txt_in)
		{
			int ex;
			for(ex=0;ex<1;ex++)
			{

				txt_c=0;
		

				

				if((bm=fopen(filename,"rb"))==NULL)
				{
				
					
					
					txt_c=1;
					
					free(show_filenm);
					iInitializeAll();
					show_filenm=(char *)malloc(100*sizeof(char));
					
				
					continue;
					
				}
				
				strcpy(show_filenm,filename);
  
				fread(&bmtype,2,1,bm);
  
				if(bmtype==19778)
				{
					type=1;
					bmpi=readFileinfo(bm);

					if(bmpi.bmbitdepth==24)
					{
						if(pixelarray!=NULL)freearray(pixelarray);
						readPixelInfo(bm);
						
					}
					
					else
					{
						
						txt_c=1;
						iInitializeAll();
									
				
						continue;
					}

				}
				
				else
				{
					type=2;
		
					tgai=read_tgainfo(bm);
		
					readPixelInfo(bm);

				}


	
	
				fclose(bm);
				iInitializeAll();
			
	
			i=0;
			break;
			
			}
			
			
		}

		else if( key=='\b')
		{
			if(i>0)
			{
				filename[i-1]='\0';
				i--;
			}
			
			temp1=zoom;
			temp2=drawing_mode;


			if(!i)
			{
				
				temp1=zoom;
				temp2=drawing_mode;
				
				iInitializeAll();

				zoom=temp1;
				drawing_mode=temp2;

			}
				if(shift_cursor)shift_cursor-=9;

			zoom=temp1;
			drawing_mode=temp2;
		
		}
		
		
		
		else if(txt_in==1)
		{
			filename[i]=key;
			filename[i+1]='\0';
			i++;
			shift_cursor+=9;
		
		}



	
}

/*
	function iSpecialKeyboard() is called whenver user hits special keys like-
	function keys, home, end, pg up, pg down, arraows etc. you have to use 
	appropriate constants to detect them. A list is:
	GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6, 
	GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12, 
	GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP, 
	GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT 
*/
void iSpecialKeyboard(unsigned char key)
{

	if(key == GLUT_KEY_END)
	{
		exit(0);	
	}
	
	if(key == GLUT_KEY_RIGHT)
	{
		if((((SCREEN_WIDTH-w*zoom)/2 + scroll_x +w*zoom-SCREEN_WIDTH)>0) && (drawing_mode==1 || drawing_mode==3))scroll_x-=10;
		
		if((((SCREEN_WIDTH-h*zoom)/2 + scroll_x +h*zoom-SCREEN_WIDTH)>0) && (drawing_mode==2 || drawing_mode==4))scroll_x-=10;
		
	}

	if(key == GLUT_KEY_LEFT)
	{
		if(((SCREEN_WIDTH-w*zoom)/2 + scroll_x )<0 && (drawing_mode==1 || drawing_mode==3))scroll_x+=10;	

		if(((SCREEN_WIDTH-h*zoom)/2 + scroll_x )<0 && (drawing_mode==2 || drawing_mode==4))scroll_x+=10;	
		
	
	}

	if(key == GLUT_KEY_UP)
	{
		

		if(  ( (SCREEN_HEIGHT-h*zoom)/2 +50 +30+ scroll_y+h*zoom-SCREEN_HEIGHT ) >0  &&  (drawing_mode==1 || drawing_mode==3))scroll_y-=10;
		
		if(  ( (SCREEN_HEIGHT-w*zoom)/2 +50 +30+ scroll_y+w*zoom-SCREEN_HEIGHT ) >0  &&  (drawing_mode==2 || drawing_mode==4))scroll_y-=10;
	
	}

	if(key == GLUT_KEY_DOWN)
	{
		
		if(  ( (SCREEN_HEIGHT-h*zoom)/2 -50  + scroll_y ) <0  &&  (drawing_mode==1 || drawing_mode==3))scroll_y+=10;
		
		if(  ( (SCREEN_HEIGHT-w*zoom)/2 -50  + scroll_y ) <0  &&  (drawing_mode==2 || drawing_mode==4))scroll_y+=10;
	
	}

}


/********************************************************************

  function "readFileinfo" reads the infformation about the bmp image 
  such as width,height,resolution,bitdepth 

*********************************************************************/



fileinfo readFileinfo(FILE *fp)                
{                                          
    fileinfo fi;                           
    fseek(fp,14,SEEK_SET);                  
    fread(&fi,sizeof(fi),1,fp);
    return fi;
}



/**********************************************************************************

   function "readPixelInfo" reads the colour table of all the pixels in the image 
 
***********************************************************************************/



void readPixelInfo(FILE *fp)
{
    int i,j,l,m,n;
    
	if(type==1)
	{
		
		fseek(fp,54,SEEK_SET);
		w=bmpi.bmwidth;
		h=bmpi.bmheight;

	}
	
	if(type==2)
	{
		fseek(fp,18,SEEK_SET);
		w=tgai.width;
		h=tgai.height;
	}
	 


	

		pixelarray=(unsigned char***)malloc(h*sizeof(unsigned char **));

    for (i=0;i<h;++i)
	
    {
        
		*(pixelarray+i)=(unsigned char **)malloc(w*sizeof(unsigned char*));

        for(j=0;j<w;++j)
	
	    {
            *(*(pixelarray+i)+j)=(unsigned char *)malloc(3*sizeof(unsigned char));
	    }
	}

	
	
	for(l=0;l<h;l++)
    {
        for(m=0;m<w;m++)
        {
            for(n=0;n<3;n++)
            {
                fread((*(*(pixelarray+l)+m)+n),sizeof(char),1,fp);

            }
        }
    }

}



/*********************************************************

	function "read_tgainfo" reads the header of tga file

 *********************************************************/



tga_header read_tgainfo(FILE *f)
{
	tga_header fi;                           
    fseek(f,0,SEEK_SET);                  
    fread(&fi,sizeof(fi),1,f);
    return fi;

}



/************************************************************************************

  "iInitializeAll" function rewinds all the global variables to their initial state
  when a new file is to be opened

**************************************************************************************/



void iInitializeAll()
{
	i=0;
	type=0;
	zoom=1;

	
	
	txt_in=0;
	
	drawing_mode=1;
	shift_cursor=0;

	scroll_x=0;
	scroll_y=0;

	zin=0;
	zout=0;
	ccw=0;
	accw=0;
	
	free(filename);
	filename=(char *)malloc(100*sizeof(char));

}



/****************************************************************************

"freearray" function frees the 3-D array each time a new file name is entered

*****************************************************************************/



void freearray(unsigned char ***pixelarray)
{	
	int x,y;
	

	for (x=0;x<h;++x)
    {

		for(y=0;y<w;++y)
	    {
            free(*(*(pixelarray+x)+y));
	    }

		free(*(pixelarray+x));
	}
	free(pixelarray);





}



/*****************************************************************

function "iBlink" makes the cursor blink while typing file's name

******************************************************************/



void cursorblink()
{
	if(cursor_color==230)cursor_color=0;
	else cursor_color=230;
}





int main()

{
	

	filename=(char *)malloc(100*sizeof(char));
	show_filenm=(char *)malloc(100*sizeof(char));
	
	
	iSetTimer(500,cursorblink);
	
	iInitializeAll();
	
	iInitialize(SCREEN_WIDTH,SCREEN_HEIGHT, "HERMOSA PICTURE VIEWER");
	
	return 0;

}	
