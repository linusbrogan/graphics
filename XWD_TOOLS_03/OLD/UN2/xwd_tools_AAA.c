
#include <stdio.h>
#include <stdlib.h>
#include <X11/X.h>
#include <X11/Xlib.h>



#define MAXNUMXWDMAPS 100
static  XImage xwdmap[MAXNUMXWDMAPS] ;
static  int    numxwdmaps = 0 ;



static void fputintB328 (int x, FILE *fp)
{
   char *p ;
   int c[4],i ;

   p = (char *) &x ;

   for (i = 0 ; i < 4 ; i++) {
        c[i] = *p ;
        p++ ;
   }

   for (i = 3 ; i >= 0 ; i--) {
        fputc(c[i], fp) ;
   }

}




static int fgetintB328 (FILE *fp)
{
   char *p ;
   int c[4],i ;
   int x ;
   char q ;

   for (i = 3 ; i >= 0 ; i--) {
        c[i] = fgetc(fp) ;
   }



   p = (char *) &x ;

   for (i = 0 ; i < 4 ; i++) {
        q = c[i] ;
        *p = q ;;
        p++ ;
   }

   return x ;
}








static int Set_color_into_XImage
                (XImage *p_buffer_ximage,
                 int xk, int yk,
                 double r, double g, double b)
{
 int numx, numy ;
 unsigned int red, green, blue ;
 unsigned int pixel ;
 unsigned int *pixel_array ;

 numx = p_buffer_ximage->width ;
 numy = p_buffer_ximage->height ;

 if ((xk < 0) || (xk >= numx)) return -1 ;
 if ((yk < 0) || (yk >= numy)) return -1 ;

 pixel_array =  (unsigned int *) p_buffer_ximage->data ;

 // this must be used as otherwise, p_buffer_ximage->data is viewed as 
 // an array of characters with erroneous consequences for both array subscript
 // calculation and data storage ... see below 

 red   = (unsigned int) (r*256) ;
 green = (unsigned int) (g*256) ;
 blue  = (unsigned int) (b*256) ;
 if (red >= 256)   red   = 255 ;
 if (green >= 256) green = 255 ;
 if (blue >= 256)  blue  = 255 ;
 red   &= 0xff ;   green &= 0xff ;   blue  &= 0xff ;
 pixel = (red << 16) | (green << 8) | blue ; 

 pixel_array [(numy - 1 - yk)*numx + xk]  = pixel ;

 return 1 ;
}






static int Get_color_from_XImage
                  (XImage *p_buffer_ximage,
                   int xk, int yk,
                   double rgb[3])
{
 int numx, numy ;
 unsigned int red, green, blue ;
 unsigned int pixel ;
 unsigned int *pixel_array ;

 numx = p_buffer_ximage->width ;
 numy = p_buffer_ximage->height ;

 if ((xk < 0) || (xk >= numx)) return -1 ;
 if ((yk < 0) || (yk >= numy)) return -1 ;

 pixel_array =  (unsigned int *) p_buffer_ximage->data ;
 // this must be used as otherwise, p_buffer_ximage->data is viewed as 
 // an array of characters with erroneous consequences for both array subscript
 // calculation and data storage ... see below 

 pixel =  pixel_array [(numy - 1 - yk)*numx + xk] ;
 blue  =  pixel & 0xff ;
 green =  (pixel >> 8) & 0xff ;
 red   =  (pixel >> 16) & 0xff ;

 rgb[0] = red   / 256.0 ;
 rgb[1] = green / 256.0 ;
 rgb[2] = blue  / 256.0 ;

 return 1 ; 
}










static void err()
{
  printf("Acquire_Image_From_XWD_File : error...may not be correct style of xwd file\n") ;
}




