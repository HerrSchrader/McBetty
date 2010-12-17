/*
   * Unloved program to convert a binary on stdin to a C include on stdout
   *
   * Jan 1999 Matt Mackall <mpm@selenic.com>
   *
   * This software may be used and distributed according to the terms
   * of the GNU General Public License, incorporated herein by reference.
   */
  
  #include <stdio.h>
  
  int main(int argc, char *argv[])
  {
          int ch, total=0, max_size = -1;
  
	  if (argc > 2) max_size =atoi(argv[2]);
	 
          if (argc > 1)
		printf("/* Generated automatically by bin2c */\n\n");
		printf("/* bootstat register */\n");
		printf("const unsigned char scart_bootstat = 0x00;\n\n");

		printf("/* bootvec register, high byte of the reset vector */\n");
		printf("const unsigned char scart_bootvec  = 0x1f;\n\n");
		
		printf("/* address of the first byte of scart_image to program to */\n");
		printf("const unsigned short scart_image_startaddr = 0x0000;\n\n");
		
		printf("const unsigned char %s[] =\n{", argv[1]);

		while ((ch = getchar()) != EOF)
		{
			  if (total > 0) printf(", ");
			  if (total % 16 == 0)
				  printf("\n\t");
			  total++;

                          printf("0x%02x",ch);
			  
			if ((max_size >= 0) && (total >= max_size)) break;
                  }
  
	  printf("\n};\n");
//          if (argc > 1)
//                  printf("\n\nconst int %s_size = %d;\n", argv[1], total);
  
          return 0;
  }

