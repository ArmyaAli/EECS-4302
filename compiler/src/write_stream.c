#include "include/write_stream.h"
#include <stdio.h>
#include <string.h>

void file_write(char* stream) {
  // open for writing
 FILE *file = fopen("asm", "w");  // "w" mode means write
 if(!file) return;
 // Write the entire buffer to the file at once
 int bytes_written = fwrite(stream, sizeof(char), strlen(stream), file);

 fclose(file);                                  

}
void stream_stdout(char* stream) {

}