static int Acquire_Image_From_XWD_File (FILE *fp)
// will return -1 on error
// otherwise returns the id number of the allocated xwd map
{
  /* unlike Recall_Image_From_XWD_File(fp), this rooutine acquires
     the data but does not render it onto the display, leaving it
     instead in the global variable, xwdmap[numxwdmaps]
     Assumes full color files .. 24 bit color

     return -1 for failure, otherwise return index of pixmap used
     ... this will aid for future reference
  */
 // assumes full color but uses full 32 bits per pixel
 int header_size, file_version, format, depth ;
 int xoffset, byte_order, bitmap_unit, bitmap_order, bitmap_pad ;
 int bits_per_pixel, bytes_per_line, visual_class ;
 int redmask, greenmask, bluemask ;
 int bitsperrgb, numentriesincolormap, numcolorstructures ;
 int windowwidth, windowheight, windowx, windowy, windowborderwidth ;
 int numbytestowrite ;
 int width,height ;
 int c, de, numbytestoread ;
 int retval ;

 header_size = fgetintB328(fp) ;
 file_version = fgetintB328(fp) ;
 format = fgetintB328(fp) ;
 depth = fgetintB328(fp) ;
 width =  fgetintB328(fp) ;
 height =  fgetintB328(fp) ;
 xoffset = fgetintB328(fp) ;
 byte_order = fgetintB328(fp) ;
 bitmap_unit = fgetintB328(fp) ;
 bitmap_order = fgetintB328(fp) ;
 bitmap_pad = fgetintB328(fp) ;
 bits_per_pixel = fgetintB328(fp) ;
 bytes_per_line = fgetintB328(fp) ;
 visual_class = fgetintB328(fp) ;
 redmask = fgetintB328(fp) ;
 greenmask = fgetintB328(fp) ;
 bluemask = fgetintB328(fp) ;
 bitsperrgb = fgetintB328(fp) ;
 numentriesincolormap = fgetintB328(fp) ;
 numcolorstructures = fgetintB328(fp) ;
 windowwidth = fgetintB328(fp) ;
 windowheight = fgetintB328(fp) ;
 windowx = fgetintB328(fp) ;
 windowy = fgetintB328(fp) ;
 windowborderwidth = fgetintB328(fp) ;

 // null terminated window name fleshed out to multiple of 4 bytes
 c = fgetc(fp) ; c = fgetc(fp) ; c = fgetc(fp) ; c = fgetc(fp) ;


 if (header_size != 104) { err() ; retval = -1 ; goto LLL ; }
 if (file_version != 7) { err() ; retval = -1 ; goto LLL ; }
 if (format != 2) { err() ; retval = -1 ; goto LLL ; } 
 if (depth != 24) { err() ; retval = -1 ; goto LLL ; }
 if (xoffset != 0) { err() ; retval = -1 ; goto LLL ; }
 if (byte_order != 0) { err() ; retval = -1 ; goto LLL ; }
 if (bitmap_unit != 32) { err() ; retval = -1 ; goto LLL ; }
 if (bitmap_order != 0) { err() ; retval = -1 ; goto LLL ; }
 if (bitmap_pad != 32) { err() ; retval = -1 ; goto LLL ; }
 if (bits_per_pixel != 32) { err() ; retval = -1 ; goto LLL ; }
 if (bytes_per_line != width * 4) { err() ; retval = -1 ; goto LLL ; }
 if (visual_class != 5) { err() ; retval = -1 ; goto LLL ; }
 if (redmask != 0x00ff0000) { err() ; retval = -1 ; goto LLL ; }
 if (greenmask != 0x0000ff00) { err() ; retval = -1 ; goto LLL ; }
 if (bluemask != 0x000000ff) { err() ; retval = -1 ; goto LLL ; }
 if (bitsperrgb != 24) { err() ; retval = -1 ; goto LLL ; }
 if (numentriesincolormap != 0) { err() ; retval = -1 ; goto LLL ; }
 if (numcolorstructures != 0) { err() ; retval = -1 ; goto LLL ; }
 if (windowwidth != width) { err() ; retval = -1 ; goto LLL ; }
 if (windowheight != height) { err() ; retval = -1 ; goto LLL ; }
 if (windowx != 0) { err() ; retval = -1 ; goto LLL ; }
 if (windowy != 0) { err() ; retval = -1 ; goto LLL ; }
 if (windowborderwidth != 0) { err() ; retval = -1 ; goto LLL ; }


 
 xwdmap[numxwdmaps].width = width ;
 xwdmap[numxwdmaps].height = height ;
 xwdmap[numxwdmaps].depth = depth ;
 xwdmap[numxwdmaps].xoffset = xoffset ;
 xwdmap[numxwdmaps].format = format ;
 xwdmap[numxwdmaps].bitmap_unit = bitmap_unit ;
 xwdmap[numxwdmaps].bitmap_pad = bitmap_pad ;
 xwdmap[numxwdmaps].bytes_per_line = bytes_per_line ;
 xwdmap[numxwdmaps].bits_per_pixel = bits_per_pixel ;
 xwdmap[numxwdmaps].byte_order = byte_order ;
 xwdmap[numxwdmaps].bitmap_bit_order = bitmap_order ;


 numbytestoread =   xwdmap[numxwdmaps].bytes_per_line *
                                  xwdmap[numxwdmaps].height ;

 xwdmap[numxwdmaps].data = (char *)malloc(numbytestoread) ;

 if (xwdmap[numxwdmaps].data == NULL) {
    printf("\ncan't malloc space for xwdmap\n\n");
    retval = -1 ;
    goto LLL ;
 }

 fread(xwdmap[numxwdmaps].data, numbytestoread, 1, fp) ;

 retval = numxwdmaps ;

 numxwdmaps++ ;



 LLL :

 fclose(fp) ; 

 return retval ;

}






