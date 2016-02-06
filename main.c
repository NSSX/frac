#include <mlx.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
# define RGB(r, g, b)(256 * 256 * (int)(r) + 256 * (int)(g) + (int)(b))

  
typedef struct                                                  s_img
{
  void                    *mlx;
  void                    *win;
  unsigned long   img_color;
  char                    *data;
  void                    *img_ptr;
  int                             sizeline;
  int                             bpp;
  int                             x;
  int                             y;
  int                             endian;
  void                    *mlx_ptr;
}                                                                               t_img;


typedef struct                                                  s_temp
{
  int x1;
  int x2;
  int y1;
  int y2;
  int val1;
  int val2;
  int color;
}                                                                               t_temp;

typedef struct                                                  s_line
{
  int dx;
  int dy;
  int i;
  int xinc;
  int yinc;
  int cumul;
  int x;
  int y;
}                                                                               t_line;

typedef struct s_mystruct
{
  void *mlx;
  void *win;
  char                    ***coord;
  int                             i;
  int                             maxline;
  int                             zoom;
  t_img                   *img;
  int ite;
  float xv;
  float yv;
  int other;
}               t_struct;

void draw_julia(t_img *myimg);
void    my_pixel_put_to_image(t_img *myimg, int x, int y, int color);
void draw_fractal(t_img *myimg, float x, float y, float h, int ite, int other, int zooma, float v1, float v2);
int motion(int x, int y, t_struct *mystruct);

void ft_putchar(char c)
{
  write(1, &c, 1);
}

t_line  *give_line(float xi, float yi, float xf, float yf)
{
  t_line *line;

  line = (t_line *)malloc(sizeof(t_line) * 12);
  line->x = xi;
  line->y = yi;
  line->dx = xf - xi;
  line->dy = yf - yi;
  line->xinc = (line->dx > 0) ? 1 : -1;
  line->yinc = (line->dy > 0) ? 1 : -1;
  line->dx = abs(line->dx);
  line->dy = abs(line->dy);
  return (line);
}

t_line  *first_while(t_line *line)
{
  line->x += line->xinc;
  line->cumul += line->dy;
  if (line->cumul >= line->dx)
    {
      line->cumul -= line->dx;
      line->y += line->yinc;
    }
  return (line);
}

t_line  *second_while(t_line *line)
{
  line->y += line->yinc;
  line->cumul += line->dx;
  if (line->cumul >= line->dy)
    {
      line->cumul -= line->dy;
      line->x += line->xinc;
    }
  return (line);
}

t_line  *norme_line(t_line *line)
{
  line->cumul = line->dy / 2;
  line->i = 1;
  return (line);
}

void    draw_line_on_img(t_img *myimg, t_temp *temp, int color)
{
  t_line *line;

  line = give_line(temp->x1, temp->y1, temp->x2, temp->y2);
  my_pixel_put_to_image(myimg, line->x, line->y, color);
  if (line->dx > line->dy)
    {
      line->cumul = line->dx / 2;
      line->i = 1;
      while (line->i <= line->dx)
	{
	  line = first_while(line);
	  my_pixel_put_to_image(myimg, line->x, line->y, color);
	  line->i++;
	}
    }
  else
    {
      line = norme_line(line);
      while (line->i <= line->dy)
	{
	  line = second_while(line);
	  my_pixel_put_to_image(myimg, line->x, line->y, color);
	  line->i++;
	}
    }
}


int             event_mlx(int keycode, t_struct *mystruct)
{
  /*  static float xv;
  static float yv;

  if(!xv)
    xv = 0.4;
  if(!yv)
    yv = 0.2;
  */
  if (keycode == 53)
    exit(1);
  /*if(keycode == 123)
    {
      mlx_destroy_image(mystruct->mlx, mystruct->img->img_ptr);
      mystruct->img->img_ptr = mlx_new_image(mystruct->mlx, 1500, 1500);
      draw_fractal(mystruct->img, xv, yv, 1, ite);
      mlx_put_image_to_window(mystruct->mlx, mystruct->win, mystruct->img->img_ptr, 0, 0);
    }
  if(keycode == 124)
    {
     
    }*/
  if (keycode == 17)
    {
      mlx_destroy_image(mystruct->mlx, mystruct->img->img_ptr);
      mystruct->img->img_ptr = mlx_new_image(mystruct->mlx, 1500, 1500);
      draw_julia(mystruct->img);
      mlx_put_image_to_window(mystruct->mlx, mystruct->win, mystruct->img->img_ptr, 0, 0);
    }
  return (0);
}

