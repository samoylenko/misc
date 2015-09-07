#define NULL 0
#define hidden    0x01
#define readonly  0x02
#define system    0x04
#define volume    0x08
#define subdir    0x10
#define archive   0x20
#define read      0x00
#define write     0x01
#define readwrite 0x02

void print(void near*);
#pragma aux print =     \
  "mov ah, 0x02"        \
  "prn1:"               \
  "mov dl, ds:[si]"     \
  "and dl, dl"          \
  "jz endprn"           \
  "int 0x21"            \
  "inc si"              \
  "jmp prn1"            \
  "endprn:"             \
  parm [si]             \
  modify [si ax dx];

char length(void near*);
#pragma aux length =    \
  "mov ah, 0xff"        \
  "len1:"               \
  "inc ah"              \
  "lodsb"               \
  "and al, al"          \
  "jnz len1"            \
  parm [si]             \
  value [ah]            \
  modify [ax si];

int fcreate(void near* name, int);
#pragma aux fcreate =   \
  "mov ah, 0x3c"        \
  "int 0x21"            \
  "jnc fc1"             \
  "xor ax, ax"          \
  "fc1:"                \
  parm [dx] [cx]        \
  value [ax]            \
  modify [ax];

int fopen(char near*, char);
#pragma aux fopen =     \
  "mov ah, 0x3d"        \
  "int 0x21"            \
  "jnc fo1"             \
  "xor ax, ax"          \
  "fo1:"                \
  parm [dx] [al]        \
  value [ax]            \
  modify [ax];

void fclose(int);
#pragma aux fclose =    \
  "mov ah, 0x3e"        \
  "int 0x21"            \
  parm [bx]             \
  modify [ax];

int fread(void near*, int, int);
#pragma aux fread =     \
  "mov ah, 0x3f"        \
  "int 0x21"            \
  "jnc fr1"             \
  "xor ax, ax"          \
  "fr1:"                \
  parm [dx] [cx] [bx]   \
  value [ax]            \
  modify [ax];

int fwrite(void near*, int, int);
#pragma aux fwrite =    \
  "mov ah, 0x40"        \
  "int 0x21"            \
  parm [dx] [cx] [bx]   \
  value [ax]            \
  modify [ax];

int main(int argc, char* argv[])
{
  int fh, fh1;
  char c, n=0;
  char s[]="00", alphabet[]="0123456789abcdef";

  print("Binary file to 'C' - char array converter by PowerMike (2:5064/21.25)\n\r");
  print("Use: bin2c.exe <infile> <outfile> <array name>\n\r");
  if (argc<4)
  {
    print("Need more parameters!");
    return 10;
  }
  fh = fopen(argv[1], read);
  if (fh == NULL)
  {
    print ("File open error!");
    return 10;
  }
  fh1 = fcreate(argv[2], 0);
  if (fh1 == NULL)
  {
    print ("File create error!");
    return 10;
  }
  fwrite("char ",5,fh1);
  fwrite(argv[3], length(argv[3]), fh1);
  fwrite("[] =\n{\n  ",9,fh1);
  while (fread(&c,1,fh)==1)
  {
    if (n!=0)
      fwrite (", ",2,fh1);
    if (n++>=12)
    {
      fwrite(" \\\n  ",5,fh1);
      n=1;
    }
    fwrite ("0x",2,fh1);
    s[1]=alphabet[c%16];
    s[0]=alphabet[c/16];
    fwrite (&s,2,fh1);
  }
  fwrite("\n};\n",4,fh1);
  fclose(fh);
  fclose(fh1);
  print("Done.\n\r\n");
  return 0;
}