/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// Functions Below are the ones that users would normally want to call
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////




int create_new_xwd_map (int map_width, int map_height)
// will return -1 on error
// otherwise returns the id number of the allocated xwd map
{
  /* unlike Recall_Image_From_XWD_File(fp), this rooutine acquires
     the data but does not render it onto the display, leaving it
     instead in the global variable, xwdmap[numxwdmaps]
     Assumes full color files .. 24 bit color

     return -1 for failure, otherwise return index of pixmap used
     ... this will aid for future reference
  */
 // assumes full color but uses full 32 bits per pixel
 int header_size, file_version, format, depth ;
 int xoffset, byte_order, bitmap_unit, bitmap_order, bitmap_pad ;
 int bits_per_pixel, bytes_per_line, visual_class ;
 int redmask, greenmask, bluemask ;
 int bitsperrgb, numentriesincolormap, numcolorstructures ;
 int windowwidth, windowheight, windowx, windowy, windowborderwidth ;
 int numbytestowrite ;
 int width,height ;
 int c, de, numbytestoread ;
 int retval ;

 header_size = 104 ;
 file_version = 7 ;
 format = 2 ;
 depth = 24 ;

 width =  map_width ;
 height =  map_height ;

 xoffset = 0 ;
 byte_order = 0 ;
 bitmap_unit = 32 ;
 bitmap_order = 0 ;
 bitmap_pad = 32 ;
 bits_per_pixel = 32 ;
 bytes_per_line = width*4 ;
 visual_class = 5 ;
 redmask   = 0x00ff0000 ;
 greenmask = 0x0000ff00 ;
 bluemask  = 0x000000ff ;
 bitsperrgb = 24 ;
 numentriesincolormap = 0 ;
 numcolorstructures = 0 ;
 windowwidth = width ;
 windowheight = height ;
 windowx = 0 ;
 windowy = 0 ;
 windowborderwidth = 0 ;


 
 xwdmap[numxwdmaps].width = width ;
 xwdmap[numxwdmaps].height = height ;
 xwdmap[numxwdmaps].depth = depth ;
 xwdmap[numxwdmaps].xoffset = xoffset ;
 xwdmap[numxwdmaps].format = format ;
 xwdmap[numxwdmaps].bitmap_unit = bitmap_unit ;
 xwdmap[numxwdmaps].bitmap_pad = bitmap_pad ;
 xwdmap[numxwdmaps].bytes_per_line = bytes_per_line ;
 xwdmap[numxwdmaps].bits_per_pixel = bits_per_pixel ;
 xwdmap[numxwdmaps].byte_order = byte_order ;
 xwdmap[numxwdmaps].bitmap_bit_order = bitmap_order ;


 numbytestoread =   xwdmap[numxwdmaps].bytes_per_line *
                                  xwdmap[numxwdmaps].height ;

 xwdmap[numxwdmaps].data = (char *)malloc(numbytestoread) ;

 if (xwdmap[numxwdmaps].data == NULL) {
    printf("\ncan't malloc space for xwdmap\n\n");
    retval = -1 ;
    goto LLL ;
 }


 retval = numxwdmaps ;

 numxwdmaps++ ;



 LLL :

 return retval ;

}










int init_xwd_map_from_file (char *fname)
{
  FILE *fp ;
  int idnum ;

  fp = fopen(fname,"r") ;
  if (fp == NULL) {
    printf("init_xwdmap_from_file : error, can't open file %s\n",fname) ;
    return -1 ;
  }

  idnum = Acquire_Image_From_XWD_File (fp) ;
  return idnum ;
}