int mouse_mlx(int button, int x, int y, t_struct *mystruct)
{
  static int h;
  static int ite;
  static int zooma;

  if(!zooma)
    zooma = 300;
  if(!ite)
    ite = 100;
  if(!h)
    h = 1;
  if(button == 1)
    {   
      zooma += 50;
      mlx_destroy_image(mystruct->mlx, mystruct->img->img_ptr);
      mystruct->img->img_ptr = mlx_new_image(mystruct->mlx, 1500, 1500);
      draw_fractal(mystruct->img, 1, 1, h, ite, 0, zooma,1,1);
      mlx_put_image_to_window(mystruct->mlx, mystruct->win, mystruct->img->img_ptr, 0, 0);
      mlx_mouse_hook (mystruct->win, mouse_mlx, mystruct);
     }
  if(button == 2)
    {
      zooma -= 50;
      mlx_destroy_image(mystruct->mlx, mystruct->img->img_ptr);
      mystruct->img->img_ptr = mlx_new_image(mystruct->mlx, 1500, 1500);
      draw_fractal(mystruct->img, 1, 1, h, ite, 0, zooma,1,1);
      mlx_put_image_to_window(mystruct->mlx, mystruct->win, mystruct->img->img_ptr, 0, 0);
      mlx_mouse_hook (mystruct->win, mouse_mlx, mystruct);
    }
  if(button == 4)
    {
      ite -= 3;
      mlx_destroy_image(mystruct->mlx, mystruct->img->img_ptr);
      mystruct->img->img_ptr = mlx_new_image(mystruct->mlx, 1500, 1500);
      draw_fractal(mystruct->img, mystruct->xv, mystruct->yv, h, ite, mystruct->other, zooma,1,1);
      mlx_put_image_to_window(mystruct->mlx, mystruct->win, mystruct->img->img_ptr, 0, 0);
      mlx_mouse_hook (mystruct->win, mouse_mlx, mystruct);
    }
  if(button == 5)
    {
      ite += 3;
      mlx_destroy_image(mystruct->mlx, mystruct->img->img_ptr);
      mystruct->img->img_ptr = mlx_new_image(mystruct->mlx, 1500, 1500);
      draw_fractal(mystruct->img, mystruct->xv, mystruct->yv, h, ite, mystruct->other, zooma,1,1);
      mlx_put_image_to_window(mystruct->mlx, mystruct->win, mystruct->img->img_ptr, 0, 0);
      mlx_mouse_hook (mystruct->win, mouse_mlx, mystruct);
    }
  mystruct->ite = ite;
  return (0);
}

int motion(int x, int y, t_struct *mystruct)
{
  static int xp;
  static int yp;
  static float xv;
  static float yv;
  static int other;
  static float v1;
  static float v2;

  if(!v1)
    v1 = 0;
  if(!v2)
    v2 = 0;
  if(!other)
    other = 2;
  if(!xv)
    xv = 0.5;
  if(!yv)
    yv = 0.1;
  if(!xp)
    xp = x;
  if(!yp)
    yp = y;
  if(x < xp)
    {
      v1 -= 0.030;
    }
  else if(x > xp)
    {
      v1 += 0.030;
    }
  if(y < yp)
    {
      v2 -= 0.030;
    }
  else if(y > yp)
    {
      v2 += 0.030;
    }
  xp = x;
  yp = y;
  mystruct->xv = xv;
  mystruct->yv = yv;
  mystruct->other = other;
  mlx_destroy_image(mystruct->mlx, mystruct->img->img_ptr);
  mystruct->img->img_ptr = mlx_new_image(mystruct->mlx, 1500, 1500);
  draw_fractal(mystruct->img, mystruct->xv, mystruct->yv, 1, mystruct->ite, mystruct->other,100,v1,v2);
  mlx_put_image_to_window(mystruct->mlx, mystruct->win, mystruct->img->img_ptr, 0, 0);
  return (0);
}

void    my_pixel_put_to_image(t_img *myimg, int x, int y, int color)
{
  int                             i;
  unsigned char   color1;
  unsigned char   color2;
  unsigned char   color3;

  myimg->data = mlx_get_data_addr(myimg->img_ptr,
				  &myimg->bpp, &myimg->sizeline, &myimg->endian);
  myimg->data[y * myimg->sizeline + x * myimg->bpp / 8] = color % 256;
  color /= 256;
  myimg->data[y * myimg->sizeline + x * myimg->bpp / 8 + 1] = color % 256;
  color /= 256;
  myimg->data[y * myimg->sizeline + x * myimg->bpp / 8 + 2] = color % 256;
}

