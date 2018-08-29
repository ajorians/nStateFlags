#ifdef _TINSPIRE
#include <os.h>
#include <libndls.h>
#else
#endif
#ifndef _WIN32
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#else
#include <stdio.h>
#endif
#include <jpeglib.h>
#include <jinclude.h>

#include "ImageLoader.h"
#include "Replacements.h"

extern Uint16 get_pixel16(SDL_Surface *surface, int x, int y);
extern void put_pixel16(SDL_Surface *surface, int x, int y, Uint16 pixel);

#ifndef _WIN32
/* Read JPEG image from a memory segment */
static void init_source (j_decompress_ptr cinfo) {}
static boolean fill_input_buffer (j_decompress_ptr cinfo)
{
   //ERREXIT(cinfo, JERR_INPUT_EMPTY);
   return TRUE;
}
static void skip_input_data (j_decompress_ptr cinfo, long num_bytes)
{
   struct jpeg_source_mgr* src = (struct jpeg_source_mgr*) cinfo->src;

   if (num_bytes > 0) {
      src->next_input_byte += (size_t) num_bytes;
      src->bytes_in_buffer -= (size_t) num_bytes;
   }
}
static void term_source (j_decompress_ptr cinfo) {}
static void jpeg_mem_src (j_decompress_ptr cinfo, void* buffer, long nbytes)
{
   struct jpeg_source_mgr* src;

   if (cinfo->src == NULL) {   /* first time for this JPEG object? */
      cinfo->src = (struct jpeg_source_mgr *)
              (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
                                          SIZEOF(struct jpeg_source_mgr));
   }

   src = (struct jpeg_source_mgr*) cinfo->src;
   src->init_source = init_source;
   src->fill_input_buffer = fill_input_buffer;
   src->skip_input_data = skip_input_data;
   src->resync_to_restart = jpeg_resync_to_restart; /* use default method */
   src->term_source = term_source;
   src->bytes_in_buffer = nbytes;
   src->next_input_byte = (JOCTET*)buffer;
}