int xwd_map_to_xwd_file (int mapid, FILE *fp)
// Always return 1
{
 // assumes full color but uses full 32 bits per pixel
 int header_size, file_version, format, depth ;
 int xoffset, byte_order, bitmap_unit, bitmap_order, bitmap_pad ;
 int bits_per_pixel, bytes_per_line, visual_class ;
 int redmask, greenmask, bluemask ;
 int bitsperrgb, numentriesincolormap, numcolorstructures ;
 int windowwidth, windowheight, windowx, windowy, windowborderwidth ;
 int numbytestowrite ;
 int width,height ;

 header_size = 104 ;
 fputintB328(header_size,fp) ;

 file_version = 7 ;
 fputintB328(file_version,fp) ; 

 format = 2 ; // ZPixmap
 fputintB328(format,fp) ;

 depth = 24 ;
 fputintB328(depth,fp) ;

 width = xwdmap[mapid].width ;
 fputintB328(width,fp) ;

 height = xwdmap[mapid].height ;
 fputintB328(height,fp) ;

 xoffset = 0 ;
 fputintB328(xoffset,fp) ;

 byte_order = 0 ;
 fputintB328(byte_order,fp) ;

 bitmap_unit = 32 ;
 fputintB328(bitmap_unit,fp) ;

 bitmap_order = 0 ;
 fputintB328(bitmap_order,fp) ;

 bitmap_pad = 32  ;
 fputintB328(bitmap_pad,fp) ;

 bits_per_pixel = 32 ;
 fputintB328(bits_per_pixel,fp) ;

 bytes_per_line = width * 4 ;
 fputintB328(bytes_per_line,fp) ;

 visual_class = 5 ; // more than one sample per pixel
 fputintB328(visual_class,fp) ;

 redmask = 0x00ff0000 ;
 fputintB328(redmask,fp) ;

 greenmask = 0x0000ff00 ;
 fputintB328(greenmask,fp) ;

 bluemask = 0x000000ff ;
 fputintB328(bluemask,fp) ;

 // bitsperrgb = 8 ; // hard to believe
 bitsperrgb = 24 ; 
 fputintB328(bitsperrgb,fp) ;

 numentriesincolormap = 0 ; // not used :
 fputintB328(numentriesincolormap,fp) ;

 numcolorstructures = 0 ;
 fputintB328(numcolorstructures,fp) ;

 windowwidth = width ;
 fputintB328(windowwidth,fp) ;

 windowheight = height ;
 fputintB328(windowheight,fp) ;

 windowx = 0 ; // how crucial is this ?
 fputintB328(windowx,fp) ;

 windowy = 0 ; // how crucial is this ?
 fputintB328(windowy,fp) ;

 windowborderwidth = 0 ;
 fputintB328(windowborderwidth,fp) ;


 // null terminated window name fleshed out to multiple of 4 bytes
 fputc('\0',fp) ;
 fputc('\0',fp) ;
 fputc('\0',fp) ;
 fputc('\0',fp) ;


 numbytestowrite =   bytes_per_line * height ;
 fwrite(xwdmap[mapid].data, numbytestowrite, 1, fp) ;
 fclose(fp) ;

 return 1 ;
}








int xwd_map_to_named_xwd_file (int mapid, char *fname)
// return 1 if successful, otherwise return 0 
// (probably because the file could not be opened)
{
  FILE *fp ;
  int s ;

  fp = fopen(fname,"w") ;
  if (fp == NULL) {
    printf("xwd_map_to_xwd_named_file : can't open file %s\n",fname) ;
    return 0 ;
  }

  s = xwd_map_to_xwd_file (mapid, fp) ;
  return s ;
}





int get_xwd_map_dimensions (int xwd_id_number, int dimensions[2])
{
  XImage *p ;
  int s ;

  if ((xwd_id_number < 0) || (xwd_id_number >= numxwdmaps)) return -1 ;
  dimensions[0] = xwdmap[xwd_id_number].width ;
  dimensions[1] = xwdmap[xwd_id_number].height ;

  return 1 ;
}









int get_xwd_map_color (int xwd_id_number, int x, int y,  double rgb[3])
{
  XImage *p ;
  int s ;

  if ((xwd_id_number < 0) || (xwd_id_number >= numxwdmaps)) return -1 ;
  p = &(xwdmap[xwd_id_number]) ;
  s = Get_color_from_XImage (p, x,y, rgb) ;

  return s ;
}



int set_xwd_map_color (int xwd_id_number, int x, int y,  
                       double r, double g, double b)
{
  XImage *p ;
  int s ;


  if (r < 0) r = 0 ;
  if (r > 1) r = 1 ;

  if (g < 0) g = 0 ;
  if (g > 1) g = 1 ;

  if (b < 0) b = 0 ;
  if (b > 1) b = 1 ;
  // the truncation above is new...to conform to behavior
  // of G_rgb()


  if ((xwd_id_number < 0) || (xwd_id_number >= numxwdmaps)) return -1 ;
  p = &(xwdmap[xwd_id_number]) ;
  s = Set_color_into_XImage (p, x,y, r,g,b) ;

  return s ;
}




int clear_xwd_map (int xwd_id_number,
                   double r, double g, double b) 
{
  int dim[2] ;
  int x,y,s ;

  get_xwd_map_dimensions(xwd_id_number, dim) ;

  for (x = 0 ; x < dim[0] ; x++) {
    for (y = 0 ; y < dim[1] ; y++) {
      s = set_xwd_map_color(xwd_id_number,x,y,r,g,b) ;
      if (s == -1) return -1 ;
    }
  }

  return 1 ;
}