void color_img(t_img *myimg)
{
  int i;
  int j;
 
  i = 0;
  j = 0;
  while(i < 800)
    {
      while(j < 800)
	{
	  my_pixel_put_to_image(myimg, j, i, 0x0000FF);
	  j++;
	}
      j = 0;
      i++;
    }
}

int ok_draw(int x, int y)
{
  if(x < 1500 && y < 1500 && x > 0 && y > 0)
    return (1);
  return (0);
}

void draw_julia(t_img *myimg)
{
  float x1 = -1.5;
  float x2 = 1.5;
  float y1 = -1.2;
  float y2 = 1.2;
  float zoom = 400;
  float max_ite = 80;
  float image_x = (x2 - x1) * zoom;
  float image_y = (y2 - y1) * zoom;

  for(int x = 0; x < image_x; x++)
    {
      for(int y = 0; y < image_y; y++)
        {
          float c_r = 0.285;//0.285;
          float c_i = 0.01;
          float z_r = x / zoom + x1;
          float z_i = y / zoom + y1;
          float i = 0;
          do
            {
              float tmp = z_r;
              z_r = z_r * z_r - z_i * z_i + c_r;
              z_i = 2*z_i*tmp + c_i;
              i = i + 1;
            }while((z_r * z_r + z_i * z_i) < 4 && i < max_ite);
          if(i == max_ite)
            {
              if(ok_draw(x,y) == 1)
                my_pixel_put_to_image(myimg, x, y, 0x000000);
            }
          else
            {
              if(ok_draw(x,y) == 1)
                my_pixel_put_to_image(myimg, x, y, RGB(0, 0, i*255/max_ite));
            }
        }
    }

}


void draw_fractal(t_img *myimg, float x, float y, float h, int ite, int other, int zooma, float v1, float v2)
{
  float x1 = -2;
  float x2 = 1;
  float y1 = -1.5;
  float y2 = 1.5;
  float zoom = 250;
  float max_ite = 100;//detaille
  float image_x = (x2 - x1) * zoom;
  float image_y = (y2 - y1) * zoom;


  for(int x = 0; x < image_x; x++)
    {
      for(int y = 0; y < image_y; y++)
	{
	  float c_r = x / zoom + x1;
	  float c_i = y / zoom + y1;;
	  float z_r = v1;//0;
	  float z_i = v2;
	  float i = 0;
	  do
	    {	      
	      float tmp = z_r; 
	      z_r = z_r * z_r - z_i * z_i + c_r;
	      z_i = 2*z_i*tmp + c_i;
	      i = i + 1;
	    }while((z_r * z_r + z_i * z_i) < 4 && i < max_ite);
	  if(i == max_ite)
	    {
	      if(ok_draw(x,y) == 1)
		my_pixel_put_to_image(myimg, x, y, 0x000000);
	    }
	  else
	    {
	      if(ok_draw(x,y) == 1)
		my_pixel_put_to_image(myimg, x, y, RGB(0, 0, i*255/max_ite));
	    }
	}
    }
}

int             main(int argc, char **argv)
{
  t_struct *mystruct;
  t_img *myimg;
  t_temp *temp;

  temp = (t_temp *)malloc(sizeof(t_temp) * 20);
  myimg = (t_img *)malloc(sizeof(t_img) * 20);
  mystruct = (t_struct *)malloc(sizeof(t_struct) * 20);
  mystruct->mlx = mlx_init();
  mystruct->win = mlx_new_window(mystruct->mlx, 1500, 1500, "42");
  myimg->img_ptr = mlx_new_image(mystruct->mlx, 1500, 1500);
  /*
  temp->x1 = 10;
  temp->y1 = 10;
  temp->x2 = 100;
  temp->y2 = 10;*/
  //draw_line_on_img(myimg, temp, 0xFF0000);
  //color_img(myimg);
  mystruct->img = myimg;
 mystruct->ite = 100;
  mystruct->xv = 0.5;
  mystruct->yv = 0.5;
  mystruct->other = 2;
  draw_fractal(myimg, 0.5, 0.5, 1, 100, mystruct->other, 300, 0, 0);
  mlx_put_image_to_window(mystruct->mlx, mystruct->win, myimg->img_ptr, 0, 0);
  mlx_key_hook(mystruct->win, event_mlx, mystruct);
  mlx_mouse_hook (mystruct->win, mouse_mlx, mystruct);
  mlx_hook(mystruct->win, 6, (1L<<6),motion, mystruct);
  mlx_loop(mystruct->mlx);
  return (0);
}