SDL_Surface* LoadImageFromData(unsigned char* pData, int nSize)
{
   int rc, j;

//   SSS    EEEEEEE  TTTTTTT  U     U  PPPP
// SS   SS  E           T     U     U  P   PP
// S        E           T     U     U  P    PP
// SS       E           T     U     U  P   PP
//   SSS    EEEE        T     U     U  PPPP
//      SS  E           T     U     U  P
//       S  E           T     U     U  P
// SS   SS  E           T      U   U   P
//   SSS    EEEEEEE     T       UUU    P

   // Variables for the source jpg
   unsigned long jpg_size;
   unsigned char *jpg_buffer;

   // Variables for the decompressor itself
   struct jpeg_decompress_struct cinfo;
   struct jpeg_error_mgr jerr;

   // Variables for the output buffer, and how long each row is
   int width, height;

   jpg_size = nSize;
   jpg_buffer = (unsigned char*) malloc(jpg_size + 100);

   memcpy(jpg_buffer, pData, jpg_size);

//   SSS    TTTTTTT     A     RRRR     TTTTTTT
// SS   SS     T       A A    R   RR      T
// S           T      A   A   R    RR     T
// SS          T     A     A  R   RR      T
//   SSS       T     AAAAAAA  RRRR        T
//      SS     T     A     A  R RR        T
//       S     T     A     A  R   R       T
// SS   SS     T     A     A  R    R      T
//   SSS       T     A     A  R     R     T

   // Allocate a new decompress struct, with the default error handler.
   // The default error handler will exit() on pretty much any issue,
   // so it's likely you'll want to replace it or supplement it with
   // your own.
   cinfo.err = jpeg_std_error(&jerr);
   jpeg_create_decompress(&cinfo);

   // Configure this decompressor to read its data from a memory
   // buffer starting at unsigned char *jpg_buffer, which is jpg_size
   // long, and which must contain a complete jpg already.
   //
   // If you need something fancier than this, you must write your
   // own data source manager, which shouldn't be too hard if you know
   // what it is you need it to do. See jpeg-8d/jdatasrc.c for the
   // implementation of the standard jpeg_mem_src and jpeg_stdio_src
   // managers as examples to work from.
   jpeg_mem_src(&cinfo, jpg_buffer, jpg_size);

   // Have the decompressor scan the jpeg header. This won't populate
   // the cinfo struct output fields, but will indicate if the
   // jpeg is valid.
   rc = jpeg_read_header(&cinfo, TRUE);

   if (rc != 1) {
      //Should not happen in this program
   }

   // By calling jpeg_start_decompress, you populate cinfo
   // and can then allocate your output bitmap buffers for
   // each scanline.
   jpeg_start_decompress(&cinfo);

   width = cinfo.output_width;
   height = cinfo.output_height;

   SDL_Surface *pSurface = SDL_CreateRGBSurface(0, width, height, SCREEN_BIT_DEPTH, 0, 0, 0, 0);
   // The row_stride is the total number of bytes it takes to store an
   // entire scanline (row).

   //
   // Now that you have the decompressor entirely configured, it's time
   // to read out all of the scanlines of the jpeg.
   //
   // By default, scanlines will come out in RGBRGBRGB...  order,
   // but this can be changed by setting cinfo.out_color_space
   //
   cinfo.out_color_space = JCS_RGB;
   cinfo.out_color_components = 3;
   cinfo.output_components = 3;

   // jpeg_read_scanlines takes an array of buffers, one for each scanline.
   // Even if you give it a complete set of buffers for the whole image,
   // it will only ever decompress a few lines at a time. For best
   // performance, you should pass it an array with cinfo.rec_outbuf_height
   // scanline buffers. rec_outbuf_height is typically 1, 2, or 4, and
   // at the default high quality decompression setting is always 1.

   JSAMPARRAY buffer = (*cinfo.mem->alloc_sarray)
           ((j_common_ptr) &cinfo, JPOOL_IMAGE, cinfo.output_width * cinfo.output_components, 1);
   while(cinfo.output_scanline<cinfo.output_height)
   {   	jpeg_read_scanlines(&cinfo, buffer,1);
      for(j=0;j<width;j++)
      {	int red, green, blue;
         red=buffer[0][j*3];
         green=buffer[0][j*3+1];
         blue=buffer[0][j*3+2];
         red=red>>3;
         blue=blue>>3;
         green=green>>2;//(is_cx?2:3);
         int color;
         color=(red<<11)|(green<<5)|blue;

         put_pixel16(pSurface, j, cinfo.output_scanline- 1, color);
      }
   }

   // Once done reading *all* scanlines, release all internal buffers,
   // etc by calling jpeg_finish_decompress. This lets you go back and
   // reuse the same cinfo object with the same settings, if you
   // want to decompress several jpegs in a row.
   //
   // If you didn't read all the scanlines, but want to stop early,
   // you instead need to call jpeg_abort_decompress(&cinfo)
   jpeg_finish_decompress(&cinfo);

   // At this point, optionally go back and either load a new jpg into
   // the jpg_buffer, or define a new jpeg_mem_src, and then start
   // another decompress operation.

   // Once you're really really done, destroy the object to free everything
   jpeg_destroy_decompress(&cinfo);
   // And free the input buffer
   free(jpg_buffer);

// DDDD       OOO    N     N  EEEEEEE
// D  DDD    O   O   NN    N  E
// D    DD  O     O  N N   N  E
// D     D  O     O  N N   N  E
// D     D  O     O  N  N  N  EEEE
// D     D  O     O  N   N N  E
// D    DD  O     O  N   N N  E
// D  DDD    O   O   N    NN  E
// DDDD       OOO    N     N  EEEEEEE

   return pSurface;

}

#endif

SDL_Surface* GetFlagImage(struct FlagInformation* pFlagInformation, enum Flags flag)
{
   return
#ifdef _WIN32
   SDL_LoadBMP( GetPathForFlag(pFlagInformation, flag) );
#else
   LoadImageFromData(GetImgDataForFlag(pFlagInformation, flag), GetImgDataSizeForFlag(pFlagInformation, flag ) );
#endif
}

