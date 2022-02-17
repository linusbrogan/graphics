
#ifndef XWD_TOOLS_39876
#define XWD_TOOLS_39876


#include <stdio.h>
#include <stdlib.h>
#include <X11/X.h>
#include <X11/Xlib.h>




/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// Functions Below are the ones that users would normally want to call
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////



int create_new_xwd_map (int map_width, int map_height) ;
// will return -1 on error
// otherwise returns the id number of the allocated xwd map


int init_xwd_map_from_file (char *fname) ;


int xwd_map_to_xwd_file (int mapid, FILE *fp) ;
// Always return 1


int xwd_map_to_named_xwd_file (int mapid, char *fname) ;
// return 1 if successful, otherwise return 0 
// (probably because the file could not be opened)


int get_xwd_map_dimensions (int xwd_id_number, int dimensions[2]) ;


int get_xwd_map_color (int xwd_id_number, int x, int y,  double rgb[3]) ;


int set_xwd_map_color (int xwd_id_number, int x, int y,  
                       double r, double g, double b) ;


int clear_xwd_map (int xwd_id_number,
                   double r, double g, double b) ;


#endif
